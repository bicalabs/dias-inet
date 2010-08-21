#include "stdafx.h"
#include "dias.h"
#include "DiasAPIChannels.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIChannels::CDiasAPIChannels () {
	m_rngInt.m_ppHead = &m_pHashHead;
}

CDiasAPIChannels::~CDiasAPIChannels () {
}

CDiasAPIChannels::CDiasAPIChannels (const CDiasAPIChannels& chnl) {
	*this = chnl;
}

CDiasAPIChannels& CDiasAPIChannels::operator= (const CDiasAPIChannels& coll) {
	return ((CDiasAPIChannels&) (CaImAPICollection<CChannel>::operator= ( (CaImAPICollection<CChannel>&)coll )));
}

CDiasAPIChnlRange& CDiasAPIChannels::operator() (aImAPIRngType type) {
	m_rngInt.Reset ();
	CHash<CChannel*, CChannel>* p;
	switch (type) {
		case aimAll:
			for (p = m_pHashHead ; p ; p = p->GetNext ())
				m_rngInt.Include (p);
			break;
		case aimVisible:
			// #### TODO: Place code here
			break;
		default:
			throw (CaImAPIException (0));
	}
	return (m_rngInt);
}

CDiasAPIChnlRange& CDiasAPIChannels::operator() (int no, ...) {
	m_rngInt.Reset ();
	int n = no;
	va_list items;
	va_start (items, no);
	while (n != -1)
		m_rngInt.Include (m_pHashHead->GetAt(n));
	va_end (items);
	return (m_rngInt);
}

CDiasAPIChnlRange& CDiasAPIChannels::operator() (char* name, ...) {
	m_rngInt.Reset ();
	char* n = name;
	va_list items;
	va_start (items, name);
	while (!n)
		m_rngInt.Include (m_pHashHead->GetAt(NameLookup(n)));
	va_end (items);
	return (m_rngInt);
}

void CDiasAPIChannels::Serialize (CArchive& ar) {
	CaImAPICollection<CChannel>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		Add (size);
		for (n = 0; n < size; ++n)
			(*this)[n].Serialize (ar);
	}
}

uvar32_64 CDiasAPIChannels::Add (uvar32_64 cnt) {
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CChannel* pChnl = new CChannel (m_pDoc, "", "");
		cnt = CaImAPICollectionExt<CChannel>::Add (*pChnl);
	}
	return (cnt);
}

uvar32_64 CDiasAPIChannels::Insert (svar32_64 pos, uvar32_64 cnt) {
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CChannel* pChnl = new CChannel (m_pDoc, "", "");
		cnt = CaImAPICollectionExt<CChannel>::Insert (pos, *pChnl);
	}
	return (cnt);
}

uvar32_64 CDiasAPIChannels::Insert (CString pos, uvar32_64 cnt) {
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CChannel* pChnl = new CChannel (m_pDoc, "", "");
		cnt = CaImAPICollectionExt<CChannel>::Insert (pos, *pChnl);
	}
	return (cnt);
}
/*
CDiasAPIChannels CDiasAPIChannels::operator+ (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl;
	chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData + img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator- (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl;
	chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData - img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator* (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl;
	chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData * img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator/ (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl;
	chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData / img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels& CDiasAPIChannels::operator+= (const CDiasAPIChannels& img) { *this = *this + img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator-= (const CDiasAPIChannels& img) { *this = *this - img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator*= (const CDiasAPIChannels& img) { *this = *this * img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator/= (const CDiasAPIChannels& img) { *this = *this / img; return (*this); }

CDiasAPIChannels CDiasAPIChannels::operator+ (const CColor& img) {
	CColor col = img;
	CDiasAPIChannels chnl = *this;
	if (img.m_bChannels != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
//		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData + col[n];
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator- (const CColor& img) {
	CColor col = img;
	CDiasAPIChannels chnl = *this;
	if (img.m_bChannels != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
//		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData - col[n];
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator* (const CColor& img) {
	CColor col = img;
	CDiasAPIChannels chnl = *this;
	if (img.m_bChannels != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
//		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData * col[n];
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator/ (const CColor& img) {
	CColor col = img;
	CDiasAPIChannels chnl = *this;
	if (img.m_bChannels != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
//		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData / col[n];
	return (chnl);
}
CDiasAPIChannels& CDiasAPIChannels::operator+= (const CColor& img) { *this = *this + img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator-= (const CColor& img) { *this = *this - img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator*= (const CColor& img) { *this = *this * img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator/= (const CColor& img) { *this = *this / img; return (*this); }

CDiasAPIChannels CDiasAPIChannels::operator~ () {
	CDiasAPIChannels chnl = *this;
	for (uvar32_64 n = 0, cnt = GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = ~(m_pHashHead->GetAt(n)->m_hData);
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator& (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData & img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator| (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData | img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels CDiasAPIChannels::operator^ (const CDiasAPIChannels& img) {
	CDiasAPIChannels chnl = *this;
	if (img.m_pHashHead->GetCount() != m_pHashHead->GetCount ())
		throw (CaImAPIException (0));
	for (uvar32_64 n = 0, cnt = m_pHashHead->GetCount (); n < cnt; ++n)
		chnl.m_pHashHead->GetAt(n)->m_hData = m_pHashHead->GetAt(n)->m_hData ^ img.m_pHashHead->GetAt(n)->m_hData;
	return (chnl);
}
CDiasAPIChannels& CDiasAPIChannels::operator&= (const CDiasAPIChannels& img) { *this = *this & img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator|= (const CDiasAPIChannels& img) { *this = *this | img; return (*this); }
CDiasAPIChannels& CDiasAPIChannels::operator^= (const CDiasAPIChannels& img) { *this = *this ^ img; return (*this); }

*/