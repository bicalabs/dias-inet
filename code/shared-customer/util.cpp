#include "stdafx.h"
#include "platform.h"
#include "util.h"
#include <aimapi/aimapi.h>
#include "../Liga-customer/resource.h"

bool	aimThemed = false;
bool	aimThemeLoaded = false;

void			aimThemeInit ();
void			aimThemeFinalize ();
PFNOPENTHEMEDATA	aimThemeOpen = NULL;
PFNCLOSETHEMEDATA	aimThemeClose = NULL;
PFNDRAWTHEMEBACKGROUND	aimThemeBackground = NULL;
PFNDRAWTHEMETEXT	aimThemeText = NULL;
PFNGETTHEMEBACKGROUNDCONTENTRECT aimThemeGetBgContentRect = NULL;

HMODULE aimThemeMod = NULL;


void aimThemeInit () {
	aimThemeMod = LoadLibrary("UXTHEME.DLL");
	if(aimThemeMod)
	{
		aimThemeOpen = (PFNOPENTHEMEDATA)GetProcAddress(aimThemeMod, "OpenThemeData");
		aimThemeClose = (PFNCLOSETHEMEDATA)GetProcAddress(aimThemeMod, "CloseThemeData");
		aimThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(aimThemeMod, "DrawThemeBackground");
		aimThemeText = (PFNDRAWTHEMETEXT)GetProcAddress(aimThemeMod, "DrawThemeText");
		aimThemeGetBgContentRect = (PFNGETTHEMEBACKGROUNDCONTENTRECT)GetProcAddress(aimThemeMod, "GetThemeBackgroundContentRect");

		if(aimThemeOpen &&
		   aimThemeBackground &&
		   aimThemeClose &&
		   aimThemeText &&
		   aimThemeGetBgContentRect) {
			aimThemeLoaded = true;
		}
		else {
			FreeLibrary(aimThemeMod);
			aimThemeMod = NULL;
		}
	}
}

void aimThemeFinalize () {
	FreeLibrary(aimThemeMod);
	aimThemeMod = NULL;
	aimThemeLoaded = false;
	aimThemed = false;
}



int aimGetSelListItem (CListCtrl& ctrlList, BOOL blFocus) {
	int state;
	int count = ctrlList.GetItemCount ();
	for (int i = 0 ; i < count ; ++i) {
		state = ctrlList.GetItemState (i, LVIS_SELECTED);
		if (state)	return (i);
	}
	return (-1);
}

