// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dias-devel.h"
#include "ImageCtrl.h"
#include "MainDlg.h"
#include "AutoProxy.h"
#include "SaxHandlers.h"
#include ".\maindlg.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#endif*/


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMainDlg dialog


ulong CMainDlg::m_nMemUsed = 0;


IMPLEMENT_DYNAMIC(CMainDlg, CDialog);
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMainDlg::IDD, pParent) , m_blPreview(FALSE)
{
	dias_fctsys = new ksmtx_factors;
	dias_ordsys = new ksmtx_order;
	memset (dias_fctsys, 0, sizeof (ksmtx_factors));
	memset (dias_ordsys, 0, sizeof (ksmtx_order));

	m_pEdit = new CEdit;
	m_pCombo = new CComboBox;
	m_pComboFct = new CComboBox;
	m_pComboOrd = new CComboBox;

	m_hImg1 = m_hImg2 = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CMainDlg::~CMainDlg () {
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	delete m_pEdit;
	delete m_pCombo;
	delete m_pComboFct;
	delete m_pComboOrd;

	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FLT_FUNCTIONS, m_ctrlFuncs);
	DDX_Control(pDX, IDC_FLT_PARAM, m_ctrlArgs);
	DDX_Control(pDX, IDC_FLT_IMG1WHO, m_cmbI1W);
	DDX_Control(pDX, IDC_FLT_IMG1NAME, m_ctrlI1N);
	DDX_Control(pDX, IDC_FLT_IMG2WHO, m_cmbI2W);
	DDX_Control(pDX, IDC_FLT_IMG2NAME, m_ctrlI2N);
	DDX_Control(pDX, IDC_FLT_IMG1, m_ctrlImg1);
	DDX_Control(pDX, IDC_FLT_IMG2, m_ctrlImg2);
	DDX_Control(pDX, IDC_FLT_HISTORY, m_ctrlMacro);
	DDX_Check(pDX, IDC_FLT_AUTOPREVIEW, m_blPreview);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_FLT_FUNCTIONS, OnFuncs_SelChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FLT_PARAM, OnArgs_ItemChanged)
	ON_NOTIFY(NM_CLICK, IDC_FLT_PARAM, OnArgs_Click)
	ON_NOTIFY(NM_DBLCLK, IDC_FLT_PARAM, OnArgs_DblClick)
	ON_COMMAND(ID_VIEW_REFRESH, OnRefresh)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FLT_IMG1NAME, OnImg1_ItemChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FLT_IMG2NAME, OnImg2_ItemChanged)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_FLT_IMG1NAME, OnImg1_ItemActivated)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_FLT_IMG2NAME, OnImg2_ItemActivated)
	ON_BN_CLICKED(IDC_FLT_EXEC, OnExecute)
	ON_WM_CHAR()
	ON_NOTIFY(NM_CLICK, IDC_FLT_IMG1NAME, OnImg1_Click)
	ON_NOTIFY(NM_CLICK, IDC_FLT_IMG2NAME, OnImg2_Click)
END_MESSAGE_MAP()


// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog () {
	CDialog::OnInitDialog ();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlIl.Create (16, 16, ILC_COLOR32 | ILC_MASK, 0, 4);
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_SYS));		//0
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_CLS));		//1
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_SUB));		//2
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_DIV));		//3
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_CMD));		//4
	m_ctrlIl.Add (theApp.LoadIcon (IDI_CMD_CMDX));		//5
	m_ctrlIl.Add (theApp.LoadIcon (IDI_NOWAY));		//6
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_STR));		//7
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_INT));		//8
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_REAL));		//9
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_MATRIX1));	//10
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_MATRIX2));	//11
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_BOOL));		//12
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ARG_COLOR));		//13
	m_ctrlIl.Add (theApp.LoadIcon (IDI_OBJ));		//14
	m_ctrlIl.Add (theApp.LoadIcon (IDI_OBJ_STOR));		//15
	m_ctrlIl.Add (theApp.LoadIcon (IDI_OBJ_IMG));		//16
	m_ctrlIl.Add (theApp.LoadIcon (IDI_OBJ_MARK));		//17
	m_ctrlIl.Add (theApp.LoadIcon (IDI_OBJ_LUT));		//18
	m_ctrlIl.Add (theApp.LoadIcon (IDI_SUCCESS));		//19
	m_ctrlIl.Add (theApp.LoadIcon (IDI_FAIL));		//20
	m_ctrlIl.Add (theApp.LoadIcon (IDI_ENTRY));		//21

	m_pEdit->Create (ES_LEFT | WS_CHILD | WS_BORDER, CRect (0, 0, 0, 0), this, 0);
	m_pCombo->Create (CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_BORDER, CRect (0, 0, 0, 0), this, 0);
	m_pComboFct->Create (CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_BORDER, CRect (0, 0, 0, 0), this, 0);
	m_pComboOrd->Create (CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_BORDER, CRect (0, 0, 0, 0), this, 0);
	m_pEdit->SetFont (m_ctrlArgs.GetFont ());
	m_pCombo->SetFont (m_ctrlArgs.GetFont ());
	m_pComboFct->SetFont (m_ctrlArgs.GetFont ());
	m_pComboOrd->SetFont (m_ctrlArgs.GetFont ());
	m_iEditItem = -1;

	m_ctrlFuncs.SetImageList (&m_ctrlIl, TVSIL_NORMAL);
	m_ctrlArgs.SetImageList (&m_ctrlIl, LVSIL_SMALL);
	m_ctrlI1N.SetImageList (&m_ctrlIl, LVSIL_SMALL);
	m_ctrlI2N.SetImageList (&m_ctrlIl, LVSIL_SMALL);

	m_ctrlArgs.InsertColumn (0, "Argument", LVCFMT_RIGHT, 150);
	m_ctrlArgs.InsertColumn (1, "Value", LVCFMT_LEFT, 150);
	m_ctrlI1N.InsertColumn (0, "Title", LVCFMT_LEFT, 200);
	m_ctrlI2N.InsertColumn (0, "Title", LVCFMT_LEFT, 200);

	CoInitialize (NULL);
	dias_cmdsys = new kscmd;

	ISAXXMLReaderPtr pSaxReader;
	HRESULT hr = pSaxReader.CreateInstance (__uuidof(SAXXMLReader));
	if (FAILED(hr)) return FALSE;

	CSaxContent* pSaxContent = new CSaxContent (&m_ctrlFuncs, dias_cmdsys, dias_fctsys, dias_ordsys);
	CSaxError* pSaxErrors = new CSaxError();
	hr = pSaxReader->putContentHandler (pSaxContent);
	hr = pSaxReader->putErrorHandler (pSaxErrors);
	hr = pSaxReader->putBaseURL ((ushort *)L"http://127.0.0.1/devel/");
	hr = pSaxReader->parseURL ((ushort *)L"dias_cmdsys.xml");
	hr = pSaxReader->parseURL ((ushort *)L"dias_mtxsys.xml");
	pSaxReader.Release ();

	CString strName;
	m_pComboFct->ResetContent ();
	for (ksmtx_factors* fc = dias_fctsys->next; fc; fc = fc->next) {
		strName.Format ("%s (%dx%d)", fc->name, fc->dimx, fc->dimy);
		m_pComboFct->AddString (strName);
	}
	m_pComboOrd->ResetContent ();
	for (ksmtx_order* or = dias_ordsys->next; or; or = or->next) {
		strName.Format ("%s (%dx%d)", or->name, or->dimx, or->dimy);
		m_pComboOrd->AddString (strName);
	}

