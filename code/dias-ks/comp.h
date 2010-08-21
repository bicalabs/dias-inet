/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service computations engine subsystem                           *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

#define	GET_OBJECT(n,o,hobj,ob,tp,pm)	\
	isvalidf (kstp_get_int (query, n, &o), c, kstp_errno); \
	hobj = (HOBJ)o; \
	isvalidf (kssec_validate_hobj (c, hobj, pm), c, KSERR_ARGTYPE); \
	notnullf (hobj->obj, c); \
	isvalidf (hobj->obj->type == tp, c, KSERR_ARGTYPE); \
	notnullf (ob = hobj->obj->data, c);

#define	HISTORY_ADD(obj,cmd,descr,c,clk,tm)	\
	histx = obj->hist_finish; \
	if (obj->hist_finish >= DIASKS_HISTORY_LENGTH) obj->hist_finish = 0; \
	if (obj->hist_start == obj->hist_finish) obj->hist_start = obj->hist_finish + 1; \
	if (obj->hist_start >= DIASKS_HISTORY_LENGTH) obj->hist_start = 0; \
	obj->hist_cont[histx].code = cmd; \
	obj->hist_cont[histx].performed = tm; \
	obj->hist_cont[histx].elapsed = clk; \
	if (strlen (descr) >= DIASKS_HISTTITLE_LENGTH) { \
		memcpy (obj->hist_cont[histx].title, descr, DIASKS_HISTTITLE_LENGTH - 1); \
		obj->hist_cont[histx].title[DIASKS_HISTTITLE_LENGTH - 1] = '\0'; \
	} else \
		strcpy (obj->hist_cont[histx].title, descr); \
	if (strlen (c->ip) >= DIASKS_HISTUSER_LENGTH) { \
		memcpy (obj->hist_cont[histx].user, c->ip, DIASKS_HISTUSER_LENGTH - 1); \
		obj->hist_cont[histx].user[DIASKS_HISTUSER_LENGTH - 1] = '\0'; \
	} else \
		strcpy (obj->hist_cont[histx].user, c->ip);


void _ce_measure (dksobj_mask* mask, uvar32_64 objno, ubyte* image, record* rec);


