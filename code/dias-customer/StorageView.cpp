// StorageView.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "MainFrame.h"
#include "ScaleWnd.h"
#include "ImageWnd.h"
#include "DataWnd.h"
#include "ReportWnd.h"
#include "StorageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStorageView

IMPLEMENT_DYNCREATE(CStorageView, CView)

CStorageView::CStorageView () {
}

CStorageView::~CStorageView () {
}


BEGIN_MESSAGE_MAP(CStorageView, CView)
	//{{AFX_MSG_MAP(CStorageView)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_COMMAND_RANGE(ID_IMAGE_POINTER, ID_IMAGE_ERASER, OnModeChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_IMAGE_POINTER, ID_IMAGE_ERASER, OnModeUpdate)

	ON_COMMAND_RANGE(ID_VIEW_ZOOM_FIT, ID_VIEW_ZOOM_OTHER, OnZoomChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ZOOM_FIT, ID_VIEW_ZOOM_OTHER, OnZoomUpdate)

	ON_COMMAND_RANGE(ID_VIEW_LAYERS_BACKGROUND, ID_VIEW_LAYERS_ETCETERA, OnLayerChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_LAYERS_BACKGROUND, ID_VIEW_LAYERS_ETCETERA, OnLayerUpdate)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStorageView diagnostics

#ifdef _DEBUG
void CStorageView::AssertValid() const
{
	CView::AssertValid();
}

void CStorageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStorageView message handlers

int CStorageView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

//	m_wndSplit.Create (NULL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect (0, 0, 4, 200), this, IDX_SPLIT);

	m_wndHScale.Create (CScaleWnd::scaleHoriz, this);
	m_wndVScale.Create (CScaleWnd::scaleVert, this);
	m_wndCScale.Create (CScaleWnd::scaleCentr, this);

	m_wndImage.Create (NULL, NULL, WS_CHILD | WS_VISIBLE, CRect (0,0,0,0), this, 0);
	m_wndData.Create (0, CRect (0,0,0,0), this, 0);
	m_wndReport.Create (NULL, NULL, WS_CHILD, CRect (0,0,0,0), this, 0);

	m_wndImage.m_pView = m_wndData.m_pView = m_wndReport.m_pView = this;
	m_pDoc = m_wndImage.m_pDoc = m_wndData.m_pDoc = m_wndReport.m_pDoc 
		= (CStorageDoc*)(CView::GetDocument());
	m_pFrame = m_wndImage.m_pFrame = m_wndData.m_pFrame = m_wndReport.m_pFrame
		= (CStorageFrame*)(GetParent());
	m_pFrame->m_pView = this;
	m_pFrame->m_barImages.m_pView = this;
	m_pFrame->m_barData.m_pView = this;
	m_pFrame->m_barReports.m_pView = this;

	return 0;
}

void CStorageView::OnDraw (CDC* pDC) {
	CRect rcSplit;
	GetClientRect (&rcSplit);
	rcSplit.right = 4;
	pDC->FillSolidRect (rcSplit, ::GetSysColor (COLOR_BTNFACE));
	pDC->Draw3dRect (rcSplit, ::GetSysColor (COLOR_BTNHILIGHT), ::GetSysColor (COLOR_BTNSHADOW));
}

