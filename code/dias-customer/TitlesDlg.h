#if !defined(AFX_TITLESDLG_H__049F6870_730E_4C10_A218_82AA28769D7C__INCLUDED_)
#define AFX_TITLESDLG_H__049F6870_730E_4C10_A218_82AA28769D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitlesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTitlesDlg dialog

class CTitlesDlg : public CDialog
{
// Construction
public:
	CTitlesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTitlesDlg)
	enum { IDD = IDD_TITLES };
	CString	m_strTitle;
	CString	m_strDescr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitlesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTitlesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLESDLG_H__049F6870_730E_4C10_A218_82AA28769D7C__INCLUDED_)
