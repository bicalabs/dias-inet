#include "stdafx.h"
#include "Dias.h"
#include "Resources.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldInfo

IMPLEMENT_SERIAL (CFieldInfo, CObject, 1);

CFieldInfo::CFieldInfo () {
	;
}
CFieldInfo::CFieldInfo (FieldType type, CString name, CString descr, CString units) :
	m_nType (type), m_strName (name), m_strDescr (descr), m_strUnits (units) {
	;
}

CFieldInfo::CFieldInfo (const CFieldInfo& scale) {
	*this = scale;
}
CFieldInfo::~CFieldInfo () {
	;
}

CFieldInfo& CFieldInfo::operator= (const CFieldInfo& scale) {
	m_nType = scale.m_nType;
	m_strName = scale.m_strName;
	m_strDescr = scale.m_strDescr;
	m_strUnits = scale.m_strUnits;
	return (*this);
}

FieldType CFieldInfo::GetType () {
	return (m_nType);
}
void CFieldInfo::SetType (FieldType type) {
	m_nType = type;
}
CString CFieldInfo::GetName () {
	return (m_strName);
}
void CFieldInfo::SetName (CString name) {
	m_strName = name;
}
CString CFieldInfo::GetDescr () {
	return (m_strDescr);
}
void CFieldInfo::SetDescr (CString descr) {
	m_strDescr = descr;
}
CString CFieldInfo::GetUnits () {
	return (m_strUnits); 
}
void CFieldInfo::SetUnits (CString units) {
	m_strUnits = units;
}

void CFieldInfo::Serialize (CArchive &ar) {
	if (ar.IsStoring ()) {
		ar << (udword)m_nType << m_strName << m_strDescr << m_strUnits;
	} else {
		ar >> (udword&)m_nType >> m_strName >> m_strDescr >> m_strUnits;
	}
}
