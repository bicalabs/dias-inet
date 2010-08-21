#include "stdafx.h"
#include "Dias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define avg(x,y)	(((x)+(y))/2)
#define	gvg(x,y)	(pow(double(x)*(y),0.5))
#define	hrg(x,y)	(2./(1./(x) + 1./(y)))

int __cdecl sortfn_rtbyte (const void *b1, const void *b2) {
	if (*((rtbyte*)b1) == *((rtbyte*)b2))
		return 0;
	if (*((rtbyte*)b1) < *((rtbyte*)b2))
		return -1;
	if (*((rtbyte*)b1) > *((rtbyte*)b2))
		return +1;
	return 0;
}
int __cdecl sortfn_ubyte (const void *b1, const void *b2) {
	if (*((ubyte*)b1) == *((ubyte*)b2))
		return 0;
	if (*((ubyte*)b1) < *((ubyte*)b2))
		return -1;
	if (*((ubyte*)b1) > *((ubyte*)b2))
		return +1;
	return 0;
}
int __cdecl sortfn_uword (const void *b1, const void *b2) {
	if (*((uword*)b1) == *((uword*)b2))
		return 0;
	if (*((uword*)b1) < *((uword*)b2))
		return -1;
	if (*((uword*)b1) > *((uword*)b2))
		return +1;
	return 0;
}
int __cdecl sortfn_udword (const void *b1, const void *b2) {
	if (*((udword*)b1) == *((udword*)b2))
		return 0;
	if (*((udword*)b1) < *((udword*)b2))
		return -1;
	if (*((udword*)b1) > *((udword*)b2))
		return +1;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CChannel

IMPLEMENT_SERIAL (CChannel, CObject, 1);

CChannel::CChannel () : m_strName (""), m_strDescript ("") {
	m_pDoc = NULL;
	m_pbPixels = NULL;

	theApp.ObjectsAdd (OBJT_CHANNEL, this);
}

CChannel::CChannel (CStorageDoc* pDoc, CString name, CString descr) {
	m_pDoc = pDoc;
	m_strName = name;
	m_strDescript = descr;
	m_pnColorModel = &(pDoc->m_nColorModel);
	m_pnBPP = &(pDoc->m_nBPP);
	m_pnDimX = &(pDoc->m_nDimX);
	m_pnDimY = &(pDoc->m_nDimY);

	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	m_pbPixels = (BYTE *)aimMemoryCommit (len, "CChannel::Create", "m_pbPixels");
	m_pb$Pixels = (BYTE *)aimMemoryCommit (len, "CChannel::Create", "m_pb$Pixels");
	if (!m_pbPixels || !m_pb$Pixels)	return;
	memset (m_pbPixels, 0, len);
	memset (m_pb$Pixels, 0, len);

	theApp.ObjectsAdd (OBJT_CHANNEL, this);
}

CChannel::~CChannel () {
	theApp.ObjectsRemove (OBJT_CHANNEL, this);

	m_pDoc = NULL;
	if (m_pbPixels == NULL || m_pb$Pixels == NULL)	return;
	aimMemoryRelease (m_pbPixels, "CChannel::~CChannel", "m_pbPixels");
	aimMemoryRelease (m_pb$Pixels, "CChannel::~CChannel", "m_pb$Pixels");
}

CChannel::CChannel (const CChannel& chnl) {
	m_pDoc = NULL;
	theApp.ObjectsAdd (OBJT_CHANNEL, this);
	*this = chnl;
}

CChannel& CChannel::operator= (const CChannel& chnl) {
	theApp.ObjectsRemove (OBJT_CHANNEL, this);
	if (m_pbPixels == NULL || m_pb$Pixels == NULL) {
		aimMemoryRelease (m_pbPixels, "CChannel::~CChannel", "m_pbPixels");
		aimMemoryRelease (m_pb$Pixels, "CChannel::~CChannel", "m_pb$Pixels");
	}

	if (!m_pDoc)	m_pDoc = chnl.m_pDoc;
	m_strName = chnl.m_strName;
	m_strDescript = chnl.m_strDescript;
	m_pnColorModel = &(m_pDoc->m_nColorModel);
	m_pnBPP = &(m_pDoc->m_nBPP);
	m_pnDimX = &(m_pDoc->m_nDimX);
	m_pnDimY = &(m_pDoc->m_nDimY);

	if (!m_pbPixels)
		aimMemoryRelease (m_pbPixels, "CChannel::~CChannel", "m_pbPixels");
	if (!m_pb$Pixels)
		aimMemoryRelease (m_pb$Pixels, "CChannel::~CChannel", "m_pb$Pixels");
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	m_pbPixels = (BYTE *)aimMemoryCommit (len, "CChannel::Create", "m_pbPixels");
	m_pb$Pixels = (BYTE *)aimMemoryCommit (len, "CChannel::Create", "m_pb$Pixels");
	if (!m_pbPixels || !m_pb$Pixels)	throw (CaImAPIException(0));
	memcpy (m_pbPixels, chnl.m_pbPixels, len);
	memcpy (m_pb$Pixels, chnl.m_pb$Pixels, len);

	theApp.ObjectsAdd (OBJT_CHANNEL, this);
	return (*this);
}

void CChannel::Serialize (CArchive& ar) {
	CObject::Serialize (ar);
	ubyte *pbData;
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY, ziplen;
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescript;
		ar << (udword)1;	// Zip compression
		pbData = aimCompressZip (m_pbPixels, len, ziplen);
		ar << ziplen;
		ar.Write (pbData, ziplen);
		aimMemoryRelease (pbData, "CChannel::Serialize", "pbData");
	} else {
		udword ct;
		ar >> m_strName >> m_strDescript;
		ar >> ct;
		if (ct == 0) {		// No compression
			ar.Read (m_pbPixels, len);
			theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
		} else if (ct == 1) {	// Zip compression
			if (m_pbPixels)
				aimMemoryRelease (m_pbPixels, "CChannel::Serialize", "m_pbPixels");
			ar >> ziplen;
			pbData = (ubyte*)aimMemoryCommit (ziplen, "CChannel::Serialize", "pbData");
			ar.Read (pbData, ziplen);
			m_pbPixels = aimDecompressZip (pbData, ziplen, len);
			aimMemoryRelease (pbData, "CChannel::Serialize", "pbData");
			theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
		}
	}
}

