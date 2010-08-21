#if !defined(AFX_DIASAPIMARKSTYLES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIMARKSTYLES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIMarkStyles : public CaImAPICollectionExt<CMarkStyle> {
friend class CDiasApp;
friend class CMarkStyle;
// Construction
public:
	CDiasAPIMarkStyles ();
	~CDiasAPIMarkStyles ();
	CDiasAPIMarkStyles& operator= (const CDiasAPIMarkStyles& coll);

// Attributes:
protected:
	uvar32_64 m_nActive;

// Operations
public:
	CMarkStyle& operator[] (uvar32_64 pos) throw (CaImAPIException);
	CMarkStyle& operator[] (aImAPIRngType t) throw (CaImAPIException);

	uvar32_64 Add (CMarkStyle& scale);
	uvar32_64 Insert (CString name, CMarkStyle& scale);
	uvar32_64 Insert (uvar32_64 pos, CMarkStyle& scale);
	uvar32_64 Select (CString strTitle);
	uvar32_64 GetSelected () { return m_nActive; };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIMarkStyles)
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIMARKSTYLES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
