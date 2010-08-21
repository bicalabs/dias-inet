#include "stdafx.h"
#include "Dias.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		SRCOFFSET_TO_COORDX(off)	(uvar16_32((off)%nDimX))
#define		SRCOFFSET_TO_COORDY(off)	(uvar16_32((off)/nDimX))
#define		MSKOFFSET_TO_COORDX(off)	(uvar16_32((off)%m_nDimX))
#define		MSKOFFSET_TO_COORDY(off)	(uvar16_32((off)/m_nDimX))
#define		COORD_TO_SRCOFFSET(x,y)		(uvar32_64((x) + (y)*nDimX))
#define		COORD_TO_MSKOFFSET(x,y)		(uvar32_64((x) + (y)*m_nDimX))

/*#define		CHECK_OFFSET_POINT(ox,oy)	(wX+(ox)>=0 && wX+(ox)<m_wDimX && wY+(oy)>=0 && wY+(oy)<m_wDimY && m_pbMask[COORD_TO_MSKOFFSET(wX+(ox),wY+(oy))]==2 )
#define		SCAN_OFFSET_POINT(ox,oy)	(wX+(ox)>=0 && wX+(ox)<m_wDimX && wY+(oy)>=0 && wY+(oy)<m_wDimY && m_pbMask[COORD_TO_MSKOFFSET(wX+(ox),wY+(oy))]&0x01==1 )
#define		SCANnSET_OFFSET_POINT(ox,oy)	if ((SCAN_OFFSET_POINT(ox,oy))) {\
							m_pbMask[COORD_TO_MSKOFFSET(wX+(ox),wY+(oy))] |= 0x04;\
							m_pbMask[COORD_TO_MSKOFFSET(wX+(ox),wY+(oy))] &= 0xFE;\
							CalculateFragments(wX+(ox),wY+(oy));\
						}*/

/////////////////////////////////////////////////////////////////////////////
// CPatch

IMPLEMENT_SERIAL (CPatch, CObject, 1);

CPatch::CPatch () {
	m_pImg = NULL;
	m_pDoc = NULL;
	m_strName = "";
	m_strDescript = "";
	
	m_pbMask = NULL;
	m_pdwContent = NULL;
	m_pdwContour = NULL;
	m_dwMskLen = 0;
	m_dwCntLen = 0;
	m_dwConLen = 0;

	theApp.ObjectsAdd (OBJT_PATCH, this);
}

CPatch::CPatch (const CPatch& obj) {
	m_pDoc = NULL;
	m_pImg = NULL;

	m_pbMask = NULL;
	m_pdwContent = NULL;
	m_pdwContour = NULL;
	m_dwMskLen = 0;
	m_dwCntLen = 0;
	m_dwConLen = 0;
	*this = obj;

	theApp.ObjectsAdd (OBJT_PATCH, this);
}

CPatch::CPatch (CPicture* pImg, CString name, CString descr) {
	m_pImg = pImg;
	m_pDoc = m_pImg->m_pDoc;
	m_strName = name;
	m_strDescript = descr;

	m_pbMask = NULL;
	m_pdwContent = NULL;
	m_pdwContour = NULL;
	m_dwMskLen = 0;
	m_dwCntLen = 0;
	m_dwConLen = 0;

	theApp.ObjectsAdd (OBJT_PATCH, this);
}

CPatch::~CPatch () {
	Destroy ();
	theApp.ObjectsRemove (OBJT_PATCH, this);
}

CPatch& CPatch::operator= (const CPatch& obj) {
	Destroy ();

	if (!m_pDoc)	m_pDoc = obj.m_pDoc;
	if (!m_pImg)	m_pImg = obj.m_pImg;

	m_nDimX = obj.m_nDimX;
	m_nDimY = obj.m_nDimY;
	m_dwMskLen = obj.m_dwMskLen;
	m_dwCntLen = obj.m_dwCntLen;
	m_dwConLen = obj.m_dwConLen;

	m_rectPatch = obj.m_rectPatch;
	m_pntApex = obj.m_pntApex;
	m_nArea = obj.m_nArea;
	m_nPerim = obj.m_nPerim;
	m_nPerimX = obj.m_nPerimX;
	m_nPerimY = obj.m_nPerimY;
	m_nPerimXY = obj.m_nPerimXY;
	m_nFragments = obj.m_nFragments;
	m_pntGravCnt = obj.m_pntGravCnt;

	m_pbMask = (BYTE*)aimMemoryCommit (m_dwMskLen, "CPatch::operator=", "m_pbMask");
	m_pdwContent = (DWORD*)aimMemoryCommit (m_dwCntLen * sizeof (uvar32_64), "CPatch::operator=", "m_pdwContent");
	m_pdwContour = (DWORD*)aimMemoryCommit (m_dwConLen * sizeof (uvar32_64), "CPatch::operator=", "m_pdwContour");
	if (!m_pbMask || !m_pdwContent || !m_pdwContour)
		return (*this);
	memcpy (m_pbMask, obj.m_pbMask, m_dwMskLen);
	memcpy (m_pdwContour, obj.m_pdwContour, m_dwConLen * sizeof (uvar32_64));
	memcpy (m_pdwContent, obj.m_pdwContent, m_dwCntLen * sizeof (uvar32_64));

	return (*this);
}

