#include "stdafx.h"
#include "dias.h"
#include "MainFrame.h"
#include "DiasAPIDocs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIDocs::CDiasAPIDocs () {
}

CDiasAPIDocs::~CDiasAPIDocs () {
}

CStorageDoc& CDiasAPIDocs::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	if (GetCount () <= 0)
		throw (CaImAPIException (0));
	CStorageFrame* pFrame = ((CStorageFrame*)(((CMainFrame*)(theApp.m_pMainWnd))->MDIGetActive()));
	return (pFrame->GetDocument ());
}
