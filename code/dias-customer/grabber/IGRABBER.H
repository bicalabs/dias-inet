//---------------------------------------------------------------------------
/*
    Changes:
        Version 3.25:
          uf/25.1.99: Function GetImage(int) changed to GetImageNo(int)

*/

#ifndef IGrabberH
#define IGrabberH

#include <windows.h>

#include "mvimage.h"
#include "igevent.h"
#include "ierror.h"

#ifndef _IFKT
#define _IFKT __stdcall
#endif

#ifndef __MV_H
#define LPDEV void*
#else
#define LPDEV DEV_T*
#endif

typedef enum { SoftLive=1, HardLive, DirectDraw } TLiveMode ;
typedef enum {Odd=1,Even,NextField} EStartMode;


//---------------------------------------------------------------------------
class IGrabber
{
public:		// Interface functions
    virtual void _IFKT SetGrabberType( int GrabberType ) = 0;
    virtual void _IFKT SetSnapSize( unsigned long xsize, unsigned long ysize ) = 0;
    virtual void _IFKT SetLiveSize( unsigned long xsize, unsigned long ysize ) = 0;
    virtual void _IFKT GetSnapSize( unsigned long *xsize, unsigned long *ysize ) = 0;
    virtual void _IFKT GetLiveSize( unsigned long *xsize, unsigned long *ysize ) = 0;
    virtual BOOL _IFKT Init() = 0;
    virtual void _IFKT Close() = 0;
    virtual void _IFKT Live() = 0;
    virtual void _IFKT StopLive() = 0;
    virtual LPIMAGE _IFKT Snap() = 0;
    virtual LPIMAGE _IFKT GetImage() = 0;
    virtual BOOL _IFKT IsInitialized() = 0;
    virtual WORD _IFKT SelChannel( WORD channel ) = 0;
    virtual WORD _IFKT IncChannel( int increment ) = 0;
    virtual WORD _IFKT SelSyncChannel( WORD channel ) = 0;
    virtual WORD _IFKT IncSyncChannel( int increment ) = 0;
    virtual BOOL _IFKT SetVideoRecoder( BOOL boSet ) = 0;
    virtual char* _IFKT Command( char * CmdString ) = 0;
    virtual void _IFKT RestartLive() = 0;
    virtual BOOL _IFKT SetLiveScale( BOOL boScaleOn ) = 0;
    virtual void _IFKT UpdateCamera() = 0;
    virtual void _IFKT UpdateAll() = 0;
    virtual void _IFKT SetDlgHWnd( HWND hwndDlg ) = 0;
    virtual WORD _IFKT SetContrast( WORD Contrast ) = 0;
    virtual WORD _IFKT SetBrightness( WORD Brightness ) = 0;
    virtual void  _IFKT UpdateDisplayPalette() = 0;
    virtual BOOL  _IFKT IsLive() = 0;
    virtual int   _IFKT GetCameraFieldTime() = 0;
    virtual int   _IFKT GetFieldsPerFrame() = 0;
    virtual LPDEV _IFKT GetDeviceHandle() = 0;
    virtual WORD  _IFKT SetSnapTimeout( WORD ms_time ) = 0;
    virtual void  _IFKT SetCameraSize( unsigned long xsize, unsigned long ysize ) = 0;
    virtual void  _IFKT GetCameraSize( unsigned long *xsize, unsigned long *ysize ) = 0;

    /* Functions added on 13.11.98 */
//    virtual LPIMAGE _IFKT GetImage(int nr) = 0;  // 25.1.99 name changed
    virtual LPIMAGE _IFKT GetImageNo(int nr) = 0;

    virtual int  _IFKT SelSnapBuffer( int nr ) = 0;  // changes dma buffer position
    virtual int  _IFKT IncSnapBuffer( int nr ) = 0;  // changes dma buffer position

    virtual int  _IFKT GetMaxSnapBuffer( void ) = 0;

    virtual int  _IFKT SelDisplayBuffer( int nr ) = 0;  //
    virtual int  _IFKT IncDisplayBuffer( int nr ) = 0;  //

    virtual BOOL _IFKT IsSnapReady( void ) = 0;
    virtual BOOL _IFKT WaitForSnapReady( void ) = 0;

    virtual int _IFKT Record(int ImageCount, BOOL boDisplay, HANDLE hEvImgFinish=0 ) = 0;
    virtual void _IFKT SnapAsync( HANDLE hEventFinish ) = 0;
    virtual BOOL _IFKT SetTrigger( BOOL boOn ) = 0;
    virtual DWORD _IFKT SetTriggerTimeout( DWORD msTime ) = 0;

    /* Functions added on 27.01.99 */
    virtual void _IFKT SetIniFilename( char * pathname ) = 0;
    virtual void _IFKT SetSnapStartmode(EStartMode mode) = 0;
    virtual IGrabEvent* _IFKT CreateGrabEvent( TGrabEvent type ) = 0;

    /* Functions added on 26.02.99 */
    virtual LPIMAGE _IFKT Accumulate(int ImageCount, int mode ) = 0;
    virtual void _IFKT SetLiveMode( TLiveMode livemode, BOOL boRestartLive = TRUE  ) = 0;

    /* Functions added on 16.03.99 */
    virtual BOOL _IFKT SelCamera( char *strCameraName ) = 0;
    virtual IError * _IFKT GetIError() = 0;

    /* functions added on 23.09.99 */
    virtual void _IFKT SetGreyScaleAcquire( BOOL boOn ) = 0;
    virtual BOOL _IFKT GetGreyScaleAcquire( void ) = 0;


};

#ifndef GRABBER_DLL
#define _GRABBER_IMPEXP  __declspec(dllimport)
//#define _GRABBER_IMPEXP
#else /* only for building the DLL */
#define _GRABBER_IMPEXP  __declspec(dllexport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

// return the actual selected Grabber
_GRABBER_IMPEXP IGrabber* __stdcall GetIGrabber();

typedef IGrabber* (__stdcall * TGetIGrabber )( void );


#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
#endif

