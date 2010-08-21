// DataWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "StorageDoc.h"
#include "StorageFrame.h"
#include "DataWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataWnd

IMPLEMENT_DYNCREATE(CDataWnd, CListCtrl)

CDataWnd::CDataWnd()
{
}

CDataWnd::~CDataWnd()
{
}


BEGIN_MESSAGE_MAP(CDataWnd, CListCtrl)
	//{{AFX_MSG_MAP(CDataWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataWnd diagnostics

#ifdef _DEBUG
void CDataWnd::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CDataWnd::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataWnd message handlers

int CDataWnd::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle (LVS_EDITLABELS | LVS_NOCOLUMNHEADER | LVS_NOSCROLL | LVS_NOSCROLL | LVS_SINGLESEL, 
	             LVS_REPORT | LVS_SHOWSELALWAYS);
	SetExtendedStyle (LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	InsertColumn (0, "", LVCFMT_RIGHT, 20, 0);
	return 0;
}

void CDataWnd::CreateSurvey () {
	uvar32_64 n, len;
	for (n = 0, len = GetHeaderCtrl()->GetItemCount(); n < len; ++n)
		DeleteColumn (0);
	DeleteAllItems ();
	if (m_pDoc->Surveis(aimActive).GetCount () != 1)
		return;
	InsertColumn (0, "", LVCFMT_RIGHT, 20, 0);
	for (n = 0, len = m_pDoc->Surveis[aimActive].Fields.GetCount (); n < len; ++n)
		InsertColumn (n + 1, m_pDoc->Surveis[aimActive].Fields[n].GetName (), LVCFMT_LEFT, 80, n + 1);
}

