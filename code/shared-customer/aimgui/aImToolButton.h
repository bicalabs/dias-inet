#pragma once


// CaImToolButton

class CaImToolButton : public CButton {
	DECLARE_DYNAMIC(CaImToolButton)

public:
	CaImToolButton();
	virtual ~CaImToolButton();

	CBitmap		m_bmpTool;
	bool		m_blHover;
	bool		m_blPressed;

protected:
	HTHEME		m_hTheme;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


