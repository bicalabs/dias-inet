#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include "aimbuttoncolor.h"


// CaImButtonColor

IMPLEMENT_DYNAMIC(CaImButtonColor, CButton)
CaImButtonColor::CaImButtonColor () {
	m_hTheme = NULL;
	m_blHover = false;
	m_blTransparent = true;
}

CaImButtonColor::~CaImButtonColor () {
}


BEGIN_MESSAGE_MAP(CaImButtonColor, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CaImButtonColor message handlers


void CaImButtonColor::DrawItem (LPDRAWITEMSTRUCT lpDIS) {
	CDC dc;
	CRect rcClient;
	CBrush brFrame (GetSysColor (COLOR_BTNSHADOW));
	CBrush brColFrm (GetSysColor (COLOR_3DSHADOW));

	dc.Attach (lpDIS->hDC);

	GetClientRect (&rcClient);

	bool bIsPressed		= (lpDIS->itemState & ODS_SELECTED);
	bool bIsFocused		= (lpDIS->itemState & ODS_FOCUS);
	bool bIsDisabled	= (lpDIS->itemState & ODS_DISABLED);
	bool bDrawFocusRect	= !(lpDIS->itemState & ODS_NOFOCUSRECT);

	SetBkMode(dc, TRANSPARENT);
	if (aimThemeLoaded) {
		if (!m_hTheme)	m_hTheme = aimThemeOpen (m_hWnd, L"Button");

		DWORD state = (bIsPressed)?PBS_PRESSED:PBS_NORMAL;
		if(state == PBS_NORMAL) {
			if (bIsFocused)
				state = PBS_DEFAULTED;
			if (m_blHover)
				state = PBS_HOT;
		}
		aimThemeBackground (m_hTheme, dc, BP_PUSHBUTTON, state, &rcClient, NULL);
	}
	else {
		dc.FillSolidRect (rcClient, GetSysColor (COLOR_3DLIGHT));
		dc.FrameRect (&rcClient, &brFrame);
	}

	rcClient.DeflateRect (0, 0, 22, 0);
	dc.DrawIcon (rcClient.right + 1, rcClient.top + 5, AfxGetApp()->LoadIcon (232));
	rcClient.DeflateRect (6, 6, 2, 6);
	if (!m_blTransparent);
		dc.FillSolidRect (&rcClient, m_clrCurrent);
	dc.FrameRect (&rcClient, &brColFrm);

	dc.Detach ();
}

void CaImButtonColor::OnMouseMove (UINT nFlags, CPoint point) {
	if (!m_blHover) {
		m_blHover = true;
		Invalidate (false);
	}
	CButton::OnMouseMove(nFlags, point);
}
