/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions: memory management subset                    *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 06.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Rewrited 10.01.2003 Maxim A. Orlovsky: new memory managing algorithm        *
*  Revised 11.01.2003 Maxim A. Orlovsky: added strict function arguments       *
*                     control and any kind of possible checks                  *
*  13.01.2003 Version 1.00 alpha                                               *
*                                                                              *
*******************************************************************************/

/* TODO: for ver. 1.00 beta add safe multithreading support */
/* TODO:
	 1. If requested memory exceed page size then allocate new page with the requested size
	 2. Use smart algorithm for choosing place for small memory pices
	 3. Use two types of pages: for small memory pices and lage ones
	 4. If we create a new free block in the end of mbat used part, then merge it with
	    the rest of free mbat space in mbat unused part
	 5. Algorithm for aggregating sequences of free blocks
	 6. Check memory stamp for memunit validation
	 7. Replace all notnull checks on the memory address validation routine call in all modules
*/

#include <malloc.h>
#include <winsock2.h>
#include <time.h>
#include <sys/timeb.h>

#include <sysconf.h>
#include <strsafex.h>
#include <libnodes.h>
#include "aimkstp.h"

#include "ks.h"
#include "ksmem.h"

memunit* mem_tail = NULL;
memunit* mem_free = NULL;

bool		mem_mgr = 1;
uvar32_64	mem_mpage = KS_MPAGE_SIZE;
uvar32_64	mem_mbat = KS_MBAT_SIZE;

#if defined _DEBUG
	#include <stdio.h>
	int cnt = 0;
	extern char* conf_log_dir;
	void _ksmem_dump (memunit* unit) {
		uvar32_64 no;
		svar32_64 ub = unit->fub, fb = unit->ffb;
		FILE* fp;
		char str[64];
		sprintf (str, "%s/dias_%04d.%08X", conf_log_dir, cnt, unit);
		fp = fopen (str, "w");
		fprintf (fp, "FUB: %03Xh, LUB: %03Xh\n", unit->fub, unit->lub);
		fprintf (fp, "FFB: %03Xh, LFB: %03Xh\n\n", unit->ffb, unit->lfb);
		fprintf (fp, "TBOTTOM: %08Xh, PBOTTOM: %08Xh\n", unit->tbottom, unit->pbottom);
		fprintf (fp, "MAXFB: %08Xh, MINFB: %08Xh\n\n", unit->maxfb, unit->minfb);
		for (no = 0; no <= unit->tbottom; no++) {
			if (no == (uvar32_64)ub) {
				fprintf (fp, "%03Xh:  u->%03Xh    (offset %08Xh, size %08Xh)\n",
				         no, unit->mbat[no].next, unit->mbat[no].offset, unit->mbat[no].size);
				ub = unit->mbat[ub].next;
			} else if (no == (uvar32_64)fb) {
				fprintf (fp, "%03Xh:     f->%03Xh (offset %08Xh, size %08Xh)\n",
				         no, unit->mbat[no].next, unit->mbat[no].offset, unit->mbat[no].size);
				fb = unit->mbat[fb].next;
			} else
				fprintf (fp, "%03Xh: ??? %03Xh    (offset %08Xh, size %08Xh)\n",
				         no, unit->mbat[no].next, unit->mbat[no].offset, unit->mbat[no].size);
		}
		fclose (fp);
		cnt++;
	}
#endif

bool ksmem_init () {
	uvar32_64 n;
	memunit* unit;

	if (mem_mgr == 0) {
		return (true);
	}

	isvalidf (!mem_tail, CLIENT_ROOT, KSERR_MEMTAILNOTNULL);

	mem_tail = _ksmem_unit_alloc (CLIENT_ROOT);
	for (n = 0; n < conf_mem_pages_minfree; ++n) {
		isvalidf (unit = (memunit*)malloc (sizeof (memunit)), CLIENT_ROOT, KSERR_OUTOFMEM);
		_ksmem_unit_init (CLIENT_ROOT, unit);
		if (!mem_free)
			mem_free = unit;
		else
			bdlist_insert (mem_free, unit);
	}
	return (mem_tail != NULL);
}

