// ImageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "StorageDoc.h"
#include "StorageFrame.h"
#include "ImageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd
CPictureWnd::CPictureWnd () {
	m_pFrame = NULL;
	m_pView = NULL;
	m_pDoc = NULL;
	m_pbBits = NULL;
	m_ppenLine = new CPen (PS_SOLID, 3, ::GetSysColor (COLOR_BTNFACE));
	m_ppenBoundary = new CPen (PS_SOLID, 1, 0xFFFFFF);
	m_ppenChord = new CPen (PS_DOT, 1, 0xFFFFFF);
	m_nVScrollPos = 0;
	m_nHScrollPos = 0;

	m_pntOldMouse = CPoint (-10000, -10000);
	m_nCaptureMode = 0;
	m_pMarkActive = NULL;
	m_pCoinActive = new CCoinStyle (16);
}

CPictureWnd::~CPictureWnd () {
	delete m_ppenLine;
	delete m_ppenBoundary;
	delete m_ppenChord;

	if (m_bmpImage.m_hObject)
		m_bmpImage.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPictureWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()

CPoint CPictureWnd::MapPntToScrCoord (const CPoint pnt) {
	CPoint point;
	CRect rectClient;
	GetClientRect (&rectClient);
	float zoom = m_pView->m_nZoomImg / 100.;
	int cx = (rectClient.Width () < m_pDoc->GetDimX() * zoom) ? (m_nHScrollPos) : ((m_pDoc->GetDimX() * zoom - rectClient.Width ())/2);
	int cy = (rectClient.Height () < m_pDoc->GetDimY() * zoom) ? (m_nVScrollPos) : ((m_pDoc->GetDimY() * zoom - rectClient.Height ())/2);
	point.x = pnt.x * zoom - cx;
	point.y = pnt.y * zoom - cy;
	return (point);
}

void CPictureWnd::MapPntToScrCoord (CPoint* ppnt, uvar32_64 cnt) {
	for (uvar32_64 n = 0; n < cnt; ++n)
		ppnt[n] = MapPntToScrCoord (ppnt[n]);
}

void CPictureWnd::PaintActiveMark () {
	if (!m_pMarkActive)
		return;

	CClientDC dcThis (this);
	CPen *ppenOld = dcThis.SelectObject (m_ppenBoundary);
	CRect rect (-3, -3, 3, 3), rect2;

	m_pMarkActive->CreateControls ();
	dcThis.SetROP2 (R2_MASKPENNOT);
	CPoint pntCenter = MapPntToScrCoord (CPoint(m_pMarkActive->m_iPosX, m_pMarkActive->m_iPosY));
	dcThis.DrawIcon (pntCenter.x - 16, pntCenter.y - 16, theApp.LoadIcon (IDI_MRK_CENTER));
	rect2 = rect;
	rect2.OffsetRect (MapPntToScrCoord(m_pMarkActive->m_pntAxisX));
	dcThis.InvertRect (rect2);
	rect2 = rect;
	rect2.OffsetRect (MapPntToScrCoord(m_pMarkActive->m_pntAxisY));
	dcThis.InvertRect (rect2);
	rect2 = rect;
	rect2.OffsetRect (MapPntToScrCoord(m_pMarkActive->m_pntAxisXY));
	dcThis.InvertRect (rect2);
	switch (m_pMarkActive->m_nShape) {
		case CMark::shpEllipse:
			MapPntToScrCoord (m_pMarkActive->m_pptPoints, m_pMarkActive->m_nShapeSizeX * 4 + 1);
			dcThis.Polyline (m_pMarkActive->m_pptPoints, m_pMarkActive->m_nShapeSizeX * 4 + 1);
			break;
		case CMark::shpRect:
			MapPntToScrCoord (m_pMarkActive->m_pptPoints, 5);
			dcThis.Polyline (m_pMarkActive->m_pptPoints, 5);
			break;
		case CMark::shpTriangle:
			MapPntToScrCoord (m_pMarkActive->m_pptPoints, 4);
			dcThis.Polyline (m_pMarkActive->m_pptPoints, 4);
			break;
	}
	dcThis.SelectObject (m_ppenChord);
	dcThis.MoveTo (MapPntToScrCoord(CPoint(m_pMarkActive->m_iPosX, m_pMarkActive->m_iPosY)));
	dcThis.LineTo (MapPntToScrCoord(m_pMarkActive->m_pntAxisX));
	dcThis.MoveTo (MapPntToScrCoord(CPoint(m_pMarkActive->m_iPosX, m_pMarkActive->m_iPosY)));
	dcThis.LineTo (MapPntToScrCoord(m_pMarkActive->m_pntAxisY));
	dcThis.MoveTo (MapPntToScrCoord(CPoint(m_pMarkActive->m_iPosX, m_pMarkActive->m_iPosY)));
	dcThis.LineTo (MapPntToScrCoord(m_pMarkActive->m_pntAxisXY));
	dcThis.SelectObject (ppenOld);
}

CRect CPictureWnd::GetImageRect (int& width, int& height) {
	CRect rectClient, rectImage;
	GetClientRect (&rectClient);

	width = m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100;
	height = m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100;
	rectImage = CRect ((rectClient.Width() < width) ? (-m_nHScrollPos) : ((rectClient.Width() - width)/2),
	                   (rectClient.Height() < height) ? (-m_nVScrollPos) : ((rectClient.Height() - height)/2),
	                   (rectClient.Width() < width) ? (width - m_nHScrollPos) : ((rectClient.Width() + width)/2),
	                   (rectClient.Height() < height) ? (height - m_nVScrollPos) : ((rectClient.Height() + height)/2));
	return (rectImage);
}

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd message handlers

BOOL CPictureWnd::OnSetCursor (CWnd* pWnd, UINT nHitTest, UINT message) {
	int iCursor;
	if (nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	if (this != GetFocus ()) {
		HCURSOR cursor = theApp.LoadStandardCursor (IDC_ARROW);
		SetCursor (cursor);
		return (true);
	}
	switch (m_pView->GetMode ()) {
		case ID_IMAGE_POINTER:		iCursor = IDC_POINTER;		break;
		case ID_IMAGE_HAND:		iCursor = IDC_HAND;		break;
		case ID_IMAGE_SELECT:		iCursor = IDC_POINTER;		break;
		case ID_IMAGE_ZOOM:		iCursor = IDC_ZOOMIN;		break;
		case ID_IMAGE_PATCH:
			if ((GetKeyState (VK_SHIFT) & 0x8000) && (GetKeyState (VK_CONTROL) & 0x8000))
				iCursor = IDC_IOBJCREATE_XOR;
			else if (GetKeyState (VK_CONTROL) & 0x8000)
				iCursor = IDC_IOBJCREATE_EXCL;
			else if (GetKeyState (VK_SHIFT) & 0x8000)
				iCursor = IDC_IOBJCREATE_INCL;
			else
				iCursor = IDC_IOBJCREATE;
			break;
		case ID_IMAGE_MARKER:		iCursor = IDC_MRKCREATE;	break;
		case ID_IMAGE_LABEL:		iCursor = IDC_TEXTCREATE;	break;
		default:			iCursor = IDC_POINTER;		break;
	}
	HCURSOR cursor = theApp.LoadCursor (iCursor);
	SetCursor (cursor);
	return (true);
}

void CPictureWnd::OnPaint () {
	CDC      dcMem;
	CPaintDC dcThis(this);
	dcMem.CreateCompatibleDC (&dcThis);
	BLENDFUNCTION bf;

	int width, height;
	CRect rectClient, rectImage = GetImageRect (width, height);
	GetClientRect (&rectClient);

	if ((m_nImage = m_pFrame->GetActiveImageNo ()) == -1) {
		dcThis.FillSolidRect ( &rectClient, ::GetSysColor ( COLOR_WINDOW ) );
		CPen *pPen = dcThis.SelectObject (m_ppenLine);
		dcThis.MoveTo ( rectClient.right / 3, rectClient.bottom / 3 );
		dcThis.LineTo ( rectClient.right * 2 / 3, rectClient.bottom * 2 / 3 );
		dcThis.MoveTo ( rectClient.right / 3, rectClient.bottom * 2 / 3 );
		dcThis.LineTo ( rectClient.right * 2 / 3, rectClient.bottom / 3 );
		dcThis.SelectObject (pPen);
		return;
	}

	if (!m_bmpImage.m_hObject)
		return;

	dcThis.SetROP2 (R2_NOP);

	if (rectClient.Height() > height) {
		dcThis.FillSolidRect (CRect (0, 0, rectClient.right, rectImage.top),
				       ::GetSysColor (COLOR_WINDOW));
		dcThis.FillSolidRect (CRect (0, rectImage.bottom, rectClient.right, rectClient.bottom),
				       ::GetSysColor (COLOR_WINDOW));
	}
	if (rectClient.Width() > width) {
		dcThis.FillSolidRect (CRect (0, 0, rectImage.left, rectClient.bottom),
				       ::GetSysColor (COLOR_WINDOW));
		dcThis.FillSolidRect (CRect (rectImage.right, 0, rectClient.right, rectClient.bottom),
				       ::GetSysColor (COLOR_WINDOW));
	}

	m_pbmpOld = dcMem.GetCurrentBitmap ();
//	if (m_pDoc->GetSetting ("ShowImages"))
		dcMem.SelectObject (&m_bmpImage);
	dcThis.SetStretchBltMode (STRETCH_HALFTONE);

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0xFF;
	bf.AlphaFormat = 0;
//	dcThis.AlphaBlend (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
//			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), bf);
//	dcThis.StretchBlt (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
//			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), SRCCOPY);

//	if (m_pDoc->GetSetting ("ShowPatches")) {
		for (uword l = 0; l <= 0xFF; l++)
			for (uvar32_64 n = 0; n < m_pDoc->Images[m_nImage].Patches.GetCount (); ++n)
				if (m_pDoc->Images[m_nImage].Patches[n].GetLayer () == l)
					m_pDoc->Images[m_nImage].Patches[n].Paint (dcMem);
		if (m_pDoc->Images[m_nImage].Patches.Active () != -1)
			m_pDoc->Images[m_nImage].Patches[aimActive].Paint (dcMem);
//	}
//	if (m_pDoc->GetSetting ("ShowMarks"))
		for (uvar32_64 n = 0; n < m_pDoc->Images[m_nImage].Marks.GetCount (); ++n)
			m_pDoc->Images[m_nImage].Marks[n].Paint (dcMem);
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0x50;
	bf.AlphaFormat = 0;
//	dcThis.AlphaBlend (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
//			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), bf);
	dcThis.StretchBlt (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), SRCCOPY);

	dcMem.SelectObject (m_pbmpOld);
	dcMem.DeleteDC ();

	PaintActiveMark ();
}

void CPictureWnd::PaintMark (uvar32_64 n) {
	int	x;
	CDC     dcMem;
	CRect	rectImage = GetImageRect (x,x);
	CClientDC dcThis (this);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0x50;
	bf.AlphaFormat = AC_SRC_ALPHA;

	dcMem.CreateCompatibleDC (&dcThis);

	m_pDoc->Images[m_nImage].Marks[n].Paint (dcMem);
	dcThis.SetROP2 (R2_NOP);
	dcThis.SetStretchBltMode (STRETCH_HALFTONE);
	dcThis.AlphaBlend (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), bf);
//	dcThis.StretchBlt (rectImage.left, rectImage.top, rectImage.Width (), rectImage.Height (),
//			   &dcMem, 0, 0, m_pDoc->GetDimX(), m_pDoc->GetDimY(), SRCCOPY);
	dcMem.DeleteDC ();
//	PaintActiveMark ();
}

