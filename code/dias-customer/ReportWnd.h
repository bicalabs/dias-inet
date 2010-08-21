#if !defined(AFX_REPORTWND_H__13D955E7_5352_4B7B_91DB_252B89B52002__INCLUDED_)
#define AFX_REPORTWND_H__13D955E7_5352_4B7B_91DB_252B89B52002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportWnd window

class CStorageDoc;
class CStorageView;
class CStorageFrame;

class CReportWnd : public CWnd
{
// Construction
public:
	CReportWnd();

// Attributes
public:
	CStorageDoc*	m_pDoc;
	CStorageView*	m_pView;
	CStorageFrame*	m_pFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTWND_H__13D955E7_5352_4B7B_91DB_252B89B52002__INCLUDED_)