bool _ksmem_unit_init (client* c, memunit* unit) {
	struct timeb tm;

	notnullf (unit, c);

	memset (unit, 0, sizeof (memunit));
	ftime (&tm);
	unit->owner = c;
	unit->created = tm.time;
	unit->psize = unit->pfree = KS_MPAGE_SIZE;
	unit->tsize = unit->tfree = KS_MBAT_SIZE;
	unit->minfb = unit->maxfb = KS_MPAGE_SIZE;
	unit->fub = -1;
	unit->lub = -1;
	unit->ffb = -1;
	unit->lfb = -1;
	unit->stamp1 = unit->stamp2 = KSMEM_STAMP;
	return (true);
}

memunit* _ksmem_unit_alloc (client* c) {
	memunit* mem;
	struct timeb tm;
	ftime (&tm);

	/* No need in quottas checking - they are checked only while allocking blocks, not units */
	isclientn (c);
	if (mem_free) {
		mem = mem_free;
		mem_free = mem_free->next;
	} else
		isvalidn (mem = (memunit*)malloc (sizeof (memunit)), c, KSERR_OUTOFMEM);

	_ksmem_unit_init (c, mem);
	mem->modified = tm.time;
atomicall
	if (mem_tail) {
		mem->prev = mem_tail;
		mem->next = NULL;
		mem_tail->next = mem;
	}
	mem_tail = mem;
endatomic

atomicall
	if (c->mem_tail) {
		mem->precessor = c->mem_tail;
		mem->successor = NULL;
		c->mem_tail->successor = mem;
	}
	c->mem_tail = mem;
endatomic
	return (mem);
}

/* TODO: Don't forget to clear every freed memory unit and block */

memunit* _ksmem_unit_find_free (client* c, uvar32_64 size) {
	memunit *mptr;

	isclientn (c);
	isvalidn (size, c, KSERR_MEMZEROSIZE);

	for (mptr = c->mem_tail; mptr; mptr = mptr->precessor)
		if (mptr->maxfb >= size)
			return (mptr);
	return (NULL);
}

memunit* _ksmem_unit_find_used (client* c, void* ptr) {
	memunit* mptr;

	isclientf (c);
	notnullf (ptr, c);

	for (mptr = c->mem_tail; mptr; mptr = mptr->precessor)
		if (ptr >= (void *)mptr->mpage &&
		    ptr < (void *)(mptr->mpage + mptr->psize))
			return (mptr);
	kserr_fire (c, KSERR_NOTKSMEM, "_ksmem_unit_find_used: memory release failed", __FILE__, __LINE__);
	return (false);
}

bool _ksmem_unit_free (client* c, memunit* unit) {
	isclientf (c);
	ismemunitf (unit, c);
	isownerf (c, unit);
//	iffailedf (unit->pfree != unit->psize, c, "_ksmem_unit_free");

	bdlist_remove (unit);
	if (mem_tail == unit)
		mem_tail = unit->prev;
	if (c->mem_tail == unit)
		c->mem_tail = c->mem_tail->precessor;
	if (unit->precessor) unit->precessor->successor = unit->successor;
	if (unit->successor) unit->successor->precessor = unit->precessor;
	if (conf_mem_pages_maxfree < bdlist_size (mem_free)) {
		_ksmem_unit_init (c, unit);
		mem_free->prev = unit;
		unit->next = mem_free;
		mem_free = unit;
	} else
		free (unit);

	return (true);
}

