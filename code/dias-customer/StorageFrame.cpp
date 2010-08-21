// StorageFrame.cpp : implementation of the CStorageFrame class
//

#include "stdafx.h"
#include "Dias.h"
#include "DocList.h"
#include "DocPane.h"
#include "ImageWnd.h"
#include "DataWnd.h"
#include "ReportWnd.h"
#include "StorageFrame.h"
#include "StorageView.h"
#include ".\storageframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStorageFrame

IMPLEMENT_DYNCREATE(CStorageFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CStorageFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStorageFrame)
	ON_COMMAND(ID_EDIT_RENAME, OnEditRename)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RENAME, OnUpdateEditRename)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_IMG, OnUpdatePane )
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStorageFrame construction/destruction


CStorageFrame::CStorageFrame ( ) {
}

CStorageFrame::~CStorageFrame ( ) {
}

int CStorageFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

BOOL CStorageFrame::OnCreateClient (LPCREATESTRUCT lpCS, CCreateContext* pContext) {
	// STAGE 0: Creating statusbar
	UINT indicators[10];
	m_wndStatusBar.Create (this);
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight (24);
	m_wndStatusBar.SetIndicators (indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo (0, ID_SEPARATOR, SBPS_STRETCH | SBPS_NOBORDERS, 100);
	m_wndStatusBar.SetPaneInfo (1, ID_INDICATOR_IMG, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo (2, ID_INDICATOR_SRV, SBPS_NORMAL, 30);
	m_wndStatusBar.SetPaneInfo (3, ID_INDICATOR_REP, SBPS_NORMAL, 30);
	m_wndStatusBar.SetPaneInfo (4, ID_INDICATOR_SIZEICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (5, ID_INDICATOR_SIZE, SBPS_NOBORDERS, 30);
	m_wndStatusBar.SetPaneInfo (6, ID_INDICATOR_CMICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (7, ID_INDICATOR_CM, SBPS_NOBORDERS, 30);
	m_wndStatusBar.SetPaneInfo (8, ID_INDICATOR_BPP, SBPS_NOBORDERS, 30);
	m_wndStatusBar.SetPaneInfo (9, ID_INDICATOR_TIME, SBPS_NORMAL, 75);
	m_wndStatusBar.SetPaneText (1, "0", TRUE);

	m_barImages.Create (this, m_pView, 0);
	m_barData.Create (this, m_pView, 1);
	m_barReports.Create (this, m_pView, 2);

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
	rbbi.cx = 500;
	rbbi.cxMinChild = 0;
	rbbi.cyChild = 200;
	rbbi.cyMinChild = 100;
	rbbi.cyMaxChild = 500;
	rbbi.cyIntegral = 1;
	m_barShortcuts.Create (this, CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS | RBS_FIXEDORDER | /*RBS_VERTICALGRIPPER | *//*RBS_AUTOSIZE |*/ CCS_LEFT,
	                       WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_ALIGN_LEFT);
	m_barShortcuts.SetBorders (0, 0, 0, 0);
	m_barShortcuts.AddBar (&m_barImages, theApp.LoadString (IDS_TBX_DOC_IMAGES), NULL, RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	m_barShortcuts.AddBar (&m_barData, theApp.LoadString (IDS_TBX_DOC_DB), NULL, RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	m_barShortcuts.AddBar (&m_barReports, theApp.LoadString (IDS_TBX_DOC_REPORTS), NULL, RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	m_barShortcuts.GetReBarCtrl().SetBandInfo (0, &rbbi);
	rbbi.cx = 0;
	rbbi.cxMinChild = 0;
	m_barShortcuts.GetReBarCtrl().SetBandInfo (1, &rbbi);
	m_barShortcuts.GetReBarCtrl().SetBandInfo (2, &rbbi);

	m_ilTbxImg.Create (20, 20, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ilTbxImg.Add (theApp.LoadBitmap (IDB_TOOLBOX_IMAGES), 0xFFFFFF);

	m_wndToolBar.CreateEx (this, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_LIST);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);
	m_wndToolBar.LoadToolBar (IDR_TOOLBOX_IMAGES);
	m_wndToolBar.GetToolBarCtrl().SetImageList (&m_ilTbxImg);
	m_wndToolBar.GetToolBarCtrl().SetHotImageList (&m_ilTbxImg);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList (&m_ilTbxImg);

	m_wndToolBar.SetButtonText (0, "Picker");
	m_wndToolBar.SetButtonText (5, "Acquire");
	m_wndToolBar.SetButtonText (7, "Measure");
	m_wndToolBar.SetButtonText (8, "Mark");
	m_wndToolBar.SetButtonText (9, "Patch");
	m_wndToolBar.SetButtonText (10, "Label");
	m_wndToolBar.SetButtonText (12, "Reader");
	m_wndToolBar.SetButtonText (13, "Pen");
	m_wndToolBar.SetButtonText (14, "Shapes");

	for (int i = 0; i < 18; ++i)
		if (m_wndToolBar.GetButtonStyle (i) != TBBS_SEPARATOR)
			m_wndToolBar.SetButtonStyle (i, TBBS_AUTOSIZE | TBBS_CHECKBOX);
	m_wndToolBar.SetButtonStyle (3, TBBS_AUTOSIZE | TBBS_CHECKBOX | TBBS_DROPDOWN);

	rbbi.cx = 34;
	rbbi.cyChild = 34;
	rbbi.cyMinChild = 34;
	rbbi.cyMaxChild = 34;
	rbbi.cyIntegral = 1;
	rbbi.cxMinChild = 34;
	m_wndReBar.Create (this);
	m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	m_wndReBar.SetBorders (0, 0, 0, 0);
	m_wndReBar.GetReBarCtrl ().SetBandInfo (0, &rbbi);

	return (CMDIChildWnd::OnCreateClient (lpCS, pContext));
}

void CStorageFrame::OnUpdatePane (CCmdUI *pCmdUI) {
	pCmdUI->Enable ();
}

BOOL CStorageFrame::PreCreateWindow (CREATESTRUCT& cs) {
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return (CMDIChildWnd::PreCreateWindow(cs));
}

/////////////////////////////////////////////////////////////////////////////
// CStorageFrame diagnostics

#ifdef _DEBUG
void CStorageFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CStorageFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

BOOL CStorageFrame::OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
	CRect rect1, rect2;

	UINT n, count, state;
	CMenu mn, *pM;
	CaImMenu menu;
	CaImMenuItem** ppItems;
	CString strMenu;
	menu.CreatePopupMenu ();

	switch (LPNMHDR(lParam)->code) {
		case TBN_DROPDOWN:
			if (LPNMTOOLBAR(lParam)->iItem != ID_IMAGE_ZOOM)
				break;
			mn.LoadMenu (IDR_POPUP_DOC_ZOOM);
			pM = mn.GetSubMenu(0);
			count = pM->GetMenuItemCount ();
			ppItems = new CaImMenuItem*[count];
			for (n = 0; n < count; ++n) {
				pM->GetMenuString (n, strMenu, MF_BYPOSITION);
				state = pM->GetMenuState (n, MF_BYPOSITION);
				if ((state & MF_SEPARATOR) != 0)
                                        ppItems[n] = new CaImMenuItem ();
				else if ((state & MF_DISABLED) != 0)
					ppItems[n] = new CaImMenuItem (strMenu, AMIS_DISABLED);
				else if ((state & MF_CHECKED) != 0)
					ppItems[n] = new CaImMenuItem (strMenu, AMIS_CHECKED);
				else
					ppItems[n] = new CaImMenuItem (strMenu);
				menu.AppendMenu (state | MF_OWNERDRAW, pM->GetMenuItemID (n), (LPCTSTR)ppItems[n]);
			}
			goto trackPopup;
	}
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
trackPopup:

	m_wndToolBar.GetWindowRect (&rect1);
	m_wndToolBar.GetToolBarCtrl ().GetRect (LPNMTOOLBAR(lParam)->iItem, &rect2);

	menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect1.left + rect2.left, rect1.top + rect2.bottom, this);
	menu.DestroyMenu ();
	for (n = 0; n < count; ++n)
		delete ppItems[n];
	if (count > 0)
		delete [] ppItems;

	return (false);
}

CStorageDoc& CStorageFrame::GetDocument () {
	return (*(CStorageDoc*)(m_pView->GetDocument()));
}

UINT CStorageFrame::GetActiveImageNo () {
	if (!IsWindow (m_barImages))
		return (-1);
	return (aimGetSelListItem (m_barImages, false));
}
void CStorageFrame::SetActiveImageNo (UINT no) {
	if (!IsWindow (m_barImages))
		return;
	aimSetSelListItem (m_barImages, no);
}
void CStorageFrame::EnsureVisibleImageNo (UINT no) {
	if (!IsWindow (m_barImages))
		return;
	m_barImages.EnsureVisible (no, FALSE);
}
UINT CStorageFrame::GetActiveSurveyNo () {
	if (!IsWindow (m_barData))
		return (-1);
	return (aimGetSelListItem (m_barData, false));
}

UINT CStorageFrame::GetActiveReportNo () {
	if (!IsWindow (m_barReports))
		return (-1);
	return (aimGetSelListItem (m_barReports, false));
}

svar32_64 CStorageFrame::GetFirstSelectedImageNo () {
	if (!IsWindow (m_barImages))
		return NULL;
	for (uvar32_64 i = 0; i < m_barImages.GetItemCount (); ++i)
		if (m_barImages.GetItemState (i, LVIS_FOCUSED) || m_barImages.GetItemState (i, LVIS_SELECTED))
			return (i);
	return (-1);
}

int CStorageFrame::GetNextSelectedImageNo (svar32_64& pos) {
	if (!IsWindow (m_barImages))
		return -1;
	for (uvar32_64 i = 0; i < m_barImages.GetItemCount (); ++i)
		if ((m_barImages.GetItemState (i, LVIS_FOCUSED) || m_barImages.GetItemState (i, LVIS_SELECTED)) && i > pos)
			return (pos = i);
	return (pos = -1);
}

POSITION CStorageFrame::GetFirstSelectedSurveyNo () {
	if (!IsWindow (m_barData))
		return NULL;
	return (m_barData.GetFirstSelectedItemPosition());
}

int CStorageFrame::GetNextSelectedSurveyNo (POSITION& pos) {
	if (!IsWindow (m_barData))
		return -1;
	return (m_barData.GetNextSelectedItem(pos));
}

void CStorageFrame::OnEditRename () {
	int active = GetActiveList ();
	if (active == 0)
		m_barImages.OnEditRename ();
	else if (active == 1)
		m_barData.OnEditRename ();
	else
		m_barReports.OnEditRename ();
}

void CStorageFrame::OnUpdateEditRename (CCmdUI* pCmdUI) {
	pCmdUI->Enable (GetFocus () != m_pView);
}
