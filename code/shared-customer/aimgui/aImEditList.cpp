// aImEditList.cpp : implementation file
//

#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include "aImEditList.h"


// CaImEditList

IMPLEMENT_DYNAMIC(CaImEditList, CListBox)
CaImEditList::CaImEditList () {
	m_dwTitlesSize = 50;
}

CaImEditList::~CaImEditList () {
}


BEGIN_MESSAGE_MAP (CaImEditList, CListBox)
END_MESSAGE_MAP ()



// CaImEditList message handlers


void CaImEditList::MeasureItem (LPMEASUREITEMSTRUCT lpMIS) {
	CRect rcClient;
	GetClientRect (rcClient);
	lpMIS->itemHeight = 64;
	lpMIS->itemWidth = rcClient.Width ();
	// TODO:  Add your code to determine the size of specified item
}

void CaImEditList::DrawItem (LPDRAWITEMSTRUCT lpDIS) {
	CDC dc;
	CRect rcItem, rcTitle, rcText, rcUnderline;

	COLORREF clEvenCol = 0xEADDDD,
		clNormalCol = 0xEEEEEE,
		clEvenBk = 0xFAEEEE,
		clNormalBk = 0xFFFFFF,
		clEvenTx = GetSysColor (COLOR_WINDOWTEXT),
		clNormalTx = GetSysColor (COLOR_WINDOWTEXT),
		clHighlightBk = GetSysColor (COLOR_HIGHLIGHT),
		clHighlightTx = GetSysColor (COLOR_HIGHLIGHTTEXT);

	CPen pnFocus (PS_DOT, 1, 0xCCCCCC);

	rcItem = lpDIS->rcItem;
	rcText = rcItem;
	rcTitle = rcItem;
	rcText.left = rcTitle.right = m_dwTitlesSize;
	rcUnderline = rcItem;
	rcUnderline.top = rcUnderline.bottom - 2;

	bool blEven = lpDIS->itemID % 2;

	dc.Attach (lpDIS->hDC);

	CPen* ppnOld = dc.SelectObject (&pnFocus);

	aimPaintGradHorz (&dc, rcTitle, blEven ? clEvenBk : clNormalBk, blEven ? clNormalBk : clEvenBk);
	aimPaintGradHorz (&dc, rcText, blEven ? clNormalBk : clEvenBk, blEven ? clEvenBk : clNormalBk);
	aimPaintGradHorz (&dc, rcUnderline, clHighlightBk, clNormalBk);
	dc.MoveTo (rcText.TopLeft ());
	dc.LineTo (rcTitle.BottomRight ());

	CString str;
	GetText (lpDIS->itemID, str);
	dc.DrawText (str, rcTitle, DT_LEFT | DT_TOP | DT_SINGLELINE);
	dc.DrawText ("", rcText, DT_LEFT | DT_TOP | DT_SINGLELINE);

	dc.SelectObject (ppnOld);
	dc.Detach ();
}
