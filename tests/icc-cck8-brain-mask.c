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

char *dirs[] = { "dbm3-colh-icac", "dbm5-colh-icac", "hlth-colh-icac" };

char *file_1[] = {
/*"dmn/dmn30_b",
"dmn/dmn33_b",
"dmn/dmn34_b",

"lha/lha39_b",

"pvn/pap08_b",
"pvn/pap09_b",
"pvn/pap12_b",
"pvn/pap14_b",
"pvn/pmm12_b",
"pvn/pmm13_1",
"pvn/pmm14_b",
"pvn/pmp15_b",
"pvn/pmp16_b",
"pvn/pmp17_2",
"pvn/pmp19_b",
"pvn/pmp20_b",
"pvn/pmp21_b",
"pvn/pmp22_b",
"pvn/pmp23_b",
"pvn/ppv08_b",
"pvn/ppv09_b",
"pvn/ppv12_b",
"pvn/ppv13_1",
"pvn/ppv15_b",
"pvn/ppv16_b",
"pvn/ppv19_b",
"pvn/ppv20_b",

"scn/scn08_b",

"son/son14_b",*/
	NULL
};
char *file_2[] = {
/*"eme/emf29_1",
"eme/emf30_b",

"lha/lha09_b",
"lha/lha10_b",
"lha/lha30_b",

"pvn/pam11_1",
"pvn/pam12_b",
"pvn/pap09_b",
"pvn/pap10_b",
"pvn/pap11_1",
"pvn/pap12_b",
"pvn/pap13_b",
"pvn/pdp18_1",
"pvn/pdp18_2",
"pvn/pdp19_b",
"pvn/pmm13_b",
"pvn/pmm14_b",
"pvn/pmm16_b",
"pvn/pmp17_b",
"pvn/pmp18_1",
"pvn/pmp18_2",
"pvn/pmp19_b",
"pvn/pmp20_b",
"pvn/pmp21_2",
"pvn/pmp23_2",
"pvn/pmp24_b",
"pvn/ppm16_b",
"pvn/ppm17_b",
"pvn/ppm18_1",
"pvn/ppv09_b",
"pvn/ppv10_b",
"pvn/ppv11_1",
"pvn/ppv12_b",
"pvn/ppv13_b",
"pvn/ppv15_b",
"pvn/ppv16_b",
"pvn/ppv18_1",
"pvn/ppv18_2",
"pvn/ppv19_b",
"pvn/ppv20_b",
"pvn/ppv21_2",

"scn/scn09_b",
"scn/scn10_b",
"scn/scn14_b",

"son/son09_b",
"son/son10_b",
"son/son13_b",*/
	NULL
};
char *file_3[] = {
/*"pvn/pam15_2",
"pvn/pam16_2",
"pvn/pap12_2",
"pvn/pap16_2",
"pvn/pap17_1",
"pvn/pap18_b",
"pvn/pap19_b",
"pvn/pap21_1",
"pvn/pap22_1",
"pvn/pap22_2",
"pvn/pap23_1",
"pvn/pdp25_1",
"pvn/pdp25_2",
"pvn/pdp32_b",
"pvn/pdp34_1",
"pvn/plp25_2",
"pvn/pmm22_1",
"pvn/pmm22_2",
"pvn/pmm23_1",
"pvn/pmm24_b",
"pvn/pmp24_b",
"pvn/pmp25_1",
"pvn/pmp25_2",
"pvn/pmp26_1",
"pvn/pmp27_1",
"pvn/pmp27_2",
"pvn/pmp28_b",
"pvn/pmp29_b",
"pvn/pmp31_1",
"pvn/pmp31_2",
"pvn/pmp32_b",
"pvn/ppm25_1",
"pvn/ppm25_2",
"pvn/ppm27_1",
"pvn/ppv11_b",
"pvn/ppv12_2",
"pvn/ppv15_2",
"pvn/ppv16_2",
"pvn/ppv17_1",
"pvn/ppv19_b",
"pvn/ppv23_1",
"pvn/ppv28_b",
"pvn/ppv29_b",
"pvn/ppv31_1",

"son/son16_2",
"son/son17_1",
"son/son18_b",
"son/son19_b",
"son/son22_2",
"son/son23_1",
"son/son24_b",

"arn/adm30_1",
"arn/adm31_1",
"arn/avl25_1",
"arn/avl25_2",
"arn/avl30_1",
"arn/avl30_2",
"arn/avl35_1",
"arn/avl35_2",
"arn/avm25_2",
"arn/avm35_1",
"arn/avm35_2",*/

"dmn/dmn37_b",
"dmn/dmn39_1",
"dmn/dmn42_1",
"dmn/dmn44_b",

/*"eme/eme25_1",
"eme/eme25_2",
"eme/eme26_1",
"eme/eme28_b",
"eme/eme30_1",
"eme/eme30_2",
"eme/eme31_1",
"eme/eme31_2",
"eme/emf25_1",
"eme/emf25_2",
"eme/emf26_1",
"eme/emf30_2",

"scn/scn03_b",
"scn/scn07_b",
"scn/scn15_2",
"scn/scn17_1",
"scn/scn17_2",
"scn/scn18_b",
"scn/scn19_b",
"scn/scn20_1",
"scn/scn21_1",
"scn/scn22_1",
"scn/scn23_1",

"lha/lha15_2",
"lha/lha19_b",
"lha/lha21_1",
"lha/lha21_2",
"lha/lha22_2",
"lha/lha23_2",
"lha/lha25_1",
"lha/lha25_2",
"lha/lha27_1",
"lha/lha30_1",
"lha/lha30_2",
"lha/lha31_1",
"lha/lha31_2",
"lha/lha35_1",
"lha/lha35_2",

"vmn/vmn25_1",
"vmn/vmn25_2",
"vmn/vmn29_b",
"vmn/vmn30_1",
"vmn/vmn30_2",
"vmn/vmn31_1",
"vmn/vmn31_2",
"vmn/vmn32_b",
"vmn/vmn33_2",
"vmn/vmn34_1",
"vmn/vmn35_1",
"vmn/vmn35_2",
"vmn/vmn37_b",*/
	NULL
};