void CStorageView::OnChangeBar (int iIndex) {
	CRect rect, rectParent;
	m_wndImage.ShowWindow ((iIndex == 0)?SW_SHOWNA:SW_HIDE);
	m_wndData.ShowWindow ((iIndex == 1)?SW_SHOWNA:SW_HIDE);
	m_wndReport.ShowWindow ((iIndex == 2)?SW_SHOWNA:SW_HIDE);
	if (iIndex == 1) {
		m_strZoom = "";
		m_wndHScale.ShowWindow (SW_HIDE);
		m_wndVScale.ShowWindow (SW_HIDE);
		m_wndCScale.ShowWindow (SW_HIDE);
//		m_pFrame->m_wndToolBar.SetButtonStyle (9, TBBS_DROPDOWN | TBBS_HIDDEN);
//		m_pFrame->m_wndToolBar.SetButtonStyle (10, TBBS_SEPARATOR | TBBS_HIDDEN);
	} else {
		m_strZoom.Format ("%d%%", (iIndex == 0)?m_nZoomImg:m_nZoomRpt);
		m_wndHScale.ShowWindow (SW_SHOWNA);
		m_wndVScale.ShowWindow (SW_SHOWNA);
		m_wndCScale.ShowWindow (SW_SHOWNA);
//		m_pFrame->m_wndToolBar.SetButtonStyle (9, TBBS_AUTOSIZE | TBBS_DROPDOWN);
//		m_pFrame->m_wndToolBar.SetButtonStyle (10, TBBS_SEPARATOR);
	}
	if (iIndex == 0) {
/*		m_pFrame->m_wndToolBar.LoadToolBar (IDR_TBX_DOC_IMG);
		m_pFrame->m_wndToolBar.SetButtonText (0, theApp.LoadString (IDS_TBX_DOC_ADD_IMAGE));
		m_pFrame->m_wndToolBar.SetButtonText (2, theApp.LoadString (IDS_TBX_DOC_ADD_ACQUIRE));
		m_pFrame->m_wndToolBar.SetButtonText (3, theApp.LoadString (IDS_TBX_DOC_REMOVE));
		m_pFrame->m_wndToolBar.SetButtonText (5, theApp.LoadString (IDS_TBX_DOC_IMPORT));
		m_pFrame->m_wndToolBar.SetButtonText (6, theApp.LoadString (IDS_TBX_DOC_EXPORT));
		m_pFrame->m_wndToolBar.SetButtonText (8, theApp.LoadString (IDS_TBX_DOC_UP));
		m_pFrame->m_wndToolBar.SetButtonText (11, theApp.LoadString (IDS_TBX_DOC_ZOOM));

		m_pFrame->m_wndToolBar.SetButtonStyle (0, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (1, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (2, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (3, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (5, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (6, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (8, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (9, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (11, TBBS_AUTOSIZE | TBBS_DROPDOWN);
		m_pFrame->m_wndToolBar.SetButtonStyle (13, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (14, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (15, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (16, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (17, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (18, TBBS_AUTOSIZE);
	} else if (iIndex == 1) {
		m_pFrame->m_wndToolBar.LoadToolBar (IDR_TBX_DOC_SRV);
		m_pFrame->m_wndToolBar.SetButtonText (0, theApp.LoadString (IDS_TBX_DOC_ADD_DB));
		m_pFrame->m_wndToolBar.SetButtonText (1, theApp.LoadString (IDS_TBX_DOC_REMOVE));
		m_pFrame->m_wndToolBar.SetButtonText (3, theApp.LoadString (IDS_TBX_DOC_FIELDS));
		m_pFrame->m_wndToolBar.SetButtonText (4, theApp.LoadString (IDS_TBX_DOC_MEASURE));
		m_pFrame->m_wndToolBar.SetButtonText (6, theApp.LoadString (IDS_TBX_DOC_IMPORT));
		m_pFrame->m_wndToolBar.SetButtonText (7, theApp.LoadString (IDS_TBX_DOC_EXPORT));
		m_pFrame->m_wndToolBar.SetButtonText (9, theApp.LoadString (IDS_TBX_DOC_UP));

		m_pFrame->m_wndToolBar.SetButtonStyle (0, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (1, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (3, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (4, TBBS_AUTOSIZE | TBBS_DROPDOWN);
		m_pFrame->m_wndToolBar.SetButtonStyle (6, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (7, TBBS_AUTOSIZE | TBBS_DROPDOWN);
		m_pFrame->m_wndToolBar.SetButtonStyle (9, TBBS_AUTOSIZE);
	} else {
		m_pFrame->m_wndToolBar.LoadToolBar (IDR_TBX_DOC_REP);
		m_pFrame->m_wndToolBar.SetButtonText (0, theApp.LoadString (IDS_TBX_DOC_ADD_IMAGE));
		m_pFrame->m_wndToolBar.SetButtonText (2, theApp.LoadString (IDS_TBX_DOC_ADD_ACQUIRE));
		m_pFrame->m_wndToolBar.SetButtonText (3, theApp.LoadString (IDS_TBX_DOC_REMOVE));
		m_pFrame->m_wndToolBar.SetButtonText (5, theApp.LoadString (IDS_TBX_DOC_UP));
		m_pFrame->m_wndToolBar.SetButtonText (8, theApp.LoadString (IDS_TBX_DOC_ZOOM));

		m_pFrame->m_wndToolBar.SetButtonStyle (0, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (1, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (2, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (3, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (5, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (6, TBBS_AUTOSIZE);
		m_pFrame->m_wndToolBar.SetButtonStyle (8, TBBS_AUTOSIZE | TBBS_DROPDOWN);*/
	}
//	m_pFrame->m_wndToolBar.GetToolBarCtrl ().RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
//	m_pFrame->GetWindowRect (&rect);
//	m_pFrame->GetParent()->GetWindowRect (&rectParent);
//	m_pFrame->SetWindowPos (NULL, rect.left - rectParent.left - 2, rect.top - rectParent.top - 2, rect.Width () - 1, rect.Height (), SWP_NOZORDER);
//	m_pFrame->SetWindowPos (NULL, rect.left - rectParent.left - 2, rect.top - rectParent.top - 2, rect.Width (), rect.Height (), SWP_NOZORDER);
}

