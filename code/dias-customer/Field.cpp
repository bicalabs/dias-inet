#include "stdafx.h"
#include "Dias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CField

IMPLEMENT_SERIAL (CField, CObject, 1);

CField::CField () {
	m_pSrv = NULL;
	m_pDoc = NULL;
	m_strName = "";
	m_strDescr = "";
	m_strVariable = "";
	m_strUnits = "";
	m_pHead = NULL;
}

CField::CField (const CField& fld) {
	m_pSrv = NULL;
	m_pDoc = NULL;
	m_pHead = NULL;
	*this = fld;
}

CField::~CField () {
	if (!m_pHead)
		return;
	m_pHead->RemoveTail ();
	m_pHead->Remove ();
}

CField& CField::operator= (const CField& fld) {
	if (!m_pDoc)	m_pDoc = fld.m_pDoc;
	if (!m_pSrv)	m_pSrv = fld.m_pSrv;

	m_strName = fld.m_strName;
	m_strDescr = fld.m_strDescr;
	m_strVariable = fld.m_strVariable;
	m_strUnits = fld.m_strUnits;
	m_nType = fld.m_nType;

	if (m_pHead) {
		m_pHead->DeleteTail ();
		m_pHead->Delete ();
	}
	// #### TODO: Implement term by term copy here
	m_pHead = fld.m_pHead->GetReplica ();

	return (*this);
}

CField::CField (CSurvey *pSrv, CString strVar, CString strName, CString strDescr) {
	m_pSrv = pSrv;
	m_pDoc = m_pSrv->m_pDoc;
	m_strName = strName;
	m_strDescr = strDescr;
	m_strVariable = strVar;
	m_nType = (FieldType)(theTesla.Variables[m_strVariable].GetType ());
	m_strUnits = theTesla.Variables[m_strVariable].GetUnits ();

	m_pHead = NULL;
}

void CField::Serialize (CArchive& ar) {
	uvar32_64 n, size;
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescr << m_strVariable << m_strUnits;
		ar << ubyte (m_nType);
		if (m_pHead == NULL || (size = m_pHead->GetCount ()) == 0)
			ar << uvar32_64 (0);

		for (n = 0; n < size; ++n) {
			if (m_nType == fieldInt)
				ar.Write (m_pHead->GetAt (n)->m_hData, sizeof (uvar32_64));
			else if (m_nType == fieldReal)
				ar.Write (m_pHead->GetAt (n)->m_hData, sizeof (rtbyte));
			else if (m_nType == fieldString)
				ar << *((CString*)(m_pHead->GetAt (n)->m_hData));
		}
	} else {
		ar >> m_strName >> m_strDescr >> m_strVariable >> m_strUnits;
		ar >> (ubyte&) (m_nType);
		if (m_pHead) {
			m_pHead->RemoveTail ();
			m_pHead->Remove ();
			m_pHead = NULL;
		}
		ar.Read (&size, sizeof (uvar32_64));
		if (size == 0)	return;

		for (n = 0; n < size; ++n) {
			if (m_nType == fieldInt) {
				m_pHead->GetAt (n)->m_hData = new uvar32_64 (0);
				ar.Read (m_pHead->GetAt (n)->m_hData, sizeof (uvar32_64));
			} else if (m_nType == fieldReal) {
				m_pHead->GetAt (n)->m_hData = new rtbyte (0);
				ar.Read (m_pHead->GetAt (n)->m_hData, sizeof (rtbyte));
			} else if (m_nType == fieldString) {
				m_pHead->GetAt (n)->m_hData = new CString ("");
				ar >> *((CString*)(m_pHead->GetAt (n)->m_hData));
			}
		}
	}
}

svar32_64 CField::GetPos () {
	m_pSrv->Fields.m_pHashHead = m_pSrv->Fields.m_pHashHead->GetHead ();
	for (CHash<CField*,CField>* p = m_pSrv->Fields.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (-1);
}

CString CField::GetName () {
	return (m_strName);
}

CString CField::GetDescr () {
	return (m_strDescr);
}

void CField::SetPos (uvar32_64 pos) {
	// #### TODO: Place code here
}

void CField::SetName (CString name) {
	m_strName = name;
	m_pDoc->Update (CUpdateHint::updtSurvey, UPDHS_FIELDS, GetPos (), GetPos ());
	theApp.Update (UPDHA_FIELD);
}

void CField::SetDescr (CString descr) {
	m_strDescr = descr;
	m_pDoc->Update (CUpdateHint::updtSurvey, UPDHS_FIELDS, GetPos (), GetPos ());
	theApp.Update (UPDHA_FIELD);
}
