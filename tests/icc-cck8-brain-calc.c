#include <sysconf.h>
#include <winsock2.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../code/dias-ks/dias.h"
#include "../../code/aimkstp/aimkstp.h"

packet query, resp;

socket_t ks_socket, s;
struct sockaddr_in ks_inetserv;
void conn () {
	if ((ks_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
		exit (1);
	if ((s = connect (ks_socket, (struct sockaddr*)&ks_inetserv, sizeof (struct sockaddr_in))) == -1)
		exit (1);
}

void cmd (int cmd, char* form, ...) {
	int n = 0, i;
	va_list al;
	va_start (al, form);
	kstp_allocate (&query, strlen (form));
	query.cmd = cmd;
	while (*form) {
		if (*form == 'i') {
			i = va_arg(al, int);
			kstp_set_int (&query, n, i);
		} else if (*form == 'r')
			kstp_set_real (&query, n, va_arg(al, double));
		else if (*form == 's')
			kstp_set_string (&query, n, strsafe_fromstr(va_arg(al, char*), 1024));
		else if (*form == 'S')
			kstp_set_string (&query, n, (string)va_arg(al, char*));
		form++;
		n++;
	}
	va_end (al);
	conn ();
	kstp_send (ks_socket, &query);
	kstp_recv (ks_socket, &resp);
	if (resp.cmd != 0)
		printf (" <ERROR %08Xh> ", resp.cmd);
	closesocket (ks_socket);
}

void genmtx (sint sz, sdword *ptr) {
	sint x,y;
	for (x = -sz; x <= sz; ++x)
		for (y = -sz; y <= sz; ++y) {
			*ptr++ = x;
			*ptr++ = y;
		}
}

sdword mtx_4x4[41*41*2];

sdword mtx_oct[] = {
	       -1,-2,  0,-2, +1,-2,
	-2,-1, -1,-1,  0,-1, +1,-1, +2,-1,
	-2, 0, -1, 0,        +1, 0, +2, 0,
	-2,+1, -1,+1,  0,+1, +1,+1, +2,+1,
	       -1,+2,  0,+2, +1,+2,
};


sdword mtx_round[] = {
	              -1,-3,  0,-3, +1,-3,

	       -2,-2, -1,-2,  0,-2, +1,-2, +2,-2,

	-3,-1, -2,-1, -1,-1,  0,-1, +1,-1, +2,-1, +3,-1,

	-3, 0, -2, 0, -1, 0,        +1, 0, +2, 0, +3, 0,

	-3,+1, -2,+1, -1,+1,  0,+1, +1,+1, +2,+1, +3,+1,

	       -2,+2, -1,+2,  0,+2, +1,+2, +2,+2,

	              -1,+3,  0,+3, +1,+3,
};

rtbyte mtx_gauss[] = {
	0.0035,	0.0123,	0.0210,	0.0123,	0.0035,
	0.0123,	0.0543,	0.0911,	0.0543,	0.0123,
	0.0210,	0.0911,	0.2224,	0.0911,	0.0210,
	0.0123,	0.0543,	0.0911,	0.0543,	0.0123,
	0.0035,	0.0123,	0.0210,	0.0123,	0.0035,
};

char *file[] = {
	"dmn/dmn37_b",
	NULL
};

char storage_src[] = "q:/aim/dias/src/srcseq";
char storage_loc[] = "N:/Images/Hypothalamus.Rat/Cholecystokinin-8.ICC/hlth-colh-icac.step1";
char mask_obj[] = "q:/aim/dias/src/objects";
char mask_bg[] = "q:/aim/dias/src/background";
char survey_obj[] = "q:/aim/dias/src/objects.data";
char survey_bg[] = "q:/aim/dias/src/background.data";

int stor_src;
int img_srcs[4], msk_obj, msk_bg, sur_obj, sur_bg;

int main () {
	int i, im, of, count;
	rtbyte mean, var;
	char objname[0x100], filename[0x100], *file_curr;

	WSADATA wsdata;
	WSAStartup(0x0101,&wsdata);

	printf ("DIAS-KS console\n\n");
	ks_inetserv.sin_family = AF_INET;
	ks_inetserv.sin_port = htons (5634);
	ks_inetserv.sin_addr.s_addr = inet_addr ("127.0.0.1");
	memset (&(ks_inetserv.sin_zero), 0, 8);

/*
 ********************************** PROGRAM
 */

									printf ("\nInitializing");
// Initialization
									printf ("\n\tlogin ");
cmd(	KSCMD_HELLO,	"ss",	"maxim",
				"escherichia");
									printf ("\n\tcleanup ");
cmd(	KSCMD_DELETE,	"s",	storage_src);

	for (i = 0; file[i]; i++) {
		file_curr = file[i];
									printf ("\n\nProcessing sequence '%s'", file_curr);
									sprintf (filename, "%s/%s.img", storage_loc, file_curr);
									printf ("\n\tloading");
cmd(	 KSCMD_LOAD,	"iissi",			// Loading source storage
				DOBJT_STORAGE,
				DOBJF_STORAGE_KONTRON,
				filename,
				storage_src,
				0777);
cmd(	 KSCMD_CREATE,	"isi",
				DOBJT_SURVEY,
				survey_obj,
				0777);
cmd(	 KSCMD_OPEN,	"sii",	survey_obj,
				0777,
				0777);					kstp_get_int (&resp, 0, &sur_obj);

cmd(	 KSCMD_CREATE,	"isi",
				DOBJT_SURVEY,
				survey_bg,
				0777);
cmd(	 KSCMD_OPEN,	"sii",	survey_bg,
				0777,
				0777);					kstp_get_int (&resp, 0, &sur_bg);

cmd(	 KSCMD_OPEN,	"sii",	storage_src,		// Opening storage with source images
				0777,
				0777);					kstp_get_int (&resp, 0, &stor_src);
cmd(	 DCMD_OBJINFO_STORAGE, "i", stor_src);				kstp_get_int (&resp, 3, &count);

									printf ("\n");
	 for (im = 0; im < count; im += 2) {
									printf ("\t                                                                 \r");
									printf ("\tprocessing image pack #%03d: \n", im/2+1);

	  for (of = 0; of < 2; ++of) {
									sprintf (objname, "%s-img%d", storage_src, im + of);
cmd(	   KSCMD_OPEN,	"sii",	objname,		// Opening source image pack
				0777,
				0777);					kstp_get_int (&resp, 0, img_srcs + of);
	  }

cmd(	  DCMD_IMG_TRC_WINDOW, "iiiiiii",
				20, 20, 20, 20,
				true, true,
				img_srcs[1]);
cmd(	  DCMD_IMG_TRC_IDENT, "ii",
				true,
				img_srcs[1]);
									sprintf (objname, "%s-%d", mask_obj, im/2+1);
cmd(	  KSCMD_CREATE,	"isiii",
				DOBJT_MASK,
				objname,
				0777,
				img_srcs[1], 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,
				0777,
				0777);					kstp_get_int (&resp, 0, &msk_obj);

cmd(	  DCMD_MEASURE, "iiii",	msk_obj,
				sur_obj,
				img_srcs[0],
				0);

cmd(	  KSCMD_SAVE, "i",	sur_obj);

cmd(	  KSCMD_CLOSE, "i",	msk_obj);
cmd(	  KSCMD_LINK,	"is",	img_srcs[0],
				objname);

cmd(	  DCMD_IMG_STAT, "i",	img_srcs[0]);				kstp_get_real (&resp, 0, &mean);
									kstp_get_real (&resp, 1, &var);
cmd(	  DCMD_IMG_PXW_THR, "iiiii",
				0x00,
				(ulong)(mean + var) - 1,
				0xFF,
				img_srcs[1],
				img_srcs[0]);
									sprintf (objname, "%s-%d", mask_bg, im/2+1);
cmd(	  KSCMD_CREATE,	"isiii",
				DOBJT_MASK,
				objname,
				0777,
				img_srcs[1], 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,
				0777,
				0777);					kstp_get_int (&resp, 0, &msk_bg);

cmd(	  DCMD_MEASURE, "iiii",	msk_bg,
				sur_bg,
				img_srcs[0],
				0);

cmd(	  KSCMD_SAVE, "i",	sur_bg);

cmd(	  KSCMD_CLOSE, "i",	msk_bg);
cmd(	  KSCMD_LINK,	"is",	img_srcs[0],
				objname);

	  for (of = 0; of < 2; ++of)
cmd(	   KSCMD_CLOSE, "i",	img_srcs[of]);
	 }

cmd(	 KSCMD_CLOSE, "i",	sur_obj);
cmd(	 KSCMD_CLOSE, "i",	sur_bg);

cmd(	 KSCMD_LINK,	"is",	stor_src, 
				survey_obj);
cmd(	 KSCMD_LINK,	"is",	stor_src, 
				survey_bg);

cmd(	 KSCMD_CLOSE,	"i",	stor_src);

									sprintf (filename, "%s/%s.obj.tab", storage_loc, file_curr);
									remove (filename);
cmd(	 KSCMD_EXTRACT, "iss",	DOBJF_SURVEY_TAB,
				filename,
				survey_obj);
									sprintf (filename, "%s/%s.bg.tab", storage_loc, file_curr);
									remove (filename);
cmd(	 KSCMD_EXTRACT, "iss",	DOBJF_SURVEY_TAB,
				filename,
				survey_bg);
									sprintf (filename, "%s/%s.new.img", storage_loc, file_curr);
									remove (filename);
cmd(	 KSCMD_DELETE,	"s",	storage_src);
	}

cmd(	KSCMD_BYE,	""	);
									printf ("\nDONE");

	getch ();

	return (0);
}
