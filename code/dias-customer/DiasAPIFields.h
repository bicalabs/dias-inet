#if !defined(AFX_DIASAPIFIELDS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIFIELDS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Field.h"

class CDiasAPIFields : public CaImAPICollectionExt<CField> {
friend class CSurvey;
// Construction
public:
	CDiasAPIFields ();
	~CDiasAPIFields ();
	CDiasAPIFields& operator= (const CDiasAPIFields& coll);

// Attributes:
protected:
	CSurvey			*m_pSrv;
	CStorageDoc		*m_pDoc;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIFields)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPIFIELDS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
