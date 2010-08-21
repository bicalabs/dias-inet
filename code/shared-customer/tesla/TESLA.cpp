// TESLA.cpp : Defines the entry point for the DLL application.
//

#undef _AIM_MAINMODULE

#include "stdafx.h"
#include <aimapi/aImAPI.h>
#include <tesla/TESLA.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE TESLAAPIDLL = { NULL, NULL };

/*extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TESLA.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TESLAAPIDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TESLAAPIDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("AIMAPI.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(TESLAAPIDLL);
	}
	return 1;   // ok
}*/

/////////////////////////////////////////////////////////////////////////////
// CTesla

CTesla::CTesla () { ; };

CTesla::~CTesla () { ; };

/////////////////////////////////////////////////////////////////////////////
// CVariable

CVariable::CVariable () 
    : m_nType (varInt), m_strUnits (""), m_blDynamic (false), m_pTesla (NULL) {
	CaImAPIObject::CaImAPIObject ();
	m_blDynamic = false;
	m_uValue.m_nVal = 0;
	m_uValue.m_rVal = 0;
	m_uValue.m_pcVal = NULL;
}

CVariable::~CVariable () {
	if (m_nType == varString && m_uValue.m_pcVal != NULL)
		delete [] m_uValue.m_pcVal;
	m_uValue.m_pcVal = NULL;
}

CVariable::CVariable (const CVariable &var) {
	CaImAPIObject::CaImAPIObject ((const CaImAPIObject&)var);
	*this = var;
}

CVariable::CVariable (CTesla *pTesla, CString strName, CString strValue, CString strUnits, CString strDescript) {
	m_pTesla = pTesla;
	m_strName = strName;
	m_strDescr = strDescript;
	m_strUnits = strUnits;
	m_blDynamic = false;
	m_nType = varString;
	m_uValue.m_nVal = 0;
	m_uValue.m_rVal = 0;
	m_uValue.m_pcVal = new uchar[strValue.GetLength () + 1];
	if (m_uValue.m_pcVal)
		strcpy ((char*)(m_uValue.m_pcVal), strValue);
}
CVariable::CVariable (CTesla *pTesla, CString strName, uvar32_64 nValue, CString strUnits, CString strDescript) {
	m_pTesla = pTesla;
	m_strName = strName;
	m_strDescr = strDescript;
	m_strUnits = strUnits;
	m_blDynamic = false;
	m_nType = varInt;
	m_uValue.m_rVal = 0;
	m_uValue.m_pcVal = NULL;
	m_uValue.m_nVal = nValue;
}
CVariable::CVariable (CTesla *pTesla, CString strName, rtbyte rValue, CString strUnits, CString strDescript) {
	m_pTesla = pTesla;
	m_strName = strName;
	m_strDescr = strDescript;
	m_strUnits = strUnits;
	m_blDynamic = false;
	m_nType = varReal;
	m_uValue.m_nVal = 0;
	m_uValue.m_pcVal = NULL;
	m_uValue.m_rVal = rValue;
}

CVariable& CVariable::operator= (const CVariable& var) {
	CaImAPIObject::operator = ((const CaImAPIObject&)var);
	m_pTesla = var.m_pTesla;
	m_nType = var.m_nType;
	m_strUnits = var.m_strUnits;
	m_blDynamic = var.m_blDynamic;
	m_strExpr = var.m_strExpr;
	m_uValue.m_nVal = var.m_uValue.m_nVal;
	m_uValue.m_rVal = var.m_uValue.m_rVal;

	m_uValue.m_pcVal = new uchar[strlen ((char*)var.m_uValue.m_pcVal) + 1];
	if (m_uValue.m_pcVal)
		strcpy ((char*)(m_uValue.m_pcVal), (char*)var.m_uValue.m_pcVal);
	return (*this);
}

CVariable& CVariable::operator= (const CString strVal) { SetValue (strVal); return (*this); }
CVariable& CVariable::operator= (const uvar32_64 nVal) { SetValue (nVal); return (*this); }
CVariable& CVariable::operator= (const rtbyte rVal) { SetValue (rVal); return (*this); }

