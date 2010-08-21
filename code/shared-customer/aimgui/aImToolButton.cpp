#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include "aimtoolbutton.h"


// CaImToolButton

IMPLEMENT_DYNAMIC (CaImToolButton, CButton)
CaImToolButton::CaImToolButton () {
	m_hTheme = NULL;
	m_blHover = false;
	m_blPressed = false;
}

CaImToolButton::~CaImToolButton () {
}


BEGIN_MESSAGE_MAP (CaImToolButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP ()



// CaImToolButton message handlers


void CaImToolButton::DrawItem (LPDRAWITEMSTRUCT lpDIS) {
	CDC dcMem;
	CDC dc;
	CRect rcClient;
	CBrush brFrame (GetSysColor (COLOR_BTNSHADOW));
	CBrush brColFrm (GetSysColor (COLOR_3DSHADOW));

	dc.Attach (lpDIS->hDC);
	dcMem.CreateCompatibleDC (&dc);

	GetClientRect (&rcClient);

	SetBkMode(dc, TRANSPARENT);
	if (aimThemeLoaded) {
		if (!m_hTheme)	m_hTheme = aimThemeOpen (m_hWnd, L"Toolbar");
		DWORD state = (m_blPressed) ? TS_HOTCHECKED : TS_HOT;
		aimThemeBackground (m_hTheme, dc, TP_BUTTON, state, &rcClient, NULL);
	}
	else {
		dc.FillSolidRect (rcClient, GetSysColor (COLOR_3DLIGHT));
		dc.FrameRect (&rcClient, &brFrame);
	}

	rcClient.DeflateRect (3, 3, 3, 3);
	dcMem.SelectObject (m_bmpTool);
	dc.BitBlt (rcClient.left, rcClient.top, rcClient.Width (), rcClient.Height (), &dcMem, 0, 0, SRCCOPY);

	dcMem.DeleteDC ();
	dc.Detach ();
}

void CaImToolButton::OnMouseMove (UINT nFlags, CPoint point) {
	if (!m_blHover) {
		m_blHover = true;
		Invalidate (false);
	}
	CButton::OnMouseMove(nFlags, point);
}

void CaImToolButton::OnLButtonDown (UINT nFlags, CPoint point) {
	m_blPressed = true;
//	SetCapture ();
	Invalidate (false);
	CButton::OnLButtonDown(nFlags, point);
}

void CaImToolButton::OnLButtonUp (UINT nFlags, CPoint point) {
	if (m_blPressed) {
		m_blPressed = false;
//		ReleaseCapture ();
		Invalidate (false);
	}
	CButton::OnLButtonUp(nFlags, point);
}
