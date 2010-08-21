// Color.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "Helpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColor

IMPLEMENT_SERIAL (CColor, CObject, 1);

CColor::CColor () {
	m_pdwValues = NULL;
	m_pbMask = NULL;
	m_blInitialized = false;
}

CColor::CColor (uvar32_64 cl) {
	m_pdwValues = new uvar32_64[m_bChannels = 1];
	m_pdwValues[0] = cl;
	m_pbMask = new ubyte[m_bChannels];
	memset (m_pbMask, 1, m_bChannels);
	m_blInitialized = true;
}

CColor::CColor (uvar32_64 c1, uvar32_64 c2) {
	m_pdwValues = new uvar32_64[m_bChannels = 2];
	m_pdwValues[0] = c1;
	m_pdwValues[1] = c2;
	m_pbMask = new ubyte[m_bChannels];
	memset (m_pbMask, 1, m_bChannels);
	m_blInitialized = true;
}

CColor::CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3) {
	m_pdwValues = new uvar32_64[m_bChannels = 3];
	m_pdwValues[0] = c1;
	m_pdwValues[1] = c2;
	m_pdwValues[2] = c3;
	m_pbMask = new ubyte[m_bChannels];
	memset (m_pbMask, 1, m_bChannels);
	m_blInitialized = true;
}

CColor::CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3, uvar32_64 c4) {
	m_pdwValues = new uvar32_64[m_bChannels = 4];
	m_pdwValues[0] = c1;
	m_pdwValues[1] = c2;
	m_pdwValues[2] = c3;
	m_pdwValues[3] = c4;
	m_pbMask = new ubyte[m_bChannels];
	memset (m_pbMask, 1, m_bChannels);
	m_blInitialized = true;
}

CColor::CColor (uvar32_64* pColors, ubyte channels) {
	m_pdwValues = new uvar32_64[m_bChannels = channels];
	if (pColors)
		memcpy (m_pdwValues, pColors, sizeof (uvar32_64) * m_bChannels);
	m_pbMask = new ubyte[m_bChannels];
	memset (m_pbMask, 1, m_bChannels);
	m_blInitialized = true;
}

CColor::CColor (const CColor& c) {
	*this = c;
}

CColor::~CColor () {
	if (!m_blInitialized)	return;
	delete [] m_pdwValues;
	delete [] m_pbMask;
}

/////////////////////////////////////////////////////////////////////////////
// CColor message handlers

CUpdateHint::CUpdateHint () {;}

CUpdateHint::CUpdateHint (UpdateTarget tp, DWORD c, DWORD s, DWORD e) : target (tp), cmd (c), start (s), end (e) {;}

CUpdateHint::~CUpdateHint () {;}
