#if !defined(AFX_DENSSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
#define AFX_DENSSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_

#pragma once

class CDensScale : public CObject {
friend class CHash<CDensScale*,CDensScale>;
friend class CaImAPICollection<CDensScale>;
friend class CaImAPICollectionExt<CDensScale>;
friend class CDiasAPIDensScales;
	DECLARE_SERIAL (CDensScale);
// Construction
public:
	CDensScale ();
	CDensScale (CString name, CString descr, CString units, DensScale scale, rqword rx, rqword ry);
	CDensScale (const CDensScale& scale);
	~CDensScale ();

	CDensScale& operator= (const CDensScale& scale);

// Attributes
protected:
	CString		m_strName;
	CString		m_strDescr;
	CString		m_strUnits;
	DensScale	m_nScale;
	rqword		m_rCoef1;
	rqword		m_rCoef2;

// Operations
public:
	uvar32_64	GetPos ();
	CString		GetName ();
	void		SetName (CString name);
	CString		GetDescr ();
	void		SetDescr (CString descr);
	CString		GetUnits ();
	void		SetUnits (CString units);
	rqword		GetCoef1 ();
	rqword		GetCoef2 ();
	DensScale	GetScale ();
    	void		SetCoefs (rqword c1, rqword c2);
	void		SetCoefs (DensScale scale, rqword c1, rqword c2);
	void		Activate ();
	rtbyte		Transform (uvar32_64 n);
	void		Transform (ubyte *p$, ubyte *p, uvar32_64 len, ubyte bpp);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDensScale)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DENSSCALE_H__F7FA55DC_271F_42E5_9ACD_E8333817DD8C__INCLUDED_)
