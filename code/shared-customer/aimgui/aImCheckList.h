#pragma once


#define	ACLIF_NORMAL	0x00
#define	ACLIF_CHECKBOX	0x01
#define	ACLIF_RATIOBOX	0x02
#define	ACLIF_GROUP	0x04
#define	ACLIF_EMPTY	0x08
#define	ACLIF_BOLD	0x10

#define	ACLIS_UNCHECKED	0x0000
#define	ACLIS_CHECKED	0x0100
#define	ACLIS_DISABLED	0x0200
#define	ACLIS_SELECTED	0x0400

// CaImCheckList

class CaImCheckList : public CListCtrl {
	DECLARE_DYNAMIC(CaImCheckList)

public:
	CaImCheckList();
	virtual ~CaImCheckList();

protected:
	HTHEME m_hTheme;

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


