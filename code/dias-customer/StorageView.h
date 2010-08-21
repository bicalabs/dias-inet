#if !defined(AFX_STORAGEVIEW_H__F21A00E6_C0F6_4128_8FAE_BFB61E445A1E__INCLUDED_)
#define AFX_STORAGEVIEW_H__F21A00E6_C0F6_4128_8FAE_BFB61E445A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StorageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStorageView view

#include "StorageDoc.h"
#include "StorageFrame.h"

#include "ImageWnd.h"
#include "DataWnd.h"
#include "ReportWnd.h"
#include "ScaleWnd.h"

class CStorageView : public CView {
friend class CStorageDoc;
friend class CStorageFrame;
friend class CPictureWnd;
friend class CDataWnd;
friend class CReportWnd;
friend class CDocList;
friend class CDocPane;
friend class CDocRuler;
protected:
	CStorageView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStorageView)

// Attributes
protected:
	CStorageDoc	*m_pDoc;
	CStorageFrame	*m_pFrame;
	CPictureWnd	m_wndImage;
	CDataWnd	m_wndData;
	CReportWnd	m_wndReport;
	CScaleWnd	m_wndHScale;
	CScaleWnd	m_wndVScale;
	CScaleWnd	m_wndCScale;
	CWnd		m_wndSplit;

	uword		m_nLayer;
	udword		m_nMode;
	UINT		m_nZoomImg;
	UINT		m_nZoomRpt;
	bool		m_blImgScale;
	bool		m_blRepScale;
	CString		m_strZoom;

// Operations
public:
	udword		GetMode () { return (m_nMode); };

	udword		GetLayer () { return (m_nLayer); };

	int		GetZoom ();
	bool		GetZoomEnable ();
	void		SetZoom (UINT n);

	void		OnChangeBar (int iIndex);
	void		OnChangeScale ();
	void		OnChangeImage (int iIndex);
	void		OnChangeSurvey (int iIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStorageView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStorageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CStorageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnModeChange(UINT nID);
	afx_msg void OnModeUpdate(CCmdUI* pCmdUI);
	afx_msg void OnZoomChange(UINT nID);
	afx_msg void OnZoomUpdate(CCmdUI* pCmdUI);
	afx_msg void OnLayerChange(UINT nID);
	afx_msg void OnLayerUpdate(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditDelete();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STORAGEVIEW_H__F21A00E6_C0F6_4128_8FAE_BFB61E445A1E__INCLUDED_)
