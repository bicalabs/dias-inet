//--------------------------------------------------------------------------
//
// MVACQU.H :  user interface functions for MVACQU-DLL
//
// -----------------------------------------------
//
//	Matrix Vision Demo for PCimageS - Grabber
//
//	01/97       DH
//
//--------------------------------------------------------------------------

#ifndef __MVACQU_H__
#define __MVACQU_H__


#define _MVACQU_VERSION_MAJOR 3
#define _MVACQU_VERSION_MINOR 3
#define _MVACQU_VERSION_STR   "3, 3, 0, 1\0"
#define _MVACQU_VERSION   ((_MVACQU_VERSION_MAJOR<<16) | (_MVACQU_VERSION_MINOR))

#define _MVACQU_DLLNAME "MVACQU.DLL\0" 

#include <mvdisp.h>

#define STRUCT_ALIGNMENT 8
#include <align.h>

typedef enum 
{ 
   // PCimage-Series
   E_FG_PCIS_C = 0, 
   E_FG_PCIS_G, 
   E_FG_PCIS_RGB, 
   E_FG_PCIS_DIG,
   
   // MVsigma-Series
   E_FG_PCIS_LC, 
   E_FG_PCIS_LG, 
   E_FG_PCIS_LD, 

   // MVcam-Cameras
   E_CAM_PCCD,
   E_CAM_PCMOS,

   // MVtitan-Series
   E_FG_TITAN_C16,
   E_FG_TITAN_G1,

   E_FG_SIMULA,

   E_FG_ACTUAL,
   E_FG_LAST
} TFRAMEGRABBER ;

typedef enum 
{ 
   E_AO_WRITEREGWORD
} TACQUIREOPTIONS ;

#define E_WRW_SET_VTR_BIT 0x00000001 

#define E_WRW_RESET_VTR_BIT 0x80000001 


// ----------------- monitor struct -----------------

typedef struct
{
   unsigned int VertRes ;
   unsigned int HorzRes ;
   unsigned int BitsPixel ;
   THICOLOR     HiColor ;
} TMONITOR, * LPMONITOR ;


// ----------------- grabber capabilities -----------------

typedef struct
{
  BOOL boHardlivePosible;         // is Hardlive actualy possible
  TLIVEMODE OptimalLiveMode;      // optimal Softlive mode (DIB or DDB)
  BOOL boRGBGrabber;              // Is RGB VideoInput available
  BOOL boVTR;                     // Is a special Video Tape Recoder Mode programable
  WORD wInputChannels;            // number of VideoInputChannels
  BOOL boHWScaler;                // has frame grabber have a hardware scaling feature
  BOOL boColor;                   // is a color grabber
  WORD wLiveColorMode;            // Colormode for color live
  WORD wLiveGreyMode;             // Colormode for greyscale live
  WORD wSnapColorMode;            // Colormode for color singlesnap
  WORD wSnapGreyMode;             // Colormdoe for greyscale snap
  BOOL boDigital;                 // Is digital Grabber
} TGRABCAPS, *LPGRABCAPS;


typedef void (__stdcall *ACQUIRE_CALLBACK) (void);

#define STRUCT_ALIGNMENT 0
#include <align.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifndef MVACQU_DLL
#define _MVACQU_IMPEXP  __declspec(dllimport) 
#else /* only for building the DLL */
#define _MVACQU_IMPEXP  __declspec(dllexport)
#endif

// user interface functions for MVACQU-DLL

_MVACQU_IMPEXP int    __stdcall mvOpenFrameGrabber( unsigned short FrameGrabberType, char *IniFileName ) ;
_MVACQU_IMPEXP int    __stdcall mvCloseFrameGrabber( void ) ;
_MVACQU_IMPEXP int    __stdcall mvCloseAllFrameGrabber( void ) ;

_MVACQU_IMPEXP void   __stdcall mvInitAcquireDef( WORD xOffset, WORD yOffset, 
                                                  WORD xSize, WORD ySize, 
                                                  TLIVEMODE liveMode, 
                                                  TSCALERMODE scalerMode, 
                                                  THICOLOR hiColor ) ;
_MVACQU_IMPEXP void   __stdcall mvInitAcquirePos( WORD xPos, WORD yPos, TLIVEMODE liveMode ) ;
_MVACQU_IMPEXP void   __stdcall mvGetCurrentAcquireData( WORD *xPos, WORD *yPos, WORD *xOffs, WORD *yOffs, WORD *xSize, WORD *ySize, WORD *Pitch ) ;
_MVACQU_IMPEXP void   __stdcall mvGetMaxAcquireSize( WORD *xMaxSize, WORD *yMaxSize ) ;
                                       
_MVACQU_IMPEXP int    __stdcall mvSelectFrameGrabber( short devNr ) ;
_MVACQU_IMPEXP char * __stdcall mvGetDeviceIniFileName( VOID ) ;
_MVACQU_IMPEXP void   __stdcall mvGetSectionName( char *section ) ;
_MVACQU_IMPEXP void * __stdcall mvGetDeviceHandle( void ) ;
_MVACQU_IMPEXP void * __stdcall mvGetAddress( void ) ;

