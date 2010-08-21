
/*#ifndef _AIM_MAINMODULE
	#define _DECLSPEC __declspec(dllexport)
#else
	#define _DECLSPEC __declspec(dllimport)
#endif*/
#define _DECLSPEC


enum VariableType { varString = 0, varInt = 1, varReal = 2 };

class CTesla;
class CVariable;
class CTeslaAPIVariables;

/////////////////////////////////////////////////////////////////////////////
// CTeslaAPIVariables

class _DECLSPEC CTeslaAPIVariables : public CaImAPICollectionExt<CVariable> {
friend class CTesla;
// Construction
public:
	CTeslaAPIVariables ();
	CTeslaAPIVariables (const CTeslaAPIVariables& coll);
	~CTeslaAPIVariables ();
	CTeslaAPIVariables& operator= (const CTeslaAPIVariables& coll);

// Attributes:
protected:
	CTesla		*m_pTesla;

// Operations
public:
	CVariable& operator[] (uvar32_64 pos);
	CVariable& operator[] (CString name);

	uvar32_64 Add (CVariable& item);
	void Delete (uvar32_64 pos);
	uvar32_64 DeleteAll ();

	uvar32_64 Import (svar32_64 pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Import (CString pos, uchar* fullname = NULL, bool errmsg = true);
	uvar32_64 Export (svar32_64 start, svar32_64 end, uchar* filenamemask = NULL, bool errmsg = true);
	uvar32_64 Export (CString start, CString end, uchar* filenamemask = NULL, bool errmsg = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeslaAPIVariables)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////
// CVariable

class _DECLSPEC CVariable : public CaImAPIObject {
friend class CHash<CVariable*,CVariable>;
friend class CaImAPIObject;
friend class CaImAPIRange<CVariable>;
friend class CaImAPICollection<CVariable>;
friend class CaImAPICollectionExt<CVariable>;
friend class CTeslaAPIVariables;
// Construction
public:
	CVariable ();
	CVariable (const CVariable &var);
	CVariable (CTesla *pTesla, CString strName, CString strValue, CString strUnits = "", CString strDescript = "");
	CVariable (CTesla *pTesla, CString strName, uvar32_64 nValue, CString strUnits = "", CString strDescript = "");
	CVariable (CTesla *pTesla, CString strName, rtbyte rValue, CString strUnits = "", CString strDescript = "");
	~CVariable ();

	CVariable& operator= (const CVariable& var);
	CVariable& operator= (const CString strVal);
	CVariable& operator= (const uvar32_64 nVal);
	CVariable& operator= (const rtbyte rVal);

	uvar32_64 GetPos ();
	CString GetName ();
	CString GetClassName ();
	CString GetEndName ();
	CString GetUnits ();
	CString GetDescr ();
	CString GetExpr ();
	VariableType GetType ();
	void GetValue (CString &strVal);
	void GetValue (uvar32_64 &nVal);
	void GetValue (rtbyte &rVal);
	bool IsDynamic ();

	bool SetPos (uvar32_64 nPos);
	bool SetName (const CString strName);
	bool SetClassName (const CString strName);
	bool SetEndName (const CString strName);
	void SetUnits (const CString strUnits);
	void SetDescr (const CString strDescr);
	bool SetExpr (const CString strExpr);
	void SetValue (const CString strVal);
	void SetValue (const uvar32_64 nVal);
	void SetValue (const rtbyte rVal);

protected:
	CTesla		*m_pTesla;

	CString		m_strUnits;
	CString		m_strExpr;
	VariableType	m_nType;
	bool		m_blDynamic;
	struct UV {
		uchar		*m_pcVal;
		uvar32_64	m_nVal;
		rtbyte		m_rVal;
	}		m_uValue;
};

/////////////////////////////////////////////////////////////////////////////
// CTesla

class _DECLSPEC CTesla {
public:
	CTesla ();
	~CTesla ();

public:
	CTeslaAPIVariables	Variables;
};