CString CChannel::GetName () {
	return (m_strName);
}

CString CChannel::GetDescription () {
	return (m_strDescript);
}

void CChannel::SetName (CString name) {
	m_strName = name;
}

void CChannel::SetDescription (CString descr) {
	m_strDescript = descr;
}

void CChannel::GetBits (ubyte* pbBits, uvar32_64 buflen, ubyte warp, bool transform) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	if (len * warp > buflen)	throw (CaImAPIException (0));
	if (transform && m_pb$Pixels[0] != (ubyte)theApp.DensScales[aimActive].Transform (m_pbPixels[0]))
		theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
	for (svar32_64 n = 0 ; n < len ; n++, pbBits += warp)
		*pbBits = transform ? m_pb$Pixels[n] : m_pbPixels[n];
}

void CChannel::SetBits (ubyte* pbBits, uvar32_64 buflen, ubyte warp) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	if (len * warp > buflen)	throw (CaImAPIException (0));
	for (svar32_64 n = 0 ; n < len ; n++, pbBits += warp)
		m_pbPixels[n] = *pbBits;
	theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
}

udword CChannel::GetPixel (uvar16_32 nX, uvar16_32 nY, bool transform) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	uvar32_64 nOffset = nX * nY * bpp;
	if (transform && m_pb$Pixels[0] != (ubyte)theApp.DensScales[aimActive].Transform (m_pbPixels[0]))
		theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
	if (nOffset >= len)		throw (CaImAPIException (0));
	if (bpp == 1)	return (transform ? m_pb$Pixels[nOffset] : m_pbPixels[nOffset]);
	if (bpp == 2)	return (transform ? ((uword*)m_pb$Pixels)[nOffset]: ((uword*)m_pbPixels)[nOffset]);
	if (bpp == 3)	return (transform ? ((udword*)m_pb$Pixels)[nOffset]: ((udword*)m_pbPixels)[nOffset]);
	throw (CaImAPIException (0));
	return (0);
}