void* _ksmem_mbat_alloc (client* c, memunit* unit, uvar32_64 size) {
	struct timeb tb;
	svar32_64 no, pf, pu, nf, nu, tmp; /* prevoius and next free and used block numbers corresopndevly */
	isclientn (c);

	ismemunitn (unit, c);
	isvalidn (size, c, KSERR_MEMZEROSIZE);
	isownern (c, unit);

	/* no = index of free and suitable unit->mbat[] entry */
	ifn ((no = _ksmem_mbat_find_free (c, unit, size)) == -1);
	_ksmem_mbat_neighboors (unit, no, unit->ffb, unit->lfb, &pf, &nf);
	_ksmem_mbat_neighboors (unit, no, unit->fub, unit->lub, &pu, &nu);

	/* Allocated new block */
atomic(unit)
	if (unit->maxfb == unit->mbat[no].size) {
		unit->maxfb = 0;
		for (tmp = unit->ffb; tmp != -1; tmp = unit->mbat[tmp].next)
			if (unit->maxfb < unit->mbat[tmp].size)
				unit->maxfb = unit->mbat[tmp].size;
		if (unit->maxfb < unit->psize - unit->pbottom)
			unit->maxfb = unit->psize - unit->pbottom;
	}
	if (unit->minfb == unit->mbat[no].size) {
		unit->minfb = (uvar32_64)-1;
		for (tmp = unit->ffb; tmp != -1; tmp = unit->mbat[tmp].next)
			if (unit->minfb > unit->mbat[tmp].size)
				unit->minfb = unit->mbat[tmp].size;
		if (unit->minfb > unit->psize - unit->pbottom)
			unit->minfb = unit->psize - unit->pbottom;
	}
	if (no == unit->ffb)
		unit->ffb = unit->mbat[no].next;

	if (no == (svar32_64)unit->tbottom) {
		unit->mbat[no].offset = unit->pbottom;
		unit->mbat[no].size = size;
		unit->pbottom += size;
		unit->tbottom++;
		unit->tfree--;
	} else if (unit->mbat[no].size != size && unit->tbottom < unit->tsize) {
		unit->mbat[unit->tbottom].size = unit->mbat[no].size - size;
		unit->mbat[unit->tbottom].offset = unit->mbat[no].offset + size;
		unit->mbat[unit->tbottom].next = -1;
		unit->mbat[no].size = size;
		if (unit->mbat[unit->tbottom].size < unit->minfb)
			unit->minfb = unit->mbat[unit->tbottom].size;
		if (unit->mbat[unit->tbottom].size > unit->maxfb)
			unit->maxfb = unit->mbat[unit->tbottom].size;
		if (unit->lfb != -1) {
			unit->mbat[unit->lfb].next = unit->tbottom;
			unit->lfb = unit->tbottom;
		} else
			unit->lfb = unit->tbottom;
		if (unit->ffb == -1)
			unit->ffb = unit->tbottom;
		if (nf == -1)
			nf = unit->tbottom;
		unit->tbottom++;
	}
	if (unit->lfb == no)
		unit->lfb = pf;
	if (unit->fub > no || unit->fub == -1)
		unit->fub = no;
	if (unit->lub < no || unit->lub == -1)
		unit->lub = no;

	if (pu != -1)
		unit->mbat[pu].next = no;
	if (pf != -1)
		unit->mbat[pf].next = nf;
	unit->mbat[no].next = nu;

	ftime (&tb);
	unit->modified = tb.time;
	unit->pfree -= size;
	unit->maxfb -= size;
	unit->minfb = (unit->minfb < unit->maxfb) ? unit->minfb : unit->maxfb;
endatomic

	return ((void *)((uvar32_64)(unit->mpage) + unit->mbat[no].offset));
}