#define	INIT_1IMGS(args,name)	\
	HOBJ s; \
	ubyte *psrc; \
	uvar32_64 size = 0; \
	uvar32_64 dimx, dimy, dimz; \
	dksobj_image *src; \
 \
	isclientf (c); \
	notnullf (query, c); \
	iffailedf (query->argc == 1 + args, c, #name ": wrong arguments count"); \
	isvalidf (kstp_get_int (query, args + 0, &(int)s), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, s, OA_READ), c, \
	           #name ": unable to access source image"); \
	notnullf (s->obj, c); \
	notnullf (src = s->obj->data, c); \
 \
	dimx = src->dimx; \
	dimy = src->dimy; \
	dimz = src->dimz; \
	size = dimx * dimy; \

#define	INIT_2IMGS(args,name)	\
	HOBJ s, d; \
	ubyte *pdst, *psrc; \
	uvar32_64 size = 0; \
	uvar32_64 dimx, dimy, dimz; \
	dksobj_image *src, *dst; \
 \
	isclientf (c); \
	notnullf (query, c); \
	iffailedf (query->argc == 2 + args, c, #name ": wrong arguments count"); \
	isvalidf (kstp_get_int (query, args + 0, &(int)d), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, d, OA_RDWR), c, \
	           #name ": unable to access destination image"); \
	notnullf (d->obj, c); \
	notnullf (dst = d->obj->data, c); \
	isvalidf (kstp_get_int (query, args + 1, &(int)s), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, s, OA_READ), c, \
	           #name ": unable to access source image"); \
	notnullf (s->obj, c); \
	notnullf (src = s->obj->data, c); \
	iffailedf (src->dimx == dst->dimx && \
		   src->dimy == dst->dimy && \
		   src->dimz == dst->dimz, c, \
		   #name ": different images dimensions"); \
 \
	dimx = dst->dimx; \
	dimy = dst->dimy; \
	dimz = dst->dimz; \
	size = dimx * dimy;

#define	INIT_3IMGS(args,name)	\
	HOBJ s1, s2, d; \
	ubyte *pdst, *psrc1, *psrc2; \
	uvar32_64 size = 0; \
	dksobj_image *src1, *src2, *dst; \
 \
	isclientf (c); \
	notnullf (query, c); \
	iffailedf (query->argc == 3 + args, c, #name ": wrong arguments count"); \
	isvalidf (kstp_get_int (query, args + 0, &(int)d), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, d, OA_RDWR), c, \
	           #name ": unable to access destination image"); \
	notnullf (d->obj, c); \
	notnullf (dst = d->obj->data, c); \
 \
	isvalidf (kstp_get_int (query, args + 1, &(int)s1), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, s1, OA_READ), c, \
	           #name ": unable to access source image"); \
	notnullf (s1->obj, c); \
	notnullf (src1 = s1->obj->data, c); \
 \
	isvalidf (kstp_get_int (query, args + 2, &(int)s2), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, s2, OA_READ), c, \
	           #name ": unable to access source image"); \
	notnullf (s2->obj, c); \
	notnullf (src2 = s2->obj->data, c); \
 \
	iffailedf (src1->dimx == src2->dimx && \
		   src1->dimy == src2->dimy && \
		   src1->dimz == src2->dimz && \
	           src1->dimx == dst->dimx && \
		   src1->dimy == dst->dimy && \
		   src1->dimz == dst->dimz, c, \
		   #name ": different images dimensions"); \
 \
	size = dst->dimx * dst->dimy;

#define	INIT_XIMGS(args,name)	\
	HOBJ p, d; \
	svar32_64 no; \
	ubyte *pdst, n; \
	uvar32_64 size = 0; \
	dksobj_image **srcs, *dst; \
 \
	isclientf (c); \
	notnullf (query, c); \
	n = (ubyte)query->argc - args - 1; \
 	iffailedf (n >= 2, c, #name ": wrong arguments count"); \
 	isvalidf (kstp_get_int (query, 0 + args, &(int)d), c, kstp_errno); \
	iffailedf (kssec_validate_hobj (c, d, OA_RDWR), c, #name ": unable to access destination image"); \
	notnullf (d->obj, c); \
	notnullf (dst = d->obj->data, c); \
	iffailedf (srcs = ksmem_commit (c, n * sizeof (dksobj_image)), c, #name ": unable to allocate srcs array"); \
 \
	for (no = 0; no < n; ++no) { \
		isvalidj (kstp_get_int (query, no + args + 1, &(int)p), c, kstp_errno, __error); \
		iffailedj (kssec_validate_hobj (c, p, OA_READ), c, \
		           #name ": unable to access one of the source images", __error); \
		notnullj (p->obj, c, __error); \
		notnullj (srcs[no] = p->obj->data, c, __error); \
		iffailedj (srcs[no]->dimx == dst->dimx && \
			   srcs[no]->dimy == dst->dimy && \
			   srcs[no]->dimz == dst->dimz, c, \
			   #name ": different images dimensions", __error); \
	} \
	size = dst->dimx * dst->dimy;

#define	FINALIZE_XIMGS	\
	ksmem_release (c, srcs); \
	return (true); \
__error: \
	ksmem_release (c, srcs); \
	return (false);

/******************************************************************************/
#define	PROC_PWX_1(op1)	\
	for (l = dst->dimz - 1; l >= 0; --l) { \
		i = size - 1; \
		pdst = dst->layers[l]; \
		while (i--) { \
			pdst[i] = srcs[0]->layers[l][i]; \
			for (no = 1; no < n; ++no) { \
				val = (sdword)( (rtbyte)( (uvar32_64)(pdst[i]) op1 (uvar32_64)(srcs[no]->layers[l][i]) ) / divider); \
				if (val < 0x00 && mode == 0) val = 0; \
				else if (val < 0x00 && mode == 1) val &= 0xFF; \
				else if (val > 0xFF && mode == 0) val = 0xFF; \
				else if (val > 0xFF && mode == 1) val &= 0xFF; \
				pdst[i] = (ubyte)(val); \
			} \
		} \
	}

