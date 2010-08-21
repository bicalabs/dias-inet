#pragma once


// CaImEditList

class CaImEditList : public CListBox {
	DECLARE_DYNAMIC(CaImEditList)

public:
	CaImEditList();
	virtual ~CaImEditList();

protected:
	udword	m_dwTitlesSize;

	DECLARE_MESSAGE_MAP()


public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