bool _ksmem_mbat_free (client* c, memunit* unit, void* ptr) {
	uvar32_64 size, no;
	svar32_64 pu, pf, nu, nf;
	struct timeb tb;

	isclientf (c);
	ismemunitf (unit, c);
	isownerf (c, unit);

	isvalid (unit->tbottom != 0, c, KSERR_MEMINT) fail
		return (false);
	otherwise;
	isvalidf ((no = _ksmem_mbat_find_used (c, unit, ptr)) != -1,
	           c, KSERR_MEMINT);
	_ksmem_mbat_neighboors (unit, no, unit->ffb, unit->lfb, &pf, &nf);
	_ksmem_mbat_neighboors (unit, no, unit->fub, unit->lub, &pu, &nu);

	size = unit->mbat[no].size;
atomic(unit);
atomic(c)
	c->mem_used -= size;
	c->mem_quot += size;
endatomic
	unit->pfree += size;
	memset (unit->mpage + unit->mbat[no].offset, 0, size);
	if (unit->fub == (svar32_64)no)
		unit->fub = unit->mbat[no].next;
	if (unit->lub == (svar32_64)no)
		unit->lub = pu;

	if (unit->tbottom != no + 1) {
		unit->mbat[no].next = nf;
		if (pf != -1)
			unit->mbat[pf].next = no;
		if (pu != -1)
			unit->mbat[pu].next = nu;
		if (unit->lfb < (svar32_64)no || unit->lfb == -1)
			unit->lfb = no;
		if (unit->ffb > (svar32_64)no || unit->ffb == -1)
			unit->ffb = no;
	} else {
		if (unit->pbottom == unit->mbat[no].offset + size)
			unit->pbottom -= size;
		size = unit->psize - unit->pbottom;
		if (pu != -1)
			unit->mbat[pu].next = -1;
		unit->mbat[no].size = 0;
		unit->mbat[no].offset = 0;
		unit->mbat[no].next = 0;
		unit->tbottom--;
	}
	if (unit->maxfb < size)
		unit->maxfb = size;
	if (unit->minfb > size)
		unit->minfb = size;
	unit->tfree++;
endatomic(unit);

	ftime (&tb);
	unit->modified = tb.time;
	return (true);
}

svar32_64 _ksmem_mbat_find_free (client* c, memunit* unit, uvar32_64 size) {
	uvar32_64 no = 0, curr_no = -1, curr_size = 0;

	isclientm (c);
	ismemunitm (unit, c);
	isvalidm (size, c, KSERR_MEMZEROSIZE);

	for (curr_no = no = unit->ffb; no != -1; no = unit->mbat[no].next)
		if (unit->mbat[no].size >= size) {
			curr_size = unit->mbat[no].size;
			curr_no = no;
			if (unit->mbat[no].size == size)
				break;
		}
	if (curr_size == 0) {
		isvalidm (no >= unit->tsize, c, KSERR_MEMINT);
		isvalid (unit->psize - unit->pbottom >= size, c, KSERR_MEMINT) fail
			return (-1);
		otherwise;
		curr_no = unit->tbottom;
	}
	return (curr_no);
}

svar32_64 _ksmem_mbat_find_used (client* c, memunit* unit, void* ptr) {
	uvar32_64 no = 0, curr_no = -1;

	isclientm (c);
	ismemunitm (unit, c);
	notnullm (ptr, c);

	isvalidm (unit->fub != -1, c, KSERR_MEMINT);
	for (no = unit->fub; no != -1; no = unit->mbat[no].next)
		if (unit->mbat[no].offset == (uvar32_64)ptr - (uvar32_64)(unit->mpage)) {
			curr_no = no;
			break;
		}
	if (curr_no == -1)
		for (no = unit->ffb; no != -1; no = unit->mbat[no].next)
			iffailedm (unit->mbat[no].offset != (uvar32_64)ptr - (uvar32_64)(unit->mpage), c, "memory is already released");
	return (curr_no);
}

void _ksmem_mbat_neighboors (memunit* unit, svar32_64 no,
			     svar32_64 start, svar32_64 end,
                             svar32_64* prev, svar32_64* next) {
	svar32_64 tmp;
	*prev = *next = -1;

	if (start != -1 && start != no)
		for (tmp = (end < no && end != -1) ? end : start;
		     tmp != -1 && tmp < no;
		     tmp = unit->mbat[tmp].next)
			*prev = tmp;

	if (end != -1 && end != no) {
		if (*prev != -1) {
			for (*next = *prev;
			     *next != -1 && *next <= no;
			     *next = unit->mbat[*next].next)
				{ ; }
			if (*next == no)
				*next = unit->mbat[*next].next;
		} else
			*next = start;
	}
}

