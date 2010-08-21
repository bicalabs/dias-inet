// PaneToolMark.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "PaneView.h"
#include "PaneToolMark.h"


extern int __cdecl sortfn_rtbyte (const void *b1, const void *b2);
// CPaneToolMark dialog

IMPLEMENT_DYNAMIC(CPaneToolMark, CDialog)
CPaneToolMark::CPaneToolMark(CWnd* pParent /*=NULL*/)
	: CDialog(CPaneToolMark::IDD, pParent)
{
}

CPaneToolMark::~CPaneToolMark()
{
}

void CPaneToolMark::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PTM_LIST, m_ctrlList);

	DDX_Control(pDX, IDC_PTM_KERNEL, m_ctrlKernel);
	DDX_Control(pDX, IDC_PTM_SHAPE, m_ctrlShape);
	DDX_Control(pDX, IDC_PTM_OUT_STYLE, m_ctrlOutStyle);
	DDX_Control(pDX, IDC_PTM_OUT_WIDTH, m_ctrlOutWidth);

	DDX_Control(pDX, IDC_PTM_KRNL_COLOR, m_btnKrnlColor);
	DDX_Control(pDX, IDC_PTM_SHP_COLOR, m_btnShpColor);
	DDX_Control(pDX, IDC_PTM_OUT_COLOR, m_btnOutColor);

	DDX_Control(pDX, IDC_PRM_KRNL_SIZE, m_sldKrnlSize);
	DDX_Control(pDX, IDC_PTM_KRNL_TRANSP, m_sldKrnlTransp);
	DDX_Control(pDX, IDC_PTM_SHP_TRANSP, m_sldShpTransp);
	DDX_Control(pDX, IDC_PTM_SHP_FORM, m_sldShpForm);

	DDX_Control(pDX, IDC_PTM_EDT_KSZ, m_edtKrnlSize);
	DDX_Control(pDX, IDC_PTM_EDT_KTP, m_edtKrnlTransp);
	DDX_Control(pDX, IDC_PTM_EDT_STP, m_edtShpTransp);
	DDX_Control(pDX, IDC_PTM_EDT_SFRM, m_edtShpForm);
	DDX_Control(pDX, IDC_PTM_EDT_KOFFX, m_edtKrnlOffX);
	DDX_Control(pDX, IDC_PTM_EDT_KOFFY, m_edtKrnlOffY);
	DDX_Control(pDX, IDC_PTM_EDT_SSZX, m_edtShpSizeX);
	DDX_Control(pDX, IDC_PTM_EDT_SSZY, m_edtShpSizeY);
	DDX_Control(pDX, IDC_PTM_EDT_SROT, m_edtShpAngle);

	DDX_Control(pDX, IDC_PTM_LCK_KSZ, m_lckKrnlOffs);
	DDX_Control(pDX, IDC_PTM_LCK_SSZ, m_lckShpSize);
	DDX_Control(pDX, IDC_PTM_LCK_SROT, m_lckShpAngle);

	DDX_Control(pDX, IDC_PTM_STATIC_KRNL, m_stKrnl);
	DDX_Control(pDX, IDC_PTM_STATIC_KRNL_LINE, m_stKrnlLine);
	DDX_Control(pDX, IDC_PTM_STATIC_SHP, m_stShp);
	DDX_Control(pDX, IDC_PTM_STATIC_SHP_LINE, m_stShpLine);
	DDX_Control(pDX, IDC_PTM_STATIC_SIZE, m_stSize);
	DDX_Control(pDX, IDC_PTM_STATIC_SIZE_LINE, m_stSizeLine);
}


BEGIN_MESSAGE_MAP(CPaneToolMark, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PTM_KRNL_COLOR, OnBnClickedPtmKrnlColor)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PTM_LIST, OnLvnItemchangedPtmList)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPaneToolMark message handlers

