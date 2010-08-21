#include "stdafx.h"
#include "dias.h"
#include "MainFrame.h"
#include "DiasAPIMarkStyles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIMarkStyles

CDiasAPIMarkStyles::CDiasAPIMarkStyles () {
}

CDiasAPIMarkStyles::~CDiasAPIMarkStyles () {
}

CDiasAPIMarkStyles& CDiasAPIMarkStyles::operator= (const CDiasAPIMarkStyles& coll) {
	return ((CDiasAPIMarkStyles&) (CaImAPICollection<CMarkStyle>::operator= ( (CaImAPICollection<CMarkStyle>&)coll )));
}

CMarkStyle& CDiasAPIMarkStyles::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CMarkStyle>::operator[](pos));
}

CMarkStyle& CDiasAPIMarkStyles::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	return (CaImAPICollectionExt<CMarkStyle>::operator[](m_nActive));
}

uvar32_64 CDiasAPIMarkStyles::Add (CMarkStyle& scale) {
	int pos;
	if ((pos = NameLookup (scale.m_strName)) != uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CMarkStyle*,CMarkStyle>* p = new CHash<CMarkStyle*,CMarkStyle>(&scale);
	if (!m_pHashHead)
		m_pHashHead = p;
	else
		m_pHashHead->GetTail()->InsertAfter (p);
	return ((uvar32_64)(p->GetPos()));
	
}

uvar32_64 CDiasAPIMarkStyles::Insert (CString name, CMarkStyle& scale) {
	int pos;
	if ((pos = NameLookup (name)) == uvar32_64(-1))
		throw (0);
	if ((pos = NameLookup (scale.m_strName)) == uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CMarkStyle*,CMarkStyle>* p = new CHash<CMarkStyle*,CMarkStyle>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

uvar32_64 CDiasAPIMarkStyles::Insert (uvar32_64 pos, CMarkStyle& scale) {
	if ((pos = NameLookup (scale.m_strName)) == uvar32_64(-1))
		return ((uvar32_64)-1);
	CHash<CMarkStyle*,CMarkStyle>* p = new CHash<CMarkStyle*,CMarkStyle>(&scale);
	m_pHashHead->GetAt(pos)->InsertAfter (p);
	m_pHashHead = m_pHashHead->GetHead ();
	return ((uvar32_64)(p->GetPos ()));
}

uvar32_64 CDiasAPIMarkStyles::Select (CString strTitle) {
	int pos, old;
	if ((pos = NameLookup (strTitle)) == uvar32_64(-1))
		return ((uvar32_64)-1);
	old = m_nActive;
	m_nActive = pos;
	return (old);
}

void CDiasAPIMarkStyles::Serialize (CArchive& ar) {
	CaImAPICollection<CMarkStyle>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			CMarkStyle* scale = new CMarkStyle ();
			scale->Serialize (ar);
			Add (*scale);
		}
		ar >> m_nActive;
	} else
		ar << m_nActive;
}