void CPatch::Create (uvar32_64 nLength, uvar32_64* pnOffsets, uword nLayer) {
	Destroy ();

	m_nLayer = nLayer;

	m_dwCntLen = nLength;
	m_pdwContent = (uvar32_64*)aimMemoryCommit (m_dwCntLen * sizeof (uvar32_64), "CPatch::Create", "m_pdwContent");
	memcpy (m_pdwContent, pnOffsets, m_dwCntLen * sizeof (uvar32_64));

	uvar16_32	nX, nY,
			nDimX = *(m_pImg->m_pnDimX),
			nDimY = *(m_pImg->m_pnDimY);

	m_pntApex = CPoint (uvar16_32 (-1), uvar16_32 (-1));
	m_rectPatch.SetRectEmpty ();
	m_rectPatch.left = m_rectPatch.top = uvar16_32 (-1);
	m_pntGravCnt = CPoint (0,0);
	for (uvar32_64 dwCntPtr = 0 ; dwCntPtr < m_dwCntLen ; ++dwCntPtr) {
		nX = SRCOFFSET_TO_COORDX (m_pdwContent[dwCntPtr]);
		nY = SRCOFFSET_TO_COORDY (m_pdwContent[dwCntPtr]);
		m_pntGravCnt.x += nX;
		m_pntGravCnt.y += nY;
		if (nX < m_rectPatch.left)	m_rectPatch.left = nX;
		if (nY < m_rectPatch.top)	m_rectPatch.top = nY;
		if (nX > m_rectPatch.right)	m_rectPatch.right = nX;
		if (nY > m_rectPatch.bottom)	m_rectPatch.bottom = nY;
		if (m_pdwContent[dwCntPtr] < COORD_TO_SRCOFFSET (m_pntApex.x, m_pntApex.y))
			m_pntApex = CPoint (nX, nY);
	}
	m_pntGravCnt.x /= m_dwCntLen;
	m_pntGravCnt.y /= m_dwCntLen;

	// Extracting mask
	m_nDimX = m_rectPatch.Width () + 1;
	m_nDimY = m_rectPatch.Height () + 1;
	m_nArea = m_dwMskLen = m_nDimX * m_nDimY;
	m_pbMask = (ubyte*)aimMemoryCommit (m_dwMskLen, "CPatch::Create", "m_pbMask");
	for (dwCntPtr = 0 ; dwCntPtr < m_dwCntLen ; ++dwCntPtr)
		m_pbMask[COORD_TO_MSKOFFSET (SRCOFFSET_TO_COORDX (m_pdwContent[dwCntPtr]) - m_rectPatch.left,
		                             SRCOFFSET_TO_COORDY (m_pdwContent[dwCntPtr]) - m_rectPatch.top)] = 1;
	m_dwConLen = 0;
	for (nX = 0 ; nX < m_nDimX ; ++nX)
		for (nY = 0 ; nY < m_nDimY ; ++nY) {
			if (m_pbMask[COORD_TO_MSKOFFSET (nX, nY)] != 1)
				continue;
			if (nX == 0 || nY == 0 || nX == m_nDimX - 1 || nY == m_nDimY - 1) {
				m_pbMask[COORD_TO_MSKOFFSET (nX, nY)] = 2;
				++m_dwConLen;
			}
			else if (m_pbMask[COORD_TO_MSKOFFSET (nX - 1, nY)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (nX + 1, nY)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (nX, nY - 1)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (nX, nY + 1)] == 0 ) {
				m_pbMask[COORD_TO_MSKOFFSET (nX, nY)] = 2;
				++m_dwConLen;
			}
		}

	// Finding out countour
	m_pdwContour = (uvar32_64*)aimMemoryCommit (m_dwConLen * sizeof (uvar32_64), "CPatch::Set", "m_pdwContour");
	for (DWORD dwMskPtr = 0, dwConPtr = 0 ; dwMskPtr < m_dwMskLen ; ++dwMskPtr)
		if (m_pbMask[dwMskPtr] == 2)
			m_pdwContour[dwConPtr++] = COORD_TO_SRCOFFSET (nX = MSKOFFSET_TO_COORDX (dwMskPtr) + m_rectPatch.left,
			                                               nY = MSKOFFSET_TO_COORDY (dwMskPtr) + m_rectPatch.top);

	// Calculating perimeter
	m_nPerimX = m_nPerimY = m_nPerimXY = 0;
	for (dwMskPtr = 0 ; dwMskPtr < m_dwMskLen ; ++dwMskPtr)
		if (m_pbMask[dwMskPtr] == 2)
			CalculatePerimeter (MSKOFFSET_TO_COORDX (dwMskPtr), MSKOFFSET_TO_COORDY (dwMskPtr));
	m_nPerim = m_nPerimX + m_nPerimY + m_nPerimXY;
	// #### TODO: Add fragments no calculation here

	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IOBJS);
}

void CPatch::Merge (uvar32_64 lenm, uvar32_64* merge, uint nOp) {
	uvar32_64 nc = 0, nm = 0, na = 0, lenc = m_dwCntLen, lena;
	uvar32_64* array = new uvar32_64[lena = lenm + lenc];
	uvar32_64* content = m_pdwContent;

	if ((nOp & PMO_XOR) == PMO_XOR) {
		while (content && merge && nc < lenc && nm < lenm) {
			if (merge[nm] == content[nc]) {
				nc++;
				nm++;
			} else if (merge[nm] < content[nc])
				array[na++] = merge[nm++];
			else
				array[na++] = content[nc++];
		}
		if (content && nc < lenc) {
			memcpy (array + na, content + nc, (lenc - nc) * sizeof (uvar32_64));
			na += lenc - nc;
		} else if (merge && nm < lenm) {
			memcpy (array + na, merge + nm, (lenm - nm) * sizeof (uvar32_64));
			na += lenm - nm;
		}
	} else if (nOp & PMO_INCLUDE) {
		while (content && merge && nc < lenc && nm < lenm) {
			if (merge[nm] == content[nc]) {
				array[na++] = merge[nm++];
				nc++;
			} else if (merge[nm] < content[nc])
				array[na++] = merge[nm++];
			else
				array[na++] = content[nc++];
		}
		if (content && nc < lenc) {
			memcpy (array + na, content + nc, (lenc - nc) * sizeof (uvar32_64));
			na += lenc - nc;
		} else if (merge && nm < lenm) {
			memcpy (array + na, merge + nm, (lenm - nm) * sizeof (uvar32_64));
			na += lenm - nm;
		}
	} else if (nOp & PMO_EXCLUDE) {
		while (content && merge && nc < lenc && nm < lenm) {
			if (merge[nm] == content[nc]) {
				nc++;
				nm++;
			} else if (merge[nm] < content[nc])
				nm++;
			else
				array[na++] = content[nc++];
		}
		if (content && nc < lenc) {
			memcpy (array + na, content + nc, (lenc - nc) * sizeof (uvar32_64));
			na += lenc - nc;
		}
	} else {
		memcpy (array, merge, lenm * sizeof (uvar32_64));
		na = lenm;
	}

	Create (na, array, m_nLayer);
	delete [] array;

	m_pDoc->Update (CUpdateHint::updtImage, UPDHI_IMAGE);
}

