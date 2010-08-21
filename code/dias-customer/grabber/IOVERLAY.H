//---------------------------------------------------------------------------
#ifndef ioverlayH
#define ioverlayH
//---------------------------------------------------------------------------

#ifndef _IFKT
#define _IFKT __stdcall
#endif

//---------------------------------------------------------------------------
class IOverlay
{
public:		// Interface functions
    virtual HDC  _IFKT  GetDC() = 0;
    virtual void  _IFKT ReleaseDC() = 0;
    virtual void  _IFKT Clear() = 0;
    virtual void  _IFKT Activate() = 0;
    virtual void  _IFKT Deactivate() = 0;
    virtual BOOL  _IFKT IsActive() = 0;
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
_GRABBER_IMPEXP IOverlay* __stdcall GetIOverlay();


typedef IOverlay* (__stdcall * TGetIOverlay )( void );


#ifdef __cplusplus
}
#endif


#endif
