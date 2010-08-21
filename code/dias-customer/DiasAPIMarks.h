#if !defined(AFX_DIASAPIMARKS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIMARKS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Mark.h"
#include "DiasAPIMrkRange.h"

class CDiasAPIMarks : public CaImAPICollectionExt<CMark> {
friend class CPicture;
// Construction
public:
	CDiasAPIMarks ();
	~CDiasAPIMarks ();
	CDiasAPIMarks& operator= (const CDiasAPIMarks& coll);

// Attributes:
protected:
	uvar32_64		m_nActive;
	CPicture		*m_pImg;
	CStorageDoc		*m_pDoc;
	CDiasAPIMrkRange	m_rngInt;

// Operations
public:
	CMark& operator[] (uvar32_64 pos) throw (CaImAPIException);
	CMark& operator[] (aImAPIRngType t) throw (CaImAPIException);
	CMark& operator[] (CPoint pnt) throw (CaImAPIException);

	CDiasAPIMrkRange& operator() (aImAPIRngType type) throw (CaImAPIException);
	CDiasAPIMrkRange& operator() (int no, ...) throw (CaImAPIException);
	CDiasAPIMrkRange& operator() (CString name, ...) throw (CaImAPIException);

	uvar32_64 Add (CPoint& pos);
	uvar32_64 Add (CPoint& pos, const CMark& style);
	uvar32_64 Add (CPoint& pos, CMark::MarkKernel krnl, UINT ksize, COLORREF kc, CMark::MarkShape shp,
	               CSize& ssize, UINT angle, bool slock, bool alock, COLORREF sc, COLORREF fc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIMarks)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIMARKS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