//	m_ctrlFuncs.SelectItem (TVI_ROOT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMainDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint () {
	if (IsIconic ()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CMainDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CMainDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CMainDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CMainDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CMainDlg::OnFuncs_SelChanged (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM itemNew = pNMTreeView->itemNew.hItem,
		  itemOld = pNMTreeView->itemOld.hItem;
	*pResult = 0;

	m_ctrlFuncs.SetItemState (itemOld, 0, TVIS_BOLD);
	kscmd* cmd = (kscmd*)m_ctrlFuncs.GetItemData (itemNew);
	while (m_ctrlFuncs.GetChildItem (itemNew) && !((kscmd*)m_ctrlFuncs.GetItemData (itemNew))->title) {
		itemNew = m_ctrlFuncs.GetChildItem (itemNew);
		cmd = (kscmd*)m_ctrlFuncs.GetItemData (itemNew);
		m_ctrlFuncs.Expand (itemNew, TVE_EXPAND);
	}
	RefreshArgs ();
	if (!cmd || !cmd->title || !cmd->args) 	return;
	m_ctrlFuncs.SetItemState (itemNew, TVIS_BOLD, TVIS_BOLD);
	m_ctrlFuncs.SelectItem (itemNew);
}

void CMainDlg::RefreshArgs () {
	int i, item;
	m_ctrlArgs.DeleteAllItems ();
	kscmd* cmd = (kscmd*)m_ctrlFuncs.GetItemData (m_ctrlFuncs.GetSelectedItem ());
	if (!cmd || !cmd->title || !cmd->args)	return;
	ksarg* arg = (ksarg*)bdlist_head (cmd->args);
	for (i = 0, arg = arg->next; arg; arg = arg->next, ++i) {
		item = m_ctrlArgs.InsertItem (i, arg->name);
		if (arg->gentype == KSARGT_INT) {
			if (arg->subtype == DARGST_GREYVAL)	arg->str.Format ("0x%02X", arg->val.n);
			else if (arg->subtype == DARGST_BOOL)	arg->str = (arg->val.n == 0 ? "< FALSE >":"< TRUE >");
			else if ((arg->subtype & (DOBJT_IMAGE << 16)) == (DOBJT_IMAGE << 16)) {
				if (arg->val.n == 0)	arg->str = "< New image >";
				else if (arg->val.n == 1)	arg->str = "< Image from pane #1 >";
				else if (arg->val.n == 2)	arg->str = "< Image from pane #2 >";
				else if (arg->val.n < m_ctrlI1N.GetItemCount () + 3)	arg->str = m_ctrlI1N.GetItemText (arg->val.n - 3, 0);
				else arg->str = "~<# No image #>~";
			} else		arg->str.Format ("%d", arg->val.n);
		}
		else if (arg->gentype == KSARGT_REAL)
			arg->str.Format ("%.4f", arg->val.r);
		else if (arg->gentype == KSARGT_STRING)
			arg->str = arg->val.str;
		else
			arg->str = "~<# Unknown type #>~";
		m_ctrlArgs.SetItemText (item, 1, arg->str);
		m_ctrlArgs.SetItemData (item, (DWORD_PTR)arg);
	}
}

void CMainDlg::OnDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
	if (GetDlgItem (nIDCtl) != &m_ctrlArgs) {
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
		return;
	}

	int i = m_ctrlArgs.GetSelectionMark (), iIcon;

	CDC*	pDC = new CDC;
	CPen	penGird ( PS_SOLID, 1, RGB ( 0xC0, 0xC0, 0xC0 ) );
	CRect	rectArg = lpDrawItemStruct->rcItem;
	CRect	rectVal = lpDrawItemStruct->rcItem;
	CRect	rectClient;
	CRect	rectBox;
	ksarg*  arg = (ksarg*)m_ctrlArgs.GetItemData (lpDrawItemStruct->itemID);
	m_ctrlArgs.GetClientRect ( &rectClient );
	rectArg.left += 24;
	rectVal.left += 156;
	rectVal.right = rectClient.right;
	lpDrawItemStruct->rcItem.left += 4;

	pDC->Attach (lpDrawItemStruct->hDC);

	if (i == (int)lpDrawItemStruct->itemID) {
		pDC->FillSolidRect (&rectArg, ::GetSysColor (COLOR_HIGHLIGHT));
		pDC->DrawFocusRect (&rectArg);
		pDC->SetTextColor (::GetSysColor (COLOR_HIGHLIGHTTEXT));
	}
	else {
		pDC->FillSolidRect ( &rectArg, m_ctrlArgs.GetBkColor ());
		pDC->SetTextColor (::GetSysColor (COLOR_WINDOWTEXT));
	}

	rectArg += CPoint (2, 2);
	pDC->DrawText (arg->name, &rectArg, DT_VCENTER | DT_LEFT );
	rectArg.left += 4;
	if (arg->gentype == KSARGT_INT && arg->subtype == DARGST_GREYVAL) {
		iIcon = 13;
		rectBox = rectVal;
		rectBox.DeflateRect (0, 1, 0, 1);
		rectBox.right = rectBox.left + 15;
		rectVal.left += 20;
		pDC->FillSolidRect (&rectBox, RGB (arg->val.n, arg->val.n, arg->val.n));
		pDC->DrawEdge (&rectBox, BDR_RAISEDOUTER, BF_RECT);
		pDC->SetTextColor (RGB (0x00, 0x00, 0x00));
	} else if (arg->gentype == KSARGT_INT && arg->subtype == DARGST_BOOL) {
		iIcon = 12;
		pDC->SetTextColor (arg->val.n == 0 ? RGB (0xFF, 0x00, 0x00) : RGB (0x00, 0x00, 0xFF));
	} else if (arg->gentype == KSARGT_INT && (arg->subtype & (DOBJT_IMAGE << 16)) == (DOBJT_IMAGE << 16)) {
		iIcon = 16;
		pDC->SetTextColor (RGB (0x7F, 0x7F, 0x7F));
	} else {
		iIcon = 7 + arg->gentype;
		pDC->SetTextColor (::GetSysColor (COLOR_WINDOWTEXT));
	}
	rectVal += CPoint (2, 2);
	if (i == (int)lpDrawItemStruct->itemID)
		pDC->SetTextColor (::GetSysColor (COLOR_HIGHLIGHTTEXT));
	m_ctrlIl.Draw (pDC, iIcon, CPoint(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top),
		       ILD_TRANSPARENT | ((i == (int)lpDrawItemStruct->itemID)?ILD_SELECTED:0));
	pDC->DrawText (arg->str, &rectVal, DT_VCENTER | DT_LEFT );

	pDC->SelectObject (&penGird);
	pDC->MoveTo (150, 0);
	pDC->LineTo (150, rectClient.Height ( ));
	pDC->Detach ();


/*	if (i == (int)lpDrawItemStruct->itemID) {
		pDC->FillSolidRect (&rectArg, ::GetSysColor (COLOR_HIGHLIGHT));
		pDC->DrawFocusRect (&rectArg);
	}
	else*/
//	if (i == (int)lpDrawItemStruct->itemID)
//		pDC->SetTextColor ( ::GetSysColor ( COLOR_HIGHLIGHTTEXT ) );
//	if (i == (int)lpDrawItemStruct->itemID)
//		pDC->SetTextColor (::GetSysColor (COLOR_HIGHLIGHTTEXT));
}

