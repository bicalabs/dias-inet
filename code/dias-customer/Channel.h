#if !defined(AFX_CHANNEL_H__F84FDF59_6664_4451_86DE_2756F54C8269__INCLUDED_)
#define AFX_CHANNEL_H__F84FDF59_6664_4451_86DE_2756F54C8269__INCLUDED_

#pragma once

class CChannel : public CObject {
friend class CHash<CChannel*,CChannel>;
friend class CaImAPICollection<CChannel>;
friend class CaImAPICollectionExt<CChannel>;
friend class CDiasAPIChannels;
friend class CDiasAPIPatches;
friend class CStorageDoc;
friend class CPatch;
// Construction
	DECLARE_SERIAL (CChannel);
protected:
	CChannel ();
	CChannel (CStorageDoc* pDoc, CString name, CString descr);
	CChannel (const CChannel& cnhl);
public:
	~CChannel ();
	CChannel& operator= (const CChannel& chnl) throw (CaImAPIException);

// Attributes
protected:
	CStorageDoc*	m_pDoc;

	CString		m_strName;
	CString		m_strDescript;
	ColorModel*	m_pnColorModel;
	ubyte*		m_pnBPP;
	uvar16_32*	m_pnDimX;
	uvar16_32*	m_pnDimY;
	ubyte*		m_pbPixels;
	ubyte*		m_pb$Pixels;

// Operations
public:

	CString		GetName ();
	CString		GetDescription ();
	void		SetName (CString name);
	void		SetDescription (CString descr);

	void		GetBits (ubyte* pbBits, uvar32_64 len, ubyte warp = 1, bool transform = false) throw (CaImAPIException);
	void		SetBits (ubyte* pbBits, uvar32_64 len, ubyte warp = 1) throw (CaImAPIException);
	udword		GetPixel (uvar16_32 nX, uvar16_32 nY, bool transform = false);
	udword		GetPixel (uvar32_64 nOffset, bool transform = false);
	void		SetPixel (uvar16_32 nX, uvar16_32 nY, udword nValue);
	void		SetPixel (uvar32_64 nOffset, udword nValue);
	void		CopyData (CChannel& chnl);

	void		Statistics (CStatistics& stat);

	CChannel operator+ (const CChannel& chnl) { return (Add (chnl)); };
	CChannel operator- (const CChannel& chnl) { return (Sub (chnl)); };
	CChannel operator* (const CChannel& chnl) { return (Mul (chnl)); };
	CChannel operator/ (const CChannel& chnl) { return (Div (chnl)); };
	CChannel operator% (const CChannel& chnl) { return (Mod (chnl)); };
	CChannel operator+= (const CChannel& chnl) { *this = *this + chnl; return (*this); };
	CChannel operator-= (const CChannel& chnl) { *this = *this - chnl; return (*this); };
	CChannel operator*= (const CChannel& chnl) { *this = *this * chnl; return (*this); };
	CChannel operator/= (const CChannel& chnl) { *this = *this / chnl; return (*this); };
	CChannel operator%= (const CChannel& chnl) { *this = *this % chnl; return (*this); };

	CChannel operator+ (const uvar32_64& val) { return (Add (val)); };
	CChannel operator- (const uvar32_64& val) { return (Sub (val)); };
	CChannel operator* (const uvar32_64& val) { return (Mul (val)); };
	CChannel operator/ (const uvar32_64& val) { return (Div (val)); };
	CChannel operator% (const uvar32_64& val) { return (Mod (val)); };
	CChannel operator+= (const uvar32_64& val) { *this = *this + val; return (*this); };
	CChannel operator-= (const uvar32_64& val) { *this = *this - val; return (*this); };
	CChannel operator*= (const uvar32_64& val) { *this = *this * val; return (*this); };
	CChannel operator/= (const uvar32_64& val) { *this = *this / val; return (*this); };
	CChannel operator%= (const uvar32_64& val) { *this = *this % val; return (*this); };

