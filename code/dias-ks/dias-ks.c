/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service main module                                             *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovskyyahoo.com)              *
*                                                                              *
*******************************************************************************/

#include <time.h>
#include <winsock2.h>
#include <stdlib.h>

#include <sysconf.h>
#include <strsafex.h>
#include "dias-ks.h"

int random(int num);

/* Configuration parameters */
char*		conf_objdb_dir = "c:/var/aim/dias";
char*		conf_objdb_host = "127.0.0.1";
int		conf_objdb_port = 0;
uvar32_64	conf_objdb_recon_attempts = 10;
char*		conf_objdb_name = "dias_objdb";
char*		conf_objdb_user = "";
char*		conf_objdb_passwd = NULL;
char*		conf_socket_ip = "127.0.0.1";
uword		conf_socket_port = 5634;
uvar32_64	conf_socket_clients = 32;
uvar32_64	conf_socket_threads = 32;
char*		conf_log_dir = "c:/log/aim/dias";
char*		conf_log_error = "c:/log/aim/dias/errors.log";
char*		conf_log_access = "c:/log/aim/dias/access.log";
char*		conf_log_clients = "c:/log/aim/dias/clients.log";
char*		conf_log_memory = "c:/log/aim/dias/memory.log";
char*		conf_log_kstp = "c:/log/aim/diaskstp.log";
uvar32_64	conf_mem_pages_maxfree = 5;
uvar32_64	conf_mem_pages_minfree = 5;
uvar32_64	conf_client_disc_int = 1200;

extern int cnt;

#if defined _DEBUG
# include <stdio.h>
  FILE *fp_kstp_log;
#endif

#if defined __OS_WINDOWS__
  int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#elif defined __OS_POSIX__
  int main (int argc, char* argv[]) {
#endif
/*
int i;
bdl_node *mem;
bdl_node *_t_mem = NULL;
*/
	/* TODO: Read parameters from conf file */
	/* TODO: Start connections manager */
	/* TODO: If connection magaer dies check exitcode and
	   maintain signals */

	clients_head = CLIENT_ROOT;
	if (!ksmem_init ())
		return (1);

	if (!kserr_init (conf_log_error, conf_log_access))
		return (1);

	if (!ks_open_socket ()) {
		kserr_fire (CLIENT_ROOT, KSERR_COULDNTSTART, "main", __FILE__, __LINE__);
		return (1);
	}

#if defined _DEBUG
	fp_kstp_log = fopen ("c:/log/aim/dias-ks_kstp.log", "w");
#endif

	kserr_fire (CLIENT_ROOT, KSERR_STARTED, "main", __FILE__, __LINE__);
	while (true)
		ks_session ();
/*
for (i = 0 ; i < 1024 ; ++i) {
	iffailedm (mem = ksmem_commit (CLIENT_ROOT, rand ()), CLIENT_ROOT, "STOP");
	if (!_t_mem)	_t_mem = mem;
	else		bdlist_add (_t_mem, mem);
}
cnt=1;
for (i = 0 ; i < 10000 ; ++i) {
	mem = bdlist_get (_t_mem, rand() * (bdlist_size (_t_mem) - 1) / RAND_MAX + 1);
	if (!mem) continue;
	bdlist_remove (mem);
	ksmem_release (CLIENT_ROOT, mem);

	mem = bdlist_get (_t_mem, rand() * (bdlist_size (_t_mem) - 1) / RAND_MAX + 1);
	if (!mem) continue;
	bdlist_remove (mem);
	ksmem_release (CLIENT_ROOT, mem);

	iffailedm (mem = ksmem_commit (CLIENT_ROOT, rand ()), CLIENT_ROOT, "STOP");
	if (!_t_mem)	_t_mem = mem;
	else		bdlist_add (_t_mem, mem);
}*/

	ks_close_socket ();
//	ksmem_stop ();
	kserr_stop();

	return (0);
}
