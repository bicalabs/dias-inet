#include "stdafx.h"
#include "Dias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDensScale

IMPLEMENT_SERIAL (CDensScale, CObject, 1);

CDensScale::CDensScale () {
	;
}
CDensScale::CDensScale (CString name, CString descr, CString units, DensScale scale, rqword rx, rqword ry) :
	m_strName (name), m_strDescr (descr), m_strUnits (units), m_nScale (scale), m_rCoef1 (rx), m_rCoef2 (ry) {
	;
}

CDensScale::CDensScale (const CDensScale& scale) {
	*this = scale;
}
CDensScale::~CDensScale () {
	;
}

CDensScale& CDensScale::operator= (const CDensScale& scale) {
	m_strName = scale.m_strName;
	m_strDescr = scale.m_strDescr;
	m_strUnits = scale.m_strUnits;
	m_nScale = scale.m_nScale;
	m_rCoef1 = scale.m_rCoef1;
	m_rCoef2 = scale.m_rCoef2;
	return (*this);
}

uvar32_64 CDensScale::GetPos () {
	for (CHash<CDensScale*,CDensScale>* p = theApp.DensScales.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			if (theApp.DensScales.m_nActive == p->GetPos ())
				return (p->GetPos ());
	return (uvar32_64(-1));
}

CString CDensScale::GetName () {
	return (m_strName);
}
void CDensScale::SetName (CString name) {
	if (theApp.DensScales.NameLookup (name) != (uvar32_64)-1)
		return;
	m_strName = name;
	theApp.Update (UPDHA_DENSSCALE);
}
CString CDensScale::GetDescr () {
	return (m_strDescr);
}
void CDensScale::SetDescr (CString descr) {
	m_strDescr = descr;
	theApp.Update (UPDHA_DENSSCALE);
}
CString CDensScale::GetUnits () {
	return (m_strUnits); 
}
void CDensScale::SetUnits (CString units) {
	m_strUnits = units;
	theApp.Update (UPDHA_DENSSCALE);
}
rqword CDensScale::GetCoef1 () {
	return (m_rCoef1); 
}
rqword CDensScale::GetCoef2 () {
	return (m_rCoef2); 
}
DensScale CDensScale::GetScale () {
	return (m_nScale); 
}
void CDensScale::SetCoefs (rqword c1, rqword c2) {
	m_rCoef1 = c1;
	m_rCoef2 = c2;
	theApp.Update (UPDHA_DENSSCALE);
}
void CDensScale::SetCoefs (DensScale scale, rqword c1, rqword c2) {
	m_nScale = scale;
	m_rCoef1 = c1;
	m_rCoef2 = c2;
	theApp.Update (UPDHA_DENSSCALE);
}

rtbyte CDensScale::Transform (uvar32_64 n) {
	rtbyte r;
	switch (m_nScale) {
		case scaleLinear:
			r = m_rCoef1 * n + m_rCoef2;
			break;
		case scalePolynom:
			r = m_rCoef1 * pow (n, m_rCoef2);
			break;
		case scaleLog:
			if (m_rCoef2 <= 0 || n <= 0)
				r = 0;
			else
				r = m_rCoef1 * log ((long double)n) / log ((long double)m_rCoef2);
			break;
		case scaleExp:
			r = m_rCoef1 * pow ((long double)m_rCoef2, (long double)n);
			break;
	}
	return (r);
}

void CDensScale::Transform (ubyte *p$, ubyte *p, uvar32_64 len, ubyte bpp) {
	rtbyte	r;
	uvar32_64 n;
	uword	*pw$ = (uword*)p$,
		*pw = (uword*)p;
	udword	*pd$ = (udword*)p$,
		*pd = (udword*)p;
	bpp /= 8;
	switch (m_nScale) {
		case scaleLinear:
			for (n = 0 ; n < len ; ++n) {
				if (bpp == 1)	p$[n] = (ubyte)(m_rCoef1 * p[n] + m_rCoef2);
				if (bpp == 2)	pw$[n] = (uword)(m_rCoef1 * pd[n] + m_rCoef2);
				if (bpp == 3)	pd$[n] = (udword)(m_rCoef1 * pw[n] + m_rCoef2);
			}
			break;
		case scalePolynom:
			for (n = 0 ; n < len ; ++n) {
				if (bpp == 1)	p$[n] = (ubyte)(m_rCoef1 * (long double)pow ((long double)(p[n]), (long double)m_rCoef2));
				if (bpp == 2)	pw$[n] = (uword)(m_rCoef1 * (long double)pow ((long double)(pw[n]), (long double)m_rCoef2));
				if (bpp == 3)	pd$[n] = (udword)(m_rCoef1 * (long double)pow ((long double)(pd[n]), (long double)m_rCoef2));
			}
			break;
		case scaleLog:
			if (m_rCoef2 <= 0)
				r = 0;
			else
				r = m_rCoef1 / log (m_rCoef2);
			for (n = 0 ; n < len ; ++n) {
				if (bpp == 1)	p$[n] = (p[n] <= 0) ? 0 : (ubyte)(r * (long double)log ((long double)(p[n])));
				if (bpp == 2)	pw$[n] = (pw[n] <= 0) ? 0 : (uword)(r * (long double)log ((long double)(pw[n])));
				if (bpp == 3)	pd$[n] = (pd[n] <= 0) ? 0 : (udword)(r * (long double)log ((long double)(pd[n])));
			}
			break;
		case scaleExp:
			for (n = 0 ; n < len ; ++n) {
				if (bpp == 1)	p$[n] = (ubyte)(m_rCoef1 * pow ((long double)m_rCoef2, p[n]));
				if (bpp == 2)	pw$[n] = (uword)(m_rCoef1 * pow ((long double)m_rCoef2, pw[n]));
				if (bpp == 3)	pd$[n] = (udword)(m_rCoef1 * pow ((long double)m_rCoef2, (long double)(pd[n])));
			}
			break;
	}
}

void CDensScale::Activate () {
	for (CHash<CDensScale*,CDensScale>* p = theApp.DensScales.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			theApp.DensScales.m_nActive = p->GetPos ();
	theApp.Update (UPDHA_DENSSCALE);
}

void CDensScale::Serialize (CArchive &ar) {
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescr << m_strUnits;
		ar << (ubyte) m_nScale;
		ar << m_rCoef1 << m_rCoef2;
	} else {
		ar >> m_strName >> m_strDescr >> m_strUnits;
		m_nScale = (DensScale) 0;
		ar >> (ubyte&) m_nScale;
		ar >> m_rCoef1 >> m_rCoef2;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGeomScale

IMPLEMENT_SERIAL (CGeomScale, CObject, 1);

CGeomScale::CGeomScale () {
	;
}
CGeomScale::CGeomScale (CString name, CString descr, CString units, rqword rx, rqword ry) :
	m_strName (name), m_strDescr (descr), m_strUnits (units), m_rCoefX (rx), m_rCoefY (ry) {
	;
}

CGeomScale::CGeomScale (const CGeomScale& scale) {
	*this = scale;
}
CGeomScale::~CGeomScale () {
	;
}

CGeomScale& CGeomScale::operator= (const CGeomScale& scale) {
	m_strName = scale.m_strName;
	m_strDescr = scale.m_strDescr;
	m_strUnits = scale.m_strUnits;
	m_rCoefX = scale.m_rCoefX;
	m_rCoefY = scale.m_rCoefY;
	return (*this);
}

uvar32_64 CGeomScale::GetPos () {
	for (CHash<CGeomScale*,CGeomScale>* p = theApp.GeomScales.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			if (theApp.GeomScales.m_nActive == p->GetPos ())
				return (p->GetPos ());
	return (uvar32_64(-1));
}

CString CGeomScale::GetName () {
	return (m_strName);
}
void CGeomScale::SetName (CString name) {
	if (theApp.GeomScales.NameLookup (name) != (uvar32_64)-1)
		return;
	m_strName = name;
	theApp.Update (UPDHA_GEOMSCALE);
}
CString CGeomScale::GetDescr () {
	return (m_strDescr);
}
void CGeomScale::SetDescr (CString descr) {
	m_strDescr = descr;
	theApp.Update (UPDHA_GEOMSCALE);
}
CString CGeomScale::GetUnits () {
	return (m_strUnits); 
}
void CGeomScale::SetUnits (CString units) {
	m_strUnits = units;
	theApp.Update (UPDHA_GEOMSCALE);
}
rqword CGeomScale::GetCoefX () {
	return (m_rCoefX); 
}
rqword CGeomScale::GetCoefY () {
	return (m_rCoefY); 
}
void CGeomScale::SetCoefs (rqword c1, rqword c2) {
	m_rCoefX = c1;
	m_rCoefY = c2;
	theApp.Update (UPDHA_GEOMSCALE);
}

void CGeomScale::Activate () {
	for (CHash<CGeomScale*,CGeomScale>* p = theApp.GeomScales.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			theApp.GeomScales.m_nActive = p->GetPos ();
	theApp.Update (UPDHA_GEOMSCALE);
}

void CGeomScale::Serialize (CArchive &ar) {
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescr << m_strUnits;
		ar << m_rCoefX << m_rCoefY;
	} else {
		ar >> m_strName >> m_strDescr >> m_strUnits;
		ar >> m_rCoefX >> m_rCoefY;
	}
}
