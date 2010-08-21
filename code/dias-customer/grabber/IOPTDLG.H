//---------------------------------------------------------------------------
/*
    Global C++ Interface for the Options Dialog Box

    Changes:
        Version 3.30:
          uf/05.02.99: First implementation

*/

#ifndef IOptDlgH
#define IOptDlgH

#define _OPTIONDIALOG_VERSION_MAJOR 3
#define _OPTIONDIALOG_VERSION_MINOR 30
#define _OPTIONDIALOG_VERSION   ((_OPTIONDIALOG_VERSION_MAJOR<<16) | (_OPTIONDIALOG_VERSION_MINOR))


#ifndef _IFKT
#define _IFKT __stdcall
#endif


class IOptionDialog
{
public:
    virtual DWORD _IFKT GetVersion() = 0;  // version of this interface
    virtual void  _IFKT Show() = 0;  // shows the Dialog
    virtual void  _IFKT Hide() = 0;  // hides the Dialog
};


//---------------------------------------------------------------------------
#endif  // IOptDlgH
