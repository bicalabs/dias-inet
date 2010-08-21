#if !defined(AFX_FIELD_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
#define AFX_FIELD_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_

#pragma once

class CField : public CObject {
friend class CHash<CField*,CField>;
friend class CaImAPIRange<CField>;
friend class CaImAPICollection<CField>;
friend class CaImAPICollectionExt<CField>;
friend class CDiasAPIFields;
friend class CSurvey;
	DECLARE_SERIAL (CField);
// Construction
public:
	CField ();
	CField (CSurvey *pSrv, CString strVar, CString strName, CString strDescr);
	CField (const CField& flg);
	~CField ();

	CField& operator= (const CField& fld);

// Attributes
protected:
	CSurvey*	m_pSrv;
	CStorageDoc*	m_pDoc;

	CString		m_strName;
	CString		m_strVariable;
	CString		m_strDescr;
	CString		m_strUnits;
	FieldType	m_nType;

	CHash<void*,void*>*	m_pHead;

// Operations
public:
	svar32_64	GetPos ();
	CString		GetName ();
	CString		GetDescr ();
	void		SetPos (uvar32_64 before);
	void		SetName (CString name);
	void		SetDescr (CString descr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CField)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELD_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
