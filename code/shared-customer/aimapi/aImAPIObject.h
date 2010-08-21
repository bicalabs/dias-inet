#if !defined(AFX_AIMAPIOBJECT_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_)
#define AFX_AIMAPIOBJECT_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_

#pragma once

class _DECLSPEC CaImAPIObject : public CObject {
	DECLARE_SERIAL (CaImAPIObject)
// Construction
public:
	CaImAPIObject ();
	CaImAPIObject (const CaImAPIObject& obj);
	~CaImAPIObject ();

	CaImAPIObject& operator= (const CaImAPIObject& obj);

// Operations
public:
	CString GetName ();
	CString GetDescr ();

	bool SetName (const CString strName);
	void SetDescr (const CString strDescr);

// Attributes
public:
	CString		m_strName;
	CString		m_strDescr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaImAPIObject)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_AIMAPIOBJECT_H__EE5F2575_0988_477E_85B9_C24BFC6C095B__INCLUDED_)