void CMainDlg::OnArgs_ItemChanged (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	m_ctrlArgs.RedrawWindow ();
	*pResult = 0;
}

void CMainDlg::OnArgs_Click(NMHDR *pNMHDR, LRESULT *pResult) {
	int i;
	char* str;
	CString	strText;
	UpdateData (TRUE);

	if (pResult != NULL) *pResult = 0;
	if (m_iEditItem == -1) {
		m_ctrlArgs.RedrawItems (0, m_ctrlArgs.GetItemCount () - 1);
		m_ctrlArgs.RedrawWindow ();
		OnArgs_DblClick (pNMHDR, pResult);
		return;
	}
	int iSelected = m_ctrlArgs.GetSelectionMark ();
	ksarg* arg = (ksarg*)m_ctrlArgs.GetItemData (m_iEditItem);

	if ((arg->subtype & (DOBJT_IMAGE << 16)) == (DOBJT_IMAGE << 16)) {
		m_pCombo->ModifyStyle (WS_VISIBLE, 0);
		if (pResult != NULL)
			arg->val.n = m_pCombo->GetCurSel ();
	}
	else if (arg->gentype == KSARGT_INT && arg->subtype == DARGST_BOOL) {
		arg->val.n = 1 - arg->val.n;
	}
	else if (arg->gentype == KSARGT_STRING && arg->subtype == DARGST_MTX_FACTORS) {
		m_pComboFct->ModifyStyle (WS_VISIBLE, 0);
		if (!pResult)
			goto $end;
		if ((i = m_pComboFct->GetCurSel ()) == -1)
			goto $end;
		ksmtx_factors *fc = (ksmtx_factors*)bdlist_get (dias_fctsys, i + 1);
		arg->val.size = fc->dimx * fc->dimy * sizeof(rtbyte);
		arg->val.ptr = fc->factors;
		m_pComboFct->GetWindowText (arg->val.str);
	}
	else if (arg->gentype == KSARGT_STRING && arg->subtype == DARGST_MTX_OFFS) {
		m_pComboOrd->ModifyStyle (WS_VISIBLE, 0);
		if (!pResult)
			goto $end;
		if ((i = m_pComboOrd->GetCurSel ()) == -1)
			goto $end;
		ksmtx_order *fc = (ksmtx_order*)bdlist_get (dias_ordsys, i + 1);
		arg->val.size = fc->dimx * fc->dimy * sizeof(sdword);
		arg->val.ptr = fc->order;
		m_pComboOrd->GetWindowText (arg->val.str);
	}
	else {
		m_pEdit->GetWindowText (strText);
		m_pEdit->ModifyStyle (WS_VISIBLE, 0);
		str = (char *)LPCTSTR (strText);
		if (arg->gentype == KSARGT_INT && arg->subtype == DARGST_GREYVAL)	arg->val.n = strtol (strText, &str, 16);
		else if (arg->gentype == KSARGT_INT ) arg->val.n = atoi (str);
		else if (arg->gentype == KSARGT_REAL)	arg->val.r = atof (strText);
		else if (arg->gentype == KSARGT_STRING)	arg->val.str = strText;
	}

$end:
	m_iEditItem = -1;
	RefreshArgs ();
	OnArgs_DblClick (pNMHDR, pResult);
}