uvar32_64 CVariable::GetPos () {
	m_pTesla->Variables.m_pHashHead = m_pTesla->Variables.m_pHashHead->GetHead ();
	for (CHash<CVariable*,CVariable>* p = m_pTesla->Variables.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (-1);
}
CString CVariable::GetName () {
	return (CaImAPIObject::GetName ());
}
CString CVariable::GetClassName () {
	return (m_strName.Left (m_strName.Find (".")));
}
CString CVariable::GetEndName () {
	return (m_strName.Right (m_strName.GetLength () + m_strName.Find (".")));
}
CString CVariable::GetUnits () { return (m_strUnits); }
CString CVariable::GetExpr () { return (m_strExpr); }
CString CVariable::GetDescr () { return (CaImAPIObject::GetDescr ()); }
void CVariable::GetValue (CString &strVal) { strVal = m_uValue.m_pcVal; }
void CVariable::GetValue (uvar32_64 &nVal) { nVal = m_uValue.m_nVal; }
void CVariable::GetValue (rtbyte &rVal) { rVal = m_uValue.m_rVal; }
VariableType CVariable::GetType () { return (m_nType); }
bool CVariable::IsDynamic () { return (m_blDynamic); }

bool CVariable::SetName (const CString strName) {
	CaImAPIObject::SetName (strName);
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (true);
}
bool CVariable::SetClassName (const CString strName) {
	m_strName = strName + "." + GetEndName (); 
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (true);
}
bool CVariable::SetEndName (const CString strName) {
	m_strName = GetClassName () + "." + strName;
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (true);
}
void CVariable::SetUnits (const CString strUnits) {
	m_strUnits = strUnits;
	// #### TODO: Add right update function call
	// pTesla->Update ()
}
bool CVariable::SetExpr (const CString strExpr) {
	m_strExpr = strExpr;
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (true);
}
void CVariable::SetDescr (const CString strDescr) {
	CaImAPIObject::SetDescr (strDescr);
	// #### TODO: Add right update function call
	// pTesla->Update ()
}
void CVariable::SetValue (const CString strVal) {
	if (m_nType == varString && !m_uValue.m_pcVal)
		delete [] m_uValue.m_pcVal;
	m_nType = varString;
	m_uValue.m_pcVal = new uchar[strVal.GetLength () + 1];
	if (m_uValue.m_pcVal)
		strcpy ((char*)(m_uValue.m_pcVal), strVal);
	// #### TODO: Add right update function call
	// pTesla->Update ()
}
void CVariable::SetValue (const uvar32_64 nVal) {
	if (m_nType == varString && !m_uValue.m_pcVal)
		delete [] m_uValue.m_pcVal;
	m_nType = varInt;
	m_uValue.m_nVal = nVal;
	// #### TODO: Add right update function call
	// pTesla->Update ()
}
void CVariable::SetValue (const rtbyte rVal) {
	if (m_nType == varString && !m_uValue.m_pcVal)
		delete [] m_uValue.m_pcVal;
	m_nType = varReal;
	m_uValue.m_rVal = rVal;
	// #### TODO: Add right update function call
	// pTesla->Update ()
}

bool CVariable::SetPos (uvar32_64 nPos) {
	// #### TODO: Implement
	return (false);
}

/////////////////////////////////////////////////////////////////////////////
// CTeslaAPIVariables

CTeslaAPIVariables::CTeslaAPIVariables () {
	m_pTesla = NULL;
	CaImAPICollectionExt<CVariable>::CaImAPICollectionExt<CVariable> ();
}

CTeslaAPIVariables::CTeslaAPIVariables (const CTeslaAPIVariables& coll) {
	m_pTesla = NULL;
	*this = coll;
}

CTeslaAPIVariables::~CTeslaAPIVariables () {
	CaImAPICollectionExt<CVariable>::~CaImAPICollectionExt<CVariable> ();
}

CTeslaAPIVariables& CTeslaAPIVariables::operator= (const CTeslaAPIVariables& coll) {
	if (!m_pTesla)
		m_pTesla = coll.m_pTesla;
	return ((CTeslaAPIVariables&) (CaImAPICollection<CVariable>::operator= ( (CaImAPICollection<CVariable>&)coll )));
}

CVariable& CTeslaAPIVariables::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CVariable>::operator[] (pos));
}
CVariable& CTeslaAPIVariables::operator[] (CString name) {
	return (CaImAPICollectionExt<CVariable>::operator[] (name));
}

uvar32_64 CTeslaAPIVariables::Add (CVariable& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CVariable>::Add (item);
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (cnt);
}
void CTeslaAPIVariables::Delete (uvar32_64 pos) {
	CaImAPICollectionExt<CVariable>::Delete (pos);
	// #### TODO: Add right update function call
	// pTesla->Update ()
}
uvar32_64 CTeslaAPIVariables::DeleteAll () {
	uvar32_64 cnt = CaImAPICollectionExt<CVariable>::DeleteAll ();
	// #### TODO: Add right update function call
	// pTesla->Update ()
	return (cnt);
}

void CTeslaAPIVariables::Serialize (CArchive& ar) {
	CaImAPICollection<CVariable>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		CVariable *var = new CVariable (m_pTesla, "", "");
		for (n = 0; n < size; ++n) {
			var->Serialize (ar);
			Add (*var);
		}
		delete var;
	}
}