char storage_src[] = "q:/aim/dias/src/srcseq";
char storage_dst[] = "q:/aim/dias/reslt/resltseq";
char storage_loc[] = "N:/Images/Hypothalamus.Rat/Cholecystokinin-8.ICC";
char image_all[] = "q:/aim/dias/reslt/result";
char image_noise[] = "q:/aim/dias/reslt/noise";
char image_terminals[] = "q:/aim/dias/reslt/term";
char image_neurons[] = "q:/aim/dias/reslt/neurons";
char image_fibers[] = "q:/aim/dias/reslt/fibers";
char image_mask[] = "q:/aim/dias/reslt/mask";

int stor_src, stor_dst;
int img_srcs[4], img_noise,
    img_tmp1, img_tmp2, img_tmp3, img_tmp4,
    img_tissue, img_mask1, img_mask2, img_terminals, img_neurons, img_fibers, img_all;

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
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_tissue);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_neurons);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_fibers);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_terminals);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_mask1);
cmd(	KSCMD_TEMP,	"iiiiiii",
				DOBJT_IMAGE, 0, 0,
				768, 576, 1, 0);			kstp_get_int (&resp, 0, &img_mask2);
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
	 for (im = 0; im < count; im += 4) {
									printf ("\t                                                                 \r");
									printf ("\tprocessing image pack #%03d: \n", im/4+1);

	  for (of = 0; of < 4; ++of) {
									sprintf (objname, "%s-img%d", storage_src, im + of);
cmd(	   KSCMD_OPEN,	"sii",	objname,		// Opening source image pack
				0777,
				0777);					kstp_get_int (&resp, 0, img_srcs + of);
	  }
									sprintf (objname, "%s-%d", image_noise, im/4+1);
cmd(	  KSCMD_CREATE,	"isiiiii",
				DOBJT_IMAGE,		
				objname,
				0777,
				768, 576, 1, 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_noise);
									sprintf (objname, "%s-%d", image_all, im/4+1);
cmd(	  KSCMD_CREATE,	"isiiiii",
				DOBJT_IMAGE,		
				objname,
				0777,
				768, 576, 1, 0);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_all);
/*									sprintf (objname, "%s-%d", image_terminals, im/4+1);
cmd(	  KSCMD_CREATE,	"isi",	DOBJT_IMAGE,		
				objname,
				0777);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_terminals);
									sprintf (objname, "%s-%d", image_neurons, im/4+1);
cmd(	  KSCMD_CREATE,	"isi",	DOBJT_IMAGE,		
				objname,
				0777);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_neurons);
									sprintf (objname, "%s-%d", image_fibers, im/4+1);
cmd(	  KSCMD_CREATE,	"isi",	DOBJT_IMAGE,		
				objname,
				0777);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_fibers);
									sprintf (objname, "%s1-%d", image_mask, im/4+1);
cmd(	  KSCMD_CREATE,	"isi",	DOBJT_IMAGE,		
				objname,
				0777);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_mask1);
									sprintf (objname, "%s2-%d", image_mask, im/4+1);
cmd(	  KSCMD_CREATE,	"isi",	DOBJT_IMAGE,		
				objname,
				0777);
cmd(	  KSCMD_OPEN,	"sii",	objname,		
				0777,
				0777);					kstp_get_int (&resp, 0, &img_mask2);*/

