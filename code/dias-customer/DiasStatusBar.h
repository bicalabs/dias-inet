#if !defined(AFX_DIASSTATUSBAR_H__E55268C2_BB5A_495C_8A68_29B96EE6B14A__INCLUDED_)
#define AFX_DIASSTATUSBAR_H__E55268C2_BB5A_495C_8A68_29B96EE6B14A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiasStatusBar.h : header file
//

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CDiasStatusBar window

class CDiasStatusBar : public CStatusBar {
// Construction
public:
	CDiasStatusBar();

// Attributes
public:
	CMainFrame	*m_pMainFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDiasStatusBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDiasStatusBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSelectGeomScale (UINT nID);
	afx_msg BOOL OnSelectDensScale (UINT nID);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASSTATUSBAR_H__E55268C2_BB5A_495C_8A68_29B96EE6B14A__INCLUDED_)