udword CChannel::GetPixel (uvar32_64 nOffset, bool transform) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	nOffset *= bpp;
	if (transform && m_pb$Pixels[0] != (ubyte)theApp.DensScales[aimActive].Transform (m_pbPixels[0]))
		theApp.DensScales[aimActive].Transform (m_pb$Pixels, m_pbPixels, len, *m_pnBPP);
	if (nOffset >= len)		throw (CaImAPIException (0));
	if (bpp == 1)	return (transform ? m_pb$Pixels[nOffset] : m_pbPixels[nOffset]);
	if (bpp == 2)	return (transform ? ((uword*)m_pb$Pixels)[nOffset]: ((uword*)m_pbPixels)[nOffset]);
	if (bpp == 3)	return (transform ? ((udword*)m_pb$Pixels)[nOffset]: ((udword*)m_pbPixels)[nOffset]);
	throw (CaImAPIException (0));
	return (0);
}

void CChannel::SetPixel (uvar16_32 nX, uvar16_32 nY, udword nValue) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	uvar32_64 nOffset = nX * nY * bpp;
	if (nOffset >= len)		throw (CaImAPIException (0));
	if (bpp == 1) {
		m_pbPixels[nOffset] = nValue;
		m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	if (bpp == 2) {
		((uword*)m_pbPixels)[nOffset] = nValue;
		((uword*)m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	if (bpp == 3) {
		((udword*)m_pbPixels)[nOffset] = nValue;
		((udword*)m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IMAGE);
}

void CChannel::SetPixel (uvar32_64 nOffset, udword nValue) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	nOffset *= bpp;
	if (nOffset >= len)		throw (CaImAPIException (0));
	if (bpp == 1) {
		m_pbPixels[nOffset] = nValue;
		m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	if (bpp == 2) {
		((uword*)m_pbPixels)[nOffset] = nValue;
		((uword*)m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	if (bpp == 3) {
		((udword*)m_pbPixels)[nOffset] = nValue;
		((udword*)m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (nValue);
	}
	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IMAGE);
}

void CChannel::CopyData (CChannel& chnl) {
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 len = bpp * *m_pnDimX * *m_pnDimY;
	memcpy (m_pbPixels, chnl.m_pbPixels, len);
	memcpy (m_pb$Pixels, chnl.m_pb$Pixels, len);
}

void CChannel::Statistics (CStatistics& stat) {
	uvar32_64 n, len = *m_pnDimX * *m_pnDimY;

	stat.Max = 0x00;
	stat.Min = 0xFF;
	for (n = 0; n < 0x100; ++n)
		stat.Freq[n] = 0;
	for (n = 0; n < len; ++n) {
		stat.Freq[m_pbPixels[n]] += 1. / len;
		stat.Max = max (stat.Max, m_pbPixels[n]);
		stat.Min = min (stat.Min, m_pbPixels[n]);
	}
	stat.Moda = stat.Freq[0];
	for (n = 1; n < 0x100; ++n)
		if (stat.Freq[n] > stat.Freq[udword(stat.Moda)])
			stat.Moda = n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arithmetic operations

#define	PROC_INTRO	\
	uvar32_64 d;\
	CChannel *result = m_pDoc->m_pCnlTmp;\
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);\
	uvar32_64 size = *m_pnDimX * *m_pnDimY;\
	uvar32_64 len = bpp * size;
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = m_pbPixels[nOffset] x chnl.m_pbPixels[nOffset];\
			result->m_pbPixels[nOffset] = (nType == 0 && d >= 0xFF) ? 0xFF : d;\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (result->m_pbPixels[nOffset]);\
		} else if (bpp == 2) {\
			d = ((uword*)m_pbPixels)[nOffset] x ((uword*)chnl.m_pbPixels)[nOffset];\
			((uword*)result->m_pbPixels)[nOffset] = (nType == 0 && d >= 0xFFFF) ? 0xFFFF : d;\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			((udword*)result->m_pbPixels)[nOffset] = ((udword*)m_pbPixels)[nOffset] x ((udword*)chnl.m_pbPixels)[nOffset];\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::Add (const CChannel& chnl, ubyte nType) { PROC_MACRO (+); }
CChannel& CChannel::Sub (const CChannel& chnl, ubyte nType) { PROC_MACRO (-); }
CChannel& CChannel::Mul (const CChannel& chnl, ubyte nType, uvar32_64 nCoef) { PROC_MACRO (/ double(nCoef) *); }
CChannel& CChannel::Div (const CChannel& chnl, ubyte nType, uvar32_64 nCoef) { PROC_MACRO (* nCoef /); }
CChannel& CChannel::Mod (const CChannel& chnl, ubyte nType) { PROC_MACRO (%); }

#undef	PROC_MACRO
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = result->m_pbPixels[nOffset] = m_pbPixels[nOffset] x chnl.m_pbPixels[nOffset];\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 2) {\
			d = ((uword*)result->m_pbPixels)[nOffset] = ((uword*)m_pbPixels)[nOffset] x ((uword*)chnl.m_pbPixels)[nOffset];\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			d = ((udword*)result->m_pbPixels)[nOffset] = ((udword*)m_pbPixels)[nOffset] x ((uword*)chnl.m_pbPixels)[nOffset];\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::And (const CChannel& chnl) { PROC_MACRO(&); }
CChannel& CChannel::Or  (const CChannel& chnl) { PROC_MACRO(|); }
CChannel& CChannel::Xor (const CChannel& chnl) { PROC_MACRO(^); }

#undef	PROC_MACRO
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = result->m_pbPixels[nOffset] = x (m_pbPixels[nOffset], chnl.m_pbPixels[nOffset]);\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 2) {\
			d = ((uword*)result->m_pbPixels)[nOffset] = x (((uword*)m_pbPixels)[nOffset], ((uword*)chnl.m_pbPixels)[nOffset]);\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			d = ((udword*)result->m_pbPixels)[nOffset] = x (((udword*)m_pbPixels)[nOffset], ((uword*)chnl.m_pbPixels)[nOffset]);\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::Min (const CChannel& chnl) { PROC_MACRO(min); }
CChannel& CChannel::Max (const CChannel& chnl) { PROC_MACRO(max); }
CChannel& CChannel::Avg (const CChannel& chnl) { PROC_MACRO(avg); }
CChannel& CChannel::Gvg (const CChannel& chnl) { PROC_MACRO(gvg); }
CChannel& CChannel::Hvg (const CChannel& chnl) { PROC_MACRO(hrg); }

#undef	PROC_MACRO
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = m_pbPixels[nOffset] x val;\
			result->m_pbPixels[nOffset] = (nType == 0 && d >= 0xFF) ? 0xFF : d;\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 2) {\
			d = ((uword*)m_pbPixels)[nOffset] x val;\
			((uword*)result->m_pbPixels)[nOffset] = (nType == 0 && d >= 0xFFFF) ? 0xFFFF : d;\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			((udword*)result->m_pbPixels)[nOffset] = ((udword*)m_pbPixels)[nOffset] x val;\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::Add (const uvar32_64& val, ubyte nType) { PROC_MACRO (+); }
CChannel& CChannel::Sub (const uvar32_64& val, ubyte nType) { PROC_MACRO (-); }
CChannel& CChannel::Mul (const uvar32_64& val, ubyte nType, uvar32_64 nCoef) { PROC_MACRO (/ double(nCoef) *); }
CChannel& CChannel::Div (const uvar32_64& val, ubyte nType, uvar32_64 nCoef) { PROC_MACRO (* nCoef /); }
CChannel& CChannel::Mod (const uvar32_64& val, ubyte nType) { PROC_MACRO (%); }

CChannel& CChannel::Not () {
	PROC_INTRO
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {
		if (bpp == 1) {
			d = result->m_pbPixels[nOffset] = !m_pbPixels[nOffset];
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);
		} else if (bpp == 2) {
			d = ((uword*)result->m_pbPixels)[nOffset] = !((uword*)m_pbPixels)[nOffset];
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);
		} else if (bpp == 3) {
			d = ((udword*)result->m_pbPixels)[nOffset] = !((udword*)m_pbPixels)[nOffset];
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);
		}
	}
	CopyData (*result);\
	return (*this);
}

#undef	PROC_MACRO
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = result->m_pbPixels[nOffset] = m_pbPixels[nOffset] x val;\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 2) {\
			d = ((uword*)result->m_pbPixels)[nOffset] = ((uword*)m_pbPixels)[nOffset] x val;\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			d = ((udword*)result->m_pbPixels)[nOffset] = ((udword*)m_pbPixels)[nOffset] x val;\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::And (const uvar32_64& val) { PROC_MACRO (&); }
CChannel& CChannel::Or  (const uvar32_64& val) { PROC_MACRO (|); }
CChannel& CChannel::Xor (const uvar32_64& val) { PROC_MACRO (^); }

#undef	PROC_MACRO
#define	PROC_MACRO(x)	\
	PROC_INTRO\
	for (uvar32_64 nOffset = 0; nOffset < size; ++nOffset) {\
		if (bpp == 1) {\
			d = result->m_pbPixels[nOffset] = x (m_pbPixels[nOffset], val);\
			result->m_pb$Pixels[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 2) {\
			d = ((uword*)result->m_pbPixels)[nOffset] = x (((uword*)m_pbPixels)[nOffset], val);\
			((uword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		} else if (bpp == 3) {\
			d = ((udword*)result->m_pbPixels)[nOffset] = x (((udword*)m_pbPixels)[nOffset], val);\
			((udword*)result->m_pb$Pixels)[nOffset] = theApp.DensScales[aimActive].Transform (d);\
		}\
	}\
	CopyData (*result);\
	return (*this);
CChannel& CChannel::Min (const uvar32_64& val) { PROC_MACRO (min); }
CChannel& CChannel::Max (const uvar32_64& val) { PROC_MACRO (max); }
CChannel& CChannel::Avg (const uvar32_64& val) { PROC_MACRO (avg); }
CChannel& CChannel::Gvg (const uvar32_64& val) { PROC_MACRO (gvg); }
CChannel& CChannel::Hvg (const uvar32_64& val) { PROC_MACRO (hrg); }

#undef	PROC_MACRO
#undef	PROC_INTRO

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Statistic operations

#define PROC_MACRO(x)	\
			for (nOffset = 0; nOffset < size; ++nOffset) {\
				mn = min (mn, ((x*)(m_pbPixels))[nOffset]);\
				mx = max (mx, ((x*)(m_pbPixels))[nOffset]);\
			}\
			for (nOffset = 0; nOffset < size; ++nOffset) {\
				((x*)(result->m_pbPixels))[nOffset] = (((x*)(m_pbPixels))[nOffset] - mn) * (uvar32_64)(x(-1)) / mx;\
				((x*)(result->m_pb$Pixels))[nOffset] = theApp.DensScales[aimActive].Transform (((x*)(result->m_pbPixels))[nOffset]);\
			}
CChannel& CChannel::Normalize () {
	uvar32_64 mn = uvar32_64 (-1), mx = 0, nOffset;
	CChannel *result = m_pDoc->m_pCnlTmp;
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 size = *m_pnDimX * *m_pnDimY;
	switch (bpp) {
		case 1:	PROC_MACRO(ubyte);
			break;
		case 2:	PROC_MACRO(uword);
			break;
		case 3:	PROC_MACRO(udword);
			break;
	}
	CopyData (*result);
	return (*this);
}
#undef	PROC_MACRO

#define PROC_MACRO(x)	\
	for (nOffset = 0 ; nOffset < size; ++nOffset) {\
		if (((x*)(m_pbPixels))[nOffset] >= nThreshold) {\
			((x*)(result->m_pbPixels))[nOffset] = (x)(nHighValue);\
			((x*)(result->m_pb$Pixels))[nOffset] = theApp.DensScales[aimActive].Transform (((x*)(result->m_pbPixels))[nOffset]);\
		} else {\
			((x*)(result->m_pbPixels))[nOffset] = (x)(nLowValue);\
			((x*)(result->m_pb$Pixels))[nOffset] = theApp.DensScales[aimActive].Transform (((x*)(result->m_pbPixels))[nOffset]);\
		}\
	}
CChannel& CChannel::Segment (uvar32_64 nThreshold, uvar32_64 nLowValue, uvar32_64 nHighValue) {
	CChannel *result = m_pDoc->m_pCnlTmp;
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 size = *m_pnDimX * *m_pnDimY, nOffset;
	switch (bpp) {
		case 1:	PROC_MACRO(ubyte);
			break;
		case 2:	PROC_MACRO(uword);
			break;
		case 3:	PROC_MACRO(udword);
			break;
	}
	CopyData (*result);
	return (*this);
}
#undef	PROC_MACRO

#define	PIXEL(t,p,ox,oy)	((t*)((p)->m_pbPixels))[(x + (ox)) + *m_pnDimX*(y + (oy))]
#define	PIXEL_IND(t,p,ox,oy)	((svar32_64(x)+(ox)<0 || svar32_64(y)+(oy)<0 || x+(ox)>=*m_pnDimX || y+(oy)>=*m_pnDimY)?(0):(PIXEL(t,p,ox,oy)))
#define ERODE_PIXEL(t,fx,fy)	if (PIXEL_IND (t, in2, fx, fy) != nSegment) {\
					PIXEL (t, in1, 0, 0) = PIXEL_IND (t, in2, fx, fy);\
					((t*)(in1->m_pb$Pixels))[x + *m_pnDimX * y] = theApp.DensScales[aimActive].Transform (PIXEL (t, in1, 0, 0));\
				}
#define	PROC_MACRO(t)		\
	for (c = 0; c < nRepeat; ++c) {\
		in2 = in1;\
		for (y = 1; y < *m_pnDimY - 1; ++y)\
			for (x = 1; x < *m_pnDimX - 1; ++x) {\
				if (PIXEL(t, in2, 0, 0) != nSegment)\
					continue;\
				switch (nType) {\
					case segHorzLine:\
							ERODE_PIXEL ( t, -1,  0 )\
						else	ERODE_PIXEL ( t, +1,  0 )\
						break;\
					case segVertLine:\
							ERODE_PIXEL ( t,  0, -1 )\
						else	ERODE_PIXEL ( t,  0, +1 )\
						break;\
					case segDiag45:\
							ERODE_PIXEL ( t, +1, -1 )\
						else	ERODE_PIXEL ( t, -1, +1 )\
						break;\
					case segDiag135:\
							ERODE_PIXEL ( t, -1, -1 )\
						else	ERODE_PIXEL ( t, +1, +1 )\
						break;\
					case segCross:\
							ERODE_PIXEL ( t,  0, -1 )\
						else	ERODE_PIXEL ( t,  0, +1 )\
						else	ERODE_PIXEL ( t, -1,  0 )\
						else	ERODE_PIXEL ( t, +1,  0 )\
						break;\
					case segSquare:\
							ERODE_PIXEL ( t, -1, -1 )\
						else	ERODE_PIXEL ( t, -1, +1 )\
						else	ERODE_PIXEL ( t,  0, -1 )\
						else	ERODE_PIXEL ( t, +1, -1 )\
						else	ERODE_PIXEL ( t, -1,  0 )\
						else	ERODE_PIXEL ( t, +1,  0 )\
						else	ERODE_PIXEL ( t,  0, +1 )\
						else	ERODE_PIXEL ( t, +1, +1 )\
						break;\
					case segOctagon:\
							ERODE_PIXEL ( t, -2, -1 )\
						else	ERODE_PIXEL ( t, -2, +1 )\
\
						else	ERODE_PIXEL ( t, -1, -2 )\
						else	ERODE_PIXEL ( t,  0, -2 )\
						else	ERODE_PIXEL ( t, +1, -2 )\
\
						else	ERODE_PIXEL ( t, -1, -1 )\
						else	ERODE_PIXEL ( t,  0, -1 )\
						else	ERODE_PIXEL ( t, +1, -1 )\
						else	ERODE_PIXEL ( t, +2, -1 )\
\
						else	ERODE_PIXEL ( t, -2,  0 )\
						else	ERODE_PIXEL ( t, -1,  0 )\
						else	ERODE_PIXEL ( t, +1,  0 )\
						else	ERODE_PIXEL ( t, +2,  0 )\
\
						else	ERODE_PIXEL ( t, -1, +1 )\
						else	ERODE_PIXEL ( t,  0, +1 )\
						else	ERODE_PIXEL ( t, +1, +1 )\
						else	ERODE_PIXEL ( t, +2, +1 )\
\
						else	ERODE_PIXEL ( t, -1, +2 )\
						else	ERODE_PIXEL ( t,  0, +2 )\
						else	ERODE_PIXEL ( t, +1, +2 )\
						break;\
				}\
			}\
	}
CChannel& CChannel::ErodeSegs (uvar32_64 nSegment, Parameter nType, uint nRepeat) {
	uint c;
	uvar32_64 x, y;
	CChannel *in1 = new CChannel (*this),
		 *in2 = new CChannel (this->m_pDoc, m_strName, "");
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);

	switch (bpp) {
		case 1:	PROC_MACRO(ubyte);
			break;
		case 2:	PROC_MACRO(uword);
			break;
		case 3:	PROC_MACRO(udword);
			break;
	}
	*this = *in1;
	delete in1;
	return (*this);
}
#undef	PIXEL
#undef	PIXEL_X
#undef	PIXEL_IND
#undef	ERODE_PIXEL
#undef	PROC_MACRO

#define	PIXEL(t,ox,oy)	((t*)(m_pbPixels))[x+(ox)+(y+(oy))*(*m_pnDimX)]
#define PROC_MACRO(t)	\
	for (y = nY; y < *m_pnDimY - nY; ++y)\
		for (x = nX; x < *m_pnDimX - nX; ++x) {\
			((t*)(result->m_pbPixels))[x+y*(*m_pnDimX)] = \
			   ( (PIXEL(t,-nX,0)-PIXEL(t,0,0))*(PIXEL(t,+nX,0)-PIXEL(t,0,0)) \
			    +(PIXEL(t,0,-nY)-PIXEL(t,0,0))*(PIXEL(t,0,+nY)-PIXEL(t,0,0)) ) / 9;\
			((t*)(result->m_pb$Pixels))[x+y*(*m_pnDimX)] = theApp.DensScales[aimActive].Transform (((t*)(result->m_pbPixels))[x+y*(*m_pnDimX)]);\
		}
CChannel& CChannel::Laplacian (uvar16_32 nX, uvar16_32 nY) {
	uvar32_64 x, y;
	CChannel *result = m_pDoc->m_pCnlTmp;
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 size = *m_pnDimX * *m_pnDimY;
	switch (bpp) {
		case 1:	PROC_MACRO(ubyte);
			break;
		case 2:	PROC_MACRO(uword);
			break;
		case 3:	PROC_MACRO(udword);
			break;
	}
	CopyData (*result);
	return (*this);
}
#undef	PROC_MACRO

#define PROC_MACRO(t)	\
	buf = new t[(2 * nX + 1) * (2 * nY + 1) + 1]; \
	for (y = nY; y < *m_pnDimY - nY; ++y)\
		for (x = nX; x < *m_pnDimX - nX; ++x) {\
			for (k = 0, i = -nX; i <= nX; ++i) \
				for (j = -nY; j <= nY; ++j) \
					((t*)buf)[k++] = PIXEL(t, i, j); \
			qsort (buf, k, sizeof (t), sortfn_##t); \
			((t*)(result->m_pbPixels))[x+y*(*m_pnDimX)] = ((t*)buf)[k / 2];\
			((t*)(result->m_pb$Pixels))[x+y*(*m_pnDimX)] = theApp.DensScales[aimActive].Transform (((t*)(result->m_pbPixels))[x+y*(*m_pnDimX)]);\
		} \
	delete [] buf;
CChannel& CChannel::Median (uvar16_32 nX, uvar16_32 nY) {
	void *buf;
	svar32_64 x, y, i, j, k;
	CChannel *result = m_pDoc->m_pCnlTmp;
	ubyte bpp = ((*m_pnBPP) >> 3) + (((*m_pnBPP % 8) == 0) ? 0 : 1);
	uvar32_64 size = *m_pnDimX * *m_pnDimY;
	switch (bpp) {
		case 1:	PROC_MACRO(ubyte);
			break;
		case 2:	PROC_MACRO(uword);
			break;
		case 3:	PROC_MACRO(udword);
			break;
	}
	CopyData (*result);
	return (*this);
}
#undef	PROC_MACRO
#undef	PIXEL
