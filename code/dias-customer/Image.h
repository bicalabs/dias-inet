#if !defined(AFX_IMAGE_H__2646CB6B_BCEF_4CDB_A6B0_CF7DEAEB4B26__INCLUDED_)
#define AFX_IMAGE_H__2646CB6B_BCEF_4CDB_A6B0_CF7DEAEB4B26__INCLUDED_

#pragma once

#include "DiasAPIChannels.h"
#include "DiasAPIMarks.h"
#include "DiasAPIPatches.h"

class CPicture : public CObject {
friend class CHash<CPicture*,CPicture>;
friend class CaImAPIRange<CPicture>;
friend class CaImAPICollection<CPicture>;
friend class CaImAPICollectionExt<CPicture>;
friend class CDiasAPIImgRange;
friend class CDiasAPIImages;
friend class CStorageDoc;
friend class CPatch;
// Construction
	DECLARE_SERIAL (CPicture);
protected:
	CPicture ();
	CPicture (const CPicture& img);
	CPicture (CStorageDoc* pDoc, CString name, CString descr);
public:
	~CPicture ();
	CPicture& operator= (const CPicture&) throw (CaImAPIException);

	CPicture operator+ (const CPicture& img) { return (Add (img)); };
	CPicture operator- (const CPicture& img) { return (Sub (img)); };
	CPicture operator* (const CPicture& img) { return (Mul (img)); };
	CPicture operator/ (const CPicture& img) { return (Div (img)); };
	CPicture operator% (const CPicture& img) { return (Mod (img)); };
	CPicture operator+= (const CPicture& img) { *this = *this + img; return (*this); };
	CPicture operator-= (const CPicture& img) { *this = *this - img; return (*this); };
	CPicture operator*= (const CPicture& img) { *this = *this * img; return (*this); };
	CPicture operator/= (const CPicture& img) { *this = *this / img; return (*this); };
	CPicture operator%= (const CPicture& img) { *this = *this % img; return (*this); };

	CPicture operator+ (const CColor& val) { return (Add (val)); };
	CPicture operator- (const CColor& val) { return (Sub (val)); };
	CPicture operator* (const CColor& val) { return (Mul (val)); };
	CPicture operator/ (const CColor& val) { return (Div (val)); };
	CPicture operator% (const CColor& val) { return (Mod (val)); };
	CPicture operator+= (const CColor& val) { *this = *this + val; return (*this); };
	CPicture operator-= (const CColor& val) { *this = *this - val; return (*this); };
	CPicture operator*= (const CColor& val) { *this = *this * val; return (*this); };
	CPicture operator/= (const CColor& val) { *this = *this / val; return (*this); };
	CPicture operator%= (const CColor& val) { *this = *this % val; return (*this); };

	CPicture& operator~ () { return (Not ()); };
	CPicture& operator& (const CPicture& img) { return (And (img)); };
	CPicture& operator| (const CPicture& img) { return (Or (img)); };
	CPicture operator^ (const CPicture& img) { return (Xor (img)); };
	CPicture operator&= (const CPicture& img) { *this = *this & img; return (*this); };
	CPicture operator|= (const CPicture& img) { *this = *this | img; return (*this); };
	CPicture operator^= (const CPicture& img) { *this = *this ^ img; return (*this); };

	CPicture& operator& (const CColor& val) { return (And (val)); };
	CPicture& operator| (const CColor& val) { return (Or (val)); };
	CPicture& operator^ (const CColor& val) { return (Xor (val)); };
	CPicture operator&= (const CColor& val) { *this = *this & val; return (*this); };
	CPicture operator|= (const CColor& val) { *this = *this | val; return (*this); };
	CPicture operator^= (const CColor& val) { *this = *this ^ val; return (*this); };

