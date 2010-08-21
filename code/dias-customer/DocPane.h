#if !defined(AFX_DOCPANE_H__DF98AB0A_BF1B_41F3_B3A0_73D7946D2603__INCLUDED_)
#define AFX_DOCPANE_H__DF98AB0A_BF1B_41F3_B3A0_73D7946D2603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DocPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDocPane window

class CStorageView;

class CDocPane : public CReBar {
	DECLARE_DYNCREATE(CDocPane)
// Construction
public:
	CDocPane();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDocPane();
	int	GetActiveIndex () { return (0); };
	CSize	CalcFixedLayout (BOOL bStretch, BOOL bHorz);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDocPane)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCPANE_H__DF98AB0A_BF1B_41F3_B3A0_73D7946D2603__INCLUDED_)
