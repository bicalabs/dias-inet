//---------------------------------------------------------------------------
/*
    Global C++ Interface for the Preview Window

    Changes:
        Version 3.30:
          uf/05.02.99: First implementation

*/

#ifndef IPrevWndH
#define IPrevWndH

#define _PREVIEWWINDOW_VERSION_MAJOR 3
#define _PREVIEWWINDOW_VERSION_MINOR 30
#define _PREVIEWWINDOW_VERSION   ((_PREVIEWWINDOW_VERSION_MAJOR<<16) | (_PREVIEWWINDOW_VERSION_MINOR))


#ifndef _IFKT
#define _IFKT __stdcall
#endif


class IPreviewWindow
{
public:
    virtual DWORD _IFKT GetVersion() = 0;  // version of this interface
    virtual void  _IFKT Show() = 0;  // shows the Preview Window
    virtual void  _IFKT Hide() = 0;  // hides the Preview Window
    virtual void  _IFKT SetLiveWindow( HWND hWnd  ) = 0;  //
    virtual void  _IFKT UpdateDisplay(BOOL boImediate = TRUE  ) = 0;
};


//---------------------------------------------------------------------------
#endif  // IPrevWndH
