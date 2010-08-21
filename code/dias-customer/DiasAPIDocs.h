#if !defined(AFX_DIASAPIDOCS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIDOCS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "StorageDoc.h"
#include "DiasAPIDocRange.h"

class CDiasAPIDocs : public CaImAPICollection<CStorageDoc> {
friend class CStorageDoc;
// Construction
public:
	CDiasAPIDocs ();
	~CDiasAPIDocs ();

// Attributes:
protected:
	CDiasAPIDocRange m_rngInt;

// Operations
public:
	CStorageDoc& operator[] (aImAPIRngType t) throw (CaImAPIException);

	bool Create (uchar* title, uchar* path, uword x, uword y, ColorModel cm, ubyte bpp, bool svae = false);
	bool Open (uchar* title, uchar* path);
	bool Open (uchar* fullname);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIDocs)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIDOCS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
