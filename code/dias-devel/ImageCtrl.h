#pragma once

// CImageCtrl

class CImageCtrl : public CWnd {
	DECLARE_DYNAMIC(CImageCtrl)

public:
	CImageCtrl ();
	virtual ~CImageCtrl ();

public:
	ubyte*		m_pbBits;
	dksobj_image*	m_pobjImage;
	CPen*		m_ppenLine;
	CPen*		m_ppenBoundary;
	CPen*		m_ppenChord;
	CBitmap		m_bmpImage;
	uint		m_nVScrollPos;
	uint		m_nHScrollPos;

public:
	void CreateImage (CPaintDC* pdcThis);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
};


