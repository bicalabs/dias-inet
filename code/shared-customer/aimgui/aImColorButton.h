#pragma once


// CaImButtonColor

class CaImColorButton : public CButton {
	DECLARE_DYNAMIC(CaImButtonColor)

public:
	CaImColorButton();
	virtual ~CaImColorButton ();

	COLORREF	m_clrCurrent;
	bool		m_blTransparent;
	bool		m_blHover;

protected:
	HTHEME		m_hTheme;

	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem (LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
};


