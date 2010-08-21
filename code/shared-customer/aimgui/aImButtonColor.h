#pragma once


// CaImButtonColor

class CaImButtonColor : public CButton {
	DECLARE_DYNAMIC(CaImButtonColor)

public:
	CaImButtonColor();
	virtual ~CaImButtonColor ();

	COLORREF	m_clrCurrent;
	bool		m_blTransparent;
	bool		m_blHover;

public:
	HTHEME		m_hTheme;

	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem (LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
};