_MVACQU_IMPEXP VOID   __stdcall mvSetTimerTimeInMS( UINT ttinms ) ;
_MVACQU_IMPEXP void   __stdcall mvStartLive( HWND hWnd, HINSTANCE hInst ) ;
_MVACQU_IMPEXP void   __stdcall mvStopLive( void ) ;
_MVACQU_IMPEXP VOID   __stdcall mvAbortLive( VOID ); 
_MVACQU_IMPEXP BOOL   __stdcall mvIsLive( VOID );
_MVACQU_IMPEXP void   __stdcall mvMakeSingleSnap( WORD wait ) ;
_MVACQU_IMPEXP ACQUIRE_CALLBACK  __stdcall mvSetAcquireCallback( ACQUIRE_CALLBACK callback_fkt );

_MVACQU_IMPEXP void * __stdcall mvGetDeviceErrStr( void ) ;
_MVACQU_IMPEXP void   __stdcall mvClearDeviceError( void ) ;

_MVACQU_IMPEXP void   __stdcall mvFillTImage( TIMAGE *ptrImg ) ;
_MVACQU_IMPEXP void   __stdcall mvCopyTImage( TIMAGE *srcImg, TIMAGE *dstImg )  ;
                              
_MVACQU_IMPEXP void   __stdcall mvUpdateImage( void )  ;
_MVACQU_IMPEXP void   __stdcall mvUpdate16BitHiColor( void ) ;
_MVACQU_IMPEXP void   __stdcall mvConvert15To16Bpp ( WORD *src, WORD *dst, int w, int h ) ;


// user interface functions for MVACQU-DLL
// some mor functions general functions

_MVACQU_IMPEXP void   __stdcall mvSetScaler( BOOL bScalerOn ) ;
_MVACQU_IMPEXP BOOL   __stdcall mvIsScalerOn( void );
_MVACQU_IMPEXP void   __stdcall mvSetScalerFilter( BOOL bAdaptFilterOn, BOOL bAdaptGeoFilterOn ) ;

_MVACQU_IMPEXP WORD   __stdcall mvSetSelInputChannel( WORD ic ) ;
_MVACQU_IMPEXP WORD   __stdcall mvSetSelVideoMode( WORD vm ) ;
_MVACQU_IMPEXP WORD   __stdcall mvSetSelColorMode( WORD cm ) ;
_MVACQU_IMPEXP WORD   __stdcall mvSetSelSyncInputChannel( WORD sic ) ;

_MVACQU_IMPEXP void * __stdcall mvGetActCameraTypePointer( void ) ;
_MVACQU_IMPEXP void * __stdcall mvGetFirstCameraTypePointer( void ) ;
_MVACQU_IMPEXP WORD   __stdcall mvGetNumOfCameraTypesDefined( void ) ;
_MVACQU_IMPEXP void * __stdcall mvGetCameraTypePointer( char *cameraName )  ;
_MVACQU_IMPEXP void   __stdcall mvDefNewCamtype( char *cameraName ) ;
_MVACQU_IMPEXP void   __stdcall mvSelCameraType( char *cameraName ) ;
                              
_MVACQU_IMPEXP void   __stdcall mvGetBoardInfo( void *info, BOOL bSowBoardInfo ) ;

_MVACQU_IMPEXP char * __stdcall mvGetLastUsedIniFileName( VOID ) ;
_MVACQU_IMPEXP char * __stdcall mvInterprString( char *istring ) ;

_MVACQU_IMPEXP void   __stdcall mvAcquireSetOption( DWORD lOption, DWORD lValue ) ;
_MVACQU_IMPEXP void   __stdcall mvSaveBitmap( char *filename ) ;


_MVACQU_IMPEXP VOID __stdcall mvSetLogOutput( char *logfilename, BOOL ClearFile ) ;
_MVACQU_IMPEXP int  __stdcall mvOutputMessage( HWND hwnd, char *messagetext, char *infotext, UINT parameter ) ;
_MVACQU_IMPEXP VOID __cdecl mvPrintInfo( LPSTR szFormat, ... ) ;
#ifdef _MSC_VER
_MVACQU_IMPEXP VOID __cdecl   _mvTraceProc( WORD TraceLevel, LPSTR szModul, LPSTR szFormat, ... );
#define mvTrace _mvTraceProc
#else
_MVACQU_IMPEXP VOID __cdecl   mvTraceProc( WORD TraceLevel, LPSTR szModul, LPSTR szFormat, ... );
#define mvTrace mvTraceProc
#endif
_MVACQU_IMPEXP VOID __stdcall mvSetTraceLevel( WORD TraceLevel );
_MVACQU_IMPEXP WORD __stdcall mvGetPixelAlignment( VOID );
_MVACQU_IMPEXP BOOL __stdcall mvGetCameraSize( WORD *xsize, WORD *ysize );


// Tools
_MVACQU_IMPEXP LPMONITOR  __stdcall mvGetMonitorCaps( VOID );
_MVACQU_IMPEXP LPGRABCAPS __stdcall mvGetGrabberCaps( VOID );

// new at V3.2
_MVACQU_IMPEXP BOOL __stdcall mvSuspendLive( BOOL Immediate ); 
_MVACQU_IMPEXP VOID __stdcall mvResumeLive( VOID );

// new at V3.3	uf/28.01.99
_MVACQU_IMPEXP DWORD __stdcall mvGetImageBufferSize( VOID );


#ifdef __cplusplus
}
#endif

#endif //  __MVACQU_H__
