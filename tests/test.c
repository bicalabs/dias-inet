#include <sysconf.h>
#include <winsock2.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include "../code/dias-ks/dias.h"
#include "../code/aimkstp/aimkstp.h"

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
		printf (" <ERROR %08Xh> \n", resp.cmd);
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

void open_img (char* name, int im, int* img) {
	char objname[0x100];
									sprintf (objname, "%s-%d", name, im);
cmd(	  KSCMD_CREATE,	"isiiiii",
				DOBJT_IMAGE,		
				objname,
				0777,
				768, 576, 1, 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, img);
}

extern int stor_dst;
void close_img (char* name, int im, int img) {
	char objname[0x100];
									sprintf (objname, "%s-%d", name, im);
cmd(	  KSCMD_SAVE,	"i",	img);
cmd(	  KSCMD_CLOSE,	"i",	img);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
}

/********************************************/

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

char *dirs[] = { "choice", "", "" };

char *file_1[] = {
//"1mth.scn",
//"1mth.scn2",
/*"1mth.scnW",
"1mth.son",
"1mth.son.noise",
"1mth.son.raspliv",
"1mth.son.we",*/
"1mth.son2",
/*"eme1mthstr",
"emx1mthW",
"fem1mth",
"ffson",
"ffson5mth",
"pm2Week",
"son2weekW",
"son5mth",
"son5mthclose",*/
	NULL
};

char *file_2[] = {
	NULL
};
char *file_3[] = {
	NULL
};

char storage_src[] = "c:/var/aim/dias/src/srcseq";
char storage_dst[] = "c:/var/aim/dias/reslt/resltseq";
char storage_loc[] = "n:/prenatal_stress/hthalam.avp.ihc/images.3-vidas.src";
char image_bright[] = "c:/var/aim/dias/reslt/result-bright";
char image_dark[] = "c:/var/aim/dias/reslt/result-dark";
char image_noise[] = "c:/var/aim/dias/reslt/noise";
char image_tissue[] = "c:/var/aim/dias/reslt/tissue";
char image_terminals[] = "c:/var/aim/dias/reslt/term";
char image_neurons[] = "c:/var/aim/dias/reslt/neurons";
char image_fibers[] = "c:/var/aim/dias/reslt/fibers";
char image_mask1[] = "c:/var/aim/dias/reslt/mask1";
char image_mask2[] = "c:/var/aim/dias/reslt/mask2";

int stor_src, stor_dst;
int img_srcs[4], img_noise,
    img_tmp1, img_tmp2, img_tmp3, img_tmp4,
    img_tissue, img_mask1, img_mask2, img_terminals, img_neurons, img_fibers, img_bright, img_dark;

int main () {
	rtbyte mean, var;
	int i, d, im, of, count;
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
									printf ("\n\tcreating temporary images ");
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp1);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp2);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp3);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tmp4);

	for (d = 0; d < 3; d++)
	for (i = 0; true; i++) {
		if (d == 0) file_curr = file_1[i];
		else if (d == 1) file_curr = file_2[i];
		else if (d == 2) file_curr = file_3[i];
		if (file_curr == NULL)	break;
									printf ("\n\nProcessing sequence '%s'", file_curr);
									sprintf (filename, "%s/%s/%s.img", storage_loc, dirs[d], file_curr);
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
				768, 576, 1);
									printf ("\n\topening sequences");
cmd(	 KSCMD_OPEN,	"sii",	storage_src,		// Opening storage with source images
				0777,
				0777);					kstp_get_int (&resp, 0, &stor_src);
cmd(	 KSCMD_OPEN,	"sii",	storage_dst,		// Opening storage for resulting images
				0777,
				0777);					kstp_get_int (&resp, 0, &stor_dst);
