#if !defined(AFX_REPORT_H__384B98C0_02E6_4AB3_B58B_A1CF741BCC5B__INCLUDED_)
#define AFX_REPORT_H__384B98C0_02E6_4AB3_B58B_A1CF741BCC5B__INCLUDED_

#pragma once

class CReport : public CObject {
	DECLARE_SERIAL (CReport);
// Construction
public:
	CReport ();
	~CReport ();

// Attributes
public:
	CString		m_strName;
	CString		m_strDescript;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReport)
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORT_H__384B98C0_02E6_4AB3_B58B_A1CF741BCC5B__INCLUDED_)
