#pragma once

#include "PaneView.h"
#include "DiasStatusBar.h"

class CDiasApp;

class CMainFrame : public CMDIFrameWnd {
// *** Constructors
DECLARE_DYNAMIC(CMainFrame)
friend class CDiasApp;
friend class CPaneView;
friend class CDiasStatusBar;
public:
	CMainFrame();
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

// *** Variables
protected:
	UINT		m_nID;
	CImageList	m_ilTbxStd;
	CImageList	m_ilTbxHotStd;
	CImageList	m_ilTbxDisStd;

	CReBar		m_wndReBar;
	CToolBar	m_wndToolBar;
	CDocPane	m_wndPane;
	CPaneView	m_paneView;
	CDiasStatusBar	m_wndStatusBar;

// *** Functions
public:
	void		Update (udword action);

// *** IDE Code
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMainFrame)
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg LONG OnColorChange(UINT wParam, LONG lParam);
	afx_msg void OnEditGeomScales();
	afx_msg void OnEditDensScales();
	afx_msg void OnIndGeomScale();
	afx_msg void OnIndDensScale();
	afx_msg void OnUpdate_IndGeomScale(CCmdUI* pCmdUI);
	afx_msg void OnUpdate_IndDensScale(CCmdUI* pCmdUI);
	afx_msg void OnSelectGeomScale (UINT nID);
	afx_msg void OnSelectDensScale (UINT nID);
	afx_msg void OnMarkerStyleAdd();
	afx_msg void OnMarkerStyleDelete();
	afx_msg void OnMarkerStyleExport();
	afx_msg void OnMarkerStyleImport();
	afx_msg void OnMarkerStyleRename();
	afx_msg void OnMarkerStyleDuplicate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
