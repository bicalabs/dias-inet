#include "stdafx.h"
#include "aImAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CaImAPIObject

IMPLEMENT_SERIAL (CaImAPIObject, CObject, 1)

CaImAPIObject::CaImAPIObject () {
	m_strName = m_strDescr = "";
}

CaImAPIObject::CaImAPIObject (const CaImAPIObject& obj) {
	*this = obj;
}

CaImAPIObject::~CaImAPIObject () {
	;
}

CaImAPIObject& CaImAPIObject::operator= (const CaImAPIObject& obj) {
	m_strName = obj.m_strName;
	m_strDescr = obj.m_strDescr;
	return (*this);
}

CString CaImAPIObject::GetName () {
	return (m_strName);
}
CString CaImAPIObject::GetDescr () {
	return (m_strName);
}

bool CaImAPIObject::SetName (const CString strName) {
	m_strName = strName; 
	return (true);
}
void CaImAPIObject::SetDescr (const CString strDescr) {
	m_strDescr = strDescr; 
}
