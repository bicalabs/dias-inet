// DensProp.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "DensProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDensProp property page

IMPLEMENT_DYNCREATE(CDensProp, CPropertyPage)

CDensProp::CDensProp() : CPropertyPage(CDensProp::IDD)
{
	//{{AFX_DATA_INIT(CDensProp)
	m_rA = 0.0;
	m_rB = 0.0;
	m_strdescr = _T("");
	m_strName = _T("");
	m_strUnits = _T("");
	//}}AFX_DATA_INIT
}

CDensProp::~CDensProp()
{
}

void CDensProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDensProp)
	DDX_Control(pDX, IDC_DS_UNITS, m_ctrlUnits);
	DDX_Control(pDX, IDC_DS_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_DS_DESCR, m_ctrlDescr);
	DDX_Control(pDX, IDC_DS_B, m_ctrlB);
	DDX_Control(pDX, IDC_DS_A, m_ctrlA);
	DDX_Control(pDX, IDC_DS_TYPES, m_ctrlTypes);
	DDX_Control(pDX, IDC_DS_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_DS_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_DS_LIST, m_ctrlList);
	DDX_Text(pDX, IDC_DS_A, m_rA);
	DDX_Text(pDX, IDC_DS_B, m_rB);
	DDX_Text(pDX, IDC_DS_DESCR, m_strdescr);
	DDX_Text(pDX, IDC_DS_NAME, m_strName);
	DDX_Text(pDX, IDC_DS_UNITS, m_strUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDensProp, CPropertyPage)
	//{{AFX_MSG_MAP(CDensProp)
	ON_EN_CHANGE(IDC_DS_A, OnChangeA)
	ON_EN_CHANGE(IDC_DS_B, OnChangeB)
	ON_EN_CHANGE(IDC_DS_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_DS_DESCR, OnChangeDescr)
	ON_EN_CHANGE(IDC_DS_UNITS, OnChangeUnits)
	ON_BN_CLICKED(IDC_DS_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DS_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_DS_SELECT, OnSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DS_LIST, OnItemChangedList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DS_TYPES, OnItemChangedTypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDensProp message handlers

BOOL CDensProp::OnInitDialog () {
	CPropertyPage::OnInitDialog();

	m_ilScales.Create (24, 24, ILC_COLOR8 | ILC_MASK, 4, 1);
	m_ilScales.Add (theApp.LoadBitmap (IDB_DENSSCALES), 0xFFFFFF);
	m_ctrlList.SetImageList (&m_ilScales, LVSIL_SMALL);
	m_ctrlList.InsertColumn (0, theApp.LoadString (IDS_DENS_COL_NAME), LVCFMT_LEFT, 140, 0);	
	m_ctrlList.InsertColumn (1, theApp.LoadString (IDS_DENS_COL_COEF1), LVCFMT_RIGHT, 50, 0);	
	m_ctrlList.InsertColumn (2, theApp.LoadString (IDS_DENS_COL_COEF2), LVCFMT_RIGHT, 50, 0);	
	m_ctrlList.InsertColumn (3, theApp.LoadString (IDS_DENS_COL_UNITS), LVCFMT_LEFT, 45, 0);	
	m_ctrlList.InsertColumn (4, theApp.LoadString (IDS_DENS_COL_DESCRIPT), LVCFMT_LEFT, 200, 0);
	m_ctrlList.SetExtendedStyle (LVS_EX_FULLROWSELECT);

	m_ilTypes.Create (32, 16, ILC_COLOR8 | ILC_MASK, 4, 1);
	m_ilTypes.Add (theApp.LoadBitmap (IDB_DENSTYPES), 0xFFFFFF);
	m_ctrlTypes.SetImageList (&m_ilTypes, LVSIL_NORMAL);
	m_ctrlTypes.InsertItem (0, theApp.LoadString (IDS_DENS_TYPE_LINEAR), 0);
	m_ctrlTypes.InsertItem (1, theApp.LoadString (IDS_DENS_TYPE_POLY), 1);
	m_ctrlTypes.InsertItem (2, theApp.LoadString (IDS_DENS_TYPE_EXP), 2);
	m_ctrlTypes.InsertItem (3, theApp.LoadString (IDS_DENS_TYPE_LOG), 3);

	UpdateList ();

	m_ctrlName.EnableWindow (false);
	m_ctrlDescr.EnableWindow (false);
	m_ctrlUnits.EnableWindow (false);
	m_ctrlA.EnableWindow (false);
	m_ctrlB.EnableWindow (false);
	m_ctrlTypes.EnableWindow (false);
	m_btnRemove.EnableWindow (false);
	m_btnSelect.EnableWindow (false);
	m_strName = m_strdescr = m_strUnits = "";
	m_ctrlA.SetWindowText ("");
	m_ctrlB.SetWindowText ("");
	UpdateData (false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDensProp::OnOK () {
	CPropertyPage::OnOK();
}

BOOL CDensProp::OnApply () {
	return CPropertyPage::OnApply();
}

void CDensProp::OnCancel () {
	CPropertyPage::OnCancel();
}

void CDensProp::OnReset () {
	CPropertyPage::OnReset();
}

void CDensProp::OnChangeA () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	CString strFormat;
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetCoefs (m_rA, m_rB);
	strFormat.Format ("%.4f", m_rA);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 1, strFormat);
}

void CDensProp::OnChangeB () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	CString strFormat;
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetCoefs (m_rA, m_rB);
	strFormat.Format ("%.4f", m_rB);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 2, strFormat);
}

