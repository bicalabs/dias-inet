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
	"hlth.@01.islt.glg.icc",
	"hlth.@02.islt.glg.icc",
	"hlth.@03.islt.glg.icc",
	"hlth-cck4-ic10.@01.islt.glg.icc",
	"hlth-cck4-ic10.@02.islt.glg.icc",
	"hlth-cck4-ip10.@01.islt.glg.icc",
	"hlth-cck4-ip10.@02.islt.glg.icc",
	"hlth-pgm2-ip10.@01.islt.glg.icc",
	"hlth-pgm2-ip10.@02.islt.glg.icc",
	"hlth-pgm4-ip10.@01.islt.glg.icc",
	"hlth-pgm4-ip10.@02.islt.glg.icc",
	"hlth-pgm4-ip10.@03.islt.glg.icc",
	NULL
};

char storage_src[] = "q:/aim/dias/src/srcseq";
char storage_dst[] = "q:/aim/dias/reslt/resltseq";
char storage_loc[] = "N:/Storages/glu";
char image_noise[] = "q:/aim/dias/reslt/noise";

int stor_src, stor_dst;
int img_srcs[4], img_noise,
    img_tmp1, img_tmp2, img_tmp3, img_tmp4,
    img_tissue, img_mask1, img_mask2, img_terminals, img_neurons, img_fibers, img_all;

int main () {
	int i, im, of, count;
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

cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp1);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp2);

	for (i = 0; file[i]; i++) {
		file_curr = file[i];
									printf ("\n\nProcessing sequence '%s'", file_curr);
									sprintf (filename, "%s/%s.img", storage_loc, file_curr);
									printf ("\n\tcleanup");
cmd(	 KSCMD_DELETE,	"s",	storage_dst);
									printf ("\n\tloading");
cmd(	 KSCMD_LOAD,	"iissi",			// Loading source storage
				DOBJT_STORAGE,
				DOBJF_STORAGE_KONTRON,
				filename,
				storage_src,
				0777);
									printf ("\n\tcreating destination sequence");
cmd(	 KSCMD_CREATE,	"isiiii",
				DOBJT_STORAGE,		// Creating destination storage
				storage_dst,
				0777,
				384, 288, 1);
									printf ("\n\topening sequences");
cmd(	 KSCMD_OPEN,	"sii",	storage_src,		// Opening storage with source images
				0777,
				0777);					kstp_get_int (&resp, 0, &stor_src);
cmd(	 KSCMD_OPEN,	"sii",	storage_dst,		// Opening storage for resulting images
				0777,
				0777);					kstp_get_int (&resp, 0, &stor_dst);
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
									sprintf (objname, "%s-%d", image_noise, im/2+1);
cmd(	  KSCMD_CREATE,	"isiiiii",
				DOBJT_IMAGE,		
				objname,
				0777,
				384, 288, 1, 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_noise);

// Removing noise
cmd(	  DCMD_IMG_PXW_MIN, "iii",			// Minimum of four images
				img_tmp1,
				img_srcs[0],
				img_srcs[1]);
cmd(	  DCMD_IMG_MTX_FILTER, "iSii",			// Gaussian filter
				2,			  //- filter size
				strsafe_frombin (mtx_gauss, 5*5*sizeof(rtbyte)),
				img_tmp2,
				img_tmp1);				printf ("\t\tnoise removed\r");
cmd(	  DCMD_IMG_WHL_RESAMPLE, "iii",
				2,
				img_noise,
				img_tmp2);				printf ("\t\timage resampled\r");

	  for (of = 0; of < 2; ++of) {
cmd(	   KSCMD_CLOSE, "i",	img_srcs[of]);
	  }
									printf ("\t\tresult is saved                  \n");

									sprintf (objname, "%s-%d", image_noise, im/2+1);
cmd(	  KSCMD_SAVE,	"i",	img_noise);
cmd(	  KSCMD_CLOSE,	"i",	img_noise);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
	 }

cmd(	 KSCMD_CLOSE,	"i",	stor_src);
cmd(	 KSCMD_SAVE,	"i",	stor_dst);
cmd(	 KSCMD_CLOSE,	"i",	stor_dst);

									sprintf (filename, "%s.step1/%s.img", storage_loc, file_curr);
									remove (filename);
cmd(	 KSCMD_EXTRACT,	"iss",	DOBJF_STORAGE_KONTRON,
				filename,
				storage_dst);
cmd(	 KSCMD_DELETE,	"s",	storage_src);
cmd(	 KSCMD_DELETE,	"s",	storage_dst);
	}

cmd(	KSCMD_CLOSE,	"i",	img_tmp1);
cmd(	KSCMD_CLOSE,	"i",	img_tmp2);
cmd(	KSCMD_BYE,	""	);
									printf ("\nDONE");

	getch ();

	return (0);
}
