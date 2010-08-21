//--------------------------------------------------------------------------
//
// MVDISP.H : header file for module MVDISP.DLL
//
// -----------------------------------------------
//
//	Matrix Vision Demo for PCimageS - Grabber
//
//	01/97       DH
//  04/98       UF
//
//--------------------------------------------------------------------------


#ifndef __MVDISP_H__
#define __MVDISP_H__

#define _MVDISP_VERSION_MAJOR 3
#define _MVDISP_VERSION_MINOR 3
#define _MVDISP_VERSION_STR   "3, 3, 0, 1\0"
#define _MVDISP_VERSION   ((_MVDISP_VERSION_MAJOR<<16) | (_MVDISP_VERSION_MINOR))

#define _MVDISP_DLLNAME "MVDISP.DLL\0" 

#include <mvimage.h>

#define STRUCT_ALIGNMENT 8
#include <align.h>

// --------------- defines -----------------
#define LIVE_WND  ((VOID*)1)

// ----------------- enums -----------------

typedef enum 
{ 
   E_LIVEMODE, 
   E_HICOLOR, 
   E_BITBLTMODE, 
   E_OVERLAY, 
   E_OVERLAY_COLOR,
   E_PALETTE,
   E_NEWCLIENTWND,  // 0=off; 1=on
   E_OWNERDRAW
} TOPTIONS ;

//----------------------- E_LIVEMODE options
typedef enum 
{ 
   E_LM_SOFT_DDB, 
   E_LM_SOFT_DIB, 
   E_LM_HARD, 
   E_LM_DIRECT_DRAW, 
   E_LM_NOCHANGE 
} TLIVEMODE ;

//----------------------- E_HICOLOR options
typedef enum 
{ 
   E_HICOLOR_15, 
   E_HICOLOR_16, 
   E_HICOLOR_NOCHANGE 
} THICOLOR ;

//----------------------- E_BITBLTMODE options
typedef enum 
{ 
   E_1_TO_1, 
   E_STRETCH 
} TBITBLTMODE ;


//----------------------- E_OVERLAY options
typedef enum 
{ 
   E_OL_NO_OVERLAY, 
   E_OL_LINE, 
   E_OL_RECT, 
   E_OL_CIRCLE, 
   E_OL_RECT_FILLED, 
   E_OL_CIRCLE_FILLED, 
   E_OL_TEXT, 
   E_OL_CROSSHAIR, 
   E_OL_USE_OVERLAY, 
   E_OL_UNCHANGED 
} TOVERLAY ;

//----------------------- E_OVERLAY_COLOR options
typedef enum 
{ 
   E_OLC_RED, 
   E_OLC_GREEN, 
   E_OLC_BLUE, 
   E_OLC_CYAN, 
   E_OLC_MAGENTA, 
   E_OLC_YELLOW, 
   E_OLC_BLACK, 
   E_OLC_WHITE, 
   E_OLC_DARKGREY, 
   E_OLC_LIGHTGREY,
   E_OLC_UNCHANGED 
} TOVERLAYCOLOR ;


//----------------------- E_PALETTE options
// only need HPALETTE as parameter


//----------------------- InitAcquireDef options
typedef enum 
{ 
   E_SM_SCALER_OFF, 
   E_SM_SCALER_ON, 
   E_SM_SCALER_ON_AD_FILT, 
   E_SM_SCALER_ON_AD_GEO_FILT, 
   E_SM_SCALER_NOCHANGE 
} TSCALERMODE ;


// ----------------- image struct -----------------

#define DEFAULT_LIVE_MODE           E_LM_SOFT_DIB
#define DEFAULT_HICOLOR             E_HICOLOR_15
#define DEFAULT_BITBLT_MODE         E_1_TO_1
#define DEFAULT_OVERLAY_MODE        E_OL_NO_OVERLAY
#define DEFAULT_OVERLAY_COLOR_MODE  E_OLC_RED
#define DEFAULT_OVERLAY_COLOR       RGB(255,0,0)



#define MAX_SUBCLASSES 10

typedef struct
{
   WNDPROC              OldProc[MAX_SUBCLASSES]; // old message handler
   HWND                 hWnd[MAX_SUBCLASSES];    // windows handle of that handler
   int                  Count;                   // number of subclassed handler
} TSUBCLASSPROCS ;