void CDensProp::OnChangeName () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetName (m_strName);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 0, m_strName);
}

void CDensProp::OnChangeDescr () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetDescr (m_strdescr);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 4, m_strdescr);
}

void CDensProp::OnChangeUnits () {
	if (aimGetSelListItem (m_ctrlList, false) == -1)
		return;
	UpdateData (true);
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetUnits (m_strUnits);
	m_ctrlList.SetItemText (aimGetSelListItem (m_ctrlList, false), 3, m_strUnits);
}

void CDensProp::OnItemChangedTypes (NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int item = aimGetSelListItem (m_ctrlTypes, false);
	if (item == -1)
		return;

	UpdateData (true);
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].SetCoefs (DensScale(item), m_rA, m_rB);
	m_ctrlList.SetItem (aimGetSelListItem (m_ctrlList, false), 0, LVIF_IMAGE, "", item, 0, 0, 0);

	*pResult = 0;
}

void CDensProp::OnItemChangedList (NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int item = aimGetSelListItem (m_ctrlList, false);
	m_ctrlName.EnableWindow (item != -1);
	m_ctrlDescr.EnableWindow (item != -1);
	m_ctrlUnits.EnableWindow (item != -1);
	m_ctrlA.EnableWindow (item != -1);
	m_ctrlB.EnableWindow (item != -1);
	m_ctrlTypes.EnableWindow (item != -1);
	m_btnRemove.EnableWindow (item != -1);
	m_btnSelect.EnableWindow (item != -1);
	if (item == -1) {
		m_strName = m_strdescr = m_strUnits = "";
		m_ctrlA.SetWindowText ("");
		m_ctrlB.SetWindowText ("");
	} else {
		m_strName = theApp.DensScales[item].GetName ();
		m_strdescr = theApp.DensScales[item].GetDescr ();
		m_strUnits = theApp.DensScales[item].GetUnits ();
		m_rA = theApp.DensScales[item].GetCoef1 ();
		m_rB = theApp.DensScales[item].GetCoef2 ();
		UpdateData (false);
		m_ctrlTypes.SetItemState (int(theApp.DensScales[item].GetScale ()), LVIS_SELECTED, LVIS_SELECTED);
	}
	UpdateData (false);
	*pResult = 0;
}

void CDensProp::OnAdd () {
	uvar32_64 pos = theApp.DensScales.Add (CDensScale ("", "", "", scaleLinear, 1, 0));
	UpdateList ();
	m_ctrlList.SetItemState (pos, LVIS_SELECTED, LVIS_SELECTED);
	m_ctrlList.EnsureVisible (pos, false);
}

void CDensProp::OnRemove () {
	theApp.DensScales.Delete (aimGetSelListItem (m_ctrlList, false));
	UpdateList ();
	m_ctrlName.EnableWindow (false);
	m_ctrlDescr.EnableWindow (false);
	m_ctrlUnits.EnableWindow (false);
	m_ctrlA.EnableWindow (false);
	m_ctrlB.EnableWindow (false);
	m_ctrlTypes.EnableWindow (false);
	m_btnRemove.EnableWindow (false);
	m_btnSelect.EnableWindow (false);
	m_strName = m_strdescr = m_strUnits = "";
	m_ctrlA.SetWindowText ("");
	m_ctrlB.SetWindowText ("");
	UpdateData (false);
}

void CDensProp::OnSelect () {
	theApp.DensScales[aimGetSelListItem (m_ctrlList, false)].Activate ();
}

void CDensProp::UpdateList () {
	CString strFormat;
	m_ctrlList.DeleteAllItems ();
	for (uvar32_64 n = 0; n < theApp.DensScales.GetCount (); ++n) {
		m_ctrlList.InsertItem (n, theApp.DensScales[n].GetName (), (int)(theApp.DensScales[n].GetScale()));
		strFormat.Format ("%.4f", theApp.DensScales[n].GetCoef1 ());
		m_ctrlList.SetItemText (n, 1, strFormat);
		strFormat.Format ("%.4f", theApp.DensScales[n].GetCoef2 ());
		m_ctrlList.SetItemText (n, 2, strFormat);
		m_ctrlList.SetItemText (n, 3, theApp.DensScales[n].GetUnits ());
		m_ctrlList.SetItemText (n, 4, theApp.DensScales[n].GetDescr ());
	}
}
