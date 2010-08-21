#if !defined(AFX_RSC_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
#define AFX_RSC_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_

#pragma once

class CFieldInfo : public CObject {
	DECLARE_SERIAL (CFieldInfo);
// Construction
public:
	CFieldInfo ();
	CFieldInfo (FieldType type, CString name, CString descr, CString units);
	CFieldInfo (const CFieldInfo& scale);
	~CFieldInfo ();

	CFieldInfo& operator= (const CFieldInfo& scale);

// Attributes
protected:
	FieldType	m_nType;
	CString		m_strName;
	CString		m_strDescr;
	CString		m_strUnits;

// Operations
public:
	FieldType	GetType ();
	void		SetType (FieldType type);
	CString		GetName ();
	void		SetName (CString name);
	CString		GetDescr ();
	void		SetDescr (CString descr);
	CString		GetUnits ();
	void		SetUnits (CString units);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldInfo)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSC_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
