#if !defined(AFX_DIASKS_H__ED6D7D84_4A8D_4712_9E57_AE4DE630D840__INCLUDED_)
#define AFX_DIASKS_H__ED6D7D84_4A8D_4712_9E57_AE4DE630D840__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiasKs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiasKs command target

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "dias_ks.h"
#include "aimkstp.h"

class CDiasKs : public CCmdTarget
{
	DECLARE_DYNCREATE(CDiasKs)

	CDiasKs();           // protected constructor used by dynamic creation
	~CDiasKs();

// Attributes
private:
	packet query, resp;

	SOCKET ks_socket, s;
	struct sockaddr_in ks_inetserv;

// Operations
public:
	int Connect ();
	int Disconnect ();
	int Cmd (int cmd, char* form, ...);
	void GenMtx (sint sz, sdword *ptr); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasKs)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiasKs)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASKS_H__ED6D7D84_4A8D_4712_9E57_AE4DE630D840__INCLUDED_)
