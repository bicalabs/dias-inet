// ..\..\code\dias-customer\PaneView.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "MainFrame.h"
#include "PaneView.h"
#include ".\paneview.h"


// CPaneView

IMPLEMENT_DYNAMIC(CPaneView, CWnd)
CPaneView::CPaneView () {
}

CPaneView::~CPaneView () {
}


BEGIN_MESSAGE_MAP(CPaneView, CWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



BOOL CPaneView::Create (CWnd* pParentWnd) {
	m_pFrame = (CMainFrame*)pParentWnd;
	m_dwSelect = 0;
	m_dwClose = 0;
	m_blSize = false;
	if (aimThemeLoaded)
		m_hTheme = aimThemeOpen (this->m_hWnd, L"Button");
	return CWnd::Create(NULL, "", WS_CHILD | WS_VISIBLE, CRect (0, 0, 100, 100), pParentWnd, 0, NULL);
}

// CPaneView message handlers


void CPaneView::OnPaint () {
	CPen* pPen;
	CFont* pFont;
	CFont* pFnt;

	CPen penBlackBold (PS_SOLID, 2, ::GetSysColor (COLOR_BTNTEXT));
	CFont fntDef;
	CRect rcClient, rcGripper, rcTitle, rcTitleText, rcCtrlIco, rcCtrlImg, rcPane, rcSplit;
	CPaintDC dc (this);

	pPen = dc.GetCurrentPen ();

	LOGFONT lfDef;
	pFont = (CFont *)dc.SelectStockObject (ANSI_VAR_FONT);
	pFnt = dc.GetCurrentFont ();
	pFnt->GetLogFont (&lfDef);
	lfDef.lfWeight = FW_BOLD;
	fntDef.CreateFontIndirect (&lfDef);
	dc.SelectObject (&fntDef);

	GetClientRect (&rcClient);
	rcSplit = rcClient;
	rcSplit.right = 7;
	rcSplit.left += 2;
	dc.FillSolidRect (rcClient, ::GetSysColor (COLOR_BTNFACE));
	dc.FillSolidRect (rcSplit, ::GetSysColor (COLOR_BTNSHADOW));
	dc.Draw3dRect (rcSplit, ::GetSysColor (COLOR_BTNSHADOW), ::GetSysColor (COLOR_BTNHILIGHT));

	rcClient.left += rcSplit.right;

	rcPane = rcClient;
	rcPane.DeflateRect (4, 20, 4, 4);
//	dc.FillSolidRect (rcPane, ::GetSysColor (COLOR_BTNHILIGHT));

	rcGripper = rcPane;
	rcGripper = rcPane;
	rcGripper.top = 0;
	rcGripper.bottom = rcGripper.top + 20;
	rcTitle = rcGripper;
	rcTitle.right -= 18;
	rcTitleText = rcTitle;
	rcTitleText.DeflateRect (6, 2, 6, 2);
	if (m_dwSelect != 0)
		aimThemeBackground (m_hTheme, dc, BP_PUSHBUTTON, m_dwSelect, &rcTitle, NULL);
	else
		dc.FillSolidRect (&rcTitle, GetSysColor (COLOR_BTNFACE));
	CString strTitle = L"Working Pane";
	aimThemeText (m_hTheme, dc, BP_PUSHBUTTON, m_dwSelect, L"Working Pane", strTitle.GetLength (), DT_LEFT | DT_EXPANDTABS | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER, 0, rcTitleText);
	//dc.ExtTextOut (rcTitleText.left + 2, rcTitleText.top + 1, ETO_CLIPPED, rcTitleText, "Working Pane", NULL);
	//dc.DrawText ("Working Pane", rcTitleText, DT_LEFT | DT_EXPANDTABS | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);
	dc.DrawIcon (rcTitleText.right - 12, rcTitleText.top, AfxGetApp()->LoadIcon (232));

	rcCtrlIco = rcGripper;
	rcCtrlIco.DeflateRect (0, 0, 2, 2);
	rcCtrlIco.left = rcCtrlIco.right - 16;
	rcCtrlImg = rcCtrlIco;
	rcCtrlIco.right += 2;
	rcCtrlImg.DeflateRect (5, 7, 4, 4);
	rcCtrlImg.OffsetRect (0, -2);
	if (m_dwClose != 0)
		aimThemeBackground (m_hTheme, dc, BP_PUSHBUTTON, m_dwClose, &rcCtrlIco, NULL);
	else
		dc.FillSolidRect (&rcCtrlIco, GetSysColor (COLOR_BTNFACE));
	dc.SelectObject (&penBlackBold);
	dc.MoveTo (rcCtrlImg.TopLeft ());
	dc.LineTo (rcCtrlImg.BottomRight ());
	dc.MoveTo (rcCtrlImg.right, rcCtrlImg.top);
	dc.LineTo (rcCtrlImg.left, rcCtrlImg.bottom);

//	rcCtrlIco.left += 16;
//	rcCtrlIco.right += 16;

	dc.SelectObject (pPen);
	dc.SelectObject (pFont);
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CPaneView::OnSetCursor (CWnd* pWnd, UINT nHitTest, UINT message) {
	if (nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	HCURSOR cursor = theApp.LoadStandardCursor (IDC_ARROW);
	SetCursor (cursor);
}

void CPaneView::OnMouseMove (UINT nFlags, CPoint point) {
	udword dwSelect, dwClose;
	CRect rcClose, rcSelect, rcClient, rcGripper;
	HCURSOR cursor;

	GetClientRect (&rcClient);
	rcClient.left += 7;
	rcGripper = rcClient;
	rcGripper.DeflateRect (4, 0, 4, 4);
	rcGripper.top = 0;
	rcGripper.bottom = 20;
	rcSelect = rcGripper;
	rcSelect.right -= 18;
	rcClose = rcGripper;
	rcClose.left = rcClose.right - 15;

	if (GetCapture () != this) {
		if (point.x < 7)
			cursor = theApp.LoadCursor (AFX_IDC_HSPLITBAR);
		else
			cursor = theApp.LoadStandardCursor (IDC_ARROW);
		SetCursor (cursor);

		dwSelect = m_dwSelect;
		dwClose = m_dwClose;
		m_dwSelect = rcSelect.PtInRect (point) ? PBS_NORMAL : 0;
		m_dwClose = rcClose.PtInRect (point) ? PBS_NORMAL : 0;
		if (dwSelect != m_dwSelect || dwClose != m_dwClose)
			RedrawWindow (rcGripper, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	} else if (m_blSize) {
		if (point.x == 0)
			return;

		CRect rcWin;
		GetClientRect (&rcWin);

		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof (rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE;
		m_pFrame->m_wndPane.GetReBarCtrl().GetBandInfo (0, &rbbi);
		rbbi.cyChild -= point.x;
		m_pFrame->m_wndPane.GetReBarCtrl().SetBandInfo (0, &rbbi);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CPaneView::OnLButtonDown (UINT nFlags, CPoint point) {
	udword dwSelect, dwClose;
	CRect rcClose, rcSelect, rcClient, rcGripper;

	GetClientRect (&rcClient);
	rcClient.left += 7;
	rcGripper = rcClient;
	rcGripper.DeflateRect (4, 0, 4, 4);
	rcGripper.bottom = rcGripper.top + 20;
	rcSelect = rcGripper;
	rcSelect.right -= 18;
	rcClose = rcGripper;
	rcClose.left = rcClose.right - 15;

	if (point.x < 7)
		m_blSize = true;
	SetCapture ();

	dwSelect = m_dwSelect;
	dwClose = m_dwClose;
	m_dwSelect = rcSelect.PtInRect (point) ? PBS_PRESSED : m_dwSelect;
	m_dwClose = rcClose.PtInRect (point) ? PBS_PRESSED : m_dwClose;
	if (dwSelect != m_dwSelect || dwClose != m_dwClose)
		RedrawWindow (rcGripper, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	CWnd::OnLButtonDown(nFlags, point);
}

void CPaneView::OnLButtonUp (UINT nFlags, CPoint point) {
	if (GetCapture () != this) {
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	m_blSize = false;
	ReleaseCapture ();

	CRect rcWnd, rcClose, rcSelect, rcClient, rcGripper;
	GetWindowRect (&rcWnd);
	GetClientRect (&rcClient);
	rcClient.left += 7;
	rcGripper = rcClient;
	rcGripper.DeflateRect (4, 0, 4, 4);
	rcGripper.bottom = rcGripper.top + 20;
	rcSelect = rcGripper;
	rcSelect.right -= 18;
	rcClose = rcGripper;
	rcClose.left = rcClose.right - 15;

	TPMPARAMS tpm;
	tpm.cbSize = sizeof (TPMPARAMS);
	tpm.rcExclude.left = rcGripper.left;
	tpm.rcExclude.top = rcGripper.top;
	tpm.rcExclude.right = rcGripper.right;
	tpm.rcExclude.bottom = rcGripper.bottom;
	if (m_dwSelect == PBS_PRESSED) {
		CaImMenu menu;
		theApp.PrepareMenu (&menu, IDR_POPUP_TOOLPANE);
		menu.TrackPopupMenuEx (TPM_LEFTALIGN | TPM_TOPALIGN, rcWnd.left + rcSelect.left, rcWnd.top + rcSelect.bottom, this, &tpm);
		m_dwSelect = 0;
		RedrawWindow (rcGripper, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CPaneView::OnSize (UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);

	m_pnToolMark.SetWindowPos (NULL, 7, 20, cx - 7, cy - 20, SWP_NOZORDER);
}

int CPaneView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pnToolMark.Create (IDD_PANE_MARKERS, this);

	return 0;
}
