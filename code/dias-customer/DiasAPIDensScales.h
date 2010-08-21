#if !defined(AFX_DIASAPIDENSSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIDENSSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIDensScales : public CaImAPICollectionExt<CDensScale> {
friend class CDiasApp;
friend class CDensScale;
// Construction
public:
	CDiasAPIDensScales ();
	~CDiasAPIDensScales ();
	CDiasAPIDensScales& operator= (const CDiasAPIDensScales& coll);

// Attributes:
protected:
	uvar32_64 m_nActive;

// Operations
public:
	CDensScale& operator[] (uvar32_64 pos) throw (CaImAPIException);
	CDensScale& operator[] (aImAPIRngType t) throw (CaImAPIException);

	uvar32_64 Add (CDensScale& scale);
	uvar32_64 Insert (CString name, CDensScale& scale);
	uvar32_64 Insert (uvar32_64 pos, CDensScale& scale);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIDensScales)
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIDENSSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