void CPaneToolMark::OnSize (UINT nType, int cx, int cy){
	CDialog::OnSize(nType, cx, cy);

	if (!IsWindow (m_ctrlKernel))
		return;

	m_ctrlList.SetWindowPos (NULL, 7, 30, cx - 14, cy - 30 - 500, SWP_NOZORDER);
	m_ctrlList.SetColumnWidth (0, (cx - 14 - 16) * 2 / 5);
	m_ctrlList.SetColumnWidth (1, (cx - 14 - 16) * 3 / 5);

	int k = 490, m = (cx - 14) / 2;
	m_stKrnl.SetWindowPos (NULL, 7, cy - k, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_stKrnlLine.SetWindowPos (NULL, 7, cy - k + 7, cx - 14, 2, SWP_NOZORDER);
	m_ctrlKernel.SetWindowPos (NULL, 7, cy - k + 20, cx - 70, cy - k + 40, SWP_NOZORDER);
	m_ctrlKernel.SetDroppedWidth (cx - 70);
	m_btnKrnlColor.SetWindowPos (NULL, cx - 55, cy - k + 19, 45, 24, SWP_NOZORDER);
	m_sldKrnlSize.SetWindowPos (NULL, 25, cy - k + 50, cx - 80, 20, SWP_NOZORDER);
	m_sldKrnlTransp.SetWindowPos (NULL, 25, cy - k + 75, cx - 80, 20, SWP_NOZORDER);
	m_edtKrnlSize.SetWindowPos (NULL, cx - 45 - 7, cy - k + 52, 45, 16, SWP_NOZORDER);
	m_edtKrnlTransp.SetWindowPos (NULL, cx - 45 - 7, cy - k + 77, 45, 16, SWP_NOZORDER);

	k -= 100;
	m_stShp.SetWindowPos (NULL, 7, cy - k, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_stShpLine.SetWindowPos (NULL, 7, cy - k + 7, cx - 14, 2, SWP_NOZORDER);
	m_ctrlShape.SetWindowPos (NULL, 7, cy - k + 20, cx - 70, 20, SWP_NOZORDER);
	m_ctrlShape.SetDroppedWidth (cx - 70);
	m_btnShpColor.SetWindowPos (NULL, cx - 55, cy - k + 19, 45, 24, SWP_NOZORDER);
	m_btnOutColor.SetWindowPos (NULL, cx - 55, cy - k + 49, 45, 24, SWP_NOZORDER);
	m_ctrlOutStyle.SetWindowPos (NULL, 7, cy - k + 50, (cx - 14 - 47) / 2 - 7, 20, SWP_NOZORDER);
	m_ctrlOutStyle.SetDroppedWidth ((cx - 14 - 47) / 2 - 7);
	m_ctrlOutWidth.SetWindowPos (NULL, (cx - 14 - 47) / 2 + 7, cy - k + 50, (cx - 14 - 47) / 2 - 7, 20, SWP_NOZORDER);
	m_ctrlOutWidth.SetDroppedWidth ((cx - 14 - 47) / 2 - 7);
	m_sldShpTransp.SetWindowPos (NULL, 25, cy - k + 80, cx - 80, 20, SWP_NOZORDER);
	m_sldShpForm.SetWindowPos (NULL, 25, cy - k + 105, cx - 80, 20, SWP_NOZORDER);
	m_edtShpTransp.SetWindowPos (NULL, cx - 45 - 7, cy - k + 82, 45, 16, SWP_NOZORDER);
	m_edtShpForm.SetWindowPos (NULL, cx - 45 - 7, cy - k + 107, 45, 16, SWP_NOZORDER);

	k -= 130;
	m_stSize.SetWindowPos (NULL, 7, cy - k, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_stSizeLine.SetWindowPos (NULL, 7, cy - k + 7, cx - 14, 2, SWP_NOZORDER);
	m_edtKrnlOffX.SetWindowPos (NULL, 7 + 20, cy - k + 20, m - 7 - 40, 16, SWP_NOZORDER);
	m_edtKrnlOffY.SetWindowPos (NULL, 7 + 20, cy - k + 40, m - 7 - 40, 16, SWP_NOZORDER);
	m_edtShpSizeX.SetWindowPos (NULL, m + 7 + 20 + 5, cy - k + 20, m - 7 - 40, 16, SWP_NOZORDER);
	m_edtShpSizeY.SetWindowPos (NULL, m + 7 + 20 + 5, cy - k + 40, m - 7 - 40, 16, SWP_NOZORDER);
	m_edtShpAngle.SetWindowPos (NULL, m / 2 + 7 + 20 + 3, cy - k + 60, m - 7 - 40 + 3, 16, SWP_NOZORDER);

	m_lckKrnlOffs.SetWindowPos (NULL, m - 7 - 40 + 27 + 4, cy - k + 30, 16, 16, SWP_NOZORDER);
	m_lckShpSize.SetWindowPos (NULL, 2 * m - 40 + 27 + 4, cy - k + 30, 16, 16, SWP_NOZORDER);
	m_lckShpAngle.SetWindowPos (NULL, m / 2 + m - 10, cy - k + 60, 16, 16, SWP_NOZORDER);
}

void CPaneToolMark::OnPaint () {
	CRect rcClient;
	CBrush brFrame (::GetSysColor (COLOR_BTNSHADOW));
	CPaintDC dc(this);

	GetClientRect (&rcClient);
	dc.FillSolidRect (rcClient, ::GetSysColor (COLOR_BTNFACE));

	int k = 490, cy = rcClient.Height (), m = (rcClient.Width () - 14) / 2;

	rcClient.top = rcClient.bottom - 175;
	rcClient.DeflateRect (7, 7);
	dc.FillSolidRect (rcClient, ::GetSysColor (COLOR_3DLIGHT));
	dc.FrameRect (&rcClient, &brFrame);

	CPen penHist (PS_SOLID, 1, ::GetSysColor (COLOR_BTNSHADOW)),
		penMed (PS_SOLID, 1, RGB (0x00, 0x00, 0xC0)),
		*penOld;
	penOld = dc.SelectObject (&penHist);

	CStatistics stat;
	if (theApp.Documents.GetCount() != 0 && 
		theApp.Documents[aimActive].Images.GetCount() != 0 &&
		theApp.Documents[aimActive].Images(aimActive).GetCount() != 0) {

		theApp.Documents[aimActive].Images[aimActive].Channels[0].Statistics (stat);
		rcClient.DeflateRect (2,2,2,2);
		dc.MoveTo (rcClient.left, rcClient.bottom);
		for (uint n = 0; n < 0x100; ++n)
			dc.LineTo (rcClient.left + rcClient.Width () * n / 0xFF, rcClient.bottom - rcClient.Height () * (stat.Freq[n] / stat.Freq[udword(stat.Moda)]));

		dc.SelectObject (&penMed);
		rtbyte rFreq[0x100];
		rtbyte rFreqMed[0x100];
		dc.MoveTo (rcClient.left, rcClient.bottom);
		for (n = 3; n < 0x100 - 3; ++n) {
			memcpy (rFreq, stat.Freq, sizeof(rtbyte)*0xFF);
			qsort (&(rFreq[n - 3]), 7, sizeof (rtbyte), sortfn_rtbyte);
			rFreqMed[n] = rFreq[n];
			dc.LineTo (rcClient.left + rcClient.Width () * n / 0xFF, rcClient.bottom - rcClient.Height () * (rFreq[n] / stat.Freq[udword(stat.Moda)]));
		}
	}

	dc.DrawIcon (7, cy - k + 52, theApp.LoadIcon (IDI_PIC_SIZE));
	dc.DrawIcon (7, cy - k + 77, theApp.LoadIcon (IDI_PIC_TRANSPARENT));
	k -= 100;
	dc.DrawIcon (7, cy - k + 82, theApp.LoadIcon (IDI_PIC_TRANSPARENT));
	dc.DrawIcon (7, cy - k + 107, theApp.LoadIcon (IDI_PIC_FORMFACTOR));
	k -= 130;
	dc.DrawIcon (7, cy - k + 20, theApp.LoadIcon (IDI_PIC_OFFX));
	dc.DrawIcon (7, cy - k + 40, theApp.LoadIcon (IDI_PIC_OFFY));
	dc.DrawIcon (m + 7 + 5, cy - k + 20, theApp.LoadIcon (IDI_PIC_SIZEX));
	dc.DrawIcon (m + 7 + 5, cy - k + 40, theApp.LoadIcon (IDI_PIC_SIZEY));
	dc.DrawIcon (m / 2 + 7 + 3, cy - k + 60, theApp.LoadIcon (IDI_PIC_ROTATE));

	dc.SelectObject (penOld);
}

int CPaneToolMark::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CPaneToolMark::OnInitDialog () {
	CDialog::OnInitDialog();

	m_ilKernels.Create (16, 16, ILC_COLOR8 | ILC_MASK, 3, 1);
	m_ilKernels.Add (theApp.LoadBitmap (IDB_IL_MARKERKERNELS), 0xFFFFFF);

	m_ctrlKernel.SetExtendedStyle (CBES_EX_NOSIZELIMIT, CBES_EX_NOSIZELIMIT);
	m_ctrlKernel.SetImageList (&m_ilKernels);

	CString str;
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;

	for (int i = 0; i < 6; ++i) {
		item.iItem = i;
		str.LoadString (IDS_MARKER_KERNEL_NONE + i);
		item.pszText = (LPTSTR)(LPCTSTR)str;
		item.cchTextMax = str.GetLength ();
		item.iImage = item.iSelectedImage = item.iOverlay = i;
		m_ctrlKernel.InsertItem (&item);
	}

	m_ctrlKernel.SetCurSel (1);

	m_ctrlList.DeleteColumn (1);
	m_ctrlList.DeleteColumn (0);
	m_ctrlList.InsertColumn (0, theApp.LoadString (IDS_MRKSTYLE_NAME), LVCFMT_LEFT, 75, 0);
	m_ctrlList.InsertColumn (1, theApp.LoadString (IDS_MRKSTYLE_DESCR), LVCFMT_LEFT, 125, 0);
	m_ctrlList.SetExtendedStyle (LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_LABELTIP | LVS_EX_UNDERLINEHOT);

	if (aimThemeLoaded) {
		m_btnKrnlColor.m_hTheme = aimThemeOpen (m_btnKrnlColor.m_hWnd, L"Button");
		m_btnShpColor.m_hTheme = aimThemeOpen (m_btnShpColor.m_hWnd, L"Button");
		m_btnOutColor.m_hTheme = aimThemeOpen (m_btnOutColor.m_hWnd, L"Button");
	}

	Update ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPaneToolMark::Update () {
	m_ctrlList.DeleteAllItems ();
	for (uvar32_64 n = 0, cnt = theApp.MarkStyles.GetCount (); n < cnt; ++n) {
		m_ctrlList.InsertItem (n, theApp.MarkStyles[n].GetName (), 0);
		m_ctrlList.SetItemText (n, 1, theApp.MarkStyles[n].GetDescr ());
	}
	m_ctrlList.SetItemState (theApp.MarkStyles[aimActive].GetPos (), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctrlList.EnsureVisible (theApp.MarkStyles[aimActive].GetPos (), false);
	RedrawWindow ();
}

void CPaneToolMark::OnBnClickedPtmKrnlColor () {
	// TODO: Add your control notification handler code here
}

void CPaneToolMark::OnLvnItemchangedPtmList (NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_ctrlList.GetSelectionMark () != -1 && aimGetSelListItem (m_ctrlList, false) != -1 &&
	    aimGetSelListItem (m_ctrlList, false) != theApp.MarkStyles[aimActive].GetPos ()) {
		theApp.MarkStyles.Select (m_ctrlList.GetItemText (aimGetSelListItem (m_ctrlList, false), 0));
		theApp.Update (UPDHA_MARKSTYLE);
	}

	*pResult = 0;
}

void CPaneToolMark::OnMouseMove (UINT nFlags, CPoint point) {
	if (m_btnKrnlColor.m_blHover) {
		m_btnKrnlColor.m_blHover = false;
		m_btnKrnlColor.Invalidate (false);
	}
	if (m_btnShpColor.m_blHover) {
		m_btnShpColor.m_blHover = false;
		m_btnShpColor.Invalidate (false);
	}
	if (m_btnOutColor.m_blHover) {
		m_btnOutColor.m_blHover = false;
		m_btnOutColor.Invalidate (false);
	}
	((CPaneView*)GetParent())->OnMouseMove (nFlags, point);
	CDialog::OnMouseMove(nFlags, point);
}
