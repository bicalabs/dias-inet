#include "stdafx.h"
#include "dias.h"
#include "DiasAPIChnlRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIChnlRange::CDiasAPIChnlRange () {
}

CDiasAPIChnlRange::~CDiasAPIChnlRange () {                           
}

void CDiasAPIChnlRange::GetBits (ubyte* pbBits, uvar32_64 len, bool transform) {
	uvar32_64 n, cnt = GetCount ();
	for (n = 0 ; n < cnt; ++n)
		m_pList->GetAt(n)->m_hData->m_hData->GetBits (pbBits + n, len / cnt, cnt, transform);
}

void CDiasAPIChnlRange::SetBits (ubyte* pbBits, uvar32_64 len) {
	uvar32_64 n, cnt = GetCount ();
	for (n = 0 ; n < cnt; ++n)
		m_pList->GetAt(n)->m_hData->m_hData->SetBits (pbBits + n, len / cnt, cnt);
}