void CStorageView::OnChangeImage (int iIndex) {
	if (iIndex != -1)
		m_wndImage.CreateImage ();
	OnChangeScale ();
	m_wndImage.Invalidate ();
	theApp.Update (UPDHA_IMAGE);
}

void CStorageView::OnChangeSurvey (int iIndex) {
	if (iIndex != -1)
		m_wndData.CreateSurvey ();
	m_wndData.Invalidate ();
	theApp.Update (UPDHA_SURVEY);
}

void CStorageView::OnChangeScale () {
	CRect rectClient, rectWnd;
	GetWindowRect (&rectClient);
	if (!m_pFrame->m_barImages || m_pFrame->GetActiveImageNo () == -1) {
		m_wndVScale.m_blInner = false;
		m_wndHScale.m_blInner = false;
	} else {
		UINT sizeX = (rectClient.Width () - m_pDoc->GetDimX() * m_nZoomImg / 100 - 21) / 2;
		UINT sizeY = (rectClient.Height () - m_pDoc->GetDimY() * m_nZoomImg / 100 - 21) / 2;
		m_wndVScale.m_blInner = true;
		m_wndHScale.m_blInner = true;
		// #### TODO: Adjust scale parameters according to selected object and current image state
		m_wndVScale.m_nIntBegin = m_wndVScale.m_nActiveBegin = sizeY;
		m_wndVScale.m_nIntEnd = m_wndVScale.m_nActiveEnd = sizeY;
		m_wndHScale.m_nIntBegin = m_wndHScale.m_nActiveBegin = sizeX;
		m_wndHScale.m_nIntEnd = m_wndHScale.m_nActiveEnd = sizeX;
	}
	m_wndVScale.RedrawWindow ();
	m_wndHScale.RedrawWindow ();
	m_wndCScale.RedrawWindow ();
}

void CStorageView::OnSize (UINT nType, int cx, int cy) {
	CView::OnSize(nType, cx, cy);

	UINT dx, dy, split = 4;
	if (m_wndImage.m_hWnd) {
		dx = dy = m_blImgScale ? 21 : 0;
		m_wndHScale.SetWindowPos (NULL, 21 + split, 0, cx - 21, 21, SWP_NOZORDER);
		m_wndVScale.SetWindowPos (NULL, 0 + split, 21, 21, cy - 21, SWP_NOZORDER);
		m_wndCScale.SetWindowPos (NULL, 0 + split, 0, 21, 21, SWP_NOZORDER);
		m_wndImage.SetWindowPos (NULL, dx + split, dy, cx - dx, cy - dy, SWP_NOZORDER);
		m_wndImage.RedrawWindow ();
		OnChangeScale ();
	}
	if (m_wndData.m_hWnd) {
		m_wndData.SetWindowPos (NULL, 0 + split, 0, cx, cy, SWP_NOZORDER);
		m_wndData.RedrawWindow ();
	}
	if (m_wndReport.m_hWnd) {
		dx = dy = m_blRepScale ? 21 : 0;
		m_wndHScale.SetWindowPos (NULL, 21 + split, 0, cx - 21, 21, SWP_NOZORDER);
		m_wndVScale.SetWindowPos (NULL, 0 + split, 21, 21, cy - 21, SWP_NOZORDER);
		m_wndCScale.SetWindowPos (NULL, 0 + split, 0, 21, 21, SWP_NOZORDER);
		m_wndReport.SetWindowPos (NULL, dx + split, dy, cx - dx, cy - dy, SWP_NOZORDER);
		m_wndReport.RedrawWindow ();
		OnChangeScale ();
	}
}

