#include "stdafx.h"
#include "Dias.h"
#include "Settings.h"
//#include "DiasKs.h"
#include "DocList.h"
#include "MainFrame.h"
#include "Mark.h"
#include "Image.h"
#include "Survey.h"
#include "Report.h"
#include "ImageWnd.h"
#include "DataWnd.h"
#include "ReportWnd.h"
#include "StorageFrame.h"
#include "StorageView.h"
#include "StorageDoc.h"
#include "Splash.h"
#include "CmdLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDiasApp	theApp;
//CDiasKs		theKs;
CTesla		theTesla;

BEGIN_MESSAGE_MAP(CDiasApp, CWinApp)
	//{{AFX_MSG_MAP(CDiasApp)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDiasApp::CDiasApp () {
}

BOOL CDiasApp::InitInstance () {
/*	if (!AfxSocketInit ()) {
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}*/

	AfxEnableControlContainer();
	AfxOleInit ();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey (_T(theApp.LoadString (IDS_REG_ROOT)));
	LoadStdProfileSettings ();  // Load standard INI file options (including MRU)
	if (!theSettings.Load ())
		return (false);

	CCmdLine cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	aimThemeInit ();

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DISTYPE,
		RUNTIME_CLASS(CStorageDoc),
		RUNTIME_CLASS(CStorageFrame),
		RUNTIME_CLASS(CStorageView));
	AddDocTemplate(pDocTemplate);

	m_pMainFrame = new CMainFrame;
	if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = m_pMainFrame;

	m_pMainWnd->DragAcceptFiles();

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	m_pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainFrame->UpdateWindow();

	Update (0xFFFFFFFFL);

	cmdInfo.ExecuteParams ();

//	if (theKs.Connect ())
//		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_DIASKS_NO), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONWARNING);

	// #### TODO: Add registry setting check for default action

	return TRUE;
}

int CDiasApp::ExitInstance () {
	for (int i = 0 ; i < m_bmpaLoad.GetSize () ; ++i)
		delete m_bmpaLoad[i];
	m_bmpaLoad.RemoveAll ();

	if (Documents.m_pHashHead) {
		Documents.m_pHashHead->RemoveTail ();
		Documents.m_pHashHead->Remove ();
		delete Documents.m_pHashHead;
	}

	aimThemeFinalize ();

//	theKs.Disconnect ();
	theSettings.Save ();
	return CWinApp::ExitInstance();
}

CString CDiasApp::LoadString (UINT nID) {
	CString strLoad;
	strLoad.LoadString (nID);
	return (_T(strLoad));
}

CBitmap* CDiasApp::LoadBitmap (UINT nID) {
	CBitmap *bmp = new CBitmap;
	bmp->LoadBitmap (nID);
	m_bmpaLoad.Add (bmp);
	return (bmp);
}

void CDiasApp::Update (udword action) {
	m_pMainFrame->Update (action);
}

void CDiasApp::MacroStart (CString strName) {
	m_pMainFrame->m_wndStatusBar.SetPaneText (10, strName);
	m_pMainFrame->m_wndStatusBar.GetStatusBarCtrl().SetIcon (9, theApp.LoadIcon (IDI_INDICATOR_PRG_START));
}

void CDiasApp::MacroStop () {
	CString strText;
	m_pMainFrame->m_wndStatusBar.SetPaneText (10, "");
	m_pMainFrame->m_wndStatusBar.GetStatusBarCtrl().SetIcon (9, theApp.LoadIcon (IDI_INDICATOR_PRG_STOP));
	strText.LoadString (IDS_STATUS_READY);
	StatusText (strText);
}

void CDiasApp::MacroPause () {
	m_pMainFrame->m_wndStatusBar.GetStatusBarCtrl().SetIcon (9, theApp.LoadIcon (IDI_INDICATOR_PRG_PAUSE));
}

void CDiasApp::StatusText (CString strText) {
	m_pMainFrame->m_wndStatusBar.SetWindowText (strText);
}

void CDiasApp::StatusState (UINT nID) {
	if (nID == 0)
		m_pMainFrame->m_wndStatusBar.GetStatusBarCtrl().SetIcon (6, NULL);
	else
		m_pMainFrame->m_wndStatusBar.GetStatusBarCtrl().SetIcon (6, theApp.LoadIcon (nID));
}

void CDiasApp::ObjectsAdd (udword type, CObject* obj) {
	CString strText;
	if (type > OBJT_FIELD)
		return;
	strText.Format ("%d", ++m_nObjects[type]);
	if (m_pMainFrame != NULL)
		m_pMainFrame->m_wndStatusBar.SetPaneText (10 + type * 2, strText, TRUE);
}

void CDiasApp::ObjectsRemove (udword type, CObject* obj) {
	CString strText;
	if (type > OBJT_FIELD || m_nObjects[type] == 0)
		return;
	strText.Format ("%d", --m_nObjects[type]);
	if (m_pMainFrame != NULL && IsWindow (m_pMainFrame->m_hWnd) && IsWindow (m_pMainFrame->m_wndStatusBar.m_hWnd))
		m_pMainFrame->m_wndStatusBar.SetPaneText (10 + type * 2, strText, TRUE);
}

void CDiasApp::PrepareMenu (CaImMenu* pMenu, UINT nID) {
	CMenu mn, *pM;
	CaImMenuItem** ppItems;
	UINT n, count, state;
	CString strMenu;

	mn.LoadMenu (nID);
	pM = mn.GetSubMenu(0);
	count = pM->GetMenuItemCount ();
	ppItems = new CaImMenuItem*[count];

	pMenu->CreatePopupMenu ();
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
		pMenu->AppendMenu (state | MF_OWNERDRAW, pM->GetMenuItemID (n), (LPCTSTR)ppItems[n]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog {
public:
	CAboutDlg();
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog (CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDiasApp::OnAppAbout () {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDiasApp message handlers

BOOL CDiasApp::PreTranslateMessage (MSG* pMsg) {
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;
	return CWinApp::PreTranslateMessage(pMsg);
}