cmd(	 DCMD_OBJINFO_STORAGE, "i", stor_src);				kstp_get_int (&resp, 3, &count);

									printf ("\n");
	 for (im = 0; im < count; im++) {
									printf ("\t                                                                 \r");
									printf ("\tprocessing image pack #%03d: \n", im);

									sprintf (objname, "%s-img%d", storage_src, im);
cmd(	  KSCMD_OPEN,	"sii",	objname,		// Opening source image pack
				0777,
				0777);					kstp_get_int (&resp, 0, img_srcs);

	open_img (image_noise, im, &img_noise);
	open_img (image_tissue, im, &img_tissue);
	open_img (image_mask1, im, &img_mask1);
	open_img (image_mask2, im, &img_mask2);
	open_img (image_bright, im, &img_bright);
	open_img (image_dark, im, &img_dark);
	open_img (image_neurons, im, &img_neurons);
	open_img (image_fibers, im, &img_fibers);
	open_img (image_terminals, im, &img_terminals);

// Removing tissue structures
cmd(	  DCMD_IMG_PXW_MAX, "iii",			// Minimum of four images
				img_noise,
				img_srcs[0],
				img_srcs[0]);
cmd(	  DCMD_IMG_MTX_MIN, "iii",			// Dilating dark (tissue) structures
				6,			  //- filter size
				img_tmp1,
				img_noise);
cmd(	  DCMD_IMG_MTX_AVG, "iii",
				4,
				img_tmp2,
				img_tmp1);
cmd(	  DCMD_IMG_PXW_SUB, "iiii",
				ce_subm_zero,
				img_tissue,
				img_noise,
				img_tmp2);				printf ("\t\tterminals and fibers is removed\r");

// Getting mask #1
cmd(	  DCMD_IMG_STAT, "i",	img_noise);				kstp_get_real (&resp, 0, &mean);
									kstp_get_real (&resp, 1, &var);
cmd(	  DCMD_IMG_PXW_THR, "iiiii",
				(ulong)(mean + 2 * var),	//- from
				0xFF,				//- to
				0xFF,				//- resulting color
				img_mask1,
				img_noise);				printf ("\t\tmask#1 is obtained              \r");
// Getting mask #2
cmd(	  DCMD_IMG_STAT, "i",	img_tissue);				kstp_get_real (&resp, 0, &mean);
									kstp_get_real (&resp, 1, &var);
cmd(	  DCMD_IMG_PXW_THR, "iiiii",
				(ulong)(mean + var),
				0xFF,
				0xFF,
				img_mask2,
				img_tissue);				printf ("\t\tmask#2 is obtained               \r");

// TERMINALS
cmd(	DCMD_IMG_TRC_SCRAP, "iiiiii",
				true,
				4,
				64,
				true,
				img_terminals,
				img_mask1);

// NEURONS
cmd(	DCMD_IMG_PXW_OR, "iii",	img_tmp4,
				img_mask1,
				img_mask2);
cmd(	DCMD_IMG_MTX_LOGIC, "iiiiiSii",
				0xFF,
				false,
				0,
				ce_logc_eq,
				ce_logr_max,
				strsafe_frombin (mtx_round, sizeof(mtx_round)),
				img_tmp2,
				img_tmp4);
cmd(	DCMD_IMG_MTX_LOGIC, "iiiiiSii",
				0xFF,
				false,
				0,
				ce_logc_eq,
				ce_logr_max,
				strsafe_frombin (mtx_round, sizeof(mtx_round)),
				img_tmp3,
				img_tmp2);
cmd(	DCMD_IMG_MTX_LOGIC, "iiiiiSii",
				0xFF,
				false,
				0,
				ce_logc_eq,
				ce_logr_max,
				strsafe_frombin (mtx_round, sizeof(mtx_round)),
				img_tmp1,
				img_tmp3);
cmd(	DCMD_IMG_TRC_SCRAP, "iiiiii",
				true,
				0,
				10000,
				true,
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_TRC_HOLES, "iii",
				true,
				img_tmp1,
				img_tmp2);
cmd(	DCMD_IMG_MTX_MIN, "iii",
				6,
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_PXW_XOR, "iii",
				img_tmp1,
				img_tmp4,
				img_tmp2);
cmd(	DCMD_IMG_MTX_MIN, "iii",
				6,
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_TRC_SCRAP, "iiiiii",
				false,
				0,
				32,
				true,
				img_tmp1,
				img_tmp2);
cmd(	DCMD_IMG_MTX_MAX, "iii",
				6,
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_PXW_OR, "iii",
				img_tmp1,
				img_tmp2,
				img_tmp4);
cmd(	DCMD_IMG_MTX_LOGIC, "iiiiiSii",
				0xFF,
				false,
				0,
				ce_logc_eq,
				ce_logr_max,
				strsafe_frombin (mtx_round, sizeof(mtx_round)),
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_MTX_LOGIC, "iiiiiSii",
				0x00,
				false,
				0,
				ce_logc_eq,
				ce_logr_max,
				strsafe_frombin (mtx_round, sizeof(mtx_round)),
				img_tmp1,
				img_tmp2);
cmd(	DCMD_IMG_TRC_SCRAP, "iiiiii",
				false,
				0,
				768,
				true,
				img_neurons,
				img_tmp1);

// FIBERS
cmd(	DCMD_IMG_MTX_MAX, "iii",
				4,
				img_tmp2,
				img_mask1);
cmd(	DCMD_IMG_MTX_MIN, "iii",
				3,
				img_fibers,
				img_tmp2);
// Putting it all together
cmd(	DCMD_IMG_PXW_OR, "iiii",
				img_bright,
				img_neurons,
				img_fibers,
				img_terminals);

//  PASS2: FOR LOW-DENSITY NEURONS
cmd(	  DCMD_IMG_PXW_THR, "iiiii",
				(ulong)mean,		//- from
				0xFF,				//- to
				0xFF,				//- resulting color
				img_dark,
				img_srcs[0]);

//...................................

cmd(	  KSCMD_CLOSE, "i",	img_srcs[0]);
									//printf ("\t\tresult is saved                  \n");
	close_img (image_noise, im, img_noise);
	close_img (image_dark, im, img_dark);
	close_img (image_bright, im, img_bright);
	close_img (image_neurons, im, img_neurons);
	close_img (image_fibers, im, img_fibers);
	close_img (image_terminals, im, img_terminals);
	close_img (image_tissue, im, img_tissue);
	close_img (image_mask1, im, img_mask1);
	close_img (image_mask2, im, img_mask2);

	 }

