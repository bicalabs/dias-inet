#include "stdafx.h"
#include "dias.h"
#include "DiasAPIImgRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IMGRANGE_INIT	if (!m_pList || !m_pList->m_hData->m_hData->m_pDoc) \
				return (0);\
			CStorageDoc* pDoc = m_pList->m_hData->m_hData->m_pDoc;


/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIImgRange::CDiasAPIImgRange () {
}

CDiasAPIImgRange::~CDiasAPIImgRange () {                           
}

uvar32_64 CDiasAPIImgRange::Remove () {
	IMGRANGE_INIT;
	uvar32_64 cnt = CaImAPIRange<CPicture>::Remove ();
	pDoc->Update (CUpdateHint::updtList, UPDHL_IMAGES | UPDHL_ALL);
	pDoc->Update (CUpdateHint::updtImage, UPDHI_IMAGE);
	return (cnt);
}
