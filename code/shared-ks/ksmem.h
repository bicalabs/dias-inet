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
*          09.01.2003 Rewrited ksmem_st (Orlovsky)                             *
*                                                                              *
*******************************************************************************/

#define	KSMEM_STAMP	0xAAAAAAAA	/* = 10101010b */

#define ismemunit(u,c)	if ((u)->stamp1 != KSMEM_STAMP || (u)->stamp2 != KSMEM_STAMP) { kserr_fire (c, KSERR_MEMSTAMP1, "##u", __FILE__, __LINE__);
#define	ismemunitn(u,c)	ismemunit(u,c) return (NULL); }
#define	ismemunitf(u,c)	ismemunit(u,c) return (false); }
#define	ismemunitm(u,c)	ismemunit(u,c) return (-1); }

extern bool mem_mgr;
extern uvar32_64 mem_mpage;
extern uvar32_64 mem_mbat;

struct memunit_st;
typedef struct memunit_st memunit;

struct memunit_st {
	memunit*	prev;			/* Next allocated ksmem_st */
	memunit*	next;			/* Prev allocated ksmem_st */

	client*		owner;			/* Client, owning this ksmem_st */
	memunit*	successor;		/* Next allocated ksmem_st for the same owner */
	memunit*	precessor;		/* Prev allocated ksmem_st for the same owner */
	time_t		created;		/* time of memory allocation */
	time_t		modified;		/* last access (=modification) time */
	uvar32_64	psize;			/* = KS_MPAGE_SIZE */
	uvar32_64	pfree;			/* no of free bytes in mpage */
	uvar32_64	pbottom;		/* mpage data bottom */
	uvar32_64	tsize;			/* = KS_MBAT_SIZE */
	uvar32_64	tfree;			/* no of free entries is mbat */
	uvar32_64	tbottom;		/* mpage data bottom */
	uvar32_64	minfb;			/* size of shortest free block */
	uvar32_64	maxfb;			/* size of longest free block */
	svar32_64	ffb;			/* offset of first free block in mbat */
	svar32_64	lfb;			/* offset of last free block in mbat */
	svar32_64	fub;			/* offset of first used block in mbat */
	svar32_64	lub;			/* offset of last used block in mbat */
	struct {
		uvar32_64	offset;
		uvar32_64	size;
		svar32_64	next;
	}		mbat[KS_MBAT_SIZE];
	uvar32_64	stamp1;			/* stamp ensuring that mbat was not overflowed */
	ubyte		mpage[KS_MPAGE_SIZE];	/* memory page */
	uvar32_64	stamp2;			/* stamp ensuring that mbat was not overflowed */
};

bool		_ksmem_unit_init (client* c, memunit* unit);
memunit*	_ksmem_unit_alloc (client* c);
bool		_ksmem_unit_free (client* c, memunit* unit);
memunit*	_ksmem_unit_find_free (client* c, uvar32_64 size);
memunit*	_ksmem_unit_find_used (client* c, void* ptr);

void*		_ksmem_mbat_alloc (client* c, memunit* unit, uvar32_64 size);
bool		_ksmem_mbat_free (client* c, memunit* unit, void* ptr);
svar32_64	_ksmem_mbat_find_free (client* c, memunit* unit, uvar32_64 size);
svar32_64	_ksmem_mbat_find_used (client* c, memunit* unit, void* ptr);
void		_ksmem_mbat_neighboors (memunit* unit, svar32_64 no,
			                svar32_64 start, svar32_64 end,
                                        svar32_64* prev, svar32_64* next);
