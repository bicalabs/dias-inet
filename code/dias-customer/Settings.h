#if !defined(AFX_SETTINGS_H__BFDE4D98_93B1_49DB_A9F8_2DB7FC73F738__INCLUDED_)
#define AFX_SETTINGS_H__BFDE4D98_93B1_49DB_A9F8_2DB7FC73F738__INCLUDED_

#pragma once

#include "Mark.h"

class CColor;

class CSettings : public CObject {
friend class CMrkPropBar;
friend class CDiasAPIMarks;
friend class CMark;
public:
	enum SettingQuery { sqMin, sqMax, sqCurrent, sqStep, sqCount, sqUnit, sqName, sqMenuID, sqBmpID };
// Construction
public:
	CSettings ();
	~CSettings ();

// Attributes
public:
	// FieldInfo
	CTree<CFieldInfo,CFieldInfo&> *m_prootFieldInfo;

protected:
	// Defaut image attributes
	CString	m_strImageName;
	CString	m_strPatchName;
	CString	m_strSurveyName;

	// Patch attributes
	udword	m_nPatchColor;
	udword	m_nPatchColorMask;

// Operations
public:
	bool Load ();
	bool Save ();

	int GetSetting (UINT nID, SettingQuery nQueryType, bool bl = true, UINT nSubNo = 0);
	CString GetSetting (UINT nID, SettingQuery nQueryType, UINT nSubNo);
	void SetSetting (UINT nID, int iVal);
	void SetSetting (UINT nID, CString strVal);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettings)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGS_H__BFDE4D98_93B1_49DB_A9F8_2DB7FC73F738__INCLUDED_)
