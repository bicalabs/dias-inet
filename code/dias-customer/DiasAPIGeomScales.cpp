#include "stdafx.h"
#include "dias.h"
#include "MainFrame.h"
#include "DiasAPIGeomScales.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIGeomScales

CDiasAPIGeomScales::CDiasAPIGeomScales () {
}

CDiasAPIGeomScales::~CDiasAPIGeomScales () {
}

CDiasAPIGeomScales& CDiasAPIGeomScales::operator= (const CDiasAPIGeomScales& coll) {
	return ((CDiasAPIGeomScales&) (CaImAPICollection<CGeomScale>::operator= ( (CaImAPICollection<CGeomScale>&)coll )));
}

CGeomScale& CDiasAPIGeomScales::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CGeomScale>::operator[](pos));
}

CGeomScale& CDiasAPIGeomScales::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	return (CaImAPICollectionExt<CGeomScale>::operator[](m_nActive));
}

uvar32_64 CDiasAPIGeomScales::Add (CGeomScale& scale) {
	int pos;
	if ((pos = NameLookup (scale.m_strName)) != uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CGeomScale*,CGeomScale>* p = new CHash<CGeomScale*,CGeomScale>(&scale);
	if (!m_pHashHead)
		m_pHashHead = p;
	else
		m_pHashHead->GetTail()->InsertAfter (p);
	return ((uvar32_64)(p->GetPos()));
	
}

uvar32_64 CDiasAPIGeomScales::Insert (CString name, CGeomScale& scale) {
	int pos;
	if ((pos = NameLookup (name)) == uvar32_64(-1))
		throw (0);
	if ((pos = NameLookup (scale.m_strName)) == uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CGeomScale*,CGeomScale>* p = new CHash<CGeomScale*,CGeomScale>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

uvar32_64 CDiasAPIGeomScales::Insert (uvar32_64 pos, CGeomScale& scale) {
	if ((pos = NameLookup (scale.m_strName)) == uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CGeomScale*,CGeomScale>* p = new CHash<CGeomScale*,CGeomScale>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

void CDiasAPIGeomScales::Serialize (CArchive& ar) {
	CaImAPICollection<CGeomScale>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			CGeomScale* scale = new CGeomScale ();
			scale->Serialize (ar);
			Add (*scale);
		}
		ar >> m_nActive;
	} else
		ar << m_nActive;
}
