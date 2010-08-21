#if !defined(AFX_DIASAPICHANNELS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPICHANNELS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

#include "Channel.h"
#include "DiasAPIChnlRange.h"

class CDiasAPIChannels : public CaImAPICollectionExt<CChannel> {
friend class CPicture;
// Construction
public:
	CDiasAPIChannels ();
	CDiasAPIChannels (const CDiasAPIChannels& chnl);
	~CDiasAPIChannels ();
	CDiasAPIChannels& operator= (const CDiasAPIChannels& coll);

// Attributes:
protected:
	CStorageDoc* m_pDoc;
	CDiasAPIChnlRange m_rngInt;

// Operations
public:
	CDiasAPIChnlRange& operator() (aImAPIRngType type) throw (CaImAPIException);
	CDiasAPIChnlRange& operator() (int no, ...) throw (CaImAPIException);
	CDiasAPIChnlRange& operator() (char* name, ...) throw (CaImAPIException);

	uvar32_64 Add (uvar32_64 cnt = 1);
	uvar32_64 Insert (svar32_64 pos, uvar32_64 cnt = 1);
	uvar32_64 Insert (CString pos, uvar32_64 cnt = 1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiasAPIChannels)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_DIASAPICHANNELS_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
