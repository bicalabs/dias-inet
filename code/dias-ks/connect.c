/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service connactions manager subsystem                           *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Revised 28.07.2004 Maxim A. Orlovsky: added automatic generation of         *
*                     connect.cm file from xml source, revrited function       *
*                     lookup algorithm                                         *
*                                                                              *
*******************************************************************************/

#include <time.h>
#include <winsock2.h>

#include <sysconf.h>
#include <libnodes.h>
#include <strsafex.h>
#include "dias-ks.h"
#include "connect.h"

typedef bool (*diascm_proc)(client* c, packet* query, packet* resp);
typedef struct {
	udword		start;
	udword		end;
	diascm_proc*	proc_table;
} diascm_lookup;

#include "cmdtabl.cm"

#define ARGS_ALLOC(q,n)		{isvalidf (kstp_allocate (q, n), c, kstp_errno);}
#define ARG_GET_STRING(n,s)	{isvalidf (kstp_get_string(query, n, &s), c, kstp_errno);}
#define ARG_GET_REAL(n,s)	{isvalidf (kstp_get_real(query, n, &s), c, kstp_errno);}
#define ARG_GET_INT(n,s)	{isvalidf (kstp_get_int(query, n, &s), c, kstp_errno);}
#define ARG_SET_STRING(n,s)	{isvalidf (kstp_set_string(resp, n, s), c, kstp_errno);}
#define ARG_SET_REAL(n,s)	{isvalidf (kstp_set_real(resp, n, s), c, kstp_errno);}
#define ARG_SET_INT(n,s)	{isvalidf (kstp_set_int(resp, n, s), c, kstp_errno);}

bool cm_proc (client* c, packet* query, packet* resp) {
	uvar32_64 i, j;
	svar32_64 int1, int2, int3;
	string str1;
	names* nm;
	dksobj_storage* stor;
	dksobj_image* img;

	isclientf (c);
	notnullf (query, c);
	notnullf (resp, c);

	switch (query->cmd) {
		case DCMD_OBJINFO_STORAGE:
			ARG_GET_INT (0, int1);
			iffailedf (kssec_validate_hobj (c, (HOBJ)int1, OA_READ), c, "cm_proc");
			isvalidf (((HOBJ)int1)->obj->type == DOBJT_STORAGE, c, KSERR_ARGCOUNT);
			ARGS_ALLOC (resp, 5);
			stor = (dksobj_storage*)((HOBJ)int1)->obj->data;
			ARG_SET_INT (0, stor->dimx);
			ARG_SET_INT (1, stor->dimy);
			ARG_SET_INT (2, stor->dimz);
			ARG_SET_INT (3, stor->img_count);
			ARG_SET_INT (4, stor->svr_count);
			return (true);
		case DCMD_OBJINFO_IMAGE:
			ARG_GET_INT (0, int1);
			iffailedf (kssec_validate_hobj (c, (HOBJ)int1, OA_READ), c, "cm_proc");
			isvalidf (((HOBJ)int1)->obj->type == DOBJT_IMAGE, c, KSERR_ARGCOUNT);
			ARGS_ALLOC (resp, 8);
			img = (dksobj_image*)((HOBJ)int1)->obj->data;
			ARG_SET_INT (0, img->dimx);
			ARG_SET_INT (1, img->dimy);
			ARG_SET_INT (2, img->dimz);
			ARG_SET_INT (3, img->msk_count);
			ARG_SET_INT (4, img->mrk_count);
			ARG_SET_INT (5, img->ovl_count);
			ARG_SET_INT (6, img->lbl_count);
			ARG_SET_INT (7, img->mtr_count);
			return (true);
		case DCMD_OBJGET_STORAGE:
			ARG_GET_INT (0, int1);
			iffailedf (kssec_validate_hobj (c, (HOBJ)int1, OA_READ), c, "cm_proc");
			isvalidf (((HOBJ)int1)->obj->type == DOBJT_STORAGE, c, KSERR_ARGCOUNT);
			stor = (dksobj_storage*)((HOBJ)int1)->obj->data;
			ARGS_ALLOC (resp, 1 + stor->img_count + stor->svr_count);
			ARG_SET_STRING (0, str1 = strsafe_frombin (((HOBJ)int1)->obj->data, sizeof(dksobj_storage)));
			strsafe_free (str1);
			for (i = 0, nm = bdlist_head (stor->img_head); i < stor->img_count && nm; ++i, nm = nm->next) {
				ARG_SET_STRING (1 + i, str1 = strsafe_frombin (nm->name, strlen(nm->name)));
				strsafe_free (str1);
			}
			for (j = 0, nm = bdlist_head (stor->svr_head); j < stor->svr_count && nm; ++j, ++i, nm = nm->next) {
				ARG_SET_STRING (1 + i, str1 = strsafe_frombin (nm->name, strlen(nm->name)));
				strsafe_free (str1);
			}
			return (true);
		case DCMD_OBJGET_IMAGE:
			ARG_GET_INT (0, int1);
			iffailedf (kssec_validate_hobj (c, (HOBJ)int1, OA_READ), c, "cm_proc");
			isvalidf (((HOBJ)int1)->obj->type == DOBJT_IMAGE, c, KSERR_ARGCOUNT);
			img = (dksobj_image*)((HOBJ)int1)->obj->data;
			ARGS_ALLOC (resp, 1 + img->dimz);
			ARG_SET_STRING (0, str1 = strsafe_frombin (((HOBJ)int1)->obj->data, sizeof(dksobj_image)));
			strsafe_free (str1);
			for (i = 0; i < img->dimz; ++i) {
				ARG_SET_STRING (1 + i, str1 = strsafe_frombin (img->layers[i], img->dimx * img->dimy));
				strsafe_free (str1);
			}
			return (true);
		default:
			ARGS_ALLOC (resp, 0);
			for (i = 0; _cm_lookup_table[i].proc_table; ++i)
				if (query->cmd >= _cm_lookup_table[i].start && query->cmd <=  _cm_lookup_table[i].end)
					return (_cm_lookup_table[i].proc_table[query->cmd - _cm_lookup_table[i].start] (c, query, resp));
			isvalidf (true, c, KSERR_UNKNOWNCMD);
//			isvalidf (query->cmd - DCMD >= 0, c, KSERR_UNKNOWNCMD);
//			isvalidf (_cm_proc_table[query->cmd - DCMD], c, KSERR_NOTIMPLEMENTED);
//			return (_cm_proc_table[query->cmd - DCMD] (c, query, resp));
	}
	return (false);
}
