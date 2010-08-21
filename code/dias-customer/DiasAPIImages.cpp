#include "stdafx.h"
#include "dias.h"
#include "Settings.h"
#include "DiasAPIImages.h"
#include "StorageFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIImages::CDiasAPIImages () {
	m_rngInt.m_ppHead = &m_pHashHead;
}

CDiasAPIImages::~CDiasAPIImages () {
}

CDiasAPIImages& CDiasAPIImages::operator= (const CDiasAPIImages& coll) {
	return ((CDiasAPIImages&) (CaImAPICollection<CPicture>::operator= ( (CaImAPICollection<CPicture>&)coll )));
}

CPicture& CDiasAPIImages::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CPicture>::operator[] (pos));
}

CPicture& CDiasAPIImages::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	int sel = m_pDoc->Frames[aimActive].GetActiveImageNo ();
	if (sel == -1)	throw (CaImAPIException(0));
	return (*(m_pHashHead->GetAt(sel)->m_hData));
}

CDiasAPIImgRange& CDiasAPIImages::operator() (aImAPIRngType type) {
	uvar32_64 sel;
	svar32_64 pos;
	m_rngInt.Reset ();
	if (!m_pHashHead)
		return (m_rngInt);
	CHash<CPicture*, CPicture>* p;
	switch (type) {
		case aimAll:
			for (p = m_pHashHead ; !p ; p = p->GetNext ())
				m_rngInt.Include (p);
			break;
		case aimVisible:
			// #### TODO: Place code here
			break;
		case aimActive:
			if ((sel = m_pDoc->Frames[aimActive].GetActiveImageNo()) != -1)
				m_rngInt.Include (m_pHashHead->GetAt(sel));
			break;
		case aimSelected:
			for (pos = m_pDoc->Frames[aimActive].GetFirstSelectedImageNo ();
			     pos != -1;
			     m_pDoc->Frames[aimActive].GetNextSelectedImageNo (pos))
			   m_rngInt.Include (m_pHashHead->GetAt (pos));
			break;
		default:
			throw (CaImAPIException (0));
	}
	return (m_rngInt);
}

CDiasAPIImgRange& CDiasAPIImages::operator() (int no, ...) {
	m_rngInt.Reset ();
	int n = no;
	va_list items;
	va_start (items, no);
	while (n != -1)
		m_rngInt.Include (m_pHashHead->GetAt(n));
	va_end (items);
	return (m_rngInt);
}

CDiasAPIImgRange& CDiasAPIImages::operator() (char* name, ...) {
	m_rngInt.Reset ();
	char* n = name;
	va_list items;
	va_start (items, name);
	while (!n)
		m_rngInt.Include (m_pHashHead->GetAt(NameLookup(n)));
	va_end (items);
	return (m_rngInt);
}

void CDiasAPIImages::Serialize (CArchive& ar) {
	CaImAPICollection<CPicture>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		Add (size);
		for (n = 0; n < size; ++n)
			(*this)[n].Serialize (ar);
	}
}

uvar32_64 CDiasAPIImages::Add (CPicture& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Add (item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPIImages::Add (CaImAPIRange<CPicture>& item) {
	uvar32_64 start = GetCount ();
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Add (item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, start, start + cnt);
	return (cnt);
}
uvar32_64 CDiasAPIImages::Insert (uvar32_64 pos, CPicture& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Insert (pos, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPIImages::Insert (CString name, CPicture& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Insert (name, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPIImages::Insert (uvar32_64 pos, CaImAPIRange<CPicture>& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Insert (pos, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos, pos + cnt);
	return (cnt);
}
uvar32_64 CDiasAPIImages::Insert (CString name, CaImAPIRange<CPicture>& item) {
	uvar32_64 pos = NameLookup (name);
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::Insert (name, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos, pos + cnt);
	return (cnt);
}
void CDiasAPIImages::Delete (uvar32_64 pos) {
	CaImAPICollectionExt<CPicture>::Delete (pos);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ALL);
}
uvar32_64 CDiasAPIImages::DeleteAll () {
	uvar32_64 cnt = CaImAPICollectionExt<CPicture>::DeleteAll ();
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ALL);
	return (cnt);
}

uvar32_64 CDiasAPIImages::Add (uvar32_64 cnt) {
	uvar32_64 pos;
	if (cnt == 0) return (0);
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CPicture* pImg = new CPicture (m_pDoc, theSettings.GetSetting (ID_IMAGE_NAME, CSettings::sqName, UINT(0)), "");
		pos = CaImAPICollectionExt<CPicture>::Add (*pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos - cnt + 1, pos);
	return (cnt);
}

uvar32_64 CDiasAPIImages::Insert (svar32_64 pos, uvar32_64 cnt) {
	if (cnt == 0) return (0);
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CPicture* pImg = new CPicture (m_pDoc, theSettings.GetSetting (ID_IMAGE_NAME, CSettings::sqName, UINT(0)), "");
		CaImAPICollectionExt<CPicture>::Insert (pos, *pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos - cnt + 1, pos);
	return (cnt);
}

uvar32_64 CDiasAPIImages::Insert (svar32_64 pos, CString strName) {
	CPicture* pImg = new CPicture (m_pDoc, strName, "");
	CaImAPICollectionExt<CPicture>::Insert (pos, *pImg);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos + 1, pos);
	return (1);
}

uvar32_64 CDiasAPIImages::Insert (CString pos, uvar32_64 cnt) {
	uvar32_64 pos2;
	if (cnt == 0) return (0);
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CPicture* pImg = new CPicture (m_pDoc, theSettings.GetSetting (ID_IMAGE_NAME, CSettings::sqName, UINT(0)), "");
		pos2 = CaImAPICollectionExt<CPicture>::Insert (pos, *pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ADDED, pos2 - cnt + 1, pos2);
	return (cnt);
}
