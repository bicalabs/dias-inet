/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions: sockets data transfer susbeystem            *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 06.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Revised 11.01.2003 Maxim A. Orlovsky: added strict function arguments       *
*                     control and any kind of possible checks                  *
*                                                                              *
*******************************************************************************/

#include <winsock2.h>

#include <time.h>
#include <sysconf.h>
#include <strsafex.h>
#include <libnodes.h>
#include "aimkstp.h"

#include "ks.h"
#include "kssock.h"

client* clients_head;
client client_root = {
	NULL, NULL,			/* prev and next pointers */
	"", "DIAS-KS",			/* cliet identification */
	0, 0, NULL,			/* groups information */
	NULL, 0, (uvar32_64)(-1),	/* memory */
	0, (uvar32_64)(-1),		/* cpu */
	0, 0, {0, "", "", 0},		/* errors */
	NULL, NULL };			/* objects */

void ks_lasterr_resp (packet* resp, client* c) {
	error* err;
	string func, file;

	isclient (c) fail return; otherwise;
	notnull (resp, c) fail return; otherwise;

	if (!(err = kserr_last (c)) ||
	    !(func = strsafe_fromstr (err->func, 1024)) ||
	    !(file = strsafe_fromstr (err->file, 1024)) )
		return;
	resp->cmd = err->err;
	memset (resp->key, 0, 64);
	kstp_allocate (resp, 3);
	kstp_set_string (resp, 0, func);
	kstp_set_string (resp, 1, file);
	kstp_set_int (resp, 2, err->line);
	strsafe_free (func);
	strsafe_free (file);
}

bool ks_open_socket () {
	int serr;
	struct sockaddr_in ks_inetserv;
#ifdef __OS_WINDOWS__
	WSADATA wsdata; 
	WSAStartup(0x0101,&wsdata); 
#endif
	
	srand ((unsigned)time (NULL));

	isvalid ((ks_socket = socket (AF_INET, SOCK_STREAM, 0)) != -1, CLIENT_ROOT, KSERR_SOCKET) fail
		serr = WSAGetLastError ();
		return (false);
	otherwise;

	ks_inetserv.sin_family = AF_INET;
	ks_inetserv.sin_port = htons (conf_socket_port);
	ks_inetserv.sin_addr.s_addr = inet_addr (conf_socket_ip);
	memset (&(ks_inetserv.sin_zero), 0, 8);

	isvalid (bind (ks_socket, (struct sockaddr*)&ks_inetserv, sizeof (struct sockaddr_in)) != -1, CLIENT_ROOT, KSERR_BIND) fail
		serr = WSAGetLastError ();
		return (false);
	otherwise
	isvalid (listen (ks_socket, conf_socket_clients) != -1, CLIENT_ROOT, KSERR_LISTEN) fail
		serr = WSAGetLastError ();
		closesocket (ks_socket);
		return (false);
	otherwise
	ks_socklen = sizeof (struct sockaddr_in);
	ks_server = (struct sockaddr *)&ks_inetserv;

        return (true);
}

void ks_close_socket () {
	closesocket (ks_socket);
}

bool ks_session () {
	bool rslt;
	packet query, resp;
	socket_t session_socket;

	isvalidf ((session_socket = accept (ks_socket, ks_server, &ks_socklen)) != -1, CLIENT_ROOT, KSERR_ACCEPTSOCKET);

	iffailed (kstp_recv (session_socket, &query), CLIENT_ROOT, "unable to receive socket message")
		closesocket (session_socket);
		ks_lasterr_resp (&resp, CLIENT_ROOT);
		kstp_send (session_socket, &resp);
		return (false);
	otherwise;

	kssec_cleanup ();

	rslt = ks_proc (&query, &resp);
	kstp_release (&query);
	rslt &= kstp_send (session_socket, &resp);
	closesocket (session_socket);

	return (rslt);
}

#define ARGS_ALLOC(q,n,c)	isvalidj (kstp_allocate (q, n), c, kstp_errno, $error);
#define ARG_GET_STRING(n,s,c)	isvalidj (kstp_get_string(query, n, &s), c, kstp_errno, $error);
#define ARG_GET_REAL(n,s,c)	isvalidj (kstp_get_real(query, n, &s), c, kstp_errno, $error);
#define ARG_GET_INT(n,s,c)	isvalidj (kstp_get_int(query, n, &s), c, kstp_errno, $error);
#define ARG_SET_STRING(n,s,c)	isvalidj (kstp_set_string(resp, n, s), c, kstp_errno, $error);
#define ARG_SET_REAL(n,s,c)	isvalidj (kstp_set_real(resp, n, s), c, kstp_errno, $error);
#define ARG_SET_INT(n,s,c)	isvalidj (kstp_set_int(resp, n, s), c, kstp_errno, $error);

