// ReportWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "StorageDoc.h"
#include "StorageFrame.h"
#include "ReportWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportWnd

CReportWnd::CReportWnd()
{
}

CReportWnd::~CReportWnd()
{
}


BEGIN_MESSAGE_MAP(CReportWnd, CWnd)
	//{{AFX_MSG_MAP(CReportWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReportWnd message handlers

int CReportWnd::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableScrollBarCtrl (0);
	return 0;
}
