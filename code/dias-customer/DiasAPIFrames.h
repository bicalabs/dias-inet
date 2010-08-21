#if !defined(AFX_DIASAPIFRAMES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIFRAMES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "StorageFrame.h"

class CDiasAPIFrames : public CaImAPICollection<CStorageFrame> {
friend class CStorageDoc;
// Construction
public:
	CDiasAPIFrames ();
	~CDiasAPIFrames ();

// Attributes:
protected:
	CStorageDoc*	m_pDoc;

// Operations
public:
	CStorageFrame& operator[] (aImAPIRngType t) throw (CaImAPIException);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIFrames)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIFRAMES_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
