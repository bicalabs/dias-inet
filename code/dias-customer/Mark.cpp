#include "stdafx.h"
#include "Dias.h"
#include "ImageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMark

IMPLEMENT_SERIAL (CMark, CObject, 1);

CMark::CMark () {
	m_pImg = NULL;
	m_pptPoints = NULL;
	CreateControls ();

	theApp.ObjectsAdd (OBJT_MARK, this);
}

CMark::~CMark () {
	theApp.ObjectsRemove (OBJT_MARK, this);

	m_pImg = NULL;
	if (m_pptPoints)
		delete [] m_pptPoints;
}

CMark::CMark (const CMark& mrk) {
	m_pImg = NULL;
	m_pptPoints = NULL;
	*this = mrk;
	CreateControls ();
}

CMark& CMark::operator= (const CMark& src) {
	if (!m_pImg)	m_pImg = src.m_pImg;
	m_iPosX = src.m_iPosX;
	m_iPosY = src.m_iPosY;
	m_blSelected = src.m_blSelected;
	m_blVersion = src.m_blVersion;
	m_nKernel = src.m_nKernel;
	m_nKrnlSize = src.m_nKrnlSize;
	m_dwKrnlColor = src.m_dwKrnlColor;
	m_nShape = src.m_nShape;
	m_nShapeSizeX = src.m_nShapeSizeX;
	m_nShapeSizeY = src.m_nShapeSizeY;
	m_nShapeAngle = src.m_nShapeAngle;
	m_blShapeSizeLock = src.m_blShapeSizeLock;
	m_blShapeAngleLock = src.m_blShapeAngleLock;
	m_dwShapeColor = src.m_dwShapeColor;
	m_dwShapeFill = src.m_dwShapeFill;

	theApp.ObjectsAdd (OBJT_MARK, this);
	return (*this);
}

CMark::CMark (CPicture* pImg, svar16_32 x, svar16_32 y, const CMark& deflt) {
	*this = CMark (pImg, CPoint (x, y), deflt);
}

CMark::CMark (CPicture* pImg, CPoint& pos, const CMark& deflt) {
	m_pImg = pImg;
	m_pptPoints = NULL;
	*this = deflt;
	m_iPosX = pos.x;
	m_iPosY = pos.y;
	CreateControls ();
}

CMark::CMark (CPicture *pImg, CPoint& pos, MarkKernel krnl, uvar16_32 ksize, COLORREF kc, 
	      MarkShape shp, CSize& ssize, ubyte angle, bool slock, bool alock, COLORREF sc, COLORREF fc) {
	m_pImg = pImg;
	m_iPosX = pos.x;
	m_iPosY = pos.y;
	m_blSelected = false;
	m_blVersion = true;
	m_nKernel = krnl;
	m_nKrnlSize = ksize;
	m_dwKrnlColor = kc;
	m_nShape = shp;
	m_nShapeSizeX = ssize.cx;
	m_nShapeSizeY = ssize.cy;
	m_nShapeAngle = angle;
	m_blShapeSizeLock = slock;
	m_blShapeAngleLock = alock;
	m_dwShapeColor = sc;
	m_dwShapeFill = fc;

	m_pptPoints = NULL;
	CreateControls ();

	theApp.ObjectsAdd (OBJT_MARK, this);
}

