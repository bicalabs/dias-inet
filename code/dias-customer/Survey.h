#if !defined(AFX_SURVEY_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
#define AFX_SURVEY_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_

#pragma once

#include "DiasAPIFields.h"

class CSurvey : public CObject {
friend class CHash<CSurvey*,CSurvey>;
friend class CaImAPIRange<CSurvey>;
friend class CaImAPICollection<CSurvey>;
friend class CaImAPICollectionExt<CSurvey>;
friend class CDiasAPISrvRange;
friend class CDiasAPISurveis;
friend class CStorageDoc;
friend class CField;
// Construction
	DECLARE_SERIAL (CSurvey);
protected:
	CSurvey () : m_strName (""), m_strDescript ("") { ; }
	CSurvey (const CSurvey& srv);
	CSurvey (CStorageDoc* pDoc, CString name, CString descr);
public:
	~CSurvey ();
	CSurvey& operator= (const CSurvey& srv) throw (CaImAPIException);

// Attributes
public:
	CDiasAPIFields	Fields;

protected:
	CStorageDoc*	m_pDoc;

	CString		m_strName;
	CString		m_strDescript;

// Operations
public:
	svar32_64	GetPos ();
	CString		GetName ();
	CString		GetDescription ();
	void		SetPos (uvar32_64 before);
	void		SetName (CString name);
	void		SetDescription (CString descr);

	void		Reset ();
	void		Append ();
	void		Remove (uvar32_64 pos);
	uvar32_64	RecCount ();

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurvey)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURVEY_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