void CPatch::Destroy () {
	if (m_pbMask)
		aimMemoryRelease (m_pbMask, "CPatch::Destroy", "m_pbMask");
	if (m_pdwContent)
		aimMemoryRelease (m_pdwContent, "CPatch::Destroy", "m_pdwContent");
	if (m_pdwContour)
		aimMemoryRelease (m_pdwContour, "CPatch::Destroy", "m_pdwContour");
	m_pbMask = NULL;
	m_pdwContent = NULL;
	m_pdwContour = NULL;
	m_dwMskLen = 0;
	m_dwCntLen = 0;
	m_dwConLen = 0;
}

void CPatch::Delete () {
	m_pImg->Patches.Delete (GetPos ());
}

svar32_64 CPatch::GetPos () {
	if (!m_pImg)
		return (-1);
	m_pImg->Patches.m_pHashHead = m_pImg->Patches.m_pHashHead->GetHead ();
	for (CHash<CPatch*,CPatch>* p = m_pImg->Patches.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (-1);
}

void CPatch::Activate () {
	m_pImg->Patches.Activate (GetPos ());
}

void CPatch::ToBinary (CPicture& pImg, ubyte cBg, ubyte cFg) {
	// #### TODO: This is function draft
	uvar32_64 p, c, len = m_dwCntLen;
	uvar32_64 dimx = m_pDoc->GetDimX (), dimy = m_pDoc->GetDimY ();
	uvar32_64* content = m_pdwContent;
	ubyte* img = new ubyte [dimx * dimy];

	memset (img, cBg, dimx * dimy);
	for (p = 0, c = 0; p < dimy * dimx && c < len; ++p) {
		if (p == content[c]) {
			img[p] = cFg;
			c++;
		}
	}

	pImg.Channels[0].SetBits (img, dimx * dimy);
}

void CPatch::Calculate (ubyte nChnlNo) {
	uvar16_32	nDimX = *(m_pImg->m_pnDimX),
			nDimY = *(m_pImg->m_pnDimY);
	uvar32_64	nValue, j;
	uvar32_64	dwConPtr;
	ubyte		nAlpha;
	rtbyte		rValue;
	rtbyte		rPI = 3.1415926535897932384626433832795,
			rRotAngle = rPI / 32.0;
	rtbyte		rDiamMin, rDiamMax, rDiamSum2,
			rX, rY, rXX, rYY,
			rTopX, rTopY, rBottomX, rBottomY,
			rFeretY, rAngle;
	rtbyte		prFactors[3];
	CVector<rtbyte> vA, vB, vC;
	CChannel* pChnl = &(m_pImg->Channels[nChnlNo]);

	// #### TODO: Put geometric scaling factors here
	prFactors[0] = theApp.GeomScales[aimActive].GetCoefX ();
	prFactors[1] = theApp.GeomScales[aimActive].GetCoefY ();
	prFactors[2] = pow ((long double) prFactors[0] * prFactors[0] + prFactors[1] * prFactors[1], 0.5L);
	m_rArea = m_nArea * prFactors[0] * prFactors[1];
	m_rPerimX = m_nPerimX * prFactors[0];
	m_rPerimY = m_nPerimY * prFactors[1];
	m_rPerimXY = m_nPerimXY * prFactors[0] * prFactors[1];
	m_rPerim = m_rPerimX + m_rPerimY + m_rPerimXY;
	m_rGravCntX = m_pntGravCnt.x * prFactors[0];
	m_rGravCntY = m_pntGravCnt.y * prFactors[1];

	// #### TODO: Allocate memory for m_prDistrib

	// Determining densitometric parameters
	m_rSum = m_rSum2 = m_rMax = 0;
	m_rMin = 1e100; // #### TODO: Place max real value here
	m_prDistrib = (rtbyte*)aimMemoryCommit (m_pImg->m_pDoc->m_nBPP, "CPatch::Measure", "m_prDistrib");
	for (uvar32_64 n = 0 ; n < m_pImg->m_pDoc->m_nBPP ; ++n)
		m_prDistrib[n] = 0;
	for (uvar32_64 dwCntPtr = 0 ; dwCntPtr < m_dwCntLen ; ++dwCntPtr) {
		nValue = pChnl->GetPixel (m_pdwContent[dwCntPtr], false);
		rValue = pChnl->GetPixel (m_pdwContent[dwCntPtr], true);
		m_rSum += rValue;
		m_rSum2 += rValue * rValue;
		m_rMin = (rValue < m_rMin) ? rValue : m_rMin;
		m_rMax = (rValue > m_rMax) ? rValue : m_rMax;
		++m_prDistrib[nValue];
	}
	m_rMean = m_rSum / m_dwCntLen;
	if (m_dwCntLen != 1)
		m_rStdDev = (long double)pow ((long double)(m_rSum2 * m_dwCntLen - m_rSum * m_rSum) / (m_dwCntLen * (m_dwCntLen - 1)), 0.5L);
	else
		m_rStdDev = 0.;
	for (n = 0 ; n < m_pImg->m_pDoc->m_nBPP ; ++n)
		m_prDistrib[n] /= rtbyte (m_dwCntLen);

	// Rotating
	m_prFeretsX[0] = m_pntApex.x * prFactors[0];
	m_prFeretsY[0] = m_pntApex.y * prFactors[1];
	m_prDiameters[0] = (m_rectPatch.bottom - m_rectPatch.top) * prFactors[0];
	m_nDiamMin = m_nDiamMax = 0;
	m_rDiamMean = rDiamMin = rDiamMax = m_prDiameters[0];
	rDiamSum2 = m_prDiameters[0] * m_prDiameters[0];
	for (ubyte rot = 1; rot < 64; ++rot) {
		rTopX = rTopY = rFeretY = 0x7FFFFFFF;
		rBottomX = rBottomY = -0x7FFFFFFF;
		for (dwConPtr = 0; dwConPtr < m_dwConLen; ++dwConPtr) {
			rXX = SRCOFFSET_TO_COORDX (m_pdwContour[dwConPtr]) - m_pntGravCnt.x;
			rYY = SRCOFFSET_TO_COORDY (m_pdwContour[dwConPtr]) - m_pntGravCnt.y;
			rX = rXX * cosl (rRotAngle * rot) - rYY * sinl (rRotAngle * rot);
			rY = rXX * sinl (rRotAngle * rot) + rYY * cosl (rRotAngle * rot);
			if (rX < rTopX) rTopX = rX;
			if (rY < rTopY) rTopY = rY;
			if (rX > rBottomX) rBottomX = rX;
			if (rY > rBottomY) rBottomY = rY;
			if (rY < rFeretY) {
				rFeretY = rY;
				m_prFeretsX[rot] = SRCOFFSET_TO_COORDX (m_pdwContour[dwConPtr]);
				m_prFeretsY[rot] = SRCOFFSET_TO_COORDY (m_pdwContour[dwConPtr]);
				if (m_prFeretsX[rot] - m_pntGravCnt.x < 0) m_prFeretsX[rot] -= .5;
				if (m_prFeretsX[rot] - m_pntGravCnt.x > 0) m_prFeretsX[rot] += .5;
				if (m_prFeretsY[rot] - m_pntGravCnt.y < 0) m_prFeretsY[rot] -= .5;
				if (m_prFeretsY[rot] - m_pntGravCnt.y > 0) m_prFeretsY[rot] += .5;
			}
		}
		if (rot >= 32)
			continue;
		nAlpha = (rot <= 16) ? rot : (32 - rot);
		rAngle = rRotAngle * nAlpha;
		m_prDiameters[rot] = rBottomX - rTopX + sinl (rAngle) + cosl (rAngle);
		m_prDiameters[rot] *= (long double)pow ((long double)prFactors[1]*sinl(rAngle) * prFactors[1]*sinl(rAngle) + prFactors[0]*cosl(rAngle) * prFactors[0]*cosl(rAngle), 0.5L);
		m_prAngles[rot] = rot * rRotAngle * 180.0 / rPI;
		m_rDiamMean += m_prDiameters[rot];
		rDiamSum2 += m_prDiameters[rot] * m_prDiameters[rot];
		if (rDiamMin > m_prDiameters[rot]) {
			rDiamMin = m_prDiameters[rot];
			m_nDiamMin = rot;
		}
		if (rDiamMax < m_prDiameters[rot]) {
			rDiamMax = m_prDiameters[rot];
			m_nDiamMax = rot;
		}
	}
	m_rDiamSigma = pow ((32.0*rDiamSum2 - m_rDiamMean*m_rDiamMean) / 992.0, 0.5);
	m_rDiamMean /= 32.0;
	
	// Calculating circumscribed shape parameters
	m_rCscArea = m_rCscPerim = 0.0;
	for (rot = 0 ; rot < 64 ; ++rot) {
		j = (rot == 63) ? 0 : rot + 1;
		vA(1) = (m_prFeretsX[j] - m_prFeretsX[rot]) * prFactors[0];
		vA(2) = (m_prFeretsY[j] - m_prFeretsY[rot]) * prFactors[1];
		vB(1) = (m_prFeretsX[rot] - m_pntGravCnt.x) * prFactors[0];
		vB(2) = (m_prFeretsY[rot] - m_pntGravCnt.y) * prFactors[1];
		vC(1) = (m_prFeretsX[j] - m_pntGravCnt.x) * prFactors[0];
		vC(2) = (m_prFeretsY[j] - m_pntGravCnt.y) * prFactors[1];
		rtbyte rp = ( vA.Module() + vB.Module() + vC.Module() ) / 2;
		m_rCscArea += pow ((long double)rp*(rp-vA.Module())*(rp-vB.Module())*(rp-vC.Module()), 0.5L);
		m_rCscPerim += vA.Module ( );
	}

	aimMemoryRelease (m_prDistrib, "CPatch::Measure", "m_prDistrib");
/*
	int		rot;
	ubyte		bAlpha;
	uvar16_32	wX, wY;
	uvar32_64	i, j, c;
	uvar32_64	dwSumX, dwSumY,
			dwSumGreyX, dwSumGreyY, dwSumGrey;
	uvar32_64	dwMskPtr,
			dwCntPtr,
			dwConPtr,
			dwSrcPtr,
			dwSrcLen = m_pDoc->GetDimX () * m_pDoc->GetDimY ();
	rtbyte		rPlane,
			rX, rY, rXX, rYY, rFeretY,
			rDimMin, rDimMax, rDiamSum2,
			rAngle,
			rPI,
			rRotAngle;
	rtbyte		lrTop[2], lrBottom[2];
	rtbyte		lrFactors[4];
	CVector<rtbyte> vA, vB, vC;

	// #### TODO: Put geometric scaling factors here
	lrFactors[0] = 1; //wieConfig.m_cfgGeomScales[wieConfig.m_cfgGeomScale].m_scaleFactorX;
	lrFactors[1] = 1; //wieConfig.m_cfgGeomScales[wieConfig.m_cfgGeomScale].m_scaleFactorY;
	lrFactors[2] = pow ( lrFactors[0] * lrFactors[0] + lrFactors[1] * lrFactors[1], 0.5 );

	// Determing statistics and bounding rectangle
	dwSumX = dwSumY = 0;
	dwSumGrey = dwSumGreyX = dwSumGreyY = 0;
	m_lwApex[0] = m_lwApex[1] = 0xFFFF;
	m_lwTop[0] = m_lwTop[1] = 0xFFFF;
	m_lwBottom[0] = m_lwBottom[1] = 0x0000;
	for (j = 0 ; j < 4 ; ++j) {
		m_lrSum[j] = m_lrSum2[j] = 0;
		m_lrMin[j] = 0xFF;
		m_lrMax[j] = 0;
		m_ldwCount[j] = m_dwCntLen;
		for (c = 0 ; c < 256 ; ++c)
			m_llrDistrib[j][c] = 0;
	}
	for (dwCntPtr = 0 ; dwCntPtr < m_dwCntLen ; ++dwCntPtr) {
		// #### TODO: Put dens transform function here
		rPlane = (MEANCOLOR (m_pdwPixels[m_pdwContent[dwCntPtr]]));
		m_lrSum[0] += rPlane;
		m_lrSum2[0] += rPlane * rPlane;
		m_lrMin[0] = (rPlane < m_lrMin[0]) ? rPlane : m_lrMin[0];
		m_lrMax[0] = (rPlane > m_lrMax[0]) ? rPlane : m_lrMax[0];
		++m_llrDistrib[0][MEANCOLOR (m_pdwPixels[m_pdwContent[dwCntPtr]])];
		wX = SRCOFFSET_TO_COORDX (m_pdwContent[dwCntPtr]);
		wY = SRCOFFSET_TO_COORDY (m_pdwContent[dwCntPtr]);
		dwSumX += wX;
		dwSumY += wY;
		dwSumGreyX += wX * SUMCOLOR (m_pdwPixels[m_pdwContent[dwCntPtr]]);
		dwSumGreyY += wY * SUMCOLOR (m_pdwPixels[m_pdwContent[dwCntPtr]]);
		dwSumGrey += SUMCOLOR (m_pdwPixels[m_pdwContent[dwCntPtr]]);
		for (j = 1 ; j < 4 ; ++j) {
			// #### TODO: Put dens transform function here
			rPlane = ((BYTE) GET_COLOR (m_pdwPixels[m_pdwContent[dwCntPtr]], j - 1));
			m_lrSum[j] += rPlane;
			m_lrSum2[j] += rPlane * rPlane;
			m_lrMin[j] = (rPlane < m_lrMin[j]) ? rPlane : m_lrMin[j];
			m_lrMax[j] = (rPlane > m_lrMax[j]) ? rPlane : m_lrMax[j];
			++m_llrDistrib[j][(BYTE) GET_COLOR (m_pdwPixels[m_pdwContent[dwCntPtr]], j - 1)];
		}
		if (wX < m_lwTop[0]) m_lwTop[0] = wX;
		if (wY < m_lwTop[1]) m_lwTop[1] = wY;
		if (wX > m_lwBottom[0]) m_lwBottom[0] = wX;
		if (wY > m_lwBottom[1]) m_lwBottom[1] = wY;
		if (m_pdwContent[dwCntPtr] < COORD_TO_SRCOFFSET (m_lwApex[0], m_lwApex[1])) {
			m_lwApex[0] = wX; m_lwApex[1] = wY;
		}
	}
	// Determing mean and deviance parameters
	for (j = 0 ; j < 4 ; ++j) {
		m_lrMean[j] = m_lrSum[j] / m_ldwCount[j];
		if (m_ldwCount[j] != 1)
			m_lrStdDev[j] = pow ( ((m_lrSum2[j] * m_ldwCount[j]) - (m_lrSum[j] * m_lrSum[j])) / (m_ldwCount[j] * (m_ldwCount[j] - 1)), 0.5);
		else
			m_lrStdDev[j] = 0.0;
		for (c = 0 ; c < 256 ; ++c)
			m_llrDistrib[j][c] /= rtbyte (m_dwCntLen);
	}
	m_lrCntGravity[0] = rtbyte (dwSumX) / rtbyte (m_dwCntLen);
	m_lrCntGravity[1] = rtbyte (dwSumY) / rtbyte (m_dwCntLen);
	m_lrGryGravity[0] = rtbyte (dwSumGreyX) / rtbyte (dwSumGrey);
	m_lrGryGravity[1] = rtbyte (dwSumGreyY) / rtbyte (dwSumGrey);

	// Creation of the Mask
	m_wDimX = m_lwBottom[0] - m_lwTop[0] + 1;
	m_wDimY = m_lwBottom[1] - m_lwTop[1] + 1;
	m_dwMskLen = m_wDimX * m_wDimY;
	if (!(m_pbMask = (ubyte*)aimMemoryCommit (m_dwMskLen, "CPatch::Set", "m_pbMask")) )
		throw (CaImAPIException(0));
	for (dwCntPtr = 0 ; dwCntPtr < m_dwCntLen ; ++dwCntPtr)
		m_pbMask[COORD_TO_MSKOFFSET (SRCOFFSET_TO_COORDX (m_pdwContent[dwCntPtr]) - m_lwTop[0],
		                             SRCOFFSET_TO_COORDY ( m_pdwContent[dwCntPtr] ) - m_lwTop[1] )] = 1;
	m_dwConLen = 0;
	for (wX = 0 ; wX < m_wDimX ; ++wX)
		for (wY = 0 ; wY < m_wDimY ; ++wY) {
			if (m_pbMask[COORD_TO_MSKOFFSET (wX, wY)] != 1)
				continue;
			if (wX == 0 || wY == 0 || wX == m_wDimX - 1 || wY == m_wDimY - 1) {
				m_pbMask[COORD_TO_MSKOFFSET (wX, wY)] = 2;
				++m_dwConLen;
			}
			else if (m_pbMask[COORD_TO_MSKOFFSET (wX - 1, wY)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (wX + 1, wY)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (wX, wY - 1)] == 0 ||
			         m_pbMask[COORD_TO_MSKOFFSET (wX, wY + 1)] == 0 ) {
				m_pbMask[COORD_TO_MSKOFFSET (wX, wY)] = 2;
				++m_dwConLen;
			}
		}

	if (!( m_pdwContour = (uvar32_64*)aimMemoryCommit (m_dwConLen * 4, "CPatch::Set", "m_pdwContour")))
		throw (CaImAPIException(0));
	for (dwMskPtr = dwConPtr = 0 ; dwMskPtr < m_dwMskLen ; ++dwMskPtr)
		if (m_pbMask[dwMskPtr] == 2)
			m_pdwContour[dwConPtr++] = COORD_TO_SRCOFFSET (MSKOFFSET_TO_COORDX (dwMskPtr) + m_lwTop[0],
			                                               MSKOFFSET_TO_COORDY ( dwMskPtr ) + m_lwTop[1] );

	// Calculating CntGravity, Perimeter, Area
	m_rPerimX = m_rPerimY = m_rPerimXY = 0;
	for (dwMskPtr = 0 ; dwMskPtr < m_dwMskLen ; ++dwMskPtr)
		if (m_pbMask[dwMskPtr] == 2)
			CalculatePerimeter (MSKOFFSET_TO_COORDX (dwMskPtr), MSKOFFSET_TO_COORDY (dwMskPtr));
	m_rArea = m_dwCntLen * lrFactors[0] * lrFactors[1];
	m_rPerim = m_rPerimX * lrFactors[0] + m_rPerimY * lrFactors[1] + m_rPerimXY * lrFactors[2];

	// Rotating
	m_llrFerets[0][0] = m_lwApex[0];
	m_llrFerets[0][1] = m_lwApex[1];
	m_lrDiameters[0] = (m_lwBottom[0] - m_lwTop[0] + 1.0)*lrFactors[0];					// !!!
	m_rDiamMean = m_lrDiameters[0];
	rDiamSum2 = m_lrDiameters[0] * m_lrDiameters[0];
	rDimMin = m_lrDiameters[0];
	rDimMax = m_lrDiameters[0];
	m_lrAngles[0] = 0.0;
	m_bDiamMaxNo = m_bDiamMinNo = 0;
	for (rot = 1 ; rot < 64 ; ++rot) {
		lrTop[0] = lrTop[1] = rFeretY = 0x7FFFFFFF;
		lrBottom[0] = lrBottom[1] = -0x7FFFFFFF;
		for (dwConPtr = 0 ; dwConPtr < m_dwConLen ; ++dwConPtr) {
			rXX = SRCOFFSET_TO_COORDX (m_pdwContour[dwConPtr]) - m_lrCntGravity[0];
			rYY = SRCOFFSET_TO_COORDY (m_pdwContour[dwConPtr]) - m_lrCntGravity[1];
			rX = rXX * cosl (rRotAngle * rot) - rYY * sinl (rRotAngle * rot);
			rY = rXX * sinl (rRotAngle * rot) + rYY * cosl (rRotAngle * rot);
			if (rX < lrTop[0]) lrTop[0] = rX;
			if (rY < lrTop[1]) lrTop[1] = rY;
			if (rX > lrBottom[0]) lrBottom[0] = rX;
			if (rY > lrBottom[1]) lrBottom[1] = rY;
			if (rY < rFeretY) {
				rFeretY = rY;
				m_llrFerets[rot][0] = SRCOFFSET_TO_COORDX (m_pdwContour[dwConPtr]);
				m_llrFerets[rot][1] = SRCOFFSET_TO_COORDY (m_pdwContour[dwConPtr]);
				if (m_llrFerets[rot][0] - m_lrCntGravity[0] < 0) m_llrFerets[rot][0] -= .5;
				if (m_llrFerets[rot][0] - m_lrCntGravity[0] > 0) m_llrFerets[rot][0] += .5;
				if (m_llrFerets[rot][1] - m_lrCntGravity[1] < 0) m_llrFerets[rot][1] -= .5;
				if (m_llrFerets[rot][1] - m_lrCntGravity[1] > 0) m_llrFerets[rot][1] += .5;
			}
		}
		if (rot >= 32)
			continue;
		bAlpha = (rot <= 16) ? rot : (32 - rot);
		rAngle = rRotAngle * bAlpha;
		m_lrDiameters[rot] = lrBottom[0] - lrTop[0] + sinl (rAngle) + cosl (rAngle);
		m_lrDiameters[rot] *= pow (lrFactors[1]*sinl(rAngle) * lrFactors[1]*sinl(rAngle) + lrFactors[0]*cosl(rAngle) * lrFactors[0]*cosl(rAngle), 0.5);
		m_lrAngles[rot] = rot * rRotAngle * 180.0 / rPI;
		m_rDiamMean += m_lrDiameters[rot];
		rDiamSum2 += m_lrDiameters[rot] * m_lrDiameters[rot];
		if (rDimMin > m_lrDiameters[rot]) {
			rDimMin = m_lrDiameters[rot];
			m_bDiamMinNo = (BYTE)rot;
		}
		if (rDimMax < m_lrDiameters[rot]) {
			rDimMax = m_lrDiameters[rot];
			m_bDiamMaxNo  = (BYTE)rot;
		}
	}
	m_rDiamSigma = pow ((32.0*rDiamSum2 - m_rDiamMean*m_rDiamMean) / 992.0, 0.5);
	m_rDiamMean /= 32.0;
	m_rCscArea = m_rCscPerim = 0.0;
	for (i = 0 ; i < 64 ; ++i) {
		j = (i == 63) ? 0 : i + 1;
		vA(1) = (m_llrFerets[j][0] - m_llrFerets[i][0]) * lrFactors[0];
		vA(2) = (m_llrFerets[j][1] - m_llrFerets[i][1]) * lrFactors[1];
		vB(1) = (m_llrFerets[i][0] - m_lrCntGravity[0]) * lrFactors[0];
		vB(2) = (m_llrFerets[i][1] - m_lrCntGravity[1]) * lrFactors[1];
		vC(1) = (m_llrFerets[j][0] - m_lrCntGravity[0]) * lrFactors[0];
		vC(2) = (m_llrFerets[j][1] - m_lrCntGravity[1]) * lrFactors[1];
		rtbyte rp = ( vA.Module() + vB.Module() + vC.Module() ) / 2;
		m_rCscArea += pow ( rp*(rp-vA.Module())*(rp-vB.Module())*(rp-vC.Module()), 0.5 );
		m_rCscPerim += vA.Module ( );
	}

	dwSumX = 0;
	dwSumY = 0;
	for (dwConPtr = 0 ; dwConPtr < m_dwConLen ; ++dwConPtr) {
		dwSumX += SRCOFFSET_TO_COORDX (m_pdwContour[dwConPtr]);
		dwSumY += SRCOFFSET_TO_COORDY (m_pdwContour[dwConPtr]);
	}
	m_lrConGravity[0] = rtbyte (dwSumX) / rtbyte (m_dwConLen);
	m_lrConGravity[1] = rtbyte (dwSumY) / rtbyte (m_dwConLen);
	rX = (m_lrCntGravity[0] - m_lrConGravity[0]) * lrFactors[0];
	rY = (m_lrCntGravity[1] - m_lrConGravity[1]) * lrFactors[1];
	m_rCntConDist = pow (rX*rX + rY*rY, 0.5);
	m_rAngleContAxis = atan2 (rY, rX);
	rX = (m_lrCntGravity[0] - m_lrGryGravity[0]) * lrFactors[0];
	rY = (m_lrCntGravity[1] - m_lrGryGravity[1]) * lrFactors[1];
	m_rCntGryDist = pow (rX*rX + rY*rY, 0.5);
	m_rAngleGreyAxis = atan2 (rY, rX);

	m_dwFragments = 0;*/
}

void CPatch::CalculatePerimeter (WORD wX, WORD wY) {
	// #### TODO: Add code here
	;
}

void CPatch::CalculateFragments (WORD wX, WORD wY) {
	// #### TODO: Add code here
	;
}

void CPatch::Measure () {
	theTesla.Variables["Patch.Name"] = m_strName;
}

void CPatch::Paint (CDC& dc) {
	udword px;
	uvar16_32 nDimX = *(m_pImg->m_pnDimX),
		  nDimY = *(m_pImg->m_pnDimY);
	dc.SetROP2 (R2_COPYPEN);
	for (uvar32_64 dw = 0 ; dw < m_dwCntLen ; ++dw) {
		px = m_pImg->GetPixel (m_pdwContent[dw]);
		switch (m_nLayer) {
			case 0: px = RGB (GetRValue (px) / 1.5, GetGValue(px) / 1.5, sqrt (double(GetBValue(px)) / 255.) * 255);	break;
			case 1:	px = RGB (GetRValue (px) / 1.5, sqrt (double(GetGValue(px)) / 255.) * 255, GetBValue(px) / 1.5);	break;
			case 2:	px = RGB (sqrt (double(GetRValue(px)) / 255.) * 255, GetGValue (px) / 1.5, GetBValue(px) / 1.5);	break;
			case 3:	px = RGB (sqrt (double(GetRValue(px)) / 255.) * 255, sqrt (double(GetGValue(px)) / 255.) * 255, GetBValue(px) / 1.5);	break;
			default:	px = RGB (GetRValue(px) / 1.5, sqrt (double(GetGValue(px)) / 255.) * 255, sqrt (double(GetBValue(px)) / 255.) * 255);	break;
		}
		dc.SetPixelV (SRCOFFSET_TO_COORDX(m_pdwContent[dw]), SRCOFFSET_TO_COORDY(m_pdwContent[dw]), px);
	}
	for (dw = 0 ; dw < m_dwConLen ; ++dw) {
		switch (m_nLayer) {
			case 0: px = RGB (0x00, 0x00, 0xFF);	break;
			case 1: px = RGB (0x00, 0xFF, 0x00);	break;
			case 2: px = RGB (0xFF, 0x00, 0x00);	break;
			case 3: px = RGB (0xFF, 0xFF, 0x00);	break;
			default: px = RGB (0x00, 0xFF, 0xFF);	break;
		}
		dc.SetPixelV (SRCOFFSET_TO_COORDX(m_pdwContour[dw]), SRCOFFSET_TO_COORDY(m_pdwContour[dw]), px);
	}

	if (m_pImg->Patches.Active () == GetPos ()) {
		CPen pn (PS_SOLID, 1, px);
		CBrush br (px);
		CBrush br2 (0xFFFFFF);

//		dc.FrameRect (m_rectPatch, &br);
		
		CPen* pPen = dc.SelectObject (&pn);
		int size = min (m_rectPatch.Height (), m_rectPatch.Width ());
		size = min (size, 10);
		dc.MoveTo (m_rectPatch.left, m_rectPatch.top + size);
		dc.LineTo (m_rectPatch.left, m_rectPatch.top);
		dc.LineTo (m_rectPatch.left + size, m_rectPatch.top);
		dc.MoveTo (m_rectPatch.right, m_rectPatch.top + size);
		dc.LineTo (m_rectPatch.right, m_rectPatch.top);
		dc.LineTo (m_rectPatch.right - size, m_rectPatch.top);

		dc.MoveTo (m_rectPatch.left, m_rectPatch.bottom - size);
		dc.LineTo (m_rectPatch.left, m_rectPatch.bottom);
		dc.LineTo (m_rectPatch.left + size, m_rectPatch.bottom);
		dc.MoveTo (m_rectPatch.right, m_rectPatch.bottom - size);
		dc.LineTo (m_rectPatch.right, m_rectPatch.bottom);
		dc.LineTo (m_rectPatch.right - size, m_rectPatch.bottom);
		dc.SelectObject (pPen);

		dc.FillSolidRect (CRect (m_pntApex - CSize (-2, -2), CSize (4, 4)), px);
		dc.FrameRect (CRect (m_pntApex - CSize (-2, -2), CSize (4, 4)), &br2);
		
		dc.FillSolidRect (CRect (m_pntGravCnt - CSize (-2, -2), CSize (4, 4)), px);
		dc.FrameRect (CRect (m_pntGravCnt - CSize (-2, -2), CSize (4, 4)), &br2);

		HTHEME hTheme;
		if (aimThemeLoaded) {
			hTheme = aimThemeOpen (theApp.m_pMainWnd->m_hWnd, L"Combobox");
			aimThemeBackground (hTheme, dc, CP_DROPDOWNBUTTON, CBXS_NORMAL, 
			                    CRect (m_rectPatch.BottomRight (), m_rectPatch.BottomRight () + CPoint (14, 14)), NULL);
		}
	}
}

void CPatch::Serialize (CArchive& ar) {
	udword empty;
	ubyte *pbData;
	uvar32_64 ziplen, len;
	CObject::Serialize (ar);
	if (ar.IsStoring ()) {
		ar << (udword)1; // Ver. 1
		ar << m_strName << m_strDescript;
		ar << m_nDimX << m_nDimY;
		ar << m_dwMskLen << m_dwCntLen << m_dwConLen;
//		ar << (udword)0;		// No compression
		ar << (udword)1;		// Zip Compression
		
		pbData = aimCompressZip ((ubyte*)m_pbMask, m_dwMskLen, ziplen);
		ar << ziplen;
		ar.Write (pbData, ziplen);
		aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");
		pbData = aimCompressZip ((ubyte*)m_pdwContent, m_dwCntLen * sizeof (uvar32_64), ziplen);
		ar << ziplen;
		ar.Write (pbData, ziplen);
		aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");
		pbData = aimCompressZip ((ubyte*)m_pdwContour, m_dwConLen * sizeof (uvar32_64), ziplen);
		ar << ziplen;
		ar.Write (pbData, ziplen);
		aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");

		ar << m_nLayer;
		ar << m_rectPatch;
		ar << m_pntApex << m_pntGravCnt;
		ar << m_nArea << m_nPerim << m_nPerimX << m_nPerimY << m_nPerimXY;
		ar << m_nFragments;
	} else {
		Destroy ();

		udword ct;
		ar >> empty;
		ar >> m_strName >> m_strDescript;
		ar >> m_nDimX >> m_nDimY;
		ar >> m_dwMskLen >> m_dwCntLen >> m_dwConLen;
		ar >> ct;

		if (ct == 0) {		// No compression
			m_pbMask = (ubyte*)aimMemoryCommit (m_dwMskLen, "CPatch::Serialize", "m_pbMask");
			m_pdwContent = (uvar32_64*)aimMemoryCommit (m_dwCntLen * sizeof (uvar32_64), "CPatch::Serialize", "m_pdwContent");
			m_pdwContour = (uvar32_64*)aimMemoryCommit (m_dwConLen * sizeof (uvar32_64), "CPatch::Serialize", "m_pdwContour");
			ar.Read (m_pbMask, m_dwMskLen);
			ar.Read (m_pdwContent, m_dwCntLen * sizeof (uvar32_64));
			ar.Read (m_pdwContour, m_dwConLen * sizeof (uvar32_64));
		} else if (ct == 1) {	// Zip Compression

			ar >> ziplen;
			pbData = (ubyte*)aimMemoryCommit (ziplen, "CPatch::Serialize", "pbData");
			ar.Read (pbData, ziplen);
			m_pbMask = aimDecompressZip (pbData, ziplen, len);
			aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");

			ar >> ziplen;
			pbData = (ubyte*)aimMemoryCommit (ziplen, "CPatch::Serialize", "pbData");
			ar.Read (pbData, ziplen);
			m_pdwContent = (uvar32_64*)aimDecompressZip (pbData, ziplen, len);
			aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");

			ar >> ziplen;
			pbData = (ubyte*)aimMemoryCommit (ziplen, "CPatch::Serialize", "pbData");
			ar.Read (pbData, ziplen);
			m_pdwContour = (uvar32_64*)aimDecompressZip (pbData, ziplen, len);
			aimMemoryRelease (pbData, "CPatch::Serialize", "pbData");
		}

		ar >> m_nLayer;
		ar >> m_rectPatch;
		ar >> m_pntApex >> m_pntGravCnt;
		ar >> m_nArea >> m_nPerim >> m_nPerimX >> m_nPerimY >> m_nPerimXY;
		ar >> m_nFragments;
	}
}