void CPictureWnd::CreateImage (CPaintDC* dcThis) {
	m_pMarkActive = NULL;

	bool del = false;
	if (m_bmpImage.m_hObject)
		m_bmpImage.DeleteObject();
	if (m_pDoc->Images(aimSelected).GetCount () != 1)
		return;
	if (del = (!dcThis))
		dcThis = new CPaintDC (this);

	DWORD size = m_pDoc->GetDimX () * m_pDoc->GetDimY ();
	RGBQUAD q = {0,0,0};
	LPBITMAPINFO lpbi;
	lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)];
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = m_pDoc->GetDimX ();
	lpbi->bmiHeader.biHeight = m_pDoc->GetDimY ();
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 24;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = size;
	lpbi->bmiHeader.biXPelsPerMeter = 0;
	lpbi->bmiHeader.biYPelsPerMeter = 0;
	lpbi->bmiHeader.biClrUsed = 0;
	lpbi->bmiHeader.biClrImportant = 0;
	lpbi->bmiColors[0] = q;

	// #### TODO: Add lut support here

	// #### TODO: This is only creation of bitmap for images with 256-color LU tables. Add support for true color images

	ubyte* pbBits = (ubyte*)aimMemoryCommit (size, "CPictureWnd::CreateImage", "pbBits");
	HBITMAP hBitmap = CreateDIBSection (dcThis->m_hDC, lpbi, DIB_RGB_COLORS, (void **)&m_pbBits, NULL, 0);
	m_pDoc->Images[aimActive].Channels(aimAll).GetBits (pbBits, size, true);
	for (uvar32_64 x, y = 0, dx = m_pDoc->GetDimX (), dy = m_pDoc->GetDimY (), t; y < dy; ++y)
		for (x = 0; x < dx; ++x) {
			t = pbBits[(dy-y-1)*dx + x];
			m_pbBits[(y*dx + x) * 3 + 0] = t;
			m_pbBits[(y*dx + x) * 3 + 1] = t;
			m_pbBits[(y*dx + x) * 3 + 2] = t;
		}
	aimMemoryRelease (pbBits, "CPictureWnd::CreateImage", "pbBits");
	m_bmpImage.Attach (hBitmap);
	delete [] (BYTE *)lpbi;

	if (del)
		delete dcThis;
}