void aimSetSelListItem (CListCtrl& ctrlList, int no) {
	ctrlList.SetItemState (no, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ctrlList.EnsureVisible (no, FALSE);
}

#define	GetAValue(c)	((c&0xFF000000)>>24)

void aimPaintGradVert (CDC* pDC, CRect rcPaint, COLORREF clTop, COLORREF clBottom) {
	TRIVERTEX        tvVert[2];
	GRADIENT_RECT    gRect;

	tvVert[0].x      = rcPaint.left;
	tvVert[0].y      = rcPaint.top;
	tvVert[0].Red    = GetRValue (clTop) * 256;
	tvVert[0].Green  = GetGValue (clTop) * 256;
	tvVert[0].Blue   = GetBValue (clTop) * 256;
	tvVert[0].Alpha  = GetAValue (clTop) * 256;

	tvVert[1].x      = rcPaint.right;
	tvVert[1].y      = rcPaint.bottom;
	tvVert[1].Red    = GetRValue (clBottom) * 256;
	tvVert[1].Green  = GetGValue (clBottom) * 256;
	tvVert[1].Blue   = GetBValue (clBottom) * 256;
	tvVert[1].Alpha  = GetAValue (clBottom) * 256;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	pDC->GradientFill (tvVert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}

void aimPaintGradHorz (CDC* pDC, CRect rcPaint, COLORREF clLeft, COLORREF clRight) {
	TRIVERTEX        tvVert[2];
	GRADIENT_RECT    gRect;

	tvVert[0].x      = rcPaint.left;
	tvVert[0].y      = rcPaint.top;
	tvVert[0].Red    = GetRValue (clLeft) * 256;
	tvVert[0].Green  = GetGValue (clLeft) * 256;
	tvVert[0].Blue   = GetBValue (clLeft) * 256;
	tvVert[0].Alpha  = GetAValue (clLeft) * 256;

	tvVert[1].x      = rcPaint.right;
	tvVert[1].y      = rcPaint.bottom;
	tvVert[1].Red    = GetRValue (clRight) * 256;
	tvVert[1].Green  = GetGValue (clRight) * 256;
	tvVert[1].Blue   = GetBValue (clRight) * 256;
	tvVert[1].Alpha  = GetAValue (clRight) * 256;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	pDC->GradientFill (tvVert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
}

#undef GetAValue

void aimRotate2D (uvar32_64 no, rtbyte *pPointsX, rtbyte *pPointsY, rtbyte rAngle, CPoint pntCenter) {
	rtbyte x, y;
	rtbyte sa = sinl (rAngle), ca = cosl (rAngle);
	for (uvar32_64 n = 0; n < no; ++n) {
		x = (pPointsX[n] - pntCenter.x) * ca - (pPointsY[n] - pntCenter.y) * sa;
		y = (pPointsX[n] - pntCenter.x) * sa + (pPointsY[n] - pntCenter.y) * ca;
		pPointsX[n] = x + pntCenter.x; pPointsY[n] = y + pntCenter.y;
	}
}

void aimRotate2D (uvar32_64 no, uvar32_64 *pOffsets, rtbyte rAngle, CPoint pntCenter, CSize szDims) {
	rtbyte x, y, sx, sy;
	rtbyte sa = sinl (rAngle), ca = cosl (rAngle);
	for (uvar32_64 n = 0; n < no; ++n) {
		sx = (rtbyte)((uvar32_64)(pOffsets[n] % szDims.cx));
		sy = (rtbyte)((uvar32_64)(pOffsets[n] / szDims.cx));
		x = (sx - pntCenter.x) * ca - (sy - pntCenter.y) * sa;
		y = (sx - pntCenter.x) * sa + (sy - pntCenter.y) * ca;
		x = (int)(x) + pntCenter.x;
		y = (int)(y) + pntCenter.y;
		pOffsets[n] = (int)(y) * szDims.cx + (int)(x);
	}
}

ubyte *aimCompressZip (ubyte *pbIn, uvar32_64 nLen, uvar32_64 &nOutLen) {
	ubyte *pbData;
	char psTempPath[1024], psTempFileIn[1024], psTempFileOut[1024];
	GetTempPath (1024, psTempPath);
	GetTempFileName (psTempPath, "~aim", 1, psTempFileIn);
	GetTempFileName (psTempPath, "~aim", 2, psTempFileOut);
	DeleteFile (psTempFileIn);
	DeleteFile (psTempFileOut);
	CFile fIn (psTempFileIn, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
	fIn.Write (pbIn, nLen);
	fIn.Close ();
	CZip zip (psTempFileIn);
	zip.SwapSize (psTempFileOut);
	CFile fOut (psTempFileOut, CFile::typeBinary | CFile::modeRead);
	pbData = (ubyte *)aimMemoryCommit (fOut.GetLength (), "aimCompressZip", "pbData");
	fOut.Read (pbData, nOutLen = fOut.GetLength ());
	fOut.Close ();
	DeleteFile (psTempFileIn);
	DeleteFile (psTempFileOut);
	return (pbData);
}

ubyte *aimDecompressZip (ubyte *pbIn, uvar32_64 nLen, uvar32_64 &nOutLen) {
	ubyte *pbData;
	char psTempPath[1024], psTempFileIn[1024], psTempFileOut[1024];
	GetTempPath (1024, psTempPath);
	GetTempFileName (psTempPath, "~aim", 1, psTempFileIn);
	GetTempFileName (psTempPath, "~aim", 2, psTempFileOut);
	DeleteFile (psTempFileOut);
	DeleteFile (psTempFileOut);
	CFile fIn (psTempFileIn, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
	fIn.Write (pbIn, nLen);
	fIn.Close ();
	CUnzip unzip (psTempFileIn);
	unzip.SwapSize (psTempFileOut);
	CFile fOut (psTempFileOut, CFile::typeBinary | CFile::modeRead);
	pbData = (ubyte *)aimMemoryCommit (fOut.GetLength (), "aimCompressZip", "pbData");
	fOut.Read (pbData, nOutLen = fOut.GetLength ());
	fOut.Close ();
	DeleteFile (psTempFileIn);
	DeleteFile (psTempFileOut);
	return (pbData);
}

void* aimMemoryReserve (DWORD dwSize, CString strProc, CString strMater, CString strTitle, CString strFile, UINT nLine) {
	if ( dwSize == 0 )
		return ( NULL );
	void* lpAddress = VirtualAlloc (NULL, dwSize, MEM_RESERVE, PAGE_READWRITE);
	if (!lpAddress) {
		void*	lpMessage;
		char	pcFormat [1024];
		CString	strMessage;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		               NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessage, 0, NULL);
		LoadString (AfxGetApp()->m_hInstance, IDS_ERR_SYS_MEMRES, pcFormat, 1024);
		strMessage.Format (pcFormat, strProc, strFile, nLine, strMater, lpMessage);
		MessageBox (NULL, strMessage, strTitle, MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return (NULL);
	}
	return (lpAddress);
}

void* aimMemoryCommit (DWORD dwSize, CString strProc, CString strMater, CString strTitle, CString strFile, UINT nLine) {
	if (dwSize == 0)
		return (NULL);
	void* lpAddress = VirtualAlloc (NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (!lpAddress) {
		void*	lpMessage;
		char	pcFormat [1024];
		CString strMessage;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		               NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessage, 0, NULL);
		LoadString (AfxGetApp()->m_hInstance, IDS_ERR_SYS_MEMCOM, pcFormat, 1024);
		strMessage.Format (pcFormat, strProc, strFile, nLine, strMater, lpMessage);
		MessageBox (NULL, strMessage, strTitle, MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return (NULL);
	}
	return (lpAddress);
}

bool aimMemoryDecommit (void* lpAddress, DWORD dwSize, CString strProc, CString strMater, CString strTitle, CString strFile, UINT nLine) {
	if (dwSize == 0 || !lpAddress)
		return (true);
	if (!VirtualFree ( lpAddress, dwSize, MEM_DECOMMIT)) {
		void*	lpMessage;
		char	pcFormat [1024];
		CString strMessage;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		               NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessage, 0, NULL);
		LoadString (AfxGetApp()->m_hInstance, IDS_ERR_SYS_MEMDECOM, pcFormat, 1024);
		strMessage.Format (pcFormat, strProc, strFile, nLine, strMater, lpMessage);
		MessageBox (NULL, strMessage, strTitle, MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return (false);
	}
	return (true);
}

bool aimMemoryRelease (void* lpAddress, CString strProc, CString strMater, CString strTitle, CString strFile, UINT nLine) {
	if (!lpAddress)
		return (true);
	if (!VirtualFree (lpAddress, 0, MEM_RELEASE)) {
		void*	lpMessage;
		char	pcFormat [1024];
		CString strMessage;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		               NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessage, 0, NULL);
		LoadString (AfxGetApp()->m_hInstance, IDS_ERR_SYS_MEMREL, pcFormat, 1024);
		strMessage.Format (pcFormat, strProc, strFile, nLine, strMater, lpMessage);
		MessageBox (NULL, strMessage, strTitle, MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return (false);
	}
	return (true);
}

rtbyte _math_power (rtbyte a, UINT p) {
  if (p == 0)
    return (1);
  if (p == 20) {
    --a;
    return (1 + a/2 - a*a/8 + a*a*a/16 - 5*a*a*a*a/128 + 7*a*a*a*a*a/156 - 21*a*a*a*a*a*a/1024);
  }
  else
    for (UINT i = 0 ; i < p ; ++i)
      a *= a;
  return (a);
}
