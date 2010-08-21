#include "stdafx.h"
#include "dias.h"
#include "DiasAPISrvRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPISrvRange::CDiasAPISrvRange () {
}

CDiasAPISrvRange::~CDiasAPISrvRange () {                           
}

uvar32_64 CDiasAPISrvRange::Remove () {
	if (!m_pList || !m_pList->m_hData->m_hData->m_pDoc)
		return (0);
	CStorageDoc* pDoc = m_pList->m_hData->m_hData->m_pDoc;
	uvar32_64 cnt = CaImAPIRange<CSurvey>::Remove ();
	pDoc->Update (CUpdateHint::updtList, UPDHL_SURVEIS | UPDHL_ALL);
	return (cnt);
}
