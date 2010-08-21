#include "stdafx.h"
#include "Dias.h"


/////////////////////////////////////////////////////////////////////////////
// CMarkStyle

IMPLEMENT_SERIAL (CMarkStyle, CObject, 1);

CMarkStyle::CMarkStyle () {
	;
}
CMarkStyle::CMarkStyle (CString name, CString descr, CMark& mark) :
	m_strName (name), m_strDescr (descr), m_mrkStyle (mark) {
	;
}

CMarkStyle::CMarkStyle (const CMarkStyle& scale) {
	*this = scale;
}
CMarkStyle::~CMarkStyle () {
	;
}

CMarkStyle& CMarkStyle::operator= (const CMarkStyle& scale) {
	m_strName = scale.m_strName;
	m_strDescr = scale.m_strDescr;
	m_mrkStyle = scale.m_mrkStyle;
	return (*this);
}

uvar32_64 CMarkStyle::GetPos () {
	for (CHash<CMarkStyle*,CMarkStyle>* p = theApp.MarkStyles.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			if (theApp.MarkStyles.m_nActive == p->GetPos ())
				return (p->GetPos ());
	return (uvar32_64(-1));
}

CString CMarkStyle::GetName () {
	return (m_strName);
}
void CMarkStyle::SetName (CString name) {
	if (theApp.MarkStyles.NameLookup (name) != (uvar32_64)-1)
		return;
	m_strName = name;
	theApp.Update (UPDHA_MARKSTYLE);
}
CString CMarkStyle::GetDescr () {
	return (m_strDescr);
}
void CMarkStyle::SetDescr (CString descr) {
	m_strDescr = descr;
	theApp.Update (UPDHA_MARKSTYLE);
}
CMark& CMarkStyle::GetStyle () {
	return (m_mrkStyle);
}
void CMarkStyle::SetStyle (CMark mark) {
	m_mrkStyle = mark;
	theApp.Update (UPDHA_MARKSTYLE);
}

void CMarkStyle::Activate () {
	for (CHash<CMarkStyle*,CMarkStyle>* p = theApp.MarkStyles.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			theApp.MarkStyles.m_nActive = p->GetPos ();
	theApp.Update (UPDHA_MARKSTYLE);
}

void CMarkStyle::Serialize (CArchive &ar) {
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescr;
		m_mrkStyle.Serialize (ar);
	} else {
		ar >> m_strName >> m_strDescr;
		m_mrkStyle.Serialize (ar);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCoinStyle

IMPLEMENT_SERIAL(CCoinStyle, CObject, 1);

CCoinStyle::CCoinStyle () {
	m_pContent = NULL;
	m_pOutline = NULL;
	m_nContentLen = 0;
	m_nOutlineLen = 0;
	m_nSizeX = 0;
	m_nSizeY = 0;
	m_nAngle = 0;
	m_nType = coinEllipse;

	m_penShape.CreatePen (PS_SOLID, 1, 0xFFFFFF);
	m_pptShape = NULL;
	m_nShapeLen = 0;
}

CCoinStyle::CCoinStyle (uword radius, uword angle, CString name, CString descr) :
	m_strName (name), m_strDescr (descr) {
	m_pContent = NULL;
	m_pOutline = NULL;
	m_nContentLen = 0;
	m_nOutlineLen = 0;
	m_nSizeX = 0;
	m_nSizeY = 0;
	m_nAngle = angle;
	m_nType = coinEllipse;

	m_penShape.CreatePen (PS_SOLID, 1, 0xFFFFFF);
	m_pptShape = NULL;
	m_nShapeLen = 0;
	SetSize (radius * 2 + 1, radius * 2 + 1);
}

CCoinStyle::CCoinStyle (const CCoinStyle& coin) {
	m_pContent = NULL;
	m_pOutline = NULL;
	m_nContentLen = 0;
	m_nOutlineLen = 0;
	m_nSizeX = 0;
	m_nSizeY = 0;
	m_nAngle = 0;

	m_penShape.CreatePen (PS_SOLID, 1, 0xFFFFFF);
	m_pptShape = NULL;
	m_nShapeLen = 0;
	*this = coin;
}

CCoinStyle::~CCoinStyle () {
	if (m_pContent)
		aimMemoryRelease (m_pContent, "CCoinStyle::~CCoinStyle");
	if (m_pOutline)
		aimMemoryRelease (m_pOutline, "CCoinStyle::~CCoinStyle");
	if (m_pptShape)
		aimMemoryRelease (m_pptShape, "CCoinStyle::~CCoinStyle");
}

CCoinStyle& CCoinStyle::operator= (const CCoinStyle& coin) {
	m_strName = coin.m_strName;
	m_strDescr = coin.m_strDescr;
	m_nSizeX = coin.m_nSizeX;
	m_nSizeY = coin.m_nSizeY;
	m_nAngle = coin.m_nAngle;
	m_nType = coin.m_nType;

	if (m_pContent) {
		aimMemoryRelease (m_pContent, "CCoinStyle::operator=");
		m_pContent = NULL;
	}
	if (m_pOutline) {
		aimMemoryRelease (m_pContent, "CCoinStyle::operator=");
		m_pOutline = NULL;
	}

	m_nContentLen = coin.m_nContentLen;
	m_nOutlineLen = coin.m_nOutlineLen;

	if (coin.m_pContent) {
		m_pContent = (uvar32_64*)aimMemoryCommit (m_nContentLen * sizeof (uvar32_64), "CCoinStyle::operator=");
		memcpy (m_pContent, coin.m_pContent, m_nContentLen * sizeof (uvar32_64));
	}
	if (coin.m_pOutline) {
		m_pOutline = (uvar32_64*)aimMemoryCommit (m_nOutlineLen * sizeof (uvar32_64), "CCoinStyle::operator=");
		memcpy (m_pOutline, coin.m_pOutline, m_nOutlineLen * sizeof (uvar32_64));
	}

	LOGPEN lp;
	CPen* ppen = (CPen*)&(coin.m_penShape);
	ppen->GetLogPen (&lp);
	m_penShape.CreatePenIndirect (&lp);

	m_nShapeLen = coin.m_nShapeLen;
	m_pptShape = (CPoint *)aimMemoryCommit (m_nShapeLen * sizeof (CPoint), "CCoinStyle::operator=");
	memcpy (m_pptShape, coin.m_pptShape, m_nShapeLen * sizeof (CPoint));
	return (*this);
}

/*uvar32_64 CCoinStyle::GetPos () {
	for (CHash<CCoinStyle*,CCoinStyle>* p = Application.CoinStyles.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (uvar32_64(-1));
}*/

CString CCoinStyle::GetName () {
	return (m_strName);
}

void CCoinStyle::SetName (CString name) {
//???	if (Application.FieldSets.NameLookup (name) != (uvar32_64)-1)
//???		return;
	m_strName = name;
//???	Application.Update (UPDHA_FIELDSET);
}

CString CCoinStyle::GetDescr () {
	return (m_strName);
}

void CCoinStyle::SetDescr (CString descr) {
	m_strDescr = descr;
//???	Application.Update (UPDHA_FIELDSET);
}

CSize CCoinStyle::GetSize () {
	return (CSize (m_nSizeX, m_nSizeY));
}

void CCoinStyle::SetSize (uword x, uword y) {
	if (x == m_nSizeX && y == m_nSizeY)
		return;

	m_nSizeX = x;
	m_nSizeY = y;

	if (m_pContent) {
		aimMemoryRelease (m_pContent, "CCoinStyle::SetSize");
		m_pContent = NULL;
		m_nContentLen = 0;
	}
	if (m_pOutline) {
		aimMemoryRelease (m_pContent, "CCoinStyle::SetSize");
		m_pOutline = NULL;
		m_nOutlineLen = 0;
	}
	if (m_pptShape) {
		aimMemoryRelease (m_pptShape, "CCoinStyle::SetSize");
		m_pptShape = NULL;
		m_nShapeLen = 0;
	}
}

uword CCoinStyle::GetAngle () {
	return (m_nAngle);
}

void CCoinStyle::SetAngle (uword angle) {
	if (angle == m_nAngle)
		return;

	m_nAngle = angle;

	if (m_pContent) {
		aimMemoryRelease (m_pContent, "CCoinStyle::SetSize");
		m_pContent = NULL;
		m_nContentLen = 0;
	}
	if (m_pOutline) {
		aimMemoryRelease (m_pContent, "CCoinStyle::SetSize");
		m_pOutline = NULL;
		m_nOutlineLen = 0;
	}
	if (m_pptShape) {
		aimMemoryRelease (m_pptShape, "CCoinStyle::SetSize");
		m_pptShape = NULL;
		m_nShapeLen = 0;
	}
}

void CCoinStyle::Paint (CDC& dc, CPoint pos, int zoom) {
	uvar32_64 dx = (m_nSizeX - 1) / 2 * zoom / 100;
	uvar32_64 dy = (m_nSizeY - 1) / 2 * zoom / 100;
	uvar32_64 dx2 = dx * dx, dy2 = dy * dy;

	CRect rect (-dx, -dy, dx, dy);
	rect.OffsetRect (pos);

	switch (m_nType) {
		case coinEllipse:
			if (m_nShapeLen != dx * 4 + 1 && m_pptShape) {
				aimMemoryRelease (m_pptShape);
				m_pptShape = NULL;
			}
			m_nShapeLen = dx * 4 + 1;
			if (!m_pptShape)
				m_pptShape = (CPoint *)aimMemoryCommit (m_nShapeLen * sizeof (CPoint), "CCoinStyle::Paint");

			double s;
			rtbyte	*px = (rtbyte *)aimMemoryCommit ((m_nShapeLen + 2) * sizeof (rtbyte), "CCoinStyle::Paint"),
				*py = (rtbyte *)aimMemoryCommit ((m_nShapeLen + 2) * sizeof (rtbyte), "CCoinStyle::Paint");

			for (int i = -signed(dx); i <= signed(dx); ++i) {
				s = double(dy) / double(dx) * pow (dx2 - i * i, 0.5);
				px[dx + i] = pos.x + i;
				px[dx*3 - i] = pos.x + i;
				py[dx + i] = pos.y + s;
				py[dx*3 - i] = pos.y - s;
			}
			aimRotate2D (m_nShapeLen, px, py, m_nAngle * 3.1415 / 180., pos);
			for (i = 0; i < m_nShapeLen; ++i)
				m_pptShape[i] = CPoint ((px[i] - int(px[i]) < 0.5) ? int(px[i]) : int (px[i] + 0.5),
							(py[i] - int(py[i]) < 0.5) ? int(py[i]) : int (py[i] + 0.5));

			aimMemoryRelease (px, "CCoinStyle::Paint");
			aimMemoryRelease (py, "CCoinStyle::Paint");
			break;
	}

	CPen *ppenOld = dc.SelectObject (&m_penShape);
	int rop2 = dc.GetROP2 ();
	dc.SetROP2 (R2_MASKPENNOT);
	dc.Polyline (m_pptShape, m_nShapeLen);
	dc.SelectObject (ppenOld);
	dc.SetROP2 (rop2);
}

bool CCoinStyle::GetOutline (uvar32_64*& result, uvar32_64& len, CPoint pnt, CSize size) {
	rtbyte x, y;
	uvar32_64 dx = (m_nSizeX - 1) / 2, dx2 = dx * dx;
	uvar32_64 dy = (m_nSizeY - 1) / 2, dy2 = dy * dy;

	uvar32_64 alen = m_nSizeX * m_nSizeY, pa = 0;
	uvar32_64* array = (uvar32_64 *)aimMemoryCommit (alen * sizeof (uvar32_64), "CCoinStyle::GetOutline");

	switch (m_nType) {
		case coinEllipse:
			for (y = -rtbyte(dy); y <= rtbyte(dy); ++y)
				for (x = -rtbyte(dx); x <= rtbyte(dx); ++x)
					if (x * x / dx2 + y * y / dy2 >= 0.95 &&
					    x * x / dx2 + y * y / dy2 <= 1.05 &&
					    y + pnt.y >= 0 && y + pnt.y < size.cy &&
					    x + pnt.x >= 0 && x + pnt.x < size.cx)
						array[pa++] = (y + pnt.y) * size.cx + x + pnt.x;
			break;
	}

	result = (uvar32_64 *)aimMemoryCommit (pa * sizeof (uvar32_64), "CCoinStyle::GetOutline");
	aimRotate2D (pa, array, m_nAngle * 3.1415 / 180., pnt, size);
	memcpy (result, array, (len = pa) * sizeof (uvar32_64));
	aimMemoryRelease (array, "CCoinStyle::GetOutline");
	return (true);
}

bool CCoinStyle::GetContent (uvar32_64*& result, uvar32_64& len, CPoint pnt, CSize size) {
	rtbyte x, y;
	uvar32_64 dx = (m_nSizeX - 1) / 2, dx2 = dx * dx;
	uvar32_64 dy = (m_nSizeY - 1) / 2, dy2 = dy * dy;

	uvar32_64 alen = m_nSizeX * m_nSizeY, pa = 0;
	uvar32_64* array = (uvar32_64 *)aimMemoryCommit (alen * sizeof (uvar32_64), "CCoinStyle::GetOutline");

	switch (m_nType) {
		case coinEllipse:
			for (y = -rtbyte(dy); y <= rtbyte(dy); ++y)
				for (x = -rtbyte(dx); x <= rtbyte(dx); ++x)
					if (x * x / dx2 + y * y / dy2 <= 1.0 &&
					    y + pnt.y >= 0 && y + pnt.y < size.cy &&
					    x + pnt.x >= 0 && x + pnt.x < size.cx)
						array[pa++] = (y + pnt.y) * size.cx + x + pnt.x;
			break;
	}

	result = (uvar32_64 *)aimMemoryCommit (pa * sizeof (uvar32_64), "CCoinStyle::GetOutline");
	aimRotate2D (pa, array, m_nAngle * 3.1415 / 180., pnt, size);
	memcpy (result, array, (len = pa) * sizeof (uvar32_64));
	aimMemoryRelease (array, "CCoinStyle::GetOutline");
	return (true);
}


void CCoinStyle::Serialize (CArchive &ar) {
	if (ar.IsStoring ()) {
		ar << m_strName << m_strDescr;
		ar << (udword)m_nType;
		ar << m_nSizeX << m_nSizeY;
	} else {
		ar >> m_strName >> m_strDescr;
		ar >> (udword&)m_nType;
		ar >> m_nSizeX >> m_nSizeY;
	}
}

/*
	uvar32_64 *array = aimMemoryCommit (m_nSizeX * m_nSizeY * sizeof (uvar32_64), "CCoinStyle::CCoinStyle");
	for (uword x = 
*/