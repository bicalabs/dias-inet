#pragma once

// CPaneView

#include "PaneToolMark.h"

class CPaneView : public CWnd {
friend class CMainFrame;
	DECLARE_DYNAMIC(CPaneView)

public:
	CPaneView();
	virtual ~CPaneView();

protected:
	CMainFrame*	m_pFrame;

	CPaneToolMark	m_pnToolMark;

	HTHEME		m_hTheme;
	udword		m_dwSelect;
	udword		m_dwClose;
	bool		m_blSize;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