void* ksmem_commit (client* c, uvar32_64 size) {
	void *ptr;
	memunit *mptr;

	isclientn (c);
	isvalidn (size, c, KSERR_MEMZEROSIZE);
	isvalidn (c->mem_used + size < c->mem_quot, c, KSERR_QUOTAEXC);

	if (mem_mgr == 0) {
		isvalidn (ptr = malloc (size), c, KSERR_MEMINT);
		memset (ptr, 0, size);
		return (ptr);
	}

/* TODO: Replace with effective dealing with large memory blocks */
	isvalidn (size <= KS_MPAGE_SIZE, c, KSERR_MEMTOOLARGE);

	mptr = c->mem_tail;
	if (!mptr || !(mptr = _ksmem_unit_find_free (c, size)))
		iffailedn (mptr = _ksmem_unit_alloc (c), c, "failed to commit memory");
	isvalidn (ptr = _ksmem_mbat_alloc (c, mptr, size), c, KSERR_MEMINT);
atomic(c)
	c->mem_used += size;
	c->mem_quot -= size;
endatomic

#if defined _DEBUG
	_ksmem_dump (mptr);
#endif

	return (ptr);
}

void* ksmem_resize (client* c, void* ptr, address_t new_size) {
	/* TODO: implement ksmem_resize */
	return (NULL);
}

bool ksmem_release (client* c, void* ptr) {
	memunit* mptr;

	isclientf (c);
	notnullf (ptr, c);

	if (mem_mgr == 0) {
		free (ptr);
		return (true);
	}

	iffailedf (mptr = _ksmem_unit_find_used (c, ptr), c, "Failed to release memory");
	iffailedf (_ksmem_mbat_free (c, mptr, ptr), c, "Failed to release memory");
#if defined _DEBUG
	_ksmem_dump (mptr);
#endif
	if (mptr->fub == -1 && mptr->lub == -1)
		_ksmem_unit_free (c, mptr);
	return (true);
}

bool ksmem_release_all (client* c) {
	memunit *mem, *next;

	isclientf (c);

	if (mem_mgr == 0)
		return (true);

	for (mem = c->mem_tail; mem && mem->precessor; mem = mem->precessor) ;
	for (; mem; mem = next) {
		next = mem->successor;
		_ksmem_unit_free (c, mem);
	}
	c->mem_tail = NULL;

	return (true);
}

int ksmem_configure (client* c, int cmd, svar32_64 param) {
	int old;
	isclientf (c);
	switch (cmd) {
		case KSCFG_MEM_MGR:
			old = mem_mgr;
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "ksmem_configure", __FILE__, __LINE__);
			return (old);
		case KSCFG_MEM_MPAGE:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "ksmem_configure", __FILE__, __LINE__);
			return (mem_mpage);
		case KSCFG_MEM_MBAT:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "ksmem_configure", __FILE__, __LINE__);
			return (mem_mbat);
		default:
			kserr_fire (c, KSERR_UNKNOWNCMD, "ksmem_configure", __FILE__, __LINE__);
			return (0);
	}
}

int ksmem_test (client* c, int test, svar32_64 param) {
	clock_t start, finish;
	uvar32_64 i, cycle;
	void** memcycles;
	isclientf (c);
	switch (test) {
		case KSTEST_MEM_MPAGES:
			notnullm (memcycles = (void **)ksmem_commit(c, (uvar32_64)param * sizeof (void*)), c);
			start = clock ();
			for (cycle = 0; cycle < (uvar32_64)param; cycle++)
				notnullm (memcycles[cycle] = ksmem_commit (c, mem_mpage), c);
			for (cycle = 0; cycle < (uvar32_64)param; cycle++)
				ksmem_release (c, memcycles[cycle]);
			finish = clock ();
			return (finish - start);
		case KSTEST_MEM_HASH:
			notnullm (memcycles = (void **)ksmem_commit(c, (uvar32_64)param * sizeof (void*)), c);
			start = clock ();
			for (cycle = 0; cycle < (uvar32_64)param; cycle++)
				notnullm (memcycles[cycle] = ksmem_commit (c, rand () * 768 * 4 / RAND_MAX * 512 + 10), c);
			for (cycle = 0; cycle < (uvar32_64)param; cycle++)
				ksmem_release (c, memcycles[cycle]);
			finish = clock ();
			return (finish - start);
			return (0);
		default:
			kserr_fire (c, KSERR_UNKNOWNCMD, "ksmem_test", __FILE__, __LINE__);
			return (0);
	}
}