	CChannel& operator~ () { return (Not ()); };
	CChannel& operator& (const CChannel& chnl) { return (And (chnl)); };
	CChannel& operator| (const CChannel& chnl) { return (Or (chnl)); };
	CChannel operator^ (const CChannel& chnl) { return (Xor (chnl)); };
	CChannel operator&= (const CChannel& chnl) { *this = *this & chnl; return (*this); };
	CChannel operator|= (const CChannel& chnl) { *this = *this | chnl; return (*this); };
	CChannel operator^= (const CChannel& chnl) { *this = *this ^ chnl; return (*this); };

	CChannel& operator& (const uvar32_64& val) { return (And (val)); };
	CChannel& operator| (const uvar32_64& val) { return (Or (val)); };
	CChannel& operator^ (const uvar32_64& val) { return (Xor (val)); };
	CChannel operator&= (const uvar32_64& val) { *this = *this & val; return (*this); };
	CChannel operator|= (const uvar32_64& val) { *this = *this | val; return (*this); };
	CChannel operator^= (const uvar32_64& val) { *this = *this ^ val; return (*this); };

	CChannel& Add (const CChannel& chnl, ubyte nType = 0) throw (CaImAPIException);
	CChannel& Sub (const CChannel& chnl, ubyte nType = 0) throw (CaImAPIException);
	CChannel& Mul (const CChannel& chnl, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CChannel& Div (const CChannel& chnl, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CChannel& Mod (const CChannel& chnl, ubyte nType = 0) throw (CaImAPIException);
	CChannel& Add (const uvar32_64& val, ubyte nType = 0) throw (CaImAPIException);
	CChannel& Sub (const uvar32_64& val, ubyte nType = 0) throw (CaImAPIException);
	CChannel& Mul (const uvar32_64& val, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CChannel& Div (const uvar32_64& val, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CChannel& Mod (const uvar32_64& val, ubyte nType = 0) throw (CaImAPIException);

	CChannel& Not () throw (CaImAPIException);
	CChannel& And (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Or  (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Xor (const CChannel& chnl) throw (CaImAPIException);
	CChannel& And (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Or  (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Xor (const uvar32_64& val) throw (CaImAPIException);
	
	CChannel& Min (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Max (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Avg (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Gvg (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Hvg (const CChannel& chnl) throw (CaImAPIException);
	CChannel& Min (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Max (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Avg (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Gvg (const uvar32_64& val) throw (CaImAPIException);
	CChannel& Hvg (const uvar32_64& val) throw (CaImAPIException);

//	CChannel& Statistics (DiasChannelStat& stat);
	CChannel& Normalize () throw (CaImAPIException);
//	CChannel& Linearize () throw (CaImAPIException);
//	CChannel& Scale (...) throw (CaImAPIException);

	CChannel& Segment (uvar32_64 nThreshold, uvar32_64 nLowValue, uvar32_64 nHighValue);
//	CChannel& Segment (uvar32_64* pnThresholds, uvar32_64* pnValues, uvar32_64 dwCount);
	CChannel& ErodeSegs (uvar32_64 nSegment, Parameter nType, uint nRepeat = 1);
//	CChannel& DilateSegs (uvar32_64 nSegment, Parameter nType, uint nRepeat = 1);
//	CChannel& OpenSegs (uvar32_64 nSegment, Parameter nType, uint nRepeat = 1);
//	CChannel& CloseSegs (uvar32_64 nSegment, Parameter nType, uint nRepeat = 1);
//	CChannel& FillHoles (uvar32_64 nThreshold);
//	CChannel& ExtractHoles (uvar32_64 nThreshold);
//	CChannel& FilterSegs (uvar32_64 nMinSize, uvar32_64 nMaxSize, Parameter nType);
//	CChannel& FilterSegs (CChannel& chnlMask, ubyte nType, Parameter nType);
//	CChannel& Segregate (uvar32_64 nThreshold);
//	CChannel& Segregate (uvar32_64 nThreshold, CMark& markBoundary, Parameter nType);

	CChannel& Laplacian (uvar16_32 nX, uvar16_32 nY);
	CChannel& Median (uvar16_32 nX, uvar16_32 nY);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannel)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNEL_H__F84FDF59_6664_4451_86DE_2756F54C8269__INCLUDED_)
