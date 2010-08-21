// ImageCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dias-devel.h"
#include "ImageCtrl.h"
#include ".\imagectrl.h"


// CImageCtrl

IMPLEMENT_DYNAMIC(CImageCtrl, CWnd)
CImageCtrl::CImageCtrl () : m_pobjImage (NULL) {
	m_pbBits = NULL;
	m_ppenLine = new CPen (PS_SOLID, 3, ::GetSysColor (COLOR_BTNFACE));
	m_ppenBoundary = new CPen (PS_SOLID, 1, 0xFFFFFF);
	m_ppenChord = new CPen (PS_DOT, 1, 0xFFFFFF);
	m_nVScrollPos = 0;
	m_nHScrollPos = 0;
	m_pobjImage = NULL;
}

CImageCtrl::~CImageCtrl () {
	delete m_ppenLine;
	delete m_ppenBoundary;
	delete m_ppenChord;
	if (m_bmpImage.m_hObject)
		m_bmpImage.DeleteObject();
}


BEGIN_MESSAGE_MAP(CImageCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CImageCtrl message handlers

void CImageCtrl::CreateImage (CPaintDC* pdcThis) {
	bool del = false;
	if (!m_pobjImage)
		return;
	if (m_bmpImage.m_hObject)
		m_bmpImage.DeleteObject();
	if (del = (!pdcThis))
		pdcThis = new CPaintDC (this);

	DWORD size = m_pobjImage->dimx * m_pobjImage->dimy;
	RGBQUAD q = {0,0,0};
	LPBITMAPINFO lpbi;
	lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)];
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = m_pobjImage->dimx;
	lpbi->bmiHeader.biHeight = m_pobjImage->dimy;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 24;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = size;
	lpbi->bmiHeader.biXPelsPerMeter = 0;
	lpbi->bmiHeader.biYPelsPerMeter = 0;
	lpbi->bmiHeader.biClrUsed = 0;
	lpbi->bmiHeader.biClrImportant = 0;
	lpbi->bmiColors[0] = q;

	ubyte* pbBits = new ubyte[size];
	HBITMAP hBitmap = CreateDIBSection (pdcThis->m_hDC, lpbi, DIB_RGB_COLORS, (void **)&m_pbBits, NULL, 0);
	for (ulong x, y = 0, dx = m_pobjImage->dimx, dy = m_pobjImage->dimy, t; y < dy; ++y)
		for (x = 0; x < dx; ++x) {
			t = m_pobjImage->layers[0][(dy-y-1)*dx + x];
			m_pbBits[(y*dx + x) * 3 + 0] = t;
			m_pbBits[(y*dx + x) * 3 + 1] = t;
			m_pbBits[(y*dx + x) * 3 + 2] = t;
		}
	delete [] pbBits;
	m_bmpImage.Attach (hBitmap);
	delete [] (BYTE *)lpbi;

	if (del)
		delete pdcThis;
}

void CImageCtrl::OnPaint () {
	CDC	dcMem;
	CPaintDC dcThis(this);
	CRect	rectClient;

	GetClientRect (&rectClient);
	dcMem.CreateCompatibleDC (&dcThis);
	dcThis.SetROP2 (R2_NOP);
	dcThis.FillSolidRect (rectClient, ::GetSysColor (COLOR_WINDOW));

	if (!m_pobjImage || !m_bmpImage.m_hObject) {
		CPen *pPen = dcThis.SelectObject (m_ppenLine);
		dcThis.MoveTo (rectClient.right / 3, rectClient.bottom / 3);
		dcThis.LineTo (rectClient.right * 2 / 3, rectClient.bottom * 2 / 3);
		dcThis.MoveTo (rectClient.right / 3, rectClient.bottom * 2 / 3);
		dcThis.LineTo (rectClient.right * 2 / 3, rectClient.bottom / 3);
		dcThis.SelectObject (pPen);
		return;
	}

	CBitmap *pBmp = dcMem.GetCurrentBitmap ();
	dcMem.SelectObject (&m_bmpImage);
	dcThis.SetStretchBltMode (STRETCH_HALFTONE);
	dcThis.StretchBlt (rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (), 
	                   &dcMem, 0, 0, m_pobjImage->dimx, m_pobjImage->dimy, SRCCOPY);

	dcMem.SelectObject (pBmp);
	dcMem.DeleteDC ();
}
