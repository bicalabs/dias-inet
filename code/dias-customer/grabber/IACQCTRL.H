//---------------------------------------------------------------------------
/*
    Global C++ Interface for the MATRIX Vision Acquire Control

    Features:
        - Multiple Frame Grabber usage
        - Access to Option Dialogs
        - Access to Frame Grabber
        - Access to Preview Window
        - Access to Error Handler     (later)
        - Access to conected Cameras  (later)

    Changes:
        Version 3.30:
          uf/05.02.99: First implementation

*/

#ifndef IAcqCtrlH
#define IAcqCtrlH

#include "igrabber.h"
#include "iOptDlg.h"
#include "iPrevWnd.h"

#define _ACQUIRECONTROL_VERSION_MAJOR 3
#define _ACQUIRECONTROL_VERSION_MINOR 30
#define _ACQUIRECONTROL_VERSION   ((_ACQUIRECONTROL_VERSION_MAJOR<<16) | (_ACQUIRECONTROL_VERSION_MINOR))


#ifndef _IFKT
#define _IFKT __stdcall
#endif

//---------------------------------------------------------------------------
class IAcquireControl
{
public:		// Interface functions
    virtual DWORD           _IFKT GetVersion() = 0;  // version of this interface
    virtual IGrabber*       _IFKT GetIGrabber() = 0;
    virtual IOptionDialog*  _IFKT GetOptionDlg() = 0;
    virtual IPreviewWindow* _IFKT GetPreviewWindow() = 0;
};

#ifndef GRABBER_DLL
#define _GRABBER_IMPEXP  __declspec(dllimport)
#else /* only for building the DLL */
#define _GRABBER_IMPEXP  __declspec(dllexport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

// creates a new Acquire Control
_GRABBER_IMPEXP IAcquireControl* __stdcall CreateAcquireControl( char* AcquireControlName );
_GRABBER_IMPEXP void __stdcall DeleteAcquireControl( IAcquireControl* AcqCtrl );

typedef IAcquireControl* (__stdcall * TCreateAcquireControl )( char* );
typedef void (__stdcall * TDeleteAcquireControl )( IAcquireControl* );


#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
#endif  // IAcqCtrlH
