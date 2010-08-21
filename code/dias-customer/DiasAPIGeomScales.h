#if !defined(AFX_DIASAPIGEOMSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIGEOMSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIGeomScales : public CaImAPICollectionExt<CGeomScale> {
friend class CDiasApp;
friend class CGeomScale;
// Construction
public:
	CDiasAPIGeomScales ();
	~CDiasAPIGeomScales ();
	CDiasAPIGeomScales& operator= (const CDiasAPIGeomScales& coll);

// Attributes:
protected:
	uvar32_64 m_nActive;

// Operations
public:
	CGeomScale& operator[] (uvar32_64 pos) throw (CaImAPIException);
	CGeomScale& operator[] (aImAPIRngType t) throw (CaImAPIException);

	uvar32_64 Add (CGeomScale& scale);
	uvar32_64 Insert (CString name, CGeomScale& scale);
	uvar32_64 Insert (uvar32_64 pos, CGeomScale& scale);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIGeomScales)
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIGEOMSCALES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
