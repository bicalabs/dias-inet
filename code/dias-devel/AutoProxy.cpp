// AutoProxy.cpp : implementation file
//

#include "stdafx.h"
#include "dias-devel.h"
#include "AutoProxy.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoProxy

IMPLEMENT_DYNCREATE(CAutoProxy, CCmdTarget)

CAutoProxy::CAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CMainDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainDlg)))
		{
			m_pDialog = reinterpret_cast<CMainDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CAutoProxy::~CAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_Idiasdevel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {3910DA92-03F4-4B8D-8BFE-439ECA15D7F1}
static const IID IID_Idiasdevel =
{ 0x3910DA92, 0x3F4, 0x4B8D, { 0x8B, 0xFE, 0x43, 0x9E, 0xCA, 0x15, 0xD7, 0xF1 } };

BEGIN_INTERFACE_MAP(CAutoProxy, CCmdTarget)
	INTERFACE_PART(CAutoProxy, IID_Idiasdevel, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {7AFF2E3E-5185-47FB-81FA-B8C36830C497}
IMPLEMENT_OLECREATE2(CAutoProxy, "diasdevel.Application", 0x7aff2e3e, 0x5185, 0x47fb, 0x81, 0xfa, 0xb8, 0xc3, 0x68, 0x30, 0xc4, 0x97)


// CAutoProxy message handlers
