#include "stdafx.h"
#include "dias.h"
#include "Settings.h"
#include "DiasAPIPatches.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIDocs

CDiasAPIPatches::CDiasAPIPatches () {
	m_pImg = NULL;
	m_pDoc = NULL;
	m_rngInt.m_ppHead = &m_pHashHead;
	m_nActive = -1;
}

CDiasAPIPatches::~CDiasAPIPatches () {
}

CDiasAPIPatches& CDiasAPIPatches::operator= (const CDiasAPIPatches& coll) {
	m_pDoc = coll.m_pDoc;
	m_pImg = coll.m_pImg;
	m_nActive = coll.m_nActive;
	return ((CDiasAPIPatches&) (CaImAPICollection<CPatch>::operator= ( (CaImAPICollection<CPatch>&)coll )));
}

CPatch& CDiasAPIPatches::operator[] (uvar32_64 pos) {
	return (CaImAPICollectionExt<CPatch>::operator[] (pos));
}

CPatch& CDiasAPIPatches::operator[] (aImAPIRngType t) {
	CPatch *pPatch = NULL;
	if (t != aimActive)
		throw (CaImAPIException (0));
	if (m_nActive == -1)
		return (*pPatch);
	return (*(m_pHashHead->GetAt(m_nActive)->m_hData));
}

CPatch& CDiasAPIPatches::operator[] (CPoint pnt) throw (CaImAPIException) {
	CPatch *pPatch = NULL;
	uvar32_64 nCoordOffs = pnt.y * m_pDoc->GetDimX () + pnt.x;
	for (CHash<CPatch*,CPatch>* p = m_pHashHead->GetTail (); p; p = p->GetPrev ()) {
		if (nCoordOffs >= p->m_hData->m_pdwContent[0] &&
		    nCoordOffs <= p->m_hData->m_pdwContent[p->m_hData->m_dwCntLen - 1])
			for (uvar32_64 n = 0; n < p->m_hData->m_dwCntLen && nCoordOffs >= p->m_hData->m_pdwContent[n]; ++n)
				if (nCoordOffs == p->m_hData->m_pdwContent[n])
					if (!pPatch || pPatch->GetLayer () <= p->m_hData->GetLayer ())
						pPatch = p->m_hData;
	}
	return (*pPatch);
}

CDiasAPIPtchRange& CDiasAPIPatches::operator() (int no, ...) {
	m_rngInt.Reset ();
	int n = no;
	va_list items;
	va_start (items, no);
	while (n != -1)
		m_rngInt.Include (m_pHashHead->GetAt(n));
	va_end (items);
	return (m_rngInt);
}

CDiasAPIPtchRange& CDiasAPIPatches::operator() (aImAPIRngType type) {
	uvar32_64 sel;
	svar32_64 pos;
	m_rngInt.Reset ();
	if (!m_pHashHead)
		return (m_rngInt);
	CHash<CPatch*, CPatch>* p;
	switch (type) {
		case aimAll:
			for (p = m_pHashHead ; !p ; p = p->GetNext ())
				m_rngInt.Include (p);
			break;
		case aimVisible:
			// #### TODO: Place code here
			break;
		case aimActive:
			if (m_nActive != -1)
				m_rngInt.Include (m_pHashHead->GetAt(m_nActive));
			break;
		case aimSelected:
			// #### TODO: Place code here
			break;
		default:
			throw (CaImAPIException (0));
	}
	return (m_rngInt);
}

CDiasAPIPtchRange& CDiasAPIPatches::operator() (CPoint pnt) {
	m_rngInt.Reset ();
	uvar32_64 nCoordOffs = pnt.y * m_pDoc->GetDimX () + pnt.x;
	for (CHash<CPatch*,CPatch>* p = m_pHashHead->GetTail (); p; p = p->GetPrev ()) {
		if (nCoordOffs >= p->m_hData->m_pdwContent[0] &&
		    nCoordOffs <= p->m_hData->m_pdwContent[p->m_hData->m_dwCntLen - 1])
			for (uvar32_64 n = 0; n < p->m_hData->m_dwCntLen && nCoordOffs >= p->m_hData->m_pdwContent[n]; ++n)
				if (nCoordOffs == p->m_hData->m_pdwContent[n])
					m_rngInt.Include (p);
	}
	return (m_rngInt);
}

void CDiasAPIPatches::Delete (uvar32_64 no) {
	CaImAPICollectionExt<CPatch>::Delete (no);
	if (m_nActive == -1)
		return;
	if (m_nActive >= no)
		m_nActive--;
	if (m_nActive == -1)
		m_nActive = 0;
	if (m_nActive >= GetCount ())
		m_nActive = -1;
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IOBJS);
}

void CDiasAPIPatches::Activate (uvar32_64 no) {
	if (no > m_pHashHead->GetCount ())
		return;
	m_nActive = no;
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IOBJS);
}

void CDiasAPIPatches::Deactivate () {
	if (m_nActive == -1)
		return;
	m_nActive = -1;
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IOBJS);
}

void CDiasAPIPatches::Serialize (CArchive& ar) {
	CaImAPICollection<CPatch>::Serialize (ar);
	uvar32_64 n, pos, size;
	if (ar.IsLoading ()) {
		ar.Read (&size, sizeof (uvar32_64));
		for (n = 0; n < size; ++n) {
			CPatch* p = new CPatch (m_pImg);
			pos = Add (*p);
			(*this)[pos].Serialize (ar);
		}
	}
}

