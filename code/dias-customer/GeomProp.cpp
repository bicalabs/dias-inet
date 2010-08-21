// GeomProp.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "GeomProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeomProp property page

IMPLEMENT_DYNCREATE(CGeomProp, CPropertyPage)

CGeomProp::CGeomProp() : CPropertyPage(CGeomProp::IDD)
{
	//{{AFX_DATA_INIT(CGeomProp)
	m_rA = 0.0;
	m_rB = 0.0;
	m_strdescr = _T("");
	m_strName = _T("");
	m_strUnits = _T("");
	//}}AFX_DATA_INIT
}

CGeomProp::~CGeomProp()
{
}

void CGeomProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeomProp)
	DDX_Control(pDX, IDC_GS_UNITS, m_ctrlUnits);
	DDX_Control(pDX, IDC_GS_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_GS_DESCR, m_ctrlDescr);
	DDX_Control(pDX, IDC_GS_B, m_ctrlB);
	DDX_Control(pDX, IDC_GS_A, m_ctrlA);
	DDX_Control(pDX, IDC_GS_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_GS_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_GS_LIST, m_ctrlList);
	DDX_Text(pDX, IDC_GS_A, m_rA);
	DDX_Text(pDX, IDC_GS_B, m_rB);
	DDX_Text(pDX, IDC_GS_DESCR, m_strdescr);
	DDX_Text(pDX, IDC_GS_NAME, m_strName);
	DDX_Text(pDX, IDC_GS_UNITS, m_strUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeomProp, CPropertyPage)
	//{{AFX_MSG_MAP(CGeomProp)
	ON_EN_CHANGE(IDC_GS_A, OnChangeA)
	ON_EN_CHANGE(IDC_GS_B, OnChangeB)
	ON_EN_CHANGE(IDC_GS_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_GS_DESCR, OnChangeDescr)
	ON_EN_CHANGE(IDC_GS_UNITS, OnChangeUnits)
	ON_BN_CLICKED(IDC_GS_ADD, OnAdd)
	ON_BN_CLICKED(IDC_GS_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_GS_SELECT, OnSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GS_LIST, OnItemChangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeomProp message handlers

BOOL CGeomProp::OnInitDialog () {
	CPropertyPage::OnInitDialog();

	m_ilScales.Create (24, 24, ILC_COLOR8 | ILC_MASK, 1, 1);
	m_ilScales.Add (theApp.LoadBitmap (IDB_GEOMSCALES), 0xFFFFFF);
	m_ctrlList.SetImageList (&m_ilScales, LVSIL_SMALL);
	m_ctrlList.InsertColumn (0, theApp.LoadString (IDS_DENS_COL_NAME), LVCFMT_LEFT, 140, 0);	
	m_ctrlList.InsertColumn (1, theApp.LoadString (IDS_DENS_COL_COEF1), LVCFMT_RIGHT, 50, 0);	
	m_ctrlList.InsertColumn (2, theApp.LoadString (IDS_DENS_COL_COEF2), LVCFMT_RIGHT, 50, 0);	
	m_ctrlList.InsertColumn (3, theApp.LoadString (IDS_DENS_COL_UNITS), LVCFMT_LEFT, 45, 0);	
	m_ctrlList.InsertColumn (4, theApp.LoadString (IDS_DENS_COL_DESCRIPT), LVCFMT_LEFT, 200, 0);
	m_ctrlList.SetExtendedStyle (LVS_EX_FULLROWSELECT);

	UpdateList ();

	m_ctrlName.EnableWindow (false);
	m_ctrlDescr.EnableWindow (false);
	m_ctrlUnits.EnableWindow (false);
	m_ctrlA.EnableWindow (false);
	m_ctrlB.EnableWindow (false);
	m_btnRemove.EnableWindow (false);
	m_btnSelect.EnableWindow (false);
	m_strName = m_strdescr = m_strUnits = "";
	m_ctrlA.SetWindowText ("");
	m_ctrlB.SetWindowText ("");
	UpdateData (false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeomProp::OnOK () {
	CPropertyPage::OnOK();
}

BOOL CGeomProp::OnApply () {
	return CPropertyPage::OnApply();
}

void CGeomProp::OnCancel () {
	CPropertyPage::OnCancel();
}

void CGeomProp::OnReset () {
	CPropertyPage::OnReset();
}

void CGeomProp::OnChangeA () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	CString strFormat;
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].SetCoefs (m_rA, m_rB);
	strFormat.Format ("%.4f", m_rA);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 1, strFormat);
}

