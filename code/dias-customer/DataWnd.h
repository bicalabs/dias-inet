#if !defined(AFX_DATAWND_H__D18EB6D3_80E2_41E5_8C4F_299EFD2A20A3__INCLUDED_)
#define AFX_DATAWND_H__D18EB6D3_80E2_41E5_8C4F_299EFD2A20A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataWnd view

class CStorageDoc;
class CStorageView;
class CStorageFrame;

class CDataWnd : public CListCtrl {
public:
	CDataWnd();           // protected constructor used by dynamic creation
	virtual ~CDataWnd();
	DECLARE_DYNCREATE(CDataWnd)

// Attributes
public:
	CStorageDoc*	m_pDoc;
	CStorageView*	m_pView;
	CStorageFrame*	m_pFrame;

// Operations
public:
	void CreateSurvey ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAWND_H__D18EB6D3_80E2_41E5_8C4F_299EFD2A20A3__INCLUDED_)
