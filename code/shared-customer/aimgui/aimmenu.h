#pragma once


#define	AMIS_TEXT	0x01
#define	AMIS_IMAGE	0x02
#define	AMIS_CHECKED	0x04
#define	AMIS_BAR	0x08
#define	AMIS_DEFAULT	0x10
#define	AMIS_DISABLED	0x20
#define	AMIS_GRIPPER	0x40

class CaImMenuItem : public CObject {
public:
	CaImMenuItem () { m_dwStyle = AMIS_BAR; };
	CaImMenuItem (CString strText) { m_dwStyle = AMIS_TEXT; m_strText = strText; };
	CaImMenuItem (CString strText, udword dwStyle) { m_dwStyle = AMIS_TEXT | dwStyle; m_strText = strText; };
	CaImMenuItem (CString strText, CBitmap* pbmpIcon) { m_dwStyle = AMIS_TEXT | AMIS_IMAGE; m_strText = strText; m_pbmpIcon = pbmpIcon; };
	virtual ~CaImMenuItem () { ; };

	bool		CheckStyle (udword dwCheck) { return ((m_dwStyle & dwCheck) != 0); };
	CString		GetText () { return (m_strText); };

protected:
	udword		m_dwStyle;
	CBitmap*	m_pbmpIcon;
	CString		m_strText;
};

// CaImMenu command target

#define	AMS_ICONS	0x0000001
#define	AMS_CHECKBOXES	0x0000002
#define	AMS_ICONANDCHECKS 0x0000004
#define	AMS_TEXT	0x0000008

class CaImMenu : public CMenu {
public:
	CaImMenu () { 
		m_dwStyle = AMS_ICONS | AMS_TEXT;
/**/		Init ();
	};

	CaImMenu (udword dwStyle) {
		m_dwStyle = dwStyle; 
/**/		Init ();
	};

	virtual ~CaImMenu () { CMenu::~CMenu (); };

//	bool		AppendMenu (UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem);
//	bool		AppendMenu (UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp);
//	bool		DeleteMenu (UINT nPosition, UINT nFlags);

	void		SetStyle (udword dwStyle) { m_dwStyle = dwStyle; };
	udword		GetStyle () { return (m_dwStyle); };

	virtual void	Serialize (CArchive& ar);
	virtual void	DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void	MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct);

protected:
/**/	void		Init () {
		COLORREF c = ::GetSysColor (COLOR_HOTLIGHT);

		m_cTextBg	= ::GetSysColor (COLOR_MENU);
		m_cTextNorm	= ::GetSysColor (COLOR_MENUTEXT);
		m_cTextDim	= ::GetSysColor (COLOR_GRAYTEXT);
		m_cTextSel	= ::GetSysColor (COLOR_MENUTEXT);

		m_cIconBg	= ::GetSysColor (COLOR_BTNFACE);

		m_cSelBg	= RGB (
				       GetRValue (c) + (255 - GetRValue (c)) * 5 / 6,
				       GetGValue (c) + (255 - GetGValue (c)) * 5 / 6,
				       GetBValue (c) + (255 - GetBValue (c)) * 5 / 6,
				       );
		m_cSelFrm	= ::GetSysColor (COLOR_HOTLIGHT);
		m_cGrayBg	= ::GetSysColor (COLOR_BTNFACE);
		m_cGrayElem	= ::GetSysColor (COLOR_BTNSHADOW);
		m_cBar		= ::GetSysColor (COLOR_BTNSHADOW);
		m_cPopup	= 0;

		m_cBoxNormBg	= RGB (
				       GetRValue (c) + (255 - GetRValue (c)) * 9 / 10,
				       GetGValue (c) + (255 - GetGValue (c)) * 9 / 10,
				       GetBValue (c) + (255 - GetBValue (c)) * 9 / 10,
				       );
		m_cBoxSelBg	= RGB (
				       GetRValue (c) + (255 - GetRValue (c)) * 1 / 2,
				       GetGValue (c) + (255 - GetGValue (c)) * 1 / 2,
				       GetBValue (c) + (255 - GetBValue (c)) * 1 / 2,
				       );
		m_cBoxDimBg	= ::GetSysColor (COLOR_BTNFACE);
		m_cBoxNormFrm	= ::GetSysColor (COLOR_HOTLIGHT);
		m_cBoxSelFrm	= ::GetSysColor (COLOR_HOTLIGHT);
		m_cBoxDimFrm	= ::GetSysColor (COLOR_BTNSHADOW);
		m_cBoxCheck	= 0;

		m_dwHeight = 24;
		m_dwWidth = 24;
	};

protected:
	udword		m_dwStyle;
	udword		m_dwHeight;
	udword		m_dwWidth;
//	CaImTheme	*m_pTheme;

	COLORREF	m_cTextBg;
	COLORREF	m_cTextNorm;
	COLORREF	m_cTextDim;
	COLORREF	m_cTextSel;

	COLORREF	m_cIconBg;

	COLORREF	m_cSelBg;
	COLORREF	m_cSelFrm;
	COLORREF	m_cGrayBg;
	COLORREF	m_cGrayElem;
	COLORREF	m_cBar;
	COLORREF	m_cPopup;

	COLORREF	m_cBoxNormBg;
	COLORREF	m_cBoxSelBg;
	COLORREF	m_cBoxDimBg;
	COLORREF	m_cBoxNormFrm;
	COLORREF	m_cBoxSelFrm;
	COLORREF	m_cBoxDimFrm;
	COLORREF	m_cBoxCheck;
};
