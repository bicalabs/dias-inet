#include "stdafx.h"
#include "dias.h"
#include "Settings.h"
#include "DiasAPISurveis.h"
#include "StorageFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPISurveis::CDiasAPISurveis () {
	m_rngInt.m_ppHead = &m_pHashHead;
}

CDiasAPISurveis::~CDiasAPISurveis () {
}

CDiasAPISurveis& CDiasAPISurveis::operator= (const CDiasAPISurveis& coll) {
	return ((CDiasAPISurveis&) (CaImAPICollection<CSurvey>::operator= ( (CaImAPICollection<CSurvey>&)coll )));
}

CSurvey& CDiasAPISurveis::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CSurvey>::operator[] (pos));
}

CSurvey& CDiasAPISurveis::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	int sel = m_pDoc->Frames[aimActive].GetActiveSurveyNo ();
	if (sel == -1)	throw (CaImAPIException(0));
	return (*(m_pHashHead->GetAt(sel)->m_hData));
}

CDiasAPISrvRange& CDiasAPISurveis::operator() (aImAPIRngType type) {
	uvar32_64 sel;
	POSITION pos;
	m_rngInt.Reset ();
	if (!m_pHashHead)
		return (m_rngInt);
	CHash<CSurvey*, CSurvey>* p;
	switch (type) {
		case aimAll:
			for (p = m_pHashHead ; !p ; p = p->GetNext ())
				m_rngInt.Include (p);
			break;
		case aimVisible:
			// #### TODO: Place code here
			break;
		case aimActive:
			if ((sel = m_pDoc->Frames[aimActive].GetActiveSurveyNo()) != -1)
				m_rngInt.Include (m_pHashHead->GetAt(sel));
			break;
		case aimSelected:
			pos = m_pDoc->Frames[aimActive].GetFirstSelectedSurveyNo ();
			while (pos)
			   m_rngInt.Include (m_pHashHead->GetAt (m_pDoc->Frames[aimActive].GetNextSelectedSurveyNo (pos)) );
			break;
		default:
			throw (CaImAPIException (0));
	}
	return (m_rngInt);
}

CDiasAPISrvRange& CDiasAPISurveis::operator() (int no, ...) {
	m_rngInt.Reset ();
	int n = no;
	va_list items;
	va_start (items, no);
	while (n != -1)
		m_rngInt.Include (m_pHashHead->GetAt(n));
	va_end (items);
	return (m_rngInt);
}

CDiasAPISrvRange& CDiasAPISurveis::operator() (char* name, ...) {
	m_rngInt.Reset ();
	char* n = name;
	va_list items;
	va_start (items, name);
	while (!n)
		m_rngInt.Include (m_pHashHead->GetAt(NameLookup(n)));
	va_end (items);
	return (m_rngInt);
}

uvar32_64 CDiasAPISurveis::Add (CSurvey& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Add (item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPISurveis::Add (CaImAPIRange<CSurvey>& item) {
	uvar32_64 start = GetCount ();
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Add (item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, start, start + cnt);
	return (cnt);
}
uvar32_64 CDiasAPISurveis::Insert (uvar32_64 pos, CSurvey& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Insert (pos, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPISurveis::Insert (CString name, CSurvey& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Insert (name, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, cnt, cnt);
	return (cnt);
}
uvar32_64 CDiasAPISurveis::Insert (uvar32_64 pos, CaImAPIRange<CSurvey>& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Insert (pos, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, pos, pos + cnt);
	return (cnt);
}
uvar32_64 CDiasAPISurveis::Insert (CString name, CaImAPIRange<CSurvey>& item) {
	uvar32_64 pos = NameLookup (name);
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::Insert (name, item);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, pos, pos + cnt);
	return (cnt);
}
void CDiasAPISurveis::Delete (uvar32_64 pos) {
	CaImAPICollectionExt<CSurvey>::Delete (pos);
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ALL);
}
uvar32_64 CDiasAPISurveis::DeleteAll () {
	uvar32_64 cnt = CaImAPICollectionExt<CSurvey>::DeleteAll ();
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ALL);
	return (cnt);
}

uvar32_64 CDiasAPISurveis::Add (uvar32_64 cnt) {
	uvar32_64 pos;
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CSurvey* pImg = new CSurvey (m_pDoc, theSettings.GetSetting (ID_SURVEY_NAME, CSettings::sqName, UINT(0)), "");
		pos = CaImAPICollectionExt<CSurvey>::Add (*pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, pos - cnt + 1, pos);
	return (cnt);
}

uvar32_64 CDiasAPISurveis::Insert (svar32_64 pos, uvar32_64 cnt) {
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CSurvey* pImg = new CSurvey (m_pDoc, theSettings.GetSetting (ID_SURVEY_NAME, CSettings::sqName, UINT(0)), "");
		CaImAPICollectionExt<CSurvey>::Insert (pos, *pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, pos - cnt + 1, pos);
	return (cnt);
}

uvar32_64 CDiasAPISurveis::Insert (CString pos, uvar32_64 cnt) {
	uvar32_64 pos2;
	for (uvar32_64 n = 0 ; n < cnt ; ++n) {
		CSurvey* pImg = new CSurvey (m_pDoc, theSettings.GetSetting (ID_SURVEY_NAME, CSettings::sqName, UINT(0)), "");
		pos2 = CaImAPICollectionExt<CSurvey>::Insert (pos, *pImg);
	}
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ADDED, pos2 - cnt + 1, pos2);
	return (cnt);
}
