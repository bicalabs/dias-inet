#ifndef __GRABBER_H__
#define __GRABBER_H__

#define _GRABBER_VERSION_MAJOR 3
#define _GRABBER_VERSION_MINOR 53
#define _GRABBER_VERSION_STR   "3, 53, 0, 1\0"
#define _GRABBER_VERSION   ((_GRABBER_VERSION_MAJOR<<16) | (_GRABBER_VERSION_MINOR))

#define _GRABBER_DLLNAME "GRABBER.DLL\0"


#include "mvimage.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GRABBER_DLL
#define _GRABBER_IMPEXP  __declspec(dllimport)
//#define _GRABBER_IMPEXP
#else /* only for building the DLL */
#define _GRABBER_IMPEXP  __declspec(dllexport)
#endif

_GRABBER_IMPEXP HWND __stdcall grabOptions ( void );
_GRABBER_IMPEXP void __stdcall grabSetLiveWindow( HWND hWnd );
_GRABBER_IMPEXP BOOL __stdcall grabInit( void );
_GRABBER_IMPEXP void __stdcall grabClose( void );
_GRABBER_IMPEXP void __stdcall grabLive( void );
_GRABBER_IMPEXP void __stdcall grabStopLive( void );
_GRABBER_IMPEXP LPIMAGE __stdcall grabSnap( void );
_GRABBER_IMPEXP void __stdcall grabCopyToClipboard( void );
_GRABBER_IMPEXP void __stdcall grabSnapToClipboard( void );

_GRABBER_IMPEXP HANDLE __stdcall grabImageToDIB( LPIMAGE pImage );

_GRABBER_IMPEXP void __stdcall grabSetSnapPostMessage( HWND hwnd, UINT Msg, WPARAM wParam );
_GRABBER_IMPEXP void __stdcall grabClearSnapPostMessage( void );

// Version 3.2
// V3.51 12.11.99: return types of the Save and Load function changed from void to BOOL
_GRABBER_IMPEXP BOOL __stdcall grabSaveConfigFile( char* filename );
_GRABBER_IMPEXP BOOL __stdcall grabLoadConfigFile( char* filename );



/* ---------- Function Pointer Types --------------------- */
typedef HWND    (__stdcall * TgrabOptions )( void ) ;
typedef void    (__stdcall * TgrabSetLiveWindow )( HWND ) ;
typedef BOOL    (__stdcall * TgrabInit )( void ) ;
typedef void    (__stdcall * TgrabClose)( void ) ;
typedef void    (__stdcall * TgrabLive )( void ) ;
typedef void    (__stdcall * TgrabStopLive )( void ) ;
typedef LPIMAGE (__stdcall * TgrabSnap )( void ) ;
typedef void    (__stdcall * TgrabCopyToClipboard )( void ) ;
typedef void    (__stdcall * TgrabSnapToClipboard )( void ) ;
typedef HANDLE  (__stdcall * TgrabImageToDIB )( LPIMAGE ) ;
typedef void    (__stdcall * TgrabSetSnapPostMessage )( HWND hwnd, UINT Msg, WPARAM wParam );
typedef void    (__stdcall * TgrabClearSnapPostMessage )( void );

typedef void    (__stdcall * TgrabSaveConfigFile )( char* );
typedef void    (__stdcall * TgrabLoadConfigFile )( char* );

#ifdef __cplusplus
}
#endif


#endif