void CMainDlg::OnArgs_DblClick(NMHDR *pNMHDR, LRESULT *pResult) {
	*pResult = 0;
	CRect	rectItem;
	CRect	rectArgs;
	CRect	rectClient;
	CRect	rectDialog;
	CString	strValue;

	int iSelected = m_ctrlArgs.GetSelectionMark ();
	if (iSelected == -1) return;
	ksarg* arg = (ksarg*)m_ctrlArgs.GetItemData (iSelected);

	GetWindowRect (&rectDialog);
	m_ctrlArgs.GetClientRect (&rectClient);
	m_ctrlArgs.GetWindowRect (&rectArgs);
	m_ctrlArgs.GetItemRect (m_iEditItem = m_ctrlArgs.GetSelectionMark (), &rectItem, LVIR_BOUNDS );
	rectItem.top	+= rectArgs.top - rectDialog.top - 48;
	rectItem.bottom	+= rectArgs.top - rectDialog.top - 48;
	rectItem.left	+= 151 + rectArgs.left - rectDialog.left;
	rectItem.right	+= rectArgs.left - rectDialog.left - 1;

	if (arg->gentype == KSARGT_INT && (arg->subtype & (DOBJT_IMAGE << 16)) == (DOBJT_IMAGE << 16)) {
		rectItem.top -= 1;
		rectItem.bottom = rectItem.top + 150;
		//rectItem.left -= 2;
		//rectItem.right += 2;
		m_pCombo->SetWindowPos (&m_ctrlArgs, rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), SWP_SHOWWINDOW);
		m_pCombo->ModifyStyle (0, WS_VISIBLE);
		m_pCombo->SetFocus ();
		m_pCombo->ShowDropDown ();
		m_iEditItem = iSelected;
	}
	else if (arg->gentype == KSARGT_INT && arg->subtype == DARGST_BOOL) {
		//rectItem.top -= 2;
		rectItem.bottom  = rectItem.top + 80;
		//rectItem.left -= 2;
		//rectItem.right += 2;
		//arg->val.n = 1 - arg->val.n;
		//RefreshArgs ();
	}
	else if (arg->gentype == KSARGT_STRING && arg->subtype == DARGST_MTX_FACTORS) {
		rectItem.bottom = rectItem.top + 150;
		m_pComboFct->SetWindowPos (&m_ctrlArgs, rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), SWP_SHOWWINDOW);
		m_pComboFct->ModifyStyle (0, WS_VISIBLE);
		m_pComboFct->SetFocus ();
		m_pComboFct->ShowDropDown ();
		m_iEditItem = iSelected;
	}
	else if (arg->gentype == KSARGT_STRING && arg->subtype == DARGST_MTX_OFFS) {
		rectItem.bottom = rectItem.top + 150;
		m_pComboOrd->SetWindowPos (&m_ctrlArgs, rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), SWP_SHOWWINDOW);
		m_pComboOrd->ModifyStyle (0, WS_VISIBLE);
		m_pComboOrd->SetFocus ();
		m_pComboOrd->ShowDropDown ();
		m_iEditItem = iSelected;
	}
	else {
		m_pEdit->SetWindowText (arg->str);
		m_pEdit->SetWindowPos (&m_ctrlArgs, rectItem.left, rectItem.top + 3, rectItem.Width(), rectItem.Height(), SWP_SHOWWINDOW);
		m_pEdit->ModifyStyleEx (0, WS_EX_TOPMOST );
		m_pEdit->SetFocus ();
		m_iEditItem = iSelected;
	}
}

