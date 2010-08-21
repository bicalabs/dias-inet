#include "stdafx.h"
#include "dias.h"
#include "MainFrame.h"
#include "DiasAPIFrames.h"
#include "StorageFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIFrames

CDiasAPIFrames::CDiasAPIFrames () {
}

CDiasAPIFrames::~CDiasAPIFrames () {
}

CStorageFrame& CDiasAPIFrames::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	return (*((CStorageFrame*)(((CMainFrame*)(theApp.m_pMainWnd))->MDIGetActive())));
}
