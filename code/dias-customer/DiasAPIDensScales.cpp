#include "stdafx.h"
#include "dias.h"
#include "MainFrame.h"
#include "DiasAPIDensScales.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDensScales

CDiasAPIDensScales::CDiasAPIDensScales () {
}

CDiasAPIDensScales::~CDiasAPIDensScales () {
}

CDiasAPIDensScales& CDiasAPIDensScales::operator= (const CDiasAPIDensScales& coll) {
	return ((CDiasAPIDensScales&) (CaImAPICollection<CDensScale>::operator= ( (CaImAPICollection<CDensScale>&)coll )));
}

CDensScale& CDiasAPIDensScales::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CDensScale>::operator[](pos));
}

CDensScale& CDiasAPIDensScales::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	return (CaImAPICollectionExt<CDensScale>::operator[](m_nActive));
}

uvar32_64 CDiasAPIDensScales::Add (CDensScale& scale) {
	int pos;
	if ((pos = NameLookup (scale.m_strName)) != uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CDensScale*,CDensScale>* p = new CHash<CDensScale*,CDensScale>(&scale);
	if (!m_pHashHead)
		m_pHashHead = p;
	else
		m_pHashHead->GetTail()->InsertAfter (p);
	return ((uvar32_64)(p->GetPos()));
	
}

uvar32_64 CDiasAPIDensScales::Insert (CString name, CDensScale& scale) {
	int pos;
	if ((pos = NameLookup (name)) == uvar32_64(-1))
		throw (0);
	if ((pos = NameLookup (scale.m_strName)) != uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CDensScale*,CDensScale>* p = new CHash<CDensScale*,CDensScale>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

uvar32_64 CDiasAPIDensScales::Insert (uvar32_64 pos, CDensScale& scale) {
	if ((pos = NameLookup (scale.m_strName)) != uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CDensScale*,CDensScale>* p = new CHash<CDensScale*,CDensScale>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

void CDiasAPIDensScales::Serialize (CArchive& ar) {
	CaImAPICollection<CDensScale>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			CDensScale* scale = new CDensScale;
			scale->Serialize (ar);
			Add (*scale);
		}
		ar >> m_nActive;
	} else
		ar << m_nActive;
}
