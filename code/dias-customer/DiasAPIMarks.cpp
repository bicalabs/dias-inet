#include "stdafx.h"
#include "dias.h"
#include "DiasAPIMarks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIMarks::CDiasAPIMarks () {
	m_pDoc = NULL;
	m_pImg = NULL;
	m_rngInt.m_ppHead = &m_pHashHead;
}

CDiasAPIMarks::~CDiasAPIMarks () {
}

CDiasAPIMarks& CDiasAPIMarks::operator= (const CDiasAPIMarks& coll) {
	m_pDoc = coll.m_pDoc;
	m_pImg = coll.m_pImg;
	return ((CDiasAPIMarks&) (CaImAPICollection<CMark>::operator= ( (CaImAPICollection<CMark>&)coll )));
}

CMark& CDiasAPIMarks::operator[] (uvar32_64 pos) {
	if (!m_pHashHead)
		throw (CaImAPIException(0));
	return (*(m_pHashHead->GetAt(pos)->m_hData));
};

CMark& CDiasAPIMarks::operator[] (aImAPIRngType t) {
	if (t != aimActive)
		throw (CaImAPIException (0));
	if (m_nActive == -1)	throw (CaImAPIException(0));
	return (*(m_pHashHead->GetAt(m_nActive)->m_hData));
}

CMark& CDiasAPIMarks::operator[] (CPoint pnt) throw (CaImAPIException) {
	CMark *pMark = NULL;
	CRect rect (-3, -3, 3, 3), rect2 = rect;
	for (CHash<CMark*,CMark>* p = m_pHashHead; p; p = p->GetNext (), rect2 = rect) {
		rect2.OffsetRect (p->m_hData->m_iPosX - 2, p->m_hData->m_iPosY - 2);
		if (rect2.PtInRect (pnt))
			return (*(p->m_hData));
	}
	return (*pMark);
}

CDiasAPIMrkRange& CDiasAPIMarks::operator() (aImAPIRngType type) {
	m_rngInt.Reset ();
	if (!m_pHashHead)
		return (m_rngInt);
	CHash<CMark*, CMark>* p;
	switch (type) {
		case aimAll:
			for (p = m_pHashHead ; !p ; p = p->GetNext ())
				m_rngInt.Include (p);
			break;
		case aimVisible:
			// #### TODO: Place code here
			break;
		case aimActive:
			// #### TODO: Check code here
			for (p = m_pHashHead ; p ; p = p->GetNext ())
				if (p->m_hData->m_blSelected)
					m_rngInt.Include (p);
			break;
		case aimSelected:
			for (p = m_pHashHead ; p ; p = p->GetNext ())
				if (p->m_hData->m_blSelected)
					m_rngInt.Include (p);
			break;
		default:
			throw (CaImAPIException (0));
	}
	return (m_rngInt);
}

CDiasAPIMrkRange& CDiasAPIMarks::operator() (int no, ...) {
	m_rngInt.Reset ();
	int n = no;
	va_list items;
	va_start (items, no);
	while (n != -1)
		m_rngInt.Include (m_pHashHead->GetAt(n));
	va_end (items);
	return (m_rngInt);
}

void CDiasAPIMarks::Serialize (CArchive& ar) {
	CaImAPICollection<CMark>::Serialize (ar);
	uvar32_64 n, pos, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			CMark* p = new CMark ();
			p->m_pImg = m_pImg;
			pos = CaImAPICollectionExt<CMark>::Add (*p);
			(*this)[pos].Serialize (ar);
		}
	}
}

uvar32_64 CDiasAPIMarks::Add (CPoint& pt) {
	CMark *pMrk = new CMark (m_pImg, pt, theApp.MarkStyles[aimActive].GetStyle ());
	uvar32_64 pos = CaImAPICollectionExt<CMark>::Add (*pMrk);
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_ADDMARKER, m_pImg->GetPos (), pos);
	return (pos);
}
