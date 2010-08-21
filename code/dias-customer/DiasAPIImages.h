#if !defined(AFX_DIASAPIIMAGES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIIMAGES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Image.h"
#include "DiasAPIImgRange.h"

class CDiasAPIImages : public CaImAPICollectionExt<CPicture> {
friend class CStorageDoc;
// Construction
public:
	CDiasAPIImages ();
	~CDiasAPIImages ();
	CDiasAPIImages& operator= (const CDiasAPIImages& coll);

// Attributes:
protected:
	CStorageDoc*	m_pDoc;
	CDiasAPIImgRange m_rngInt;

// Operations
public:
	CPicture& operator[] (uvar32_64 pos);
	CPicture& operator[] (aImAPIRngType t) throw (CaImAPIException);
	CDiasAPIImgRange& operator() (aImAPIRngType type) throw (CaImAPIException);
	CDiasAPIImgRange& operator() (int no, ...) throw (CaImAPIException);
	CDiasAPIImgRange& operator() (char* name, ...) throw (CaImAPIException);

	uvar32_64 Add (CPicture& item);
	uvar32_64 Add (CaImAPIRange<CPicture>& item);
	uvar32_64 Insert (uvar32_64 pos, CPicture& item);
	uvar32_64 Insert (CString name, CPicture& item);
	uvar32_64 Insert (uvar32_64 pos, CaImAPIRange<CPicture>& item);
	uvar32_64 Insert (CString name, CaImAPIRange<CPicture>& item);
	void Delete (uvar32_64 pos);
	uvar32_64 DeleteAll ();

	uvar32_64 Add (uvar32_64 cnt = 1);
	uvar32_64 Add (CString name);
	uvar32_64 Insert (svar32_64 pos, uvar32_64 cnt = 1);
	uvar32_64 Insert (svar32_64 pos, CString name);
	uvar32_64 Insert (CString pos, uvar32_64 cnt = 1);
	uvar32_64 Insert (CString pos, CString name);
	uvar32_64 Import (svar32_64 pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Import (CString pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Export (svar32_64 start, svar32_64 end, uchar* filenamemask = NULL, bool errmsg = true);
	uvar32_64 Export (CString start, CString end, uchar* filenamemask = NULL, bool errmsg = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIImages)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIIMAGES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