// Removing noise
cmd(	  DCMD_IMG_PXW_MIN, "iiiii",			// Minimum of four images
				img_tmp1,
				img_srcs[0],
				img_srcs[1],
				img_srcs[2],
				img_srcs[3]);
cmd(	  DCMD_IMG_MTX_FILTER, "iSii",			// Gaussian filter
				2,			  //- filter size
				strsafe_frombin (mtx_gauss, 5*5*sizeof(rtbyte)),
				img_noise,
				img_tmp1);				printf ("\t\tnoise removed\r");
// Removing tissue structures
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
				img_all,
				img_neurons,
				img_fibers,
				img_terminals);

/*cmd(	DCMD_IMG_TRC_SCRAP, "iiiiii",
				true,
				0,
				1024,
				true,
				img_tmp1,
				img_tmp3);
cmd(	DCMD_IMG_TRC_MARK, "iiii",
				true,
				true,
				img_tmp1,
				img_neurons);
cmd(	DCMD_IMG_PXW_ADD, "iiii",
				2,
				img_tmp1,
				img_tmp1,
				img_mask2);
cmd(	DCMD_IMG_MTX_AVG, "iii",
				3,
				img_tmp2,
				img_tmp1);
cmd(	DCMD_IMG_PXW_THR, "iiiii",
				0x80,
				0xFF,
				0xFF,
				img_tmp2,
				img_tmp2);				printf ("\t\tfibers                           \r");
cmd(	DCMD_IMG_MTX_MAX, "iii",
				2,
				img_fibers,
				img_tmp2);*/

//...................................

	  for (of = 0; of < 4; ++of) {
cmd(	   KSCMD_CLOSE, "i",	img_srcs[of]);
	  }
									printf ("\t\tresult is saved                  \n");

									sprintf (objname, "%s-%d", image_noise, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_noise);
cmd(	  KSCMD_CLOSE,	"i",	img_noise);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
									sprintf (objname, "%s-%d", image_all, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_all);
cmd(	  KSCMD_CLOSE,	"i",	img_all);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
/*									sprintf (objname, "%s-%d", image_terminals, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_terminals);
cmd(	  KSCMD_CLOSE,	"i",	img_terminals);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
									sprintf (objname, "%s-%d", image_neurons, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_neurons);
cmd(	  KSCMD_CLOSE,	"i",	img_neurons);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
									sprintf (objname, "%s-%d", image_fibers, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_fibers);
cmd(	  KSCMD_CLOSE,	"i",	img_fibers);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
									sprintf (objname, "%s1-%d", image_mask, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_mask1);
cmd(	  KSCMD_CLOSE,	"i",	img_mask1);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);
									sprintf (objname, "%s2-%d", image_mask, im/4+1);
cmd(	  KSCMD_SAVE,	"i",	img_mask2);
cmd(	  KSCMD_CLOSE,	"i",	img_mask2);
cmd(	  KSCMD_LINK,	"is",	stor_dst, 
				objname);*/
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

/******************************************************/
			/*********************************************************
			cmd (DCMD_IMG_TRC_HOLES,  "ii", true, htmps[1]);
			cmd (DCMD_IMG_MTX_MIN, "iii", 2, htmps[2], htmps[1]);
			cmd (DCMD_IMG_MTX_LOGIC, "iiiiiSii", 10, 0, 1, ce_logc_ge, ce_logr_count, strsafe_frombin (mtx_oct, sizeof(mtx_oct)), htmps[3], htmps[2]);
			cmd (DCMD_IMG_PXW_THR,  "iiiii", 0x0A, 0xFF, 0xFF, hdsts[1], htmps[3]);
			//cmd (DCMD_IMG_PXW_NOT,  "i", hdsts[1]);
			/*********************************************************/

			/*********************************************************
			cmd (DCMD_IMG_TRC_SCRAP,  "iiiii", true, 0, 128, false, hdsts[3]);
			//cmd (DCMD_IMG_MTX_LOGIC, "iiiiiSii", 10, 0, 1, ce_logc_ge, ce_logr_count, strsafe_frombin (mtx_oct, sizeof(mtx_oct)), htmps[3], htmps[2]);
			//cmd (DCMD_IMG_PXW_THR,  "iiiii", 0x0A, 0xFF, 0xFF, hdsts[2], htmps[3]);
			//cmd (DCMD_IMG_PXW_NOT,  "i", hdsts[2]);
			/*********************************************************/

	getch ();

	return (0);
}
