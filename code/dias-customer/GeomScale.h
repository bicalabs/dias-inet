#if !defined(AFX_GEOMSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
#define AFX_GEOMSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_

#pragma once

class CGeomScale : public CObject {
friend class CHash<CGeomScale*,CGeomScale>;
friend class CaImAPICollection<CGeomScale>;
friend class CaImAPICollectionExt<CGeomScale>;
friend class CDiasAPIGeomScales;
	DECLARE_SERIAL (CGeomScale);
// Construction
public:
	CGeomScale ();
	CGeomScale (CString name, CString descr, CString units, rqword rx, rqword ry);
	CGeomScale (const CGeomScale& scale);
	~CGeomScale ();
	
	CGeomScale& operator= (const CGeomScale& scale);

// Attributes
protected:
	CString		m_strName;
	CString		m_strDescr;
	CString		m_strUnits;
	rqword		m_rCoefX;
	rqword		m_rCoefY;

// Operations
public:
	uvar32_64	GetPos ();
	CString		GetName ();
	void		SetName (CString name);
	CString		GetDescr ();
	void		SetDescr (CString descr);
	CString		GetUnits ();
	void		SetUnits (CString units);
	rqword		GetCoefX ();
	rqword		GetCoefY ();
	void		SetCoefs (rqword c1, rqword c2);
	void		Activate ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeomScale)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
