#include "stdafx.h"
#include "Dias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurvey

IMPLEMENT_SERIAL (CSurvey, CObject, 1);

CSurvey::CSurvey (CStorageDoc* pDoc, CString name, CString descr) {
	Fields.m_pDoc = m_pDoc;
	Fields.m_pSrv = this;

	m_pDoc = pDoc;
	m_strName = name;
	m_strDescript = descr;
}

CSurvey::~CSurvey () {
}

CSurvey::CSurvey (const CSurvey& srv) {
	*this = srv;
}

CSurvey& CSurvey::operator= (const CSurvey& srv) {
	m_pDoc = srv.m_pDoc;
	m_strName = srv.m_strName;
	m_strDescript = srv.m_strDescript;
	Fields = srv.Fields;
	Fields.m_pSrv = this;
	return (*this);
}

void CSurvey::Serialize (CArchive& ar) {
	CObject::Serialize (ar);
	if (ar.IsStoring ())
		ar << m_strName << m_strDescript;
	else
		ar >> m_strName >> m_strDescript;
	Fields.m_pSrv = this;
	Fields.Serialize (ar);
}

svar32_64 CSurvey::GetPos () {
	m_pDoc->Surveis.m_pHashHead = m_pDoc->Surveis.m_pHashHead->GetHead ();
	for (CHash<CSurvey*,CSurvey>* p = m_pDoc->Surveis.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (-1);
}

CString CSurvey::GetName () {
	return (m_strName);
}

CString CSurvey::GetDescription () {
	return (m_strDescript);
}

void CSurvey::SetPos (uvar32_64 pos) {
	// #### TODO: Place code here
}

void CSurvey::SetName (CString name) {
	m_strName = name;
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_NAME, GetPos (), GetPos ());
}

void CSurvey::SetDescription (CString descr) {
	m_strDescript = descr;
	m_pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_NAME, GetPos (), GetPos ());
}

void CSurvey::Reset () {
	uvar32_64 n, cnt;
	if ((cnt = Fields.GetCount ()) == 0)
		return;
	for (n = 0; n < cnt; ++n) {
		Fields[n].m_pHead->RemoveTail ();
		Fields[n].m_pHead->Remove ();
		Fields[n].m_pHead = NULL;
	}
}

void CSurvey::Append () {
	uvar32_64 n, cnt;
	uvar32_64* pn;
	rtbyte* pr;
	CString* pstr;
	if ((cnt = Fields.GetCount ()) == 0)
		return;
	CHash<void*,void*>* p;
	for (n = 0; n < cnt; ++n) {
		p = new CHash<void*,void*>;
		Fields[n].m_pHead->GetTail()->InsertAfter (p);
		if (Fields[n].m_nType == fieldInt) {
			pn = new uvar32_64;
			theTesla.Variables[Fields[n].m_strVariable].GetValue (*pn);
			p->m_hData = pn;
		}
		if (Fields[n].m_nType == fieldReal) {
			pr = new rtbyte;
			theTesla.Variables[Fields[n].m_strVariable].GetValue (*pr);
			p->m_hData = pr;
		}
		if (Fields[n].m_nType == fieldString) {
			pstr = new CString;
			theTesla.Variables[Fields[n].m_strVariable].GetValue (*pstr);
			p->m_hData = pstr;
		}
	}
}

void CSurvey::Remove (uvar32_64 pos) {
	uvar32_64 n, cnt;
	if ((cnt = Fields.GetCount ()) == 0)
		return;
	if (pos >= Fields[uvar32_64(0)].m_pHead->GetCount ())
		return;
	for (n = 0; n < cnt; ++n) {
		delete Fields[n].m_pHead->GetAt(pos)->m_hData;
		Fields[n].m_pHead->GetAt(pos)->Remove ();
	}
}

uvar32_64 CSurvey::RecCount () {
	if (Fields.GetCount () == 0)
		return (0);
	return (Fields[uvar32_64(0)].m_pHead->GetCount ());
}