void CGeomProp::OnChangeB () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	CString strFormat;
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].SetCoefs (m_rA, m_rB);
	strFormat.Format ("%.4f", m_rB);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 2, strFormat);
}

void CGeomProp::OnChangeName () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].SetName (m_strName);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 0, m_strName);
}

void CGeomProp::OnChangeDescr () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].SetDescr (m_strdescr);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 4, m_strdescr);
}

void CGeomProp::OnChangeUnits () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].SetUnits (m_strUnits);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 3, m_strUnits);
}

void CGeomProp::OnItemChangedList (NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int item = aimGetSelListItem (m_ctrlList, false);
	m_ctrlName.EnableWindow (item != -1);
	m_ctrlDescr.EnableWindow (item != -1);
	m_ctrlUnits.EnableWindow (item != -1);
	m_ctrlA.EnableWindow (item != -1);
	m_ctrlB.EnableWindow (item != -1);
	m_btnRemove.EnableWindow (item != -1);
	m_btnSelect.EnableWindow (item != -1);
	if (item == -1) {
		m_strName = m_strdescr = m_strUnits = "";
		m_ctrlA.SetWindowText ("");
		m_ctrlB.SetWindowText ("");
	} else {
		m_strName = theApp.GeomScales[item].GetName ();
		m_strdescr = theApp.GeomScales[item].GetDescr ();
		m_strUnits = theApp.GeomScales[item].GetUnits ();
		m_rA = theApp.GeomScales[item].GetCoefX ();
		m_rB = theApp.GeomScales[item].GetCoefY ();
		UpdateData (false);
	}
	UpdateData (false);
	*pResult = 0;
}

void CGeomProp::OnAdd () {
	uvar32_64 pos = theApp.GeomScales.Add (CGeomScale ("", "", "", 1, 1));
	UpdateList ();
	m_ctrlList.SetItemState (pos, LVIS_SELECTED, LVIS_SELECTED);
	m_ctrlList.EnsureVisible (pos, false);
}

void CGeomProp::OnRemove () {
	theApp.GeomScales.Delete (aimGetSelListItem (m_ctrlList, false));
	UpdateList ();
	m_ctrlName.EnableWindow (false);
	m_ctrlDescr.EnableWindow (false);
	m_ctrlUnits.EnableWindow (false);
	m_ctrlA.EnableWindow (false);
	m_ctrlB.EnableWindow (false);
	m_btnRemove.EnableWindow (false);
	m_btnSelect.EnableWindow (false);
	m_strName = m_strdescr = m_strUnits = "";
	m_ctrlA.SetWindowText ("");
	m_ctrlB.SetWindowText ("");
	UpdateData (false);
}

void CGeomProp::OnSelect () {
	theApp.GeomScales[aimGetSelListItem (m_ctrlList, false)].Activate ();
}

void CGeomProp::UpdateList () {
	CString strFormat;
	m_ctrlList.DeleteAllItems ();
	for (uvar32_64 n = 0; n < theApp.GeomScales.GetCount (); ++n) {
		m_ctrlList.InsertItem (n, theApp.GeomScales[n].GetName (), 0);
		strFormat.Format ("%.4f", theApp.GeomScales[n].GetCoefX ());
		m_ctrlList.SetItemText (n, 1, strFormat);
		strFormat.Format ("%.4f", theApp.GeomScales[n].GetCoefY ());
		m_ctrlList.SetItemText (n, 2, strFormat);
		m_ctrlList.SetItemText (n, 3, theApp.GeomScales[n].GetUnits ());
		m_ctrlList.SetItemText (n, 4, theApp.GeomScales[n].GetDescr ());
	}
}