void CMark::Serialize (CArchive& ar) {
	CObject::Serialize (ar);
	if (ar.IsStoring ()) {
		ar << m_iPosX << m_iPosY;
		ar << uchar(m_blSelected) << uchar(m_blVersion);
		ar << (uchar) (m_nKernel);
		ar << (uchar) (m_nShape);
		ar << m_nKrnlSize << m_nShapeSizeX << m_nShapeSizeY << (uword)m_nShapeAngle;
		ar << m_dwKrnlColor << m_dwShapeColor << m_dwShapeFill;
		ar << uchar(m_blShapeSizeLock) << uchar(m_blShapeAngleLock);
	} else {
		m_blSelected = m_blVersion = false;
		m_blShapeSizeLock = m_blShapeAngleLock = false;
		ar >> m_iPosX >> m_iPosY;
		ar >> (uchar&)m_blSelected >> (uchar&)m_blVersion;
		(int&)(m_nKernel) = 0;
		(int&)(m_nShape) = 0;
		ar >> (uchar&) (m_nKernel);
		ar >> (uchar&) (m_nShape);
		ar >> m_nKrnlSize >> m_nShapeSizeX >> m_nShapeSizeY >> (uword&)m_nShapeAngle;
		ar >> m_dwKrnlColor >> m_dwShapeColor >> m_dwShapeFill;
		ar >> (uchar&)m_blShapeSizeLock >> (uchar&)m_blShapeAngleLock;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMark message handlers

svar32_64 CMark::GetPos () {
	if (!m_pImg)
		return (-1);
	m_pImg->Marks.m_pHashHead = m_pImg->Marks.m_pHashHead->GetHead ();
	for (CHash<CMark*,CMark>* p = m_pImg->Marks.m_pHashHead; p; p = p->GetNext ())
		if (p->m_hData == this)
			return (p->GetPos ());
	return (-1);
}

void CMark::Show (bool blShow) {
	m_blVersion = blShow;
	m_pImg->GetDoc().Update (CUpdateHint::updtImage, UPDHI_MARKERS, m_pImg->GetPos (), GetPos ());
}

void CMark::Select (bool blSelect) {
	m_blSelected = blSelect;
	m_pImg->GetDoc().Update (CUpdateHint::updtImage, UPDHI_MARKERS, m_pImg->GetPos (), GetPos ());
	theApp.Update (UPDHA_MARK);
}

CMark::MarkPoint CMark::GetPointType (CPoint point) {
	CRect rect (-3, -3, 3, 3), rect2;
	rect2 = rect;
	rect2.OffsetRect (m_iPosX - 2, m_iPosY - 2);
	if (rect2.PtInRect (point))
		return (pntCenter);
	rect2 = rect;
	rect2.OffsetRect (m_pntAxisX);
	if (rect2.PtInRect (point))
		return (pntAxisX);
	rect2 = rect;
	rect2.OffsetRect (m_pntAxisY);
	if (rect2.PtInRect (point))
		return (pntAxisY);
	rect2 = rect;
	rect2.OffsetRect (m_pntAxisXY);
	if (rect2.PtInRect (point))
		return (pntAxisXY);
	return (pntNone);
}

#define UPDATEMRK	if (!m_pImg) return; m_pImg->GetDoc().Update (CUpdateHint::updtImage, UPDHI_MARKERS, m_pImg->GetPos (), GetPos ())
#define UPDATEIMG	if (!m_pImg) return; m_pImg->GetDoc().Update (CUpdateHint::updtImage, UPDHI_IMAGE)
void CMark::SetKernel (MarkKernel krnl) { m_nKernel = krnl; UPDATEIMG; }
void CMark::SetShape (MarkShape shape) { m_nShape = shape; UPDATEIMG; }
void CMark::SetKernelSize (uvar16_32 size) { m_nKrnlSize = size; UPDATEIMG; }
void CMark::SetShapeSize (CSize size) { m_nShapeSizeX = size.cx; m_nShapeSizeY = size.cy; UPDATEIMG; }
void CMark::SetPoint (CPoint point) { m_iPosX = point.x; m_iPosY = point.y; UPDATEIMG; }
void CMark::SetAngle (sword angle) { m_nShapeAngle = angle; UPDATEIMG; }
void CMark::SetSizeLock (bool lock) { m_blShapeSizeLock = lock; UPDATEMRK; }
void CMark::SetAngleLock (bool lock) { m_blShapeAngleLock = lock; UPDATEMRK; }
void CMark::SetKernelColor (COLORREF color) { m_dwKrnlColor = color; UPDATEMRK; }
void CMark::SetShapeColor (COLORREF color) { m_dwShapeColor = color; UPDATEMRK; }
void CMark::SetFillColor (COLORREF color) { m_dwShapeFill = color; UPDATEMRK; }
#undef UPDATEIMG
#undef UPDATEMRK

void CMark::Paint (CDC& dc) {
	PaintFill (dc);
	PaintKernel (dc);
	PaintShape (dc);
//	PaintControls (dc);
}

void CMark::PaintKernel (CDC& dc) {
	int x = m_iPosX;
	int y = m_iPosY;

	CPen   penKernel (PS_SOLID, 2, m_dwKrnlColor/* ^ (0xFFFFFF * m_blSelected)*/);
	CPen   *penOld = dc.SelectObject (&penKernel);
	CBrush brushKernel (m_dwKrnlColor/* ^ (0xFFFFFF * m_blSelected)*/);
	CBrush *brushOld = dc.SelectObject (&brushKernel);

	switch (m_nKernel) {
		case krnlRound:
			dc.SelectObject ((CPen*)NULL);
			dc.Ellipse (x - m_nKrnlSize, y - m_nKrnlSize,
			            x + m_nKrnlSize, y + m_nKrnlSize);
			dc.SelectObject (penKernel);
			break;
		case krnlX:
			dc.MoveTo (x - m_nKrnlSize, y - m_nKrnlSize);
			dc.LineTo (x + m_nKrnlSize + 1, y + m_nKrnlSize);
			dc.MoveTo (x - m_nKrnlSize, y + m_nKrnlSize);
			dc.LineTo (x + m_nKrnlSize + 1, y - m_nKrnlSize);
			break;
		case krnlPlus:
			dc.MoveTo (x, y - m_nKrnlSize);
			dc.LineTo (x, y + m_nKrnlSize);
			dc.MoveTo (x - m_nKrnlSize, y);
			dc.LineTo (x + m_nKrnlSize, y);
			break;
	}

	dc.SelectObject (penOld);
	dc.SelectObject (brushOld);
}

void CMark::PaintShape (CDC& dc) {
	CPen	penShape (PS_SOLID, 1, m_dwShapeColor/* ^ (0xFFFFFF * m_blSelected)*/);
	CPen	*penOld = dc.SelectObject (&penShape);
	CBrush	brushShape (m_dwShapeColor/* ^ (0xFFFFFF * m_blSelected)*/);
	CBrush	*brushOld = dc.SelectObject (&brushShape);

	CreateControls ();
	switch (m_nShape) {
		case shpEllipse:
			dc.Polyline (m_pptPoints, m_nShapeSizeX * 4 + 1);
			break;
		case shpRect:
			dc.Polyline (m_pptPoints, 5);
			break;
		case shpTriangle:
			dc.Polyline (m_pptPoints, 4);
			break;
	}

	dc.SelectObject (penOld);
	dc.SelectObject (brushOld);
}

void CMark::PaintFill (CDC& dc) {
}

void CMark::CreateControls () {
	int x = m_iPosX, i;
	int y = m_iPosY;
	double s;
	uvar32_64 dx = m_nShapeSizeX, dx2 = dx * dx,
	          dy = m_nShapeSizeY, dy2 = dy * dy;
	rtbyte	*px = new rtbyte[m_nShapeSizeX * 4 + 3],
	        *py = new rtbyte[m_nShapeSizeX * 4 + 3];

	if (m_pptPoints)
		delete [] m_pptPoints;
	m_pptPoints = new CPoint[m_nShapeSizeX * 4 + 1];
	px[0] = m_iPosX + m_nShapeSizeX,
	py[0] = m_iPosY,
	px[1] = m_iPosX,
	py[1] = m_iPosY - m_nShapeSizeY,
	px[2] = px[0],
	py[2] = py[1];
	aimRotate2D (3, px, py, m_nShapeAngle * 3.1415 / 180., CPoint (m_iPosX, m_iPosY));
	m_pntAxisX = CPoint (int(px[0]), int(py[0]));
	m_pntAxisY = CPoint (int(px[1]), int(py[1]));
	m_pntAxisXY = CPoint (int(px[2]), int(py[2]));

	switch (m_nShape) {
		case shpEllipse:
			for (i = -signed(dx); i <= signed(dx); ++i) {
				s = double(dy) / double(dx) * pow (dx2 - i * i, 0.5);
				px[dx + i] = x + i;
				px[dx*3 - i] = x + i;
				py[dx + i] = y + s;
				py[dx*3 - i] = y - s;
			}
			aimRotate2D (dx * 4 + 1, px, py, m_nShapeAngle * 3.1415 / 180., CPoint (x, y));
			for (i = 0; i <= dx * 4; ++i)
				m_pptPoints[i] = CPoint ((px[i] - int(px[i]) < 0.5) ? int(px[i]) : int (px[i] + 0.5),
				                         (py[i] - int(py[i]) < 0.5) ? int(py[i]) : int (py[i] + 0.5));
			break;
		case shpRect:
			px[0] = x - m_nShapeSizeX, py[0] = y - m_nShapeSizeY;
			px[1] = x + m_nShapeSizeX, py[1] = y - m_nShapeSizeY;
			px[2] = x + m_nShapeSizeX, py[2] = y + m_nShapeSizeY;
			px[3] = x - m_nShapeSizeX, py[3] = y + m_nShapeSizeY;
			px[4] = px[0], py[4] = py[0];
			aimRotate2D (5, px, py, m_nShapeAngle * 3.1415 / 180., CPoint (x, y));
			for (i = 0; i < 5; ++i)
				m_pptPoints[i] = CPoint ((px[i] - int(px[i]) < 0.5) ? int(px[i]) : int (px[i] + 0.5),
				                         (py[i] - int(py[i]) < 0.5) ? int(py[i]) : int (py[i] + 0.5));
			break;
		case shpTriangle:
			px[0] = x, py[0] = y - m_nShapeSizeY;
			px[1] = x + m_nShapeSizeX, py[1] = y + m_nShapeSizeY;
			px[2] = x - m_nShapeSizeX, py[2] = y + m_nShapeSizeY;
			px[3] = px[0], py[3] = py[0];
			aimRotate2D (4, px, py, m_nShapeAngle * 3.1415 / 180., CPoint (x, y));
			for (i = 0; i < 4; ++i)
				m_pptPoints[i] = CPoint ((px[i] - int(px[i]) < 0.5) ? int(px[i]) : int (px[i] + 0.5),
				                         (py[i] - int(py[i]) < 0.5) ? int(py[i]) : int (py[i] + 0.5));
			break;
	}

	delete [] px; delete [] py;
}
