#if !defined(AFX_DIASAPIPATCHS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIPATCHS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Patch.h"
#include "DiasAPIPtchRange.h"

class CDiasAPIPatches : public CaImAPICollectionExt<CPatch> {
friend class CPicture;
// Construction
public:
	CDiasAPIPatches ();
	~CDiasAPIPatches ();
	CDiasAPIPatches& operator= (const CDiasAPIPatches& coll);

// Attributes:
protected:
	uvar32_64		m_nActive;
	CPicture		*m_pImg;
	CStorageDoc		*m_pDoc;
	CDiasAPIPtchRange	m_rngInt;

// Operations
public:
	CPatch& operator[] (uvar32_64 pos);
	CPatch& operator[] (aImAPIRngType t) throw (CaImAPIException);
	CPatch& operator[] (CPoint pnt) throw (CaImAPIException);
	CDiasAPIPtchRange& operator() (aImAPIRngType type) throw (CaImAPIException);
	CDiasAPIPtchRange& operator() (int no, ...) throw (CaImAPIException);
	CDiasAPIPtchRange& operator() (CString name, ...) throw (CaImAPIException);
	CDiasAPIPtchRange& operator() (CPoint pnt) throw (CaImAPIException);

	uvar32_64 Add (uvar32_64 nLength, uvar32_64* pnOffsets) throw (CaImAPIException);
	uvar32_64 Add (CPatch& item);
	uvar32_64 Create (CColor& cBackground) throw (CaImAPIException);
	uvar32_64 Create (CPicture& imgSource, CColor& cBackground) throw (CaImAPIException);

	void	Activate (uvar32_64 no);
	void	Deactivate ();
	uvar32_64 Active () { return (m_nActive); };
	void	Delete (uvar32_64 no);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIPatches)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIPATCHS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