void CMainDlg::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	// TODO: Add your message handler code here and/or call default

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainDlg::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	// TODO: Add your message handler code here and/or call default

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMainDlg::OnImg1_ItemChanged (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CMainDlg::OnImg1Changed () {
	int i;
	sdword himg;
	ulong dimx, dimy, dimz;

	if (m_ctrlI1N.GetSelectionMark () == -1)
		return;
	himg = m_ctrlI1N.GetItemData (m_ctrlI1N.GetSelectionMark ());

	if (!theApp.m_kstp.Communicate (DCMD_OBJGET_IMAGE, "i", himg))
		return;

	if (m_ctrlImg1.m_pobjImage) {
		for (i = 0; i < m_ctrlImg1.m_pobjImage->dimz; ++i)
			delete [] m_ctrlImg1.m_pobjImage->layers[i];
		delete [] m_ctrlImg1.m_pobjImage->layers;
		delete m_ctrlImg1.m_pobjImage;
	}

	m_ctrlImg1.m_pobjImage = (dksobj_image*)New(sizeof(dksobj_image));
	memcpy (m_ctrlImg1.m_pobjImage, theApp.m_kstp[0](""), sizeof (dksobj_image));
	dimx = m_ctrlImg1.m_pobjImage->dimx;
	dimy = m_ctrlImg1.m_pobjImage->dimy;
	dimz = m_ctrlImg1.m_pobjImage->dimz;
	m_ctrlImg1.m_pobjImage->layers = (ubyte **)New (sizeof (ubyte*) * dimz);
	for (i = 0; i < dimz; ++i) {
		m_ctrlImg1.m_pobjImage->layers[i] = (ubyte*)New (sizeof (ubyte) * dimx * dimy);
		memcpy (m_ctrlImg1.m_pobjImage->layers[i], theApp.m_kstp[1 + i](""), dimx * dimy);
	}
	m_hImg1 = himg;
	m_ctrlImg1.CreateImage (NULL);
	m_ctrlImg1.Invalidate (false);
}

void CMainDlg::OnImg2_ItemChanged (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	if (!(pNMLV->uChanged & LVIF_STATE))
		return;
	if (pNMLV->iItem != m_ctrlI2N.GetSelectionMark ())
		return;
}

void CMainDlg::OnImg2Changed () {
	int i;
	sdword himg;
	ulong dimx, dimy, dimz;

	if (m_ctrlI2N.GetSelectionMark () == -1)
		return;
	himg = m_ctrlI2N.GetItemData (m_ctrlI2N.GetSelectionMark ());

	if (!theApp.m_kstp.Communicate (DCMD_OBJGET_IMAGE, "i", himg))
		return;

	if (m_ctrlImg2.m_pobjImage) {
		for (i = 0; i < m_ctrlImg2.m_pobjImage->dimz; ++i)
			delete [] m_ctrlImg2.m_pobjImage->layers[i];
		delete [] m_ctrlImg2.m_pobjImage->layers;
		delete m_ctrlImg2.m_pobjImage;
	}

	m_ctrlImg2.m_pobjImage = (dksobj_image*)New (sizeof (dksobj_image));
	memcpy (m_ctrlImg2.m_pobjImage, theApp.m_kstp[0](""), sizeof (dksobj_image));
	dimx = m_ctrlImg2.m_pobjImage->dimx;
	dimy = m_ctrlImg2.m_pobjImage->dimy;
	dimz = m_ctrlImg2.m_pobjImage->dimz;
	m_ctrlImg2.m_pobjImage->layers = (ubyte**)New (sizeof (ubyte*) * dimz);
	for (i = 0; i < dimz; ++i) {
		m_ctrlImg2.m_pobjImage->layers[i] = (ubyte*)New (sizeof (ubyte) * dimx * dimy);
		memcpy (m_ctrlImg2.m_pobjImage->layers[i], theApp.m_kstp[1 + i](""), dimx * dimy);
	}
	m_hImg2 = himg;
	m_ctrlImg2.CreateImage (NULL);
	m_ctrlImg2.Invalidate (false);
}

void CMainDlg::OnImg1_ItemActivated (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

}

void CMainDlg::OnImg2_ItemActivated (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}

void CMainDlg::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	if ((VkKeyScan (nChar) & 0xFF) == VK_RETURN && m_iEditItem != -1) {
		LRESULT r;
		OnArgs_Click (NULL, &r);
		return;
	}
	else if ((VkKeyScan (nChar) & 0xFF) == VK_ESCAPE && m_iEditItem != -1) {
		OnArgs_Click (NULL, NULL);
		return;
	}
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CMainDlg::OnImg1_Click(NMHDR *pNMHDR, LRESULT *pResult) {
	OnImg1Changed ();
	*pResult = 0;
}

void CMainDlg::OnImg2_Click(NMHDR *pNMHDR, LRESULT *pResult) {
	OnImg2Changed ();
	*pResult = 0;
}

void CMainDlg::OnExecute () {
	int argc, i;
	kscmd* cmd;
	ksarg* arg;
	ksarg* ret;
	bool blRefresh = false;

	cmd = (kscmd *)m_ctrlFuncs.GetItemData (m_ctrlFuncs.GetSelectedItem ());
	argc = m_ctrlArgs.GetItemCount ();

	theApp.m_kstp.SetCmd (cmd->code);
	theApp.m_kstp.Alloc (argc);

	for (i = 0; i < argc; ++i) {
		arg = (ksarg*)m_ctrlArgs.GetItemData (i);
		if (arg->gentype == KSARGT_INT && (arg->subtype & (DOBJT_IMAGE << 16)) == (DOBJT_IMAGE << 16)) {
			if (arg->val.n == 0) {
				theApp.m_kstp[i].SetInt (m_hImg1);
				blRefresh = true;
			} else if (arg->val.n < 3)
				theApp.m_kstp[i].SetInt (arg->val.n == 1 ? m_hImg1 : m_hImg2);
			else
				theApp.m_kstp[i].SetInt (m_ctrlI1N.GetItemData (arg->val.n - 3));
		} else if (arg->gentype == KSARGT_INT)
			theApp.m_kstp[i].SetInt (arg->val.n);
		else if (arg->gentype == KSARGT_REAL)
			theApp.m_kstp[i].SetReal (arg->val.r);
		else if (arg->gentype == KSARGT_STRING && (arg->subtype == DARGST_MTX_FACTORS || arg->subtype == DARGST_MTX_OFFS))
			theApp.m_kstp[i].SetBin ((ubyte*)arg->val.ptr, arg->val.size);
		else if (arg->gentype == KSARGT_STRING)
			theApp.m_kstp[i].SetBin ((char *)LPCTSTR(arg->val.str));
	}

	if (!theApp.m_kstp.Process ())
		return;

	
	CString msg;
	if (theApp.m_kstp() > 0) {
		for (i = 0, ret = cmd->rets->next; i < theApp.m_kstp() && ret; ++i, ret = ret->next) {
			if (theApp.m_kstp[i].Type () == e_kstp_int && ret->gentype == KSARGT_INT)
				msg.AppendFormat ("%s:\t0x%08X\t(%d)\n", ret->title, theApp.m_kstp[i](), theApp.m_kstp[i]());
			else if (theApp.m_kstp[i].Type () == e_kstp_real && ret->gentype == KSARGT_REAL)
				msg.AppendFormat ("%s:\t%f\t(0x%08X)\n", ret->title, theApp.m_kstp[i](.0), int(theApp.m_kstp[i](.0)));
			else if (theApp.m_kstp[i].Type () == e_kstp_bin && ret->gentype == KSARGT_STRING)
				msg.AppendFormat ("%s:\t'%s'\n", ret->title, theApp.m_kstp[i](""));
		}
	}
	if (blRefresh)
		OnRefresh ();
	else {
		for (int i = 0; i < m_ctrlI1N.GetItemCount (); ++i)
			theApp.m_kstp.Communicate (KSCMD_SAVE, "i", m_ctrlI1N.GetItemData (i));
		OnImg1Changed ();
		OnImg2Changed ();
	}
	if (msg != "")
		::MessageBox (NULL, msg, "DIAS-KS return values", MB_OK | MB_ICONINFORMATION);
}

void CMainDlg::OnRefresh () {
	int nOldI1, nOldI2;
	CString msg, strTitle, strFile;
	if (!theApp.m_hobj) {
		msg.LoadString (IDS_MBC_GEN);
		MessageBox ("There is no selected storage. Use File|Connect, File|Upload and File|Select Storage commands", msg, MB_OK | MB_ICONSTOP);
		return;
	}

/*!!!!!	for (int i = 0; i < m_ctrlI1N.GetItemCount (); ++i) {
		theApp.m_kstp.Communicate (KSCMD_SAVE, "i", m_ctrlI1N.GetItemData (i));
		if (!theApp.m_kstp.Communicate (KSCMD_CLOSE, "i", m_ctrlI1N.GetItemData (i)))
			return;
	}*/
	nOldI1 = m_ctrlI1N.GetSelectionMark ();
	nOldI2 = m_ctrlI2N.GetSelectionMark ();
	m_ctrlI1N.DeleteAllItems ();
	m_ctrlI2N.DeleteAllItems ();
	m_pCombo->ResetContent ();

	if (!theApp.m_kstp.Communicate (DCMD_OBJGET_STORAGE, "i", theApp.m_hobj))
		return;

	CKstp kstp;
	udword himg;
	dksobj_storage* stor = (dksobj_storage*)New (sizeof (dksobj_storage));
	memcpy (stor, theApp.m_kstp[0](""), theApp.m_kstp[0].Size ());
	udword count = stor->img_count;

	m_pCombo->InsertString (0, "< New image >");
	m_pCombo->InsertString (1, "< Image from pane #1 >");
	m_pCombo->InsertString (2, "< Image from pane #2 >");
	for (int i = 0; i < count; ++i) {
		strFile = theApp.m_kstp[i + 1]("");
		strTitle = strFile.Right (strFile.GetLength () - strFile.ReverseFind ('/') - 1);
		if (!kstp.Communicate (KSCMD_OPEN, "sii", LPCTSTR(strFile), 0777, 0777))
			return;
		himg = kstp[0]();
		m_ctrlI1N.InsertItem (i, strTitle, 14);
		m_ctrlI2N.InsertItem (i, strTitle, 14);
		m_ctrlI1N.SetItemData (i, himg);
		m_ctrlI2N.SetItemData (i, himg);
		m_pCombo->InsertString (i + 3, m_ctrlI1N.GetItemText (i, 0));
	}
	delete stor;

	m_ctrlI1N.SetItemState (nOldI1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_ctrlI2N.SetItemState (nOldI2, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	OnImg1Changed ();
	OnImg2Changed ();
	RefreshArgs ();
}