cmd(	 KSCMD_CLOSE,	"i",	stor_src);
cmd(	 KSCMD_SAVE,	"i",	stor_dst);
cmd(	 KSCMD_CLOSE,	"i",	stor_dst);

									sprintf (filename, "%s/%s.step1/%s.img", storage_loc, dirs[d], file_curr);
									remove (filename);
cmd(	 KSCMD_EXTRACT,	"iss",	DOBJF_STORAGE_KONTRON,
				filename,
				storage_dst);
cmd(	 KSCMD_DELETE,	"s",	storage_src);
cmd(	 KSCMD_DELETE,	"s",	storage_dst);
	}


cmd(	KSCMD_CLOSE,	"i",	img_tissue);
cmd(	KSCMD_CLOSE,	"i",	img_mask1);
cmd(	KSCMD_CLOSE,	"i",	img_mask2);
cmd(	KSCMD_CLOSE,	"i",	img_terminals);
cmd(	KSCMD_CLOSE,	"i",	img_neurons);
cmd(	KSCMD_CLOSE,	"i",	img_fibers);
cmd(	KSCMD_CLOSE,	"i",	img_tmp1);
cmd(	KSCMD_CLOSE,	"i",	img_tmp2);
cmd(	KSCMD_CLOSE,	"i",	img_tmp3);
cmd(	KSCMD_CLOSE,	"i",	img_tmp4);
cmd(	KSCMD_BYE,	""	);
									printf ("\nDONE");
	getch ();

	return (0);
}
