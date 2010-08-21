#pragma once


class CMarkStyle : public CObject {
friend class CHash<CMarkStyle*,CMarkStyle>;
friend class CaImAPICollection<CMarkStyle>;
friend class CaImAPICollectionExt<CMarkStyle>;
friend class CDiasAPIMarkStyles;
	DECLARE_SERIAL (CMarkStyle);
public:
	CMarkStyle ();
	CMarkStyle (CString name, CString descr, CMark& mark);
	CMarkStyle (const CMarkStyle& scale);
	~CMarkStyle ();
	
	CMarkStyle& operator= (const CMarkStyle& scale);

protected:
	CString		m_strName;
	CString		m_strDescr;
	CMark		m_mrkStyle;

public:
	uvar32_64	GetPos ();
	CString		GetName ();
	void		SetName (CString name);
	CString		GetDescr ();
	void		SetDescr (CString descr);
	CMark&		GetStyle ();
	void		SetStyle (CMark mark);
	void		Activate ();

public:
	void Serialize (CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////
// CCoinStyle

enum CoinType { coinEllipse = 0, coinRect = 1, coinBitmap = 2, coinRandom = 3 };

class CCoinStyle : public CObject {
DECLARE_SERIAL (CCoinStyle);

public:
	CCoinStyle ();
	CCoinStyle (uword radius, uword angle = 0, CString name = "", CString descr = "");
	CCoinStyle (const CCoinStyle& coin);
	~CCoinStyle ();

	CCoinStyle& operator= (const CCoinStyle& coin);

protected:
	// Stored values
	CString		m_strName;
	CString		m_strDescr;
	CoinType	m_nType;
	uword		m_nSizeX;
	uword		m_nSizeY;
	uword		m_nAngle;

	// Non-stored values
	uvar32_64	m_nContentLen;
	uvar32_64	m_nOutlineLen;
	uvar32_64*	m_pContent;
	uvar32_64*	m_pOutline;

	uvar32_64	m_nShapeLen;
	CPoint*		m_pptShape;
	CPen		m_penShape;

public:
//	uvar32_64	GetPos ();
	CString		GetName ();
	void		SetName (CString name);
	CString		GetDescr ();
	void		SetDescr (CString descr);
	CSize		GetSize ();
	void		SetSize (uword x, uword y);
	uword		GetAngle ();
	void		SetAngle (uword angle);

	void		Paint (CDC& dc, CPoint pos, int zoom);
	bool		GetOutline (uvar32_64*& array, uvar32_64& len, CPoint pnt, CSize size);
	bool		GetContent (uvar32_64*& array, uvar32_64& len, CPoint pnt, CSize size);

	void		Serialize (CArchive& ar);
};