void CStorageView::OnInitialUpdate () {
	CView::OnInitialUpdate();

	m_nMode = ID_IMAGE_POINTER;
	m_nLayer = 0;
	m_nZoomImg = 100;
	m_nZoomRpt = 100;
	m_strZoom = "100%";
	m_blImgScale = true;
	m_blRepScale = true;

	m_wndImage.UpdateScroll ();
}

void CStorageView::OnUpdate (CView* pSender, LPARAM lHint, CObject* ph) {
	bool	blAll = (ph == NULL);
	CUpdateHint	*pHint;	
	CString tmp;
	uvar32_64 i;

	if (blAll)
		pHint = new CUpdateHint (CUpdateHint::updtList, UPDHLM_TARGET | UPDHL_ALL | 
		                         UPDHI_PALETTE | UPDHI_BITMAP | UPDHI_IMAGE | UPDHI_SCALE |
		                         UPDHI_MARKERS | UPDHI_TEXT | UPDHI_IOBJS | UPDHI_MEASURES,
		                         // #### TODO: Place additional update options for data and images here 
		                         NULL);
	else
		pHint = new CUpdateHint (((CUpdateHint*)ph)->target, ((CUpdateHint*)ph)->cmd,
	                                 ((CUpdateHint*)ph)->start, ((CUpdateHint*)ph)->end);

	if (pHint == NULL || pHint->target == CUpdateHint::updtList) {
		CDocList *pBar = NULL;
		while (((pHint->cmd)&UPDHLM_TARGET)) {
			if (((pHint->cmd)&UPDHL_IMAGES) == UPDHL_IMAGES) {
				pHint->cmd ^= UPDHL_IMAGES;
				pBar = &(m_pFrame->m_barImages);
				if (!IsWindow (*pBar))
					return;
				i = m_pDoc->Images.GetCount ();
				tmp.Format ("%d (3:%.1f; 4:%.1f; 5:%.1f)", i, i/3., i/4., i/5.);
				if (IsWindow (m_pFrame->m_wndStatusBar))
					m_pFrame->m_wndStatusBar.SetPaneText (1, tmp);
			}
			else if (((pHint->cmd)&UPDHL_SURVEIS) == UPDHL_SURVEIS) {
				pHint->cmd ^= UPDHL_SURVEIS;
				pBar = &(m_pFrame->m_barData);
			}
			else if (((pHint->cmd)&UPDHL_REPORTS) == UPDHL_REPORTS) {
				pHint->cmd ^= UPDHL_REPORTS;
				pBar = &(m_pFrame->m_barReports);
			}
			else return;
			if (((pHint->cmd)&UPDHL_ALL) == UPDHL_ALL) {
				pHint->cmd ^= UPDHL_ALL;
				pBar->DeleteAllItems ();
				for (int i = 0, count = m_pDoc->Images.GetCount(); i < count; ++i) {
					if (pBar == &(m_pFrame->m_barImages))
						pBar->InsertItem (i, m_pDoc->Images[i].GetName ());
					else if (pBar == &(m_pFrame->m_barData))
						pBar->InsertItem (i, m_pDoc->Surveis[i].GetName ());
					else if (pBar == &(m_pFrame->m_barReports))
						;//pBar->InsertItem (i, m_pDoc->Reports[i].GetName ());
				}
			}
			if (((pHint->cmd)&UPDHL_ADDED) == UPDHL_ADDED)
				for (DWORD i = pHint->start; i <= pHint->end; ++i) {
					if (pBar == &(m_pFrame->m_barImages))
						pBar->InsertItem (i, m_pDoc->Images[i].GetName ());
					else if (pBar == &(m_pFrame->m_barData))
						pBar->InsertItem (i, m_pDoc->Surveis[i].GetName ());
					else if (pBar == &(m_pFrame->m_barReports))
						;//pBar->InsertItem (i, m_pDoc->Reports[i].GetName ());
				}
			else if (((pHint->cmd)&UPDHL_REMOVED) == UPDHL_REMOVED)
				for (DWORD i = pHint->start; i <= pHint->end; ++i)
					pBar->DeleteItem (i);
			else if (((pHint->cmd)&UPDHL_NAME) == UPDHL_NAME)
				for (DWORD i = pHint->start; i <= pHint->end; ++i) {
					if (pBar == &(m_pFrame->m_barImages))
						pBar->SetItemText (i, 0, m_pDoc->Images[i].GetName ());
					else if (pBar == &(m_pFrame->m_barData))
						pBar->SetItemText (i, 0, m_pDoc->Surveis[i].GetName ());
					else if (pBar == &(m_pFrame->m_barReports))
						;//pBar->SetItemText (i, 0, m_pDoc->Reports[i].GetName ());
				}
			pBar->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	} else if (pHint == NULL || pHint->target == CUpdateHint::updtImage) {
		if (!IsWindow (m_wndImage))
			return;
		if (((pHint->cmd)&UPDHI_PALETTE) == UPDHI_PALETTE) {
			m_wndImage.CreateImage ();
			m_wndImage.Invalidate ();
		} if (((pHint->cmd)&UPDHI_BITMAP) == UPDHI_BITMAP)
			m_wndImage.CreateImage ();
		if (((pHint->cmd)&UPDHI_IMAGE) == UPDHI_IMAGE)
			m_wndImage.Invalidate ();
		if (((pHint->cmd)&UPDHI_SCALE) == UPDHI_SCALE) {
			m_wndImage.UpdateScroll ();
			OnChangeScale ();
		}
		// The following updates applicable only if the current image in view equival to pHint->start
//		if (pHint->start != m_pFrame->GetActiveImageNo ())
//			return;
		if ((pHint->cmd) & UPDHI_ADDPATCH ||
		    (pHint->cmd) & UPDHI_ADDMARKER ||
		    (pHint->cmd) & UPDHI_MARKERS ||
		    (pHint->cmd) & UPDHI_IOBJS) {
			m_wndImage.CreateImage ();
			m_wndImage.Invalidate ();
			m_wndImage.RedrawWindow ();
		}
	} else if (pHint == NULL || pHint->target == CUpdateHint::updtSurvey) {
		if (!IsWindow (m_wndData))
			return;
		if ((pHint->cmd) & UPDHS_FIELDS) {
			// #### TODO: Replace with sutable code
			m_wndData.CreateSurvey ();
			m_wndData.Invalidate ();
		}
	}
}

void CStorageView::SetZoom (UINT n) {
	CUpdateHint *pHint;
	if (m_pFrame->GetActiveList () == 0) {
		m_nZoomImg = n;
		m_strZoom.Format ("%d%%", m_nZoomImg);
		pHint = new CUpdateHint (CUpdateHint::updtImage, UPDHIM_ALLVIEWABLE);
	} else if (m_pFrame->GetActiveList () == 2) {
		m_nZoomRpt = n;
		m_strZoom.Format ("%d%%", m_nZoomRpt);
		pHint = new CUpdateHint (CUpdateHint::updtReport, UPDHRM_ALLVIEWABLE);
	} else return;
	m_wndImage.m_nHScrollPos = 0;
	m_wndImage.m_nVScrollPos = 0;
	OnUpdate (NULL, NULL, pHint);
}

int CStorageView::GetZoom () {
	UINT nPartition = m_pFrame->GetActiveList ();
	return ((nPartition == 0)?m_nZoomImg:((nPartition == 2)?m_nZoomRpt:-2));
}

bool CStorageView::GetZoomEnable () {
	UINT nPartition = m_pFrame->GetActiveList ();
	return (!(nPartition == 1));
}

void CStorageView::OnModeChange (UINT nID) {
	m_nMode = nID;
}

void CStorageView::OnModeUpdate (CCmdUI* pCmdUI) {
	pCmdUI->Enable ();
	pCmdUI->SetCheck (pCmdUI->m_nID == m_nMode);
}

void CStorageView::OnLayerChange (UINT nID) {
	m_nLayer = nID - ID_VIEW_LAYERS_BACKGROUND;
}

void CStorageView::OnLayerUpdate (CCmdUI* pCmdUI) {
	pCmdUI->Enable ();
	pCmdUI->SetCheck (pCmdUI->m_nID - ID_VIEW_LAYERS_BACKGROUND == m_nLayer);
}

void CStorageView::OnZoomChange (UINT nID) {
	switch (nID) {
		case ID_VIEW_ZOOM_25:	SetZoom (25); break;
		case ID_VIEW_ZOOM_33:	SetZoom (33); break;
		case ID_VIEW_ZOOM_50:	SetZoom (50); break;
		case ID_VIEW_ZOOM_66:	SetZoom (66); break;
		case ID_VIEW_ZOOM_75:	SetZoom (75); break;
		case ID_VIEW_ZOOM_1TO1:	SetZoom (100); break;
		case ID_VIEW_ZOOM_125:	SetZoom (125); break;
		case ID_VIEW_ZOOM_150:	SetZoom (150); break;
		case ID_VIEW_ZOOM_200:	SetZoom (200); break;
		case ID_VIEW_ZOOM_250:	SetZoom (250); break;
		case ID_VIEW_ZOOM_300:	SetZoom (300); break;
		case ID_VIEW_ZOOM_400:	SetZoom (400); break;
		case ID_VIEW_ZOOM_500:	SetZoom (500); break;
		case ID_VIEW_ZOOM_750:	SetZoom (750); break;
		case ID_VIEW_ZOOM_1000:	SetZoom (1000); break;
		case ID_VIEW_ZOOM_FIT:
			SetZoom (100);
			break;
		case ID_VIEW_ZOOM_OTHER:
			SetZoom (100);
			break;
	};
}

void CStorageView::OnZoomUpdate (CCmdUI* pCmdUI) {
	pCmdUI->Enable (GetZoomEnable());
	pCmdUI->SetCheck (pCmdUI->m_nID == GetZoom());
}

/*void CStorageView::OnUpdateViewZoom(CCmdUI* pCmdUI) {
	if (m_strZoom == m_pFrame->m_wndToolBar.GetButtonText (8))
		return;
	m_pFrame->m_wndToolBar.SetButtonText (11, m_strZoom);
	m_pFrame->m_wndToolBar.GetToolBarCtrl ().RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}*/

void CStorageView::OnLButtonDown (UINT nFlags, CPoint point) {
	SetCapture ();
	CView::OnLButtonDown(nFlags, point);
}

void CStorageView::OnLButtonUp (UINT nFlags, CPoint point) {
	if (GetCapture () != this) {
		CView::OnLButtonUp(nFlags, point);
		return;
	}
	ReleaseCapture ();
}

void CStorageView::OnMouseMove (UINT nFlags, CPoint point) {
	if (GetCapture () != this) {
		CView::OnMouseMove(nFlags, point);
		return;
	}

	if (point.x == 0)
		return;

	CRect rcWin;
	GetClientRect (&rcWin);

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE;
	m_pFrame->m_barShortcuts.GetReBarCtrl().GetBandInfo (0, &rbbi);
	rbbi.cyChild += point.x;
	m_pFrame->m_barShortcuts.GetReBarCtrl().SetBandInfo (0, &rbbi);

	m_pFrame->m_barShortcuts.GetReBarCtrl().GetBandInfo (1, &rbbi);
	rbbi.cyChild += point.x;
	m_pFrame->m_barShortcuts.GetReBarCtrl().SetBandInfo (1, &rbbi);

	m_pFrame->m_barShortcuts.GetReBarCtrl().GetBandInfo (2, &rbbi);
	rbbi.cyChild += point.x;
	m_pFrame->m_barShortcuts.GetReBarCtrl().SetBandInfo (2, &rbbi);
}

BOOL CStorageView::OnSetCursor (CWnd* pWnd, UINT nHitTest, UINT message){
	if (nHitTest != HTCLIENT)
		return CView::OnSetCursor(pWnd, nHitTest, message);
	HCURSOR cursor = theApp.LoadCursor (AFX_IDC_HSPLITBAR);
	SetCursor (cursor);
}

void CStorageView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (m_pFrame->GetActiveList () == 0)
		m_wndImage.OnKeyDown (nChar, nRepCnt, nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CStorageView::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt) {
	if (m_pFrame->GetActiveList () == 0)
		return m_wndImage.OnMouseWheel (nFlags, zDelta, pt);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CStorageView::OnEditDelete () {
	if (GetActiveWindow ()->m_hWnd == m_wndImage.m_hWnd)
		return;
	UINT n = m_pDoc->Frames[aimActive].GetActiveImageNo ();
	m_pDoc->Images(aimSelected).Remove ();
	if (m_pDoc->Images.GetCount() <= n)
		m_pDoc->Frames[aimActive].SetActiveImageNo (m_pDoc->Images.GetCount() - 1);
	else
		m_pDoc->Frames[aimActive].SetActiveImageNo (n);
}
