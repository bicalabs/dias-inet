// ScaleWnd.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "ScaleWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleWnd

CScaleWnd::CScaleWnd () {
}

CScaleWnd::~CScaleWnd () {
}


BEGIN_MESSAGE_MAP(CScaleWnd, CWnd)
	//{{AFX_MSG_MAP(CScaleWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScaleWnd message handlers

BOOL CScaleWnd::Create (EScaleType nType, CWnd* pParentWnd) {
	m_nType = nType;
	m_blInner = false;
	m_nBorder = 4;
	return CWnd::Create(NULL, "Scale", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect (0,0,0,0), pParentWnd, 0, NULL);
}

void CScaleWnd::OnPaint () {
	int i;
	CPaintDC dc(this);

	CRect rectClient, rectInternal, rectActive;
	GetClientRect (&rectClient);
	switch (m_nType) {
		case scaleVert:
			rectInternal = CRect (rectClient.left + m_nBorder, rectClient.top + m_nIntBegin, rectClient.right - m_nBorder, rectClient.bottom - m_nIntEnd);
			rectActive = CRect (rectInternal.left, rectClient.top + m_nActiveBegin, rectInternal.right, rectClient.bottom - m_nActiveEnd);
			break;
		case scaleHoriz:
			rectInternal = CRect (rectClient.left + m_nIntBegin, rectClient.top + m_nBorder, rectClient.right - m_nIntEnd, rectClient.bottom - m_nBorder);
			rectActive = CRect (rectClient.left + m_nActiveBegin, rectInternal.top, rectClient.right - m_nActiveEnd, rectInternal.bottom);
			break;
		case scaleCentr:
			rectInternal = CRect (0,0,0,0);
			rectActive = rectInternal;
			break;
	}

	dc.FillSolidRect (rectClient, ::GetSysColor (COLOR_BTNFACE));
	if (m_blInner) {
		dc.FillSolidRect (rectInternal, ::GetSysColor (COLOR_APPWORKSPACE));
		dc.FillSolidRect (rectActive, ::GetSysColor (COLOR_WINDOW));
		dc.DrawEdge (rectInternal, BDR_SUNKENOUTER, BF_RECT);
	}
	dc.DrawEdge (rectClient, BDR_RAISEDINNER, BF_RECT);

	if (m_nType == scaleHoriz && m_blInner) {
		for (i = rectClient.left + m_nIntBegin; i < rectClient.right - m_nIntEnd; i += 50) {
			dc.MoveTo (i, 5);
			dc.LineTo (i, 6);
			dc.MoveTo (i, 15);
			dc.LineTo (i, 16);
		}
		for (i = rectClient.left + m_nIntBegin; i < rectClient.right - m_nIntEnd; i += 25) {
			dc.MoveTo (i, 7);
			dc.LineTo (i, 14);
		}
		for (i = rectClient.left + m_nIntBegin; i < rectClient.right - m_nIntEnd; i += 5) {
			dc.MoveTo (i, 9);
			dc.LineTo (i, 12);
		}
	} else if (m_nType == scaleVert && m_blInner) {
		for (i = rectClient.top + m_nIntBegin; i < rectClient.bottom - m_nIntEnd; i += 50) {
			dc.MoveTo (5, i);
			dc.LineTo (6, i);
			dc.MoveTo (15, i);
			dc.LineTo (16, i);
		}
		for (i = rectClient.top + m_nIntBegin; i < rectClient.bottom - m_nIntEnd; i += 25) {
			dc.MoveTo (7, i);
			dc.LineTo (14, i);
		}
		for (i = rectClient.top + m_nIntBegin; i < rectClient.bottom - m_nIntEnd; i += 5) {
			dc.MoveTo (9, i);
			dc.LineTo (12, i);
		}
	} else if (m_nType == scaleCentr)  {
		dc.DrawEdge (CRect (m_nBorder, m_nBorder, rectClient.Width () - m_nBorder, rectClient.Height () - m_nBorder), EDGE_ETCHED, BF_RECT);
	}
}

void CScaleWnd::OnMouseMove (UINT nFlags, CPoint point) {
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}
