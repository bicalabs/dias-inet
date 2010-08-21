// DocPane.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include "Dias.h"
#include "DocPane.h"
#include "StorageView.h"
#include ".\docpane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocPane

CDocPane::CDocPane () {
}

CDocPane::~CDocPane () {
}

IMPLEMENT_DYNCREATE(CDocPane, CReBar)

BEGIN_MESSAGE_MAP(CDocPane, CReBar)
	//{{AFX_MSG_MAP(CDocPane)
	//}}AFX_MSG_MAP 
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDocPane message handlers

CSize CDocPane::CalcFixedLayout (BOOL bStretch, BOOL bHorz) {
	int nCount = int (DefWindowProc (RB_GETBANDCOUNT, 0, 0));
	REBARBANDINFO rbBand;
	rbBand.cbSize = sizeof(rbBand);
	int nTemp;

	// sync up hidden state of the bands
	for (nTemp = nCount; nTemp--; ) {
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(DefWindowProc(RB_SHOWBAND, nTemp, bWindowVisible));
	}

	// determine bounding rect of all visible bands
	CRect rectBound; rectBound.SetRectEmpty();
	for (nTemp = nCount; nTemp--; ) {
		rbBand.fMask = RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		if ((rbBand.fStyle & RBBS_HIDDEN) == 0) {
			CRect rect;
			VERIFY(DefWindowProc(RB_GETRECT, nTemp, (LPARAM)&rect));
			rectBound |= rect;
		}
	}

	// add borders as part of bounding rect
	if (!rectBound.IsRectEmpty ()) {
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, bHorz);
		rectBound.right -= rect.Width();
		rectBound.bottom -= rect.Height();
	}

	CRect rectClient;
	GetParent()->GetClientRect (&rectClient);

	return CSize(rectBound.Height(), rectClient.Height());
}