	CPicture& Add (const CPicture& img, ubyte nType = 0) throw (CaImAPIException);
	CPicture& Sub (const CPicture& img, ubyte nType = 0) throw (CaImAPIException);
	CPicture& Mul (const CPicture& img, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CPicture& Div (const CPicture& img, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CPicture& Mod (const CPicture& img, ubyte nType = 0) throw (CaImAPIException);
	CPicture& Add (const CColor& col, ubyte nType = 0) throw (CaImAPIException);
	CPicture& Sub (const CColor& col, ubyte nType = 0) throw (CaImAPIException);
	CPicture& Mul (const CColor& col, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CPicture& Div (const CColor& col, ubyte nType = 0, uvar32_64 nCoef = 1) throw (CaImAPIException);
	CPicture& Mod (const CColor& col, ubyte nType = 0) throw (CaImAPIException);

	CPicture& Not () throw (CaImAPIException);
	CPicture& And (const CPicture& img) throw (CaImAPIException);
	CPicture& Or  (const CPicture& img) throw (CaImAPIException);
	CPicture& Xor (const CPicture& img) throw (CaImAPIException);
	CPicture& And (const CColor& col) throw (CaImAPIException);
	CPicture& Or  (const CColor& col) throw (CaImAPIException);
	CPicture& Xor (const CColor& col) throw (CaImAPIException);

	CPicture& Min (const CPicture& img) throw (CaImAPIException);
	CPicture& Max (const CPicture& img) throw (CaImAPIException);
	CPicture& Avg (const CPicture& img) throw (CaImAPIException);
	CPicture& Gvg (const CPicture& img) throw (CaImAPIException);
	CPicture& Hvg (const CPicture& img) throw (CaImAPIException);
	CPicture& Min (const CColor& col) throw (CaImAPIException);
	CPicture& Max (const CColor& col) throw (CaImAPIException);
	CPicture& Avg (const CColor& col) throw (CaImAPIException);
	CPicture& Gvg (const CColor& col) throw (CaImAPIException);
	CPicture& Hvg (const CColor& col) throw (CaImAPIException);

//	CPicture& Statistics (DiasChannelStat& stat);
	CPicture& Normalize () throw (CaImAPIException);
//	CPicture& Linearize () throw (CaImAPIException);
//	CPicture& Scale (...) throw (CaImAPIException);

	CPicture& Segment (CColor nThreshold, CColor nLowValue, CColor nHighValue);
//	CPicture& Segment (CColor* pnThresholds, CColor* pnValues, CColor dwCount);
	CPicture& ErodeSegs (CColor nSegment, Parameter nType, uint nRepeat = 1);
//	CPicture& DilateSegs (CColor nSegment, Parameter nType, uint nRepeat = 1);
//	CPicture& OpenSegs (CColor nSegment, Parameter nType, uint nRepeat = 1);
//	CPicture& CloseSegs (CColor nSegment, Parameter nType, uint nRepeat = 1);
//	CPicture& FillHoles (CColor nThreshold);
//	CPicture& ExtractHoles (CColor nThreshold);
//	CPicture& FilterSegs (CColor nMinSize, CColor nMaxSize, Parameter nType);
//	CPicture& FilterSegs (CPicture& chnlMask, ubyte nType, Parameter nType);
//	CPicture& Segregate (CColor nThreshold);
//	CPicture& Segregate (CColor nThreshold, CMark& markBoundary, Parameter nType);

	CPicture& Laplacian (uvar16_32 nX, uvar16_32 nY);
	CPicture& Median (uvar16_32 nX, uvar16_32 nY);

// Attributes
public:
	CDiasAPIChannels	Channels;
	CDiasAPIMarks		Marks;
	CDiasAPIPatches		Patches;

protected:
	CStorageDoc*	m_pDoc;

	CString		m_strName;
	CString		m_strDescript;
	ColorModel*	m_pnColorModel;
	ubyte*		m_pnBPP;
	uvar16_32*	m_pnDimX;
	uvar16_32*	m_pnDimY;

// Operations
public:
	CStorageDoc&	GetDoc () { return (*m_pDoc); };
	svar32_64	GetPos ();
	CString		GetName ();
	CString		GetDescription ();
	void		SetPos (uvar32_64 before);
	void		SetName (CString name);
	void		SetDescription (CString descr);

	CColor		GetPixel (uvar16_32 nX, uvar16_32 nY);
	udword		GetPixel (uvar32_64 nOffset);
	void		SetPixel (uvar16_32 nX, uvar16_32 nY, CColor& col);
	void		SetPixel (uvar32_64 nOffset, CColor& col);
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicture)
public:
	void Serialize (CArchive& ar);
	//}}AFX_VIRTUAL
};
/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE_H__2646CB6B_BCEF_4CDB_A6B0_CF7DEAEB4B26__INCLUDED_)
