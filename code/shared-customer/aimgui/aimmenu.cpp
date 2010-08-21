#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include "aimmenu.h"


// CaImMenu member functions

void CaImMenu::DrawItem (LPDRAWITEMSTRUCT lpDIS) {
	CFont fntDef;
	LOGFONT lfDef;
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CaImMenuItem* pItem = (CaImMenuItem*)lpDIS->itemData;

	MENUITEMINFO iInfo;
	GetMenuItemInfo (lpDIS->CtlID, &iInfo);

	CRect rcWhole = lpDIS->rcItem, rcIcon, rcText, rcText2, rcCheck;
	rcIcon = rcText = rcWhole;
	rcIcon.right = rcIcon.left + m_dwWidth;
	rcText.left += m_dwWidth;
	rcText2 = rcText;
	rcText2.DeflateRect (4, 4);
	rcWhole.DeflateRect (1, 1);

	CFont* pFont = (CFont *)pDC->SelectStockObject (ANSI_VAR_FONT);
	if (pItem->CheckStyle (AMIS_DEFAULT)) {
		CFont* pFnt = pDC->GetCurrentFont ();
		pFnt->GetLogFont (&lfDef);
		lfDef.lfWeight = FW_BOLD;
		fntDef.CreateFontIndirect (&lfDef);
		pDC->SelectObject (&fntDef);
	}

	pDC->SetBkColor (m_cTextBg);
		pDC->SetTextColor (pItem->CheckStyle (AMIS_DISABLED) ? m_cTextDim : (pItem->CheckStyle (AMIS_GRIPPER) ? m_cGrayElem : m_cTextNorm));

	if (pItem->CheckStyle (AMIS_BAR)) {
		CPen pn;
		pn.CreatePen (PS_SOLID, 1, m_cBar);
		CPen* pPen = pDC->SelectObject (&pn);
		pDC->FillSolidRect (&rcIcon, m_cIconBg);
		pDC->FillSolidRect (&rcText, m_cTextBg);
		pDC->MoveTo (rcText2.left, rcText2.top + rcText2.Height () / 2);
		pDC->LineTo (lpDIS->rcItem.right, rcText2.top + rcText2.Height () / 2);
		pDC->SelectObject (pPen);
		pn.DeleteObject ();
	} else {
		pDC->FillSolidRect (&rcIcon, m_cIconBg);
		pDC->FillSolidRect (&rcText, m_cTextBg);
	}

	if ((lpDIS->itemState & ODS_SELECTED) && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
		pDC->SetBkColor (m_cTextBg);
		pDC->SetTextColor (pItem->CheckStyle (AMIS_DISABLED) ? m_cTextDim : (pItem->CheckStyle (AMIS_GRIPPER) ? m_cGrayElem : m_cTextSel));

		pDC->FillSolidRect (&rcWhole, m_cSelBg);
		CBrush br (m_cSelFrm);
		pDC->FrameRect(&rcWhole, &br);
	}

	if (pItem->CheckStyle (AMIS_TEXT))
		pDC->DrawText (pItem->GetText (), rcText2, DT_LEFT | DT_EXPANDTABS | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);

	if (pItem->CheckStyle (AMIS_CHECKED)) {
		rcCheck = rcIcon;
		rcCheck.DeflateRect (2, 2);
		rcCheck.DeflateRect (1, 1);

		CPoint st, nd, rd, cn;
		cn = CPoint (rcCheck.left, rcCheck.top) + CPoint (rcCheck.Width () / 2, rcCheck.Height () / 2) - CPoint (1, 1);
		st = cn - CPoint (rcCheck.Width () / 4, 0);
		nd = cn + CPoint (0, rcCheck.Height () / 4);
		rd = cn + CPoint (rcCheck.Width () / 3, -rcCheck.Height () / 4);

		CPen pn;
		pn.CreatePen (PS_SOLID, 2, m_cBoxCheck);
		CPen* pPen = pDC->SelectObject (&pn);
		pDC->MoveTo (st);
		pDC->LineTo (nd);
		pDC->LineTo (rd);
		pDC->SelectObject (pPen);
		pn.DeleteObject ();
	}

	pDC->SelectObject (pFont);
	if (pItem->CheckStyle (AMIS_DEFAULT))
		fntDef.DeleteObject ();
}

void CaImMenu::MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
	MENUITEMINFO iInfo;
	GetMenuItemInfo (lpMeasureItemStruct->CtlID, &iInfo);
	CaImMenuItem* pItem = (CaImMenuItem*)lpMeasureItemStruct->itemData;

	CFont fntDef;
	LOGFONT lfDef;
	CClientDC dc (0);
	CFont* pFont = (CFont *)dc.SelectStockObject (ANSI_VAR_FONT);
	if (pItem->CheckStyle (AMIS_DEFAULT)) {
		CFont* pFnt = dc.GetCurrentFont ();
		pFnt->GetLogFont (&lfDef);
		lfDef.lfWeight = FW_BOLD;
		fntDef.CreateFontIndirect (&lfDef);
		dc.SelectObject (&fntDef);
	}

	if (pItem->CheckStyle (AMIS_BAR))
		lpMeasureItemStruct->itemHeight = 4;
	else
		lpMeasureItemStruct->itemHeight = m_dwHeight;
	lpMeasureItemStruct->itemWidth = 25 + 8 + 8 + 2 + dc.GetTextExtent (pItem->GetText ()).cx;

	dc.SelectObject (pFont);
	if (pItem->CheckStyle (AMIS_DEFAULT))
		fntDef.DeleteObject ();
}

void CaImMenu::Serialize (CArchive& ar) {
	if (ar.IsStoring ()) {
	}
	else {
	}
}
