#if !defined(AFX_DIASAPISURVEIS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPISURVEIS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Survey.h"
#include "DiasAPISrvRange.h"

class CDiasAPISurveis : public CaImAPICollectionExt<CSurvey> {
friend class CStorageDoc;
// Construction
public:
	CDiasAPISurveis ();
	~CDiasAPISurveis ();
	CDiasAPISurveis& operator= (const CDiasAPISurveis& coll);

// Attributes:
protected:
	CStorageDoc*	m_pDoc;
	CDiasAPISrvRange m_rngInt;

// Operations
public:
	CSurvey& operator[] (uvar32_64 pos);
	CSurvey& operator[] (aImAPIRngType t) throw (CaImAPIException);
	CDiasAPISrvRange& operator() (aImAPIRngType type) throw (CaImAPIException);
	CDiasAPISrvRange& operator() (int no, ...) throw (CaImAPIException);
	CDiasAPISrvRange& operator() (char* name, ...) throw (CaImAPIException);

	uvar32_64 Add (CSurvey& item);
	uvar32_64 Add (CaImAPIRange<CSurvey>& item);
	uvar32_64 Insert (uvar32_64 pos, CSurvey& item);
	uvar32_64 Insert (CString name, CSurvey& item);
	uvar32_64 Insert (uvar32_64 pos, CaImAPIRange<CSurvey>& item);
	uvar32_64 Insert (CString name, CaImAPIRange<CSurvey>& item);
	void Delete (uvar32_64);
	uvar32_64 DeleteAll ();

	uvar32_64 Add (uvar32_64 cnt = 1);
	uvar32_64 Insert (svar32_64 pos, uvar32_64 cnt = 1);
	uvar32_64 Insert (CString pos, uvar32_64 cnt = 1);
	uvar32_64 Import (svar32_64 pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Import (CString pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Export (svar32_64 start, svar32_64 end, uchar* filenamemask = NULL, bool errmsg = true);
	uvar32_64 Export (CString start, CString end, uchar* filenamemask = NULL, bool errmsg = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPISurveis)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPISURVEIS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