typedef struct
{
   HWND                 hSubWindow ;      // handle to this display window
   signed short         DispHandle ;      // number of window
   LPIMAGE              pImage; //ImageDescr ;      // pointer to image struct
   HANDLE               hDIB ;            // handle to empty DIB
   HBITMAP              MemBmp ;          // handle to bitmap in memory for image data
   HBITMAP              OldMemBmp ;       // old handle to bitmap in memory for image data
   HDC                  MemDC ;           // memory DC 

   // old: +++++++
   HBITMAP              MemOverlayBmp ;   // handle to bitmap in memory for overlay data
   HBITMAP              OldMemOverlayBmp ;// old handle to bitmap in memory for overlay data
   HDC                  MemOverlayDC ;    // overlay memory DC

   HBITMAP              MemMaskBmp ;   // handle to bitmap in memory for mask data
   HBITMAP              OldMemMaskBmp ;// old handle to bitmap in memory for mask data
   HDC                  MemMaskDC ;    // mask memory DC
   // old --------

   LPBITMAPINFOHEADER   lpBMPHdr ;        // pointer to bitmap info header
   TLIVEMODE            LiveMode ;        // live mode 
   THICOLOR             HiColor ;
   TBITBLTMODE          BitBltMode ;
   TOVERLAY             OverlayMode ;
   TOVERLAYCOLOR        OverlayColorMode ;
   COLORREF             OverlayColor ;
   BOOL                 bLiveWnd ;
   BOOL                 bFreeFlag ;

   void *               hOverlay;        // handle for internal overlay functions
   void *               ImageMemory;
   HPALETTE             Palette;
   BOOL                 boClientWndCreated;  // true, if own client window was created

   TSUBCLASSPROCS       SubClass;    // array of subclassed windows procedures
   BOOL                 boOwnerDraw;     // true, if user wants to update window in own WM_PAINT
   int                  XPos;            // horizontal scroll position
   int                  YPos;            // vertical scroll position

   // ddraw data
   void *               DDWinDescr;   // DirectDraw window descriptor
   void *               DDOvlDescr;   // DirectDraw overlay descriptor
   HANDLE               hDDMutex;     // DirectDraw access mutex

   BOOL                 boDisabled;   // display update is not enabled

} TDISPLAY ;

typedef TDISPLAY * LPDISPLAY ;



#define STRUCT_ALIGNMENT 0
#include <align.h>



#ifdef __cplusplus
extern "C" {
#endif

#ifndef MVDISP_DLL
#define _MVDISP_IMPEXP  __declspec(dllimport)
#else /* only for building the DLL */
#define _MVDISP_IMPEXP  __declspec(dllexport)
#endif

// ----------------- function declarations -----------------

_MVDISP_IMPEXP VOID * __stdcall mvDispAllocate( HWND hWnd, LPIMAGE pImage ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispSetLiveWindow( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID * __stdcall mvDispGetLiveWindow( VOID ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispUpdateWindow( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispSetOption( VOID *pDispDescr, DWORD lOption, DWORD lValue ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispFree( VOID *pDispDescr ) ;

_MVDISP_IMPEXP HDC    __stdcall mvDispGetOverlayDC( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispReleaseOverlayDC( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispClearOverlay( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID * __stdcall mvDispSearchHWnd( HWND hWindow );

_MVDISP_IMPEXP VOID   __stdcall mvDispScroll( VOID *pDispDescr, int hpos, int vpos ) ;


/* deleted functions ---------------------

_MVDISP_IMPEXP VOID   __stdcall mvDispUpdateLiveWindow( VOID ) ;
_MVDISP_IMPEXP HDC    __stdcall mvDispLiveGetMemDC( VOID );
_MVDISP_IMPEXP VOID * __stdcall mvDispGetLiveWindowStructPointer( VOID ) ;
_MVDISP_IMPEXP VOID * __stdcall mvDispGetWindowStructPointer( VOID *pDispDescr ) ;
_MVDISP_IMPEXP HDC    __stdcall mvDispLiveCreateOverlayDC( VOID ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispLiveWindowMakeMask( VOID ) ;
_MVDISP_IMPEXP HDC    __stdcall mvDispCreateOverlayDC( VOID *pDispDescr ) ;
_MVDISP_IMPEXP HDC    __stdcall mvDispGetOverlayMemDC( VOID *pDispDescr ) ;
_MVDISP_IMPEXP HDC    __stdcall mvDispGetMemDC( VOID *pDispDescr ) ;
_MVDISP_IMPEXP VOID   __stdcall mvDispWindowMakeMask( VOID *pDispDescr ) ;


------------------------------------------ */
#ifdef __cplusplus
}
#endif


#endif  //  __MVDISP_H__