void CPictureWnd::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	CRect rectClient;
	GetClientRect (&rectClient);
	switch (nSBCode) {
		case SB_LINEUP:		m_nVScrollPos-=2; break;
		case SB_LINEDOWN:	m_nVScrollPos+=2; break;
		case SB_PAGEUP:		m_nVScrollPos-=20; break;
		case SB_PAGEDOWN:	m_nVScrollPos+=20; break;
		case SB_TOP:		m_nVScrollPos=0; break;
		case SB_BOTTOM:		m_nVScrollPos=m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100 - rectClient.Height (); break;
		case SB_THUMBPOSITION:	m_nVScrollPos=nPos; break;
	}
	m_nVScrollPos = (m_nVScrollPos < 0) ? 0 : ((m_nVScrollPos > (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100 - rectClient.Height ())) ? (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100 - rectClient.Height ()) : m_nVScrollPos);
	SetScrollPos (SB_VERT, m_nVScrollPos);
	RedrawWindow ();
}

void CPictureWnd::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
//	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	CRect rectClient;
	GetClientRect (&rectClient);
	switch (nSBCode) {
		case SB_LINEUP:		m_nHScrollPos-=2; break;
		case SB_LINEDOWN:	m_nHScrollPos+=2; break;
		case SB_PAGEUP:		m_nHScrollPos-=20; break;
		case SB_PAGEDOWN:	m_nHScrollPos+=20; break;
		case SB_TOP:		m_nHScrollPos=0; break;
		case SB_BOTTOM:		m_nHScrollPos=m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100 - rectClient.Width (); break;
		case SB_THUMBPOSITION:	m_nHScrollPos=nPos; break;
	}
	m_nHScrollPos = (m_nHScrollPos < 0) ? 0 : ((m_nHScrollPos > (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100 - rectClient.Width ())) ? (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100 - rectClient.Width ()) : m_nHScrollPos);
	SetScrollPos (SB_HORZ, m_nHScrollPos);
	RedrawWindow ();
}

