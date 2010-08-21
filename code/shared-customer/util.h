#pragma once

class CLigaApp;

typedef HRESULT(__stdcall *PFNCLOSETHEMEDATA)(HTHEME hTheme);
typedef HRESULT(__stdcall *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc,
							int iPartId, int iStateId, const RECT *pRect,  const RECT *pClipRect);
typedef HTHEME(__stdcall *PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT (__stdcall *PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId,
							int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags,
							DWORD dwTextFlags2, const RECT *pRect);
typedef HRESULT (__stdcall *PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME hTheme,  HDC hdc,
							int iPartId, int iStateId,  const RECT *pBoundingRect,
							RECT *pContentRect);

extern bool	aimThemed;
extern bool	aimThemeLoaded;

void			aimThemeInit ();
void			aimThemeFinalize ();
extern PFNOPENTHEMEDATA		aimThemeOpen;
extern PFNCLOSETHEMEDATA	aimThemeClose;
extern PFNDRAWTHEMEBACKGROUND	aimThemeBackground;
extern PFNDRAWTHEMETEXT		aimThemeText;
extern PFNGETTHEMEBACKGROUNDCONTENTRECT aimThemeGetBgContentRect;

extern HMODULE aimThemeMod;


int	aimGetSelListItem (CListCtrl& ctrlList, BOOL blFocus);
void	aimSetSelListItem (CListCtrl& ctrlList, int no);

void	aimPaintGradVert (CDC* pDC, CRect rcPaint, COLORREF clTop, COLORREF clBottom);
void	aimPaintGradHorz (CDC* pDC, CRect rcPaint, COLORREF clTop, COLORREF clBottom);

void	aimRotate2D (uvar32_64 no, rtbyte *pPointsX, rtbyte *pPointsY, rtbyte rAngle, CPoint pntCenter);
void	aimRotate2D (uvar32_64 no, uvar32_64 *pOffsets, rtbyte rAngle, CPoint pntCenter, CSize szDims);
ubyte	*aimCompressZip (ubyte *pbIn, uvar32_64 nLen, uvar32_64 &nOutLen);
ubyte	*aimDecompressZip (ubyte *pbIn, uvar32_64 nLen, uvar32_64 &nOutLen);
void*	aimMemoryReserve (DWORD dwSize, CString strProc = "Unknown", CString strMater = "Unknown", CString strTitle = AfxGetApp()->m_pszAppName, CString strFile = __FILE__, UINT nLine = __LINE__);
void*	aimMemoryCommit (DWORD dwSize, CString strProc = "Unknown", CString strMater = "Unknown", CString strTitle = AfxGetApp()->m_pszAppName, CString strFile = __FILE__, UINT nLine = __LINE__);
bool	aimMemoryDecommit (void* lpAddress, DWORD dwSize, CString strProc = "Unknown", CString strMater = "Unknown", CString strTitle = AfxGetApp()->m_pszAppName, CString strFile = __FILE__, UINT nLine = __LINE__);
bool	aimMemoryRelease (void* lpAddress, CString strProc = "Unknown", CString strMater = "Unknown", CString strTitle = AfxGetApp()->m_pszAppName, CString strFile = __FILE__, UINT nLine = __LINE__);
