//---------------------------------------------------------------------------
/*
    Changes:
        Version 3.32:
          uf/17.3.99: first implementation


    Notes:
        Ask the Grabber interface for an error handler object with
        IGrabber::GetIError()      
*/

#ifndef IErrorH
#define IErrorH


#ifndef _IFKT
#define _IFKT __stdcall
#endif

#define ERRID_MV_DRV      0x00100
#define ERRMODULE_MV_DRV  "MV-DRV"

#define ERRID_WINDOWS      0x00200
#define ERRMODULE_WINDOWS  "WINDOWS"

#define ERRID_ACQCTRL      0x00300
#define ERRMODULE_ACQCTRL  "ACQCTRL"


typedef enum { etWarning, etError, etFatal, etLast } TErrorType;

class IError
{
public:
    virtual DWORD _IFKT CheckErr() = 0;        // check's new error's
    virtual BOOL  _IFKT IsError() = 0;         // TRUE if there is an unhandled errorcode
    virtual DWORD _IFKT GetErrCode() = 0;      // returns the error code of this module
    virtual TErrorType _IFKT GetErrType() = 0; // returns the error type (warning, error or fatal )
    virtual void  _IFKT Clear() = 0;           // only clears the stored error code
    virtual DWORD _IFKT GetModuleID() = 0;     // give the modul id number
    virtual char * _IFKT GetModuleName() = 0;  // returns module name
    virtual char * _IFKT GetErrCodeText() = 0; // return description of the last error code
    virtual int _IFKT GetErrString( char* pStr, int nMaxStringLen ) = 0;
    virtual char * _IFKT GetLastErrInfo() = 0;
    virtual void _IFKT SetErrorFromObj( char * reference, int line, IError* pErr ) = 0;
    virtual void _IFKT SetError( char * reference, int line, TErrorType errType, DWORD errCode, char * format,...) = 0;
    virtual void _IFKT SetReference( char * reference, int line ) = 0;
};




#ifndef GRABBER_DLL
#define _GRABBER_IMPEXP  __declspec(dllimport)
#else /* only for building the DLL */
#define _GRABBER_IMPEXP  __declspec(dllexport)
#endif



//---------------------------------------------------------------------------
#endif  // IErrorH

