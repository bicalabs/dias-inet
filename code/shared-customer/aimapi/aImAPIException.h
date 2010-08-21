#if !defined(AFX_AIMAPIEXCEPTION_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_)
#define AFX_AIMAPIEXCEPTION_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_

#pragma once

class _DECLSPEC CaImAPIException {
// Construction
public:
	CaImAPIException (udword type);
	~CaImAPIException ();

// Attributes
public:
	udword	m_nType;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaImAPIEXCEPTION)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_AIMAPIEXCEPTION_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_)
