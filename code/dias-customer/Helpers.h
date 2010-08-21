#if !defined(AFX_COLOR_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_)
#define AFX_COLOR_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CColor

class CColor : public CObject {
	DECLARE_SERIAL (CColor);
// Construction
public:
	CColor ();
	CColor (uvar32_64 c1);
	CColor (uvar32_64 c1, uvar32_64 c2);
	CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3);
	CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3, uvar32_64 c4);
	CColor (uvar32_64* pColors, ubyte channels);
	CColor (const CColor& c);
	~CColor ();

	CColor& operator= (const CColor& c);
	uvar32_64& operator[] (ubyte channel);
	bool& operator() (ubyte channel);
	ubyte operator~ ();

// Attributes
protected:
	bool	m_blInitialized;

public:
	ubyte		m_bChannels;
	uvar32_64*	m_pdwValues;
	ubyte*		m_pbMask;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColor)
	//}}AFX_VIRTUAL
};

inline uvar32_64& CColor::operator[] (ubyte channel) {
	return (m_pdwValues[channel]);
}

inline bool& CColor::operator() (ubyte channel) {
	return ((bool&)(m_pbMask[channel]));
}

inline ubyte CColor::operator~ () {
	return (m_bChannels);
}

inline CColor& CColor::operator= (const CColor& c) {
	m_pdwValues = new uvar32_64[m_bChannels = c.m_bChannels];
	memcpy (m_pdwValues, c.m_pdwValues, sizeof (uvar32_64) * m_bChannels);
	m_pbMask = new ubyte[m_bChannels];
	memcpy (m_pbMask, c.m_pdwValues, m_bChannels);
	m_blInitialized = c.m_blInitialized;
	return (*this);
}

/////////////////////////////////////////////////////////////////////////////
// Measure
struct CDiasMeasure {
	struct CPicture {
		CString		Name, Descr;
		uvar32_64	MarksNo, PatchesNo, MetersNo, TextsNo;
	} Image;
	struct CChannel {
		CString		Name, Descr;
	} Channel;
	struct CPatch {
		CString		Name, Descr;

		uvar16_32	Left, Top, Right, Bottom, ApexX, ApexY;
		uvar32_64	Area;
		uvar16_32	Perimeter, PerimeterX, PerimeterY, PerimeterXY;
		uvar16_32	DiamMax, DiamMix, DiamSum;
		rtbyte		DiamMean, DiamSigma;
		rtbyte		DiamMaxAngle, DiamMinAngle;
		uvar32_64	CscArea, CscPerimeter;
		uvar32_64	Fragments;
		rtbyte		GravX, GravY;
		uvar32_64	Sum, Sum2, Min, Max;
		rtbyte		Mean, StdDev;

		rtbyte		$Left, $Top, $Right, $Bottom, $ApexX, $ApexY;
		rtbyte		$Area;
		rtbyte		$Perimeter, $PerimeterX, $PerimeterY, $PerimeterXY;
		rtbyte		$DiamMax, $DiamMix, $DiamSum;
		rtbyte		$DiamMean, $DiamSigma;
		rtbyte		$DiamMaxAngle, $DiamMinAngle;
		rtbyte		$CscArea, $CscPerimeter;
		rtbyte		$Fragments;
		rtbyte		$GravX, $GravY;
		rtbyte		$Sum, $Sum2, $Min, $Max;
		rtbyte		$Mean, $StdDev;
	} Patch;
	struct CMark {
		uvar16_32	PosX, PosY;
		rtbyte		$PosX, $PosY;
		CString		KrnlType, ShpType;
		uvar16_32	KrnlSize, ShpSizeX, ShpSizeY, ShpSizeAngle;
	} Mark;
};

extern CDiasMeasure DiasMeasure;

/////////////////////////////////////////////////////////////////////////////
// CStatistics

struct CStatistics {
	rtbyte	Min;
	rtbyte	Max;
	rtbyte	Q1;
	rtbyte	Q3;
	rtbyte	Med;
	rtbyte	Moda;
	rtbyte	Modas;
	rtbyte	Avg;
	rtbyte	Gvg;
	rtbyte	Hvg;

	rtbyte	Freq[0x100];
};

/////////////////////////////////////////////////////////////////////////////
// CUpdateHint

#define	UPDHLM_TARGET		0xF0000000
#define	UPDHL_ALL		0x00000001
#define	UPDHL_ADDED		0x00000002
#define	UPDHL_REMOVED		0x00000004
#define	UPDHL_NAME		0x00000008
#define UPDHL_IMAGES		0x10000000
#define	UPDHL_SURVEIS		0x20000000
#define	UPDHL_REPORTS		0x40000000

#define	UPDHI_PALETTE		0x00000010
#define	UPDHI_BITMAP		0x00000020
#define	UPDHI_IMAGE		0x00000040
#define	UPDHI_SCALE		0x00000080
#define	UPDHI_ADDMARKER		0x00000100
#define	UPDHI_ADDPATCH		0x00000200
#define	UPDHI_ADDTEXT		0x00000400
#define	UPDHI_ADDMEASURE	0x00000800
#define	UPDHI_MARKERS		0x00001000
#define	UPDHI_IOBJS		0x00002000
#define	UPDHI_TEXT		0x00004000
#define	UPDHI_MEASURES		0x00008000
#define	UPDHIM_ALLVIEWABLE	UPDHI_BITMAP | UPDHI_IMAGE | UPDHI_SCALE | UPDHI_MARKERS | UPDHI_IOBJS | UPDHI_TEXT | UPDHI_MEASURES

#define	UPDHS_ADDRECORD		0x00010000
#define	UPDHS_ADDFIELD		0x00020000
#define	UPDHS_RECORDS		0x00040000
#define	UPDHS_FIELDS		0x00080000
#define UPDHS_VALUE		0x00100000
#define	UPDHSM_ALLVIEWABLE	UPDHS_RECORDS | UPDHS_FIELDS

#define	UPDHRM_ALLVIEWABLE	0/* #### TODO: Place right definition here */

#define UPDH_ALL		UPDHL_ALL | UPDHIM_ALLVIEWABLE | UPDRM_ALLVIEWABLE | UPDDM_ALLVIEWABLE

class CUpdateHint : public CObject {
public:
	enum UpdateTarget {updtList, updtImage, updtSurvey, updtReport};

public:
	CUpdateHint ();
	CUpdateHint (UpdateTarget tp, DWORD cmd, DWORD start = 0, DWORD end = 0);
	~CUpdateHint ();

// Attributes
public:
	UpdateTarget	target;
	DWORD		cmd;
	DWORD		start;
	DWORD		end;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_)