bool ks_proc (packet* query, packet* resp) {
	client*	cli = NULL;
	sdword int1 = 0, int2 = 0, int3 = 0;
	rtbyte real1 = .0, real2 = .0, real3 = .0;
	string string1 = NULL, string2 = NULL, string3 = NULL;

	notnullf (query, CLIENT_ROOT);
	notnullf (resp, CLIENT_ROOT);

	if (query->cmd != KSCMD_HELLO && !(cli = kssec_identify (query->key))) {
		kserr_fire (CLIENT_ROOT, KSERR_CLIENTKEY, "", __FILE__, __LINE__);
		cli = CLIENT_ROOT;
		goto $error;
	}

	switch (query->cmd) {
		case KSCMD_HELLO:
			isvalidj (clients_head || bdlist_size (clients_head) >= conf_socket_clients, CLIENT_ROOT, KSERR_MAXCLIENTS, $error);
			ARG_GET_STRING (0, string1, CLIENT_ROOT);
			ARG_GET_STRING (1, string2, CLIENT_ROOT);
			ifj (!(cli = (client *)ksmem_commit (CLIENT_ROOT, sizeof (client))), $error);
			if (!kssec_authentificate (string1, string2, cli)) {
				ksmem_release (CLIENT_ROOT, cli);
				cli = NULL;
				goto $error;
			}
			ifj (!kssec_client_init (cli), $error);
			kserr_fire (CLIENT_ROOT, KSERR_CLIENTCONNECTED, "ks_proc", __FILE__, __LINE__);
			kssec_generatekey (cli);
			memcpy (resp->key, cli->key, sizeof (cli->key));
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_BYE:
			kserr_fire (CLIENT_ROOT, KSERR_BYECLIENT, "ks_proc", __FILE__, __LINE__);
			kserr_fire (cli, KSERR_BYECLIENT, "ks_proc", __FILE__, __LINE__);
			ARGS_ALLOC (resp, 1, cli);
			ARG_SET_INT (0, KSERR_SUCCESS, cli);

			kssec_client_release (cli);
			ksmem_release (CLIENT_ROOT, cli);
			kserr_fire (CLIENT_ROOT, KSERR_CLIENTDISCONN, "ks_proc", __FILE__, __LINE__);
			break;

		case KSCMD_LOAD:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_INT (1, int2, cli);
			ARG_GET_STRING (2, string1, cli);
			ARG_GET_STRING (3, string2, cli);
			ARG_GET_INT (4, int3, cli);
			ifj (!ksobj_load (cli, int1, int2, string1, string2, (uword)int3), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_EXTRACT:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_STRING (1, string1, cli);
			ARG_GET_STRING (2, string2, cli);
			ifj (!ksobj_extract (cli, int1, string1, string2), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_LINK:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_STRING (1, string1, cli);
			ifj (!ksobj_link (cli, (HOBJ)int1, string1), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_UNLINK:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_STRING (1, string1, cli);
			ifj (!ksobj_unlink (cli, (HOBJ)int1, string1), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_OPEN:
			ARG_GET_STRING (0, string1, cli);
			ARG_GET_INT (1, int1, cli);
			ARG_GET_INT (2, int2, cli);
			ifj (!(int1 = (int)ksobj_open (cli, string1, int1, int2)), $error);
			ARGS_ALLOC (resp, 1, cli);
			ARG_SET_INT (0, int1, cli);
			break;

		case KSCMD_CLOSE:
			ARG_GET_INT (0, int1, cli);
			ifj (!ksobj_close (cli, (HOBJ)int1), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_SAVE:
			ARG_GET_INT (0, int1, cli);
			ifj (!ksobj_save (cli, (HOBJ)int1), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_CREATE:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_STRING (1, string1, cli);
			ARG_GET_INT (2, int2, cli);
			ifj (!(int1 = (int)ksobj_create (cli, int1, string1, int2, query)), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_TEMP:
			ARG_GET_INT (0, int1, cli);
			ifj (!(int1 = (int)ksobj_temp (cli, int1, query)), $error);
			ARGS_ALLOC (resp, 1, cli);
			ARG_SET_INT (0, int1, cli);
			break;

		case KSCMD_DELETE:
			ARG_GET_STRING (0, string1, cli);
			ifj (!ksobj_delete (cli, string1), $error);
			ARGS_ALLOC (resp, 0, cli);
			break;

		case KSCMD_CFG_MEM:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_INT (1, int2, cli);
			int1 = ksmem_configure (cli, int1, int2);
			ARGS_ALLOC (resp, 1, cli);
			ARG_SET_INT (0, int1, cli);
			break;

		case KSCMD_TEST_MEM:
			ARG_GET_INT (0, int1, cli);
			ARG_GET_INT (1, int2, cli);
			int1 = ksmem_test (cli, int1, int2);
			ARGS_ALLOC (resp, 1, cli);
			ARG_SET_INT (0, int1, cli);
			break;

		default:
			ifj (!cm_proc (cli, query, resp), $error)
			break;
			/*kserr_fire (cli, KSERR_UNKNOWNCMD, "ks_proc", __FILE__, __LINE__);
			break;*/
	}
	resp->cmd = KSERR_SUCCESS;
//	kssec_generatekey (cli);
	return (true);
$error:
	ks_lasterr_resp (resp, cli);
	return (false);
}
