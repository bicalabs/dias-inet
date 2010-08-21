#include "stdafx.h"
#include "Dias.h"

#include "MainFrame.h"

#include "GeomProp.h"
#include "DensProp.h"
#include "Splash.h"
#include "TitlesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_GEOMSCALES, OnEditGeomScales)
	ON_COMMAND(ID_EDIT_DENSSCALES, OnEditDensScales)
	ON_COMMAND(ID_INDICATOR_GEOMSCALE, OnIndGeomScale)
	ON_COMMAND(ID_INDICATOR_DENSSCALE, OnIndDensScale)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_GEOMSCALE, OnUpdate_IndGeomScale)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DENSSCALE, OnUpdate_IndDensScale)
	ON_COMMAND_RANGE(ID_TOOLS_GEOMSCALE_1, ID_TOOLS_GEOMSCALE_999, OnSelectGeomScale)
	ON_COMMAND_RANGE(ID_TOOLS_DENSSCALE_1, ID_TOOLS_DENSSCALE_999, OnSelectDensScale)
	ON_COMMAND(ID_MARKER_STYLE_ADD, OnMarkerStyleAdd)
	ON_COMMAND(ID_MARKER_STYLE_DELETE, OnMarkerStyleDelete)
	ON_COMMAND(ID_MARKER_STYLE_EXPORT, OnMarkerStyleExport)
	ON_COMMAND(ID_MARKER_STYLE_IMPORT, OnMarkerStyleImport)
	ON_COMMAND(ID_MARKER_STYLE_RENAME, OnMarkerStyleRename)
	ON_COMMAND(ID_MARKER_STYLE_DUPLICATE, OnMarkerStyleDuplicate)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_TOOL,
	ID_INDICATOR_GEOMSCALEICO,
	ID_INDICATOR_GEOMSCALE,
	ID_INDICATOR_DENSSCALEICO,
	ID_INDICATOR_DENSSCALE,
	ID_INDICATOR_SAVE,
	ID_INDICATOR_PRINTICO,
	ID_INDICATOR_PRINT,
	ID_INDICATOR_PROGRAMICO,
	ID_INDICATOR_PROGRAM,
	ID_INDICATOR_IMGICO,
	ID_INDICATOR_IMG,
	ID_INDICATOR_CNLICO,
	ID_INDICATOR_CNL,
	ID_INDICATOR_PTCICO,
	ID_INDICATOR_PTC,
	ID_INDICATOR_MRKICO,
	ID_INDICATOR_MRK,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame () {
}

CMainFrame::~CMainFrame () {
}

int CMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CSplashWnd::ShowSplashScreen(this);

	// Status Bar Creation
	m_wndStatusBar.Create (this);
	m_wndStatusBar.m_pMainFrame = this;
	m_wndStatusBar.SetIndicators (indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight (32);
	m_wndStatusBar.SetPaneInfo (1, ID_INDICATOR_TOOL, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo (2, ID_INDICATOR_GEOMSCALEICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (3, ID_INDICATOR_GEOMSCALE, SBPS_NOBORDERS, 100);
	m_wndStatusBar.SetPaneInfo (4, ID_INDICATOR_DENSSCALEICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (5, ID_INDICATOR_DENSSCALE, SBPS_NOBORDERS, 100);
	m_wndStatusBar.SetPaneInfo (6, ID_INDICATOR_SAVE, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (7, ID_INDICATOR_PRINTICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (8, ID_INDICATOR_PRINT, SBPS_NOBORDERS, 24);
	m_wndStatusBar.SetPaneInfo (9, ID_INDICATOR_PROGRAMICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (10, ID_INDICATOR_PROGRAM, SBPS_NOBORDERS, 100);
	m_wndStatusBar.SetPaneInfo (11, ID_INDICATOR_IMGICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (12, ID_INDICATOR_IMG, SBPS_NOBORDERS, 24);
	m_wndStatusBar.SetPaneInfo (13, ID_INDICATOR_CNLICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (14, ID_INDICATOR_CNL, SBPS_NOBORDERS, 24);
	m_wndStatusBar.SetPaneInfo (15, ID_INDICATOR_PTCICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (16, ID_INDICATOR_PTC, SBPS_NOBORDERS, 24);
	m_wndStatusBar.SetPaneInfo (17, ID_INDICATOR_MRKICO, SBPS_NOBORDERS, 16);
	m_wndStatusBar.SetPaneInfo (18, ID_INDICATOR_MRK, SBPS_NOBORDERS, 24);

	m_wndStatusBar.GetStatusBarCtrl().SetIcon (2, theApp.LoadIcon (IDI_INDICATOR_GEOMSCALE));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (4, theApp.LoadIcon (IDI_INDICATOR_DENSSCALE));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (9, theApp.LoadIcon (IDI_INDICATOR_PRG_STOP));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (11, theApp.LoadIcon (IDI_INDICATOR_IMGICO));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (13, theApp.LoadIcon (IDI_INDICATOR_CNLICO));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (15, theApp.LoadIcon (IDI_INDICATOR_PTCICO));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon (17, theApp.LoadIcon (IDI_INDICATOR_MRKICO));

	m_wndStatusBar.SetPaneText (12, "0", TRUE);
	m_wndStatusBar.SetPaneText (14, "0", TRUE);
	m_wndStatusBar.SetPaneText (16, "0", TRUE);
	m_wndStatusBar.SetPaneText (18, "0", TRUE);

	// ToolBar Creation
	m_ilTbxStd.Create (24, 24, ILC_COLOR24 | ILC_MASK, 10, 1);
	m_ilTbxStd.Add (theApp.LoadBitmap (IDB_TBX_STD_N), 0xFFFFFF);
	m_ilTbxHotStd.Create (24, 24, ILC_COLOR24 | ILC_MASK, 10, 1);
	m_ilTbxHotStd.Add (theApp.LoadBitmap (IDB_TBX_STD_H), 0xFFFFFF);
	m_ilTbxDisStd.Create (24, 24, ILC_COLOR24 | ILC_MASK, 10, 1);
	m_ilTbxDisStd.Add (theApp.LoadBitmap (IDB_TBX_STD_D), 0xFFFFFF);

	m_wndToolBar.CreateEx (this, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);
	m_wndToolBar.LoadToolBar (IDR_MAINFRAME);
	m_wndToolBar.GetToolBarCtrl().SetImageList (&m_ilTbxStd);
	m_wndToolBar.GetToolBarCtrl().SetHotImageList (&m_ilTbxHotStd);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList (&m_ilTbxDisStd);
	m_wndToolBar.SetButtonStyle (0, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (1, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (2, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (6, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (7, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (10, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (20, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (21, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (30, TBBS_DROPDOWN);
	m_wndToolBar.SetButtonStyle (33, TBBS_CHECKBOX);

	m_wndReBar.Create (this, RBS_VARHEIGHT | RBS_BANDBORDERS | RBS_AUTOSIZE | RBS_DBLCLKTOGGLE | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_TOP);
	m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_USECHEVRON);

	m_paneView.Create (this);

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
	rbbi.cx = 500;
	rbbi.cxMinChild = 0;
	rbbi.cyChild = 260;
	rbbi.cyMinChild = 100;
	rbbi.cyMaxChild = 500;
	rbbi.cyIntegral = 1;
	m_wndPane.Create (this, CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS | RBS_FIXEDORDER | /*RBS_VERTICALGRIPPER | *//*RBS_AUTOSIZE |*/ CCS_RIGHT,
	                       WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_ALIGN_RIGHT);
	m_wndPane.SetBorders (0, 0, 0, 0);
	m_wndPane.AddBar (&m_paneView, NULL, NULL, RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	m_wndPane.GetReBarCtrl().SetBandInfo (0, &rbbi);

	return 0;
}

BOOL CMainFrame::PreCreateWindow (CREATESTRUCT& cs) {
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

void CMainFrame::Update (udword action) {
	if (action & UPDHA_MODE) {
		;;
	}
	if (action & UPDHA_IMAGE) {
		// #### TODO: Uncomment the following code
		if (theApp.Documents.GetCount() == 0 || 
		    theApp.Documents[aimActive].Images.GetCount() == 0 ||
		    theApp.Documents[aimActive].Images(aimActive).GetCount() == 0) {
		} else {
		}
		m_paneView.m_pnToolMark.Update ();
	}
	if (action & UPDHA_SURVEY) {
		// #### TODO: Update surveis bar here
	}
	if (action & UPDHA_MARKSTYLE || action & UPDHA_MARK)
		;
	if (action & UPDHA_DENSSCALE)
		m_wndStatusBar.SetPaneText (5, theApp.DensScales[aimActive].GetName ());
	if (action & UPDHA_GEOMSCALE)
		m_wndStatusBar.SetPaneText (3, theApp.GeomScales[aimActive].GetName ());
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/*
LONG CMainFrame::OnColorChange(UINT wParam, LONG lParam) {
	theSettings.SetSetting (m_nID, lParam);
	return 0;
}
*/
void CMainFrame::OnEditGeomScales () {
	CGeomProp pageGeom;
	CPropertySheet sheetPref (theApp.LoadString (IDS_PREFERENCES));

	sheetPref.AddPage (&pageGeom);
	sheetPref.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheetPref.DoModal ();
}
void CMainFrame::OnEditDensScales () {
	CDensProp pageDens;
	CPropertySheet sheetPref (theApp.LoadString (IDS_PREFERENCES));

	sheetPref.AddPage (&pageDens);
	sheetPref.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheetPref.DoModal ();
}
void CMainFrame::OnIndGeomScale() { OnEditGeomScales (); }
void CMainFrame::OnIndDensScale() { OnEditDensScales (); }
void CMainFrame::OnUpdate_IndGeomScale(CCmdUI* pCmdUI) { pCmdUI->SetText (theApp.GeomScales[aimActive].GetName()); }
void CMainFrame::OnUpdate_IndDensScale(CCmdUI* pCmdUI) { pCmdUI->SetText (theApp.DensScales[aimActive].GetName()); }
void CMainFrame::OnSelectGeomScale (UINT nID) {	theApp.GeomScales[nID - ID_TOOLS_GEOMSCALE_1].Activate (); }
void CMainFrame::OnSelectDensScale (UINT nID) {	theApp.DensScales[nID - ID_TOOLS_DENSSCALE_1].Activate (); }

void CMainFrame::OnMarkerStyleAdd () {
	CString strText, strTitle;
	CTitlesDlg dlgTitles;
$Repeat:
	if (dlgTitles.DoModal () != IDOK)
		return;
	CMark mrkStyle = theApp.MarkStyles[aimActive].GetStyle ();
	CMarkStyle *pStyle = new CMarkStyle (dlgTitles.m_strTitle, dlgTitles.m_strDescr, mrkStyle);
	if (theApp.MarkStyles.Insert (theApp.MarkStyles[aimActive].GetPos (), *pStyle) == -1) {
		strText.LoadString (IDS_MBTEXT_COLLISION);
		strTitle.LoadString (IDS_MBTITLE_COLLISION);
		if (MessageBox (strText, strTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
			goto $Repeat;
		else
			return;
	}
	Update (UPDHA_MARKSTYLE);
}

void CMainFrame::OnMarkerStyleDelete () {
	CString strText, strTitle;
	if (theApp.MarkStyles[aimActive].GetPos () == 0) {
		strText.LoadString (IDS_MBTEXT_BUILTINDELETION);
		strTitle.LoadString (IDS_MBTITLE_ERROR);
		MessageBox (strText, strTitle, MB_OK | MB_ICONSTOP);
		return;
	}
	strText.LoadString (IDS_MBTEXT_DELETE);
	strTitle.LoadString (IDS_MBTITLE_DELETE);
	if (MessageBox (strText, strTitle, MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	theApp.MarkStyles.Delete (theApp.MarkStyles[aimActive].GetPos ());
	Update (UPDHA_MARKSTYLE);
}

void CMainFrame::OnMarkerStyleExport () {
}

void CMainFrame::OnMarkerStyleImport () {
}

void CMainFrame::OnMarkerStyleRename () {
	CTitlesDlg dlgTitles;
	dlgTitles.m_strTitle = theApp.MarkStyles[aimActive].GetName ();
	dlgTitles.m_strDescr = theApp.MarkStyles[aimActive].GetDescr ();
	if (dlgTitles.DoModal () != IDOK)
		return;
	CMark mrkStyle = theApp.MarkStyles[aimActive].GetStyle ();
	theApp.MarkStyles[aimActive].SetName (dlgTitles.m_strTitle);
	theApp.MarkStyles[aimActive].SetDescr (dlgTitles.m_strDescr);
	Update (UPDHA_MARKSTYLE);
}

void CMainFrame::OnMarkerStyleDuplicate () {
	CString strText, strTitle;
	CTitlesDlg dlgTitles;
	dlgTitles.m_strTitle = theApp.MarkStyles[aimActive].GetName ();
	dlgTitles.m_strDescr = theApp.MarkStyles[aimActive].GetDescr ();
$Repeat:
	if (dlgTitles.DoModal () != IDOK)
		return;
	CMark mrkStyle = theApp.MarkStyles[aimActive].GetStyle ();
	CMarkStyle *pStyle = new CMarkStyle (dlgTitles.m_strTitle, dlgTitles.m_strDescr, mrkStyle);
	if (theApp.MarkStyles.Insert (theApp.MarkStyles[aimActive].GetPos (), *pStyle) == -1) {
		strText.LoadString (IDS_MBTEXT_COLLISION);
		strTitle.LoadString (IDS_MBTITLE_COLLISION);
		if (MessageBox (strText, strTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
			goto $Repeat;
		else
			return;
	}
	Update (UPDHA_MARKSTYLE);
}