void CPictureWnd::OnSize (UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);
	UpdateScroll ();
}

void CPictureWnd::UpdateScroll () {
	if (!m_pFrame || !m_pView || !m_pDoc)
		return;

	bool blHorz, blVert;
	static bool blHorzOld = false, blVertOld = false;
	CRect rectClient;
	GetClientRect (&rectClient);
	SCROLLINFO info;
	info.cbSize = sizeof (SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	if (::IsWindow (m_pFrame->m_barImages.m_hWnd) && m_pFrame->GetActiveImageNo () == -1) {
		ShowScrollBar (SB_VERT, false);
		ShowScrollBar (SB_HORZ, false);
		return;
	}
	m_nHScrollPos = (m_nHScrollPos < 0) ? 0 : m_nHScrollPos;
	m_nVScrollPos = (m_nVScrollPos < 0) ? 0 : m_nVScrollPos;
	m_nHScrollPos = (m_nHScrollPos > (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100 - rectClient.Width ())) ? (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100 - rectClient.Width ()) : m_nHScrollPos;
	m_nVScrollPos = (m_nVScrollPos > (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100 - rectClient.Height ())) ? (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100 - rectClient.Height ()) : m_nVScrollPos;
	if (blVert = (rectClient.Height () < (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100))) {
		info.nMin = 0;
		info.nMax = (m_pDoc->GetDimY() * m_pView->m_nZoomImg / 100);
		info.nPage = rectClient.Height ();
		info.nPos = m_nVScrollPos;
		SetScrollInfo (SB_VERT, &info, false);
	} else m_nVScrollPos = 0;
	if (blHorz = (rectClient.Width () < (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100))) {
		info.nMin = 0;
		info.nMax = (m_pDoc->GetDimX() * m_pView->m_nZoomImg / 100);
		info.nPage = rectClient.Width ();
		info.nPos = m_nHScrollPos;
		SetScrollInfo (SB_HORZ, &info, false);
	} else m_nHScrollPos = 0;
	if (blVert != blVertOld)
		ShowScrollBar (SB_VERT, blVert);
	if (blHorz != blHorzOld)
		ShowScrollBar (SB_HORZ, blHorz);

	blVertOld = blVert;
	blHorzOld = blHorz;
	RedrawWindow ();
}

void CPictureWnd::OnLButtonDown (UINT nFlags, CPoint point) {
	if (m_pFrame->GetActiveImageNo () == -1)
		return;

	if (this != GetFocus ()) {
		SetFocus ();
		return;
	}

	CRect rectClient;
	GetClientRect (&rectClient);
	CPoint mouse = point;
	float zoom = m_pView->m_nZoomImg / 100.;
	int cx = (rectClient.Width () < m_pDoc->GetDimX() * zoom) ? (m_nHScrollPos) : ((m_pDoc->GetDimX() * zoom - rectClient.Width ())/2);
	int cy = (rectClient.Height () < m_pDoc->GetDimY() * zoom) ? (m_nVScrollPos) : ((m_pDoc->GetDimY() * zoom - rectClient.Height ())/2);
	point.x = (point.x + cx) / zoom;
	point.y = (point.y + cy) / zoom;

	uvar32_64 cnt;
	switch (m_pView->GetMode ()) {
		case ID_IMAGE_HAND:
			m_nCaptureMode = 1;
			break;
		case ID_IMAGE_MARKER:
			m_pDoc->Images[aimActive].Marks.Add (point);
			break;
// #### TODO: Replace ID_IMAGE_PATCH with ID_IMAGE_MARKER in editing mode
		case ID_IMAGE_POINTER:
			if (m_pDoc->Images[aimActive].Marks.GetCount () == 0)
				goto $Patch;
			if (m_pMarkActive) {
				m_nStart = m_pMarkActive->m_nShapeAngle;
				switch (m_pMarkActive->GetPointType (point)) {
					case CMark::pntNone:
						PaintActiveMark ();
						m_pMarkActive->Select (false);
						m_pMarkActive = NULL;
						goto $Patch;
					case CMark::pntCenter:
						m_nCaptureMode = 1;
						break;
					case CMark::pntAxisX:
						m_nCaptureMode = 2;
						break;
					case CMark::pntAxisY:
						m_nCaptureMode = 3;
						break;
					case CMark::pntAxisXY:
						m_nCaptureMode = 4;
						break;
				}
			}
			if (!m_pMarkActive) {
				m_nCaptureMode = 0;
				m_pMarkActive = &(m_pDoc->Images[aimActive].Marks[point]);
				if (m_pMarkActive) {
					m_pMarkActive->Select ();
					PaintActiveMark ();
				} else goto $Patch;
			}
			break;

$Patch:		if (m_pDoc->Images[aimActive].Patches.GetCount () == 0 ||
			    m_pDoc->Images[aimActive].Patches(point).GetCount () == 0) {
				m_pDoc->Images[aimActive].Patches.Deactivate ();
				break;
			}
			m_pDoc->Images[aimActive].Patches[point].Activate ();
			break;
		case ID_IMAGE_PATCH:
			uvar32_64 clen;
			uvar32_64* content;
			CClientDC dcThis (this);
			m_pCoinActive->Paint (dcThis, mouse, m_pView->m_nZoomImg);
			m_pCoinActive->GetContent (content, clen, point, CSize (m_pDoc->GetDimX (), m_pDoc->GetDimY ()));

			// I. For a single selected (active) patch
			//	click		- add to an active patch
			//	ctrl+click	- erase from an active patch
			//	ctrl+shift+click - xor with an active patch
			//	shift+click	- select another patch or deselect all (depends on state of tool which can be changed by pressing right mouse key
			//	esc		- cancel selection
			// II. For a no selection
			//	click		- creates a new patch
			// III. For a multiple selection
			//	click on any of the selected patches - change active item (drag - move all selected items)
			//	click on the empty area - deselect all of the items (drag - create a new patch and select only it)
			//	click on the non-selected patch - select it
			//	ctrl+click
			//	shift+click	- add to selection
			//

			if (m_pDoc->Images[aimActive].Patches(aimActive).GetCount () == 0 || (
			    (nFlags & (MK_SHIFT | MK_CONTROL)) == 0 &&
			    m_pDoc->Images[aimActive].Patches(aimActive).GetCount () > 0 )) {
				CPatch* pPatch = new CPatch (&(m_pDoc->Images[aimActive]));
				pPatch->Create (clen, content, m_pView->GetLayer ());
				m_pDoc->Images[aimActive].Patches.Add (*pPatch);
				pPatch->Activate ();
			} else
				m_pDoc->Images[aimActive].Patches[aimActive].Merge (clen, content, nFlags & (MK_SHIFT | MK_CONTROL));
			aimMemoryRelease (content, "CPictureWnd::OnLButtonDown");
			m_pCoinActive->Paint (dcThis, mouse, m_pView->m_nZoomImg);
			break;
	}
	m_pntOld = point;
	m_pntStart = point;
	SetCapture ();
	CWnd::OnLButtonDown(nFlags, point);
}

void CPictureWnd::OnLButtonUp (UINT nFlags, CPoint point) {
	if (GetCapture () != this) {
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	m_nCaptureMode = 0;
	ReleaseCapture ();
	CMark *p = m_pMarkActive;
	switch (m_pView->GetMode ()) {
		case ID_IMAGE_HAND:
			m_nCaptureMode = 0;
			break;
// #### TODO: Replace ID_IMAGE_PATCH with ID_IMAGE_MARKER in editing mode
		case ID_IMAGE_POINTER:
			if (m_pMarkActive) {
				CreateImage ();
				m_pMarkActive = p;
				RedrawWindow ();
			}
			break;
		case ID_IMAGE_PATCH:
			m_nCaptureMode = 0;
			break;
	}
}

void CPictureWnd::OnMouseMove (UINT nFlags, CPoint point) {
	if (m_pFrame->GetActiveImageNo () == -1)
		return;

	if (this != GetFocus ()) {
	CWnd::OnMouseMove(nFlags, point);
		return;
	}

	rtbyte b2, d2, l2, sign, var1;
	CRect rectClient;
	GetClientRect (&rectClient);
	CPoint mouse = point;
	float zoom = m_pView->m_nZoomImg / 100.;
	int cx = (rectClient.Width () < m_pDoc->GetDimX() * zoom) ? (m_nHScrollPos) : ((m_pDoc->GetDimX() * zoom - rectClient.Width ())/2);
	int cy = (rectClient.Height () < m_pDoc->GetDimY() * zoom) ? (m_nVScrollPos) : ((m_pDoc->GetDimY() * zoom - rectClient.Height ())/2);
	point.x = (point.x + cx) / zoom;
	point.y = (point.y + cy) / zoom;

	if (m_pntOld == point)
		return;
	switch (m_pView->GetMode ()) {
		case ID_IMAGE_HAND:
			if (!m_nCaptureMode)
				break;
			SetCursor (theApp.LoadCursor (IDC_HANDSHAKE));
			m_nHScrollPos -= (point.x - m_pntOld.x);
			m_nVScrollPos -= (point.y - m_pntOld.y);
			UpdateScroll ();
			break;
// #### TODO: Replace ID_IMAGE_PATCH with ID_IMAGE_MARKER in editing mode
		case ID_IMAGE_POINTER:
			if (!m_pMarkActive)
				break;
			if (m_nCaptureMode)
				PaintActiveMark ();
			switch (m_nCaptureMode) {
				case 1:	m_pMarkActive->m_iPosX += point.x - m_pntOld.x;
					m_pMarkActive->m_iPosY += point.y - m_pntOld.y;
					break;
				case 2:	m_pMarkActive->m_nShapeSizeX = point.x - m_pMarkActive->m_iPosX;
					break;
				case 3:	m_pMarkActive->m_nShapeSizeY = m_pMarkActive->m_iPosY - point.y;
					break;
				case 4:
					b2 = m_pMarkActive->m_nShapeSizeX * m_pMarkActive->m_nShapeSizeX + m_pMarkActive->m_nShapeSizeY * m_pMarkActive->m_nShapeSizeY;
					d2 = (point.x - m_pntStart.x) * (point.x - m_pntStart.x) + (point.y - m_pntStart.y) * (point.y - m_pntStart.y);
					l2 = (point.x - m_pMarkActive->m_iPosX) * (point.x - m_pMarkActive->m_iPosX) + (point.y - m_pMarkActive->m_iPosY) * (point.y - m_pMarkActive->m_iPosY);
					if (point.y - m_pntStart.y == 0) sign = 2;
					else sign = 2 * abs (point.y - m_pntStart.y) / (point.y - m_pntStart.y);
					var1 = sign * pow (b2, 0.5L);
					var1 *= pow ((long double)12, (long double)1/2);
					m_pMarkActive->m_nShapeAngle = m_nStart + acos ( (b2 + l2 - d2) / var1 ) * 180.0L / 3.1415L;
					break;
			}
			m_pMarkActive->CreateControls ();
			if (m_nCaptureMode)
				PaintActiveMark ();
			switch (m_pMarkActive->GetPointType (point)) {
				case CMark::pntNone:
					SetCursor (theApp.LoadCursor (IDC_MRKPTR));
					break;
				case CMark::pntCenter:
					SetCursor (theApp.LoadCursor (IDC_MRKPTR_MOVE));
					break;
				case CMark::pntAxisX:
					SetCursor (theApp.LoadCursor (IDC_MRKPTR_SIZEX));
					break;
				case CMark::pntAxisY:
					SetCursor (theApp.LoadCursor (IDC_MRKPTR_SIZEY));
					break;
				case CMark::pntAxisXY:
					SetCursor (theApp.LoadCursor (IDC_MRKPTR_SIZEXY));
					break;
			}
			break;
		case ID_IMAGE_PATCH:
			CClientDC dcThis (this);
			if (m_pntOldMouse != CPoint (-10000, -10000))
				m_pCoinActive->Paint (dcThis, m_pntOldMouse, m_pView->m_nZoomImg);

			if ((nFlags & MK_SHIFT) && (nFlags & MK_CONTROL))
				SetCursor (theApp.LoadCursor (IDC_IOBJCREATE_XOR));
			else if (nFlags & MK_CONTROL)
				SetCursor (theApp.LoadCursor (IDC_IOBJCREATE_EXCL));
			else if (nFlags & MK_SHIFT)
				SetCursor (theApp.LoadCursor (IDC_IOBJCREATE_INCL));
			else
				SetCursor (theApp.LoadCursor (IDC_IOBJCREATE));

			if (GetCapture () == this) {
				uvar32_64 clen;
				uvar32_64* content;
				m_pCoinActive->GetContent (content, clen, point, CSize (m_pDoc->GetDimX (), m_pDoc->GetDimY ()));
				if ((nFlags & (MK_SHIFT | MK_CONTROL)) == 0)
					nFlags |= MK_SHIFT;
				m_pDoc->Images[aimActive].Patches[aimActive].Merge (clen, content, nFlags & (MK_SHIFT | MK_CONTROL));
				aimMemoryRelease (content, "CPictureWnd::OnLButtonDown");
			}

			m_pCoinActive->Paint (dcThis, mouse, m_pView->m_nZoomImg);
			break;
	}
	m_pntOld = point;
	m_pntOldMouse = mouse;
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CPictureWnd::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt) {
	switch (m_pView->GetMode ()) {
		case ID_IMAGE_PATCH:
			sword a = m_pCoinActive->GetAngle ();
			CSize sz = m_pCoinActive->GetSize ();
			if ((nFlags & MK_SHIFT) && (nFlags & MK_CONTROL))
				a += zDelta / WHEEL_DELTA;
			else if (nFlags & MK_SHIFT)
				sz.cx += zDelta / WHEEL_DELTA;
			else if (nFlags & MK_CONTROL)
				sz.cy += zDelta / WHEEL_DELTA;
			else {
				sz.cx += zDelta / WHEEL_DELTA;
				sz.cy += zDelta / WHEEL_DELTA;
			}
			CClientDC dcThis (this);
			if (m_pntOldMouse != CPoint (-10000, -10000))
				m_pCoinActive->Paint (dcThis, m_pntOldMouse, m_pView->m_nZoomImg);
			while (a < 0)
				a += 360;
			m_pCoinActive->SetAngle (a = (a >= 360) ? 0 : a);
			m_pCoinActive->SetSize (sz.cx, sz.cy);
			m_pCoinActive->Paint (dcThis, m_pntOldMouse, m_pView->m_nZoomImg);
			break;
	}

	return false;
}

void CPictureWnd::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {

	switch (nChar) {
		case 0x56:
			m_pView->OnModeChange (ID_IMAGE_POINTER); return;
		case 0x50:
			m_pView->OnModeChange (ID_IMAGE_PATCH); return;
	}

	switch (m_pView->GetMode ()) {
		case ID_IMAGE_POINTER:
			if (nChar == VK_BACK && m_pDoc->Images[aimActive].Patches(aimActive).GetCount () > 0)
				m_pDoc->Images[aimActive].Patches[aimActive].Delete ();
			break;
		case ID_IMAGE_PATCH:
			CSize sz = m_pCoinActive->GetSize ();
			if (nChar == VkKeyScan ('+') || nChar == VK_ADD) {
				sz.cx += (sz.cx >= 255) ? 0 : (sz.cx / 3);
				sz.cy += (sz.cy >= 255) ? 0 : (sz.cy / 3);
			} else if (nChar == VkKeyScan ('-') || nChar == VK_SUBTRACT) {
				sz.cx -= (sz.cx <= 2) ? 0 : (sz.cx / 3);
				sz.cy -= (sz.cy <= 2) ? 0 : (sz.cy / 3);
			} else if (nChar == VkKeyScan ('*') || nChar == VK_MULTIPLY)
				sz = CSize (255, 255);
			else if (nChar == VkKeyScan ('/') || nChar == VK_DIVIDE)
				sz = CSize (2, 2);

			if (nChar == VK_DELETE && m_pDoc->Images[aimActive].Patches(aimActive).GetCount () > 0)
				m_pDoc->Images[aimActive].Patches[aimActive].Delete ();
			if (nChar == VK_BACK && m_pDoc->Images[aimActive].Patches(aimActive).GetCount () > 0)
				m_pDoc->Images[aimActive].Patches[aimActive].Delete ();

			if (nChar >= 0x30 && nChar <= 0x39)
				m_pView->OnLayerChange (nChar - 0x30 + ID_VIEW_LAYERS_BACKGROUND);

			if (sz != m_pCoinActive->GetSize ()) {
				CClientDC dcThis (this);
				if (m_pntOldMouse != CPoint (-10000, -10000))
					m_pCoinActive->Paint (dcThis, m_pntOldMouse, m_pView->m_nZoomImg);
				m_pCoinActive->SetSize (sz.cx, sz.cy);
				m_pCoinActive->Paint (dcThis, m_pntOldMouse, m_pView->m_nZoomImg);
			}
			break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPictureWnd::OnSysKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CPictureWnd::OnContextMenu (CWnd* pWnd, CPoint point) {
	CaImMenu menu;
	theApp.PrepareMenu (&menu, IDR_POPUP_IMG);

	menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_TOPALIGN, point.x, point.y, this);
}
