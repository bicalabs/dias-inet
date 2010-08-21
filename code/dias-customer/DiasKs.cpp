// DiasKs.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "DiasKs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasKs

IMPLEMENT_DYNCREATE(CDiasKs, CCmdTarget)

CDiasKs::CDiasKs () {
	WSADATA wsdata;
	WSAStartup(0x0101,&wsdata);
	ks_inetserv.sin_family = AF_INET;
	ks_inetserv.sin_port = htons (5634);
	ks_inetserv.sin_addr.s_addr = inet_addr ("127.0.0.1");
	memset (&(ks_inetserv.sin_zero), 0, 8);
}

CDiasKs::~CDiasKs () {
}


BEGIN_MESSAGE_MAP(CDiasKs, CCmdTarget)
	//{{AFX_MSG_MAP(CDiasKs)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiasKs message handlers

int CDiasKs::Connect () {
	return Cmd (KSCMD_HELLO, "ss", "dias2", "pass");
}

int CDiasKs::Disconnect () {
	return Cmd (KSCMD_BYE, "");
}

int CDiasKs::Cmd (int cmd, char* form, ...) {
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
	if ((ks_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
		exit (1);
	if ((s = connect (ks_socket, (struct sockaddr*)&ks_inetserv, sizeof (struct sockaddr_in))) == -1)
		exit (1);
	kstp_send (ks_socket, &query);
	kstp_recv (ks_socket, &resp);
	if (resp.cmd != 0)
		return resp.cmd;
	closesocket (ks_socket);
	return 0;
}

void CDiasKs::GenMtx (sint sz, sdword *ptr) {
	sint x,y;
	for (x = -sz; x <= sz; ++x)
		for (y = -sz; y <= sz; ++y) {
			*ptr++ = x;
			*ptr++ = y;
		}
}
