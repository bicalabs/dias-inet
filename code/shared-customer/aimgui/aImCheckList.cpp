#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include "aimchecklist.h"


// CaImCheckList

IMPLEMENT_DYNAMIC(CaImCheckList, CListCtrl)
CaImCheckList::CaImCheckList () {
	m_hTheme = NULL;
}

CaImCheckList::~CaImCheckList () {
}

BEGIN_MESSAGE_MAP(CaImCheckList, CListCtrl)
END_MESSAGE_MAP()



// CaImCheckList message handlers


void CaImCheckList::DrawItem (LPDRAWITEMSTRUCT lpDIS) {
	udword	dwFlags = lpDIS->itemData;
	dwFlags |= (lpDIS->itemState & ODS_SELECTED) ? ACLIS_SELECTED : 0;

	COLORREF clEvenCol = 0xEADDDD,
		clNormalCol = 0xEEEEEE,
		clEvenBk = 0xFAEEEE,
		clNormalBk = 0xFFFFFF,
		clEvenTx = GetSysColor (COLOR_WINDOWTEXT),
		clNormalTx = GetSysColor (COLOR_WINDOWTEXT),
		clHighlightBk = GetSysColor (COLOR_HIGHLIGHT),
		clHighlightTx = GetSysColor (COLOR_HIGHLIGHTTEXT);

	CDC dc;
	CPen pnFocus (PS_SOLID, 1, clHighlightBk);
	CBrush brOut (HS_BDIAGONAL, clHighlightBk);

	CRect rcClient, rcItem, rcRow, rcCheck, rcEntry, rcUnderline;

	bool blEven = lpDIS->itemID % 2;

	GetClientRect (&rcClient);
	rcItem = lpDIS->rcItem;
	rcRow = rcItem;
	rcRow.right = rcClient.right;
	rcCheck = rcItem;
	rcCheck.right = rcCheck.left + 24;
	rcEntry = rcItem;
	rcUnderline = rcItem;
	rcUnderline.top = rcUnderline.bottom - 3;

	if (dwFlags & ACLIF_EMPTY)
		return;

	dc.Attach (lpDIS->hDC);
	CPen* ppnOld = dc.SelectObject (&pnFocus);
	CBrush* pbrOld = dc.SelectObject (&brOut);

	// Drawing Row
	if (dwFlags & ACLIF_GROUP) {
		aimPaintGradHorz (&dc, rcUnderline, clHighlightBk, clNormalBk);
		dc.SetTextColor (clNormalTx);
	} else if (dwFlags & ACLIS_SELECTED) {
		aimPaintGradHorz (&dc, rcItem, blEven ? clEvenBk : clNormalBk, blEven ? clNormalBk : clEvenBk);
		dc.DrawFocusRect (rcItem);
		dc.SetTextColor (clNormalTx);
	} else {
		aimPaintGradHorz (&dc, rcItem, blEven ? clEvenBk : clNormalBk, blEven ? clNormalBk : clEvenBk);
		dc.SetTextColor (clNormalTx);
	}

	// Drawing Checkboxes
	if (dwFlags & ACLIF_CHECKBOX) {
		if (aimThemeLoaded) {
			if (!m_hTheme)	m_hTheme = aimThemeOpen (this->m_hWnd, L"Button");
			DWORD state = (dwFlags & ACLIS_CHECKED) ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL;
			aimThemeBackground (m_hTheme, dc, BP_CHECKBOX, state, &rcCheck, NULL);
		} else {
			;
		}
	}
	if (dwFlags & ACLIF_RATIOBOX) {
		if (aimThemeLoaded) {
			if (!m_hTheme)	m_hTheme = aimThemeOpen (this->m_hWnd, L"Button");
			DWORD state = (dwFlags & ACLIS_CHECKED) ? RBS_CHECKEDNORMAL : RBS_UNCHECKEDNORMAL;
			aimThemeBackground (m_hTheme, dc, BP_RADIOBUTTON, state, &rcCheck, NULL);
		} else {
			;
		}
	}

	// Drawing text
	if (dwFlags & ACLIF_GROUP) {
		rcEntry.left = ((dwFlags & ACLIF_CHECKBOX) || (dwFlags & ACLIF_RATIOBOX)) ? 24 : 8;
		rcEntry.SubtractRect (rcEntry, rcUnderline);
		dc.DrawText (GetItemText (lpDIS->itemID, 0), rcEntry, DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
	} else
		for (uvar32_64 n = 0, w = 0; n < GetHeaderCtrl()->GetItemCount (); n++) {
			rcEntry.left = w;
			rcEntry.right = rcEntry.left + GetColumnWidth (n);
			if (n == 0)
				rcEntry.left = ((dwFlags & ACLIF_CHECKBOX) || (dwFlags & ACLIF_RATIOBOX)) ? 24 : 8;
			w += GetColumnWidth (n);
			dc.DrawText (GetItemText (lpDIS->itemID, n), rcEntry, DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
		}

	dc.SelectObject (ppnOld);
	dc.SelectObject (pbrOld);
	dc.Detach ();
}