uvar32_64 CDiasAPIPatches::Add (CPatch& item) {
	uvar32_64 cnt = CaImAPICollectionExt<CPatch>::Add (item);
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_ADDPATCH, m_pImg->GetPos (), cnt);
	return (cnt);
}
uvar32_64 CDiasAPIPatches::Add (uvar32_64 nLength, uvar32_64* pnOffsets) {
	CPatch* pPatch = new CPatch (m_pImg, theSettings.GetSetting (ID_PATCH_NAME, CSettings::sqName, UINT(0)), "");
	uvar32_64 pos = CaImAPICollectionExt<CPatch>::Add (*pPatch);
	(*this)[pos].Create (nLength, pnOffsets);
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_ADDPATCH, m_pImg->GetPos (), pos);
	delete pPatch;
	return (pos);
}

uvar32_64 CDiasAPIPatches::Create (CColor& cBackground) {
	return (Create (*m_pImg, cBackground));
}

uvar32_64 CDiasAPIPatches::Create (CPicture& imgSource, CColor& cBackground) throw (CaImAPIException) {
	uvar32_64 nCreated = 0;

	if ((~cBackground) != m_pDoc->GetChannelsCount ())
		throw (CaImAPIException (0));

	uvar32_64 size = m_pDoc->GetDimX () * m_pDoc->GetDimY ();
	uvar32_64 *pdwSizes = (uvar32_64*)aimMemoryCommit (size * sizeof(uvar32_64), "CDiasAPIPatches::Create", "pdwSizes"),
		  **ppdwPatches = (uvar32_64**)aimMemoryCommit (size * sizeof(uvar32_64*), "CDiasAPIPatches::Create", "ppdwPatches");
	CColor	*pcColors = (CColor*)aimMemoryCommit (size * sizeof(CColor), "CDiasAPIPatches::Create", "pcColors");

	uvar32_64 nChnls = imgSource.Channels.GetCount ();
	ubyte	**ppbPixel = (ubyte**)aimMemoryCommit (nChnls * sizeof(ubyte*), "CDiasAPIPatches::Create", "ppbPixel");
	for (int c = 0 ; c < nChnls ; ++c) {
		ppbPixel[c] = (ubyte*)aimMemoryCommit (size, "CDiasAPIPatches::Create", "ppbPixel[c]");
		imgSource.Channels[c].GetBits (ppbPixel[c], size);
	}
	for (uvar32_64 i = 0 , j ; i < size ; ++i) {
		for (c = 0 ; c < nChnls ; ++c)
			if (ppbPixel[c][i] != cBackground[c] && cBackground(c) == true)
				goto listPatches;
		continue;
listPatches:	for (j = 0 ; j < nCreated ; ++j) {
			for (c = 0 ; c < nChnls ; ++c)
				if (ppbPixel[c][i] != pcColors[j][c] && cBackground(c) == true)
					goto nextPatch;
			++pdwSizes[j];
			goto nextPixel;
nextPatch:;
		}
		pcColors[nCreated] = cBackground;
		for (c = 0 ; c < nChnls ; ++c)
			pcColors[nCreated] = ppbPixel[c][i];
		pdwSizes[nCreated++] = 1;
nextPixel:;
	}

	aimMemoryRelease (ppdwPatches,"CDiasAPIPatches::Create", "ppdwPatches");
	ppdwPatches = (uvar32_64**)aimMemoryCommit (nCreated * sizeof(uvar32_64*), "CDiasAPIPatches::Create", "ppdwPatches");
	for (i = 0 ; i < nCreated ; ++i) {
		ppdwPatches[i] = (DWORD*)aimMemoryCommit (pdwSizes[i] * sizeof(DWORD), "CDiasAPIPatches::Create", "ppdwPatches[n]");
		pdwSizes[i] = 0;
	}
	for (i = 0 ; i < size ; ++i) {
		for (c = 0 ; c < nChnls ; ++c)
			if (ppbPixel[c][i] != cBackground[c] && cBackground(c) == true)
				goto listPatches2;
		continue;
listPatches2:	for (j = 0 ; j < nCreated ; ++j) {
			for (c = 0 ; c < nChnls ; ++c)
				if (ppbPixel[c][i] != pcColors[j][c] && cBackground(c) == true)
					goto nextPatch2;
			ppdwPatches[j][pdwSizes[j]++] = i;
			break;
nextPatch2:;	}
	}
	for (c = 0 ; c < nChnls ; ++c)
		aimMemoryRelease (ppbPixel[c], "CDiasAPIPatches::Create", "ppbPixel[c]");
	aimMemoryRelease (ppbPixel,"CDiasAPIPatches::Create", "ppbPixel");

	uvar32_64 pos = 0;
	for (i = 0 ; i < nCreated ; ++i) {
		CPatch* pPatch = new CPatch (&imgSource, theSettings.GetSetting (ID_PATCH_NAME, CSettings::sqName, UINT(0)), "");
		uvar32_64 pos = CaImAPICollectionExt<CPatch>::Add (*pPatch);
		(*this)[pos].Create (pdwSizes[i], ppdwPatches[i]);
		m_pDoc->Update (CUpdateHint::updtImage, UPDHI_ADDPATCH, m_pImg->GetPos (), pos);
	}
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_ADDPATCH, pos - nCreated + 1, pos);

	aimMemoryRelease (ppdwPatches,"CDiasAPIPatches::Create", "ppdwPatches");
	aimMemoryRelease (pdwSizes,"CDiasAPIPatches::Create", "pdwSizes");
	return (nCreated);
}
