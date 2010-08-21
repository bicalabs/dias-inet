#include "stdafx.h"
#include "dias.h"
#include "DiasAPIFields.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIFields::CDiasAPIFields () {
	m_pDoc = NULL;
	m_pSrv = NULL;
}

CDiasAPIFields::~CDiasAPIFields () {
}

CDiasAPIFields& CDiasAPIFields::operator= (const CDiasAPIFields& coll) {
	m_pDoc = coll.m_pDoc;
	m_pSrv = coll.m_pSrv;
	return ((CDiasAPIFields&) (CaImAPICollection<CField>::operator= ( (CaImAPICollection<CField>&)coll )));
}

void CDiasAPIFields::Serialize (CArchive& ar) {
	CaImAPICollection<CField>::Serialize (ar);
	uvar32_64 n, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			Add (CField(m_pSrv,"","",""));
			(*this)[n].Serialize (ar);
		}
	}
}
