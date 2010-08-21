//--------------------------------------------------------------------------
//
// MVCAMDLG.H : header file for module MVCAMDLG.DLL
//
// -----------------------------------------------
//
//	Matrix Vision Demo for PCimageS - Grabber
//
//	01/97       DH
//
//--------------------------------------------------------------------------


#ifndef __MVCAMDLG_H__
#define __MVCAMDLG_H__

#define _MVCAMDLG_VERSION_MAJOR 3
#define _MVCAMDLG_VERSION_MINOR 1
#define _MVCAMDLG_VERSION_STR   "3, 1, 0, 1\0"
#define _MVCAMDLG_VERSION   ((_MVCAMDLG_VERSION_MAJOR<<16) | (_MVCAMDLG_VERSION_MINOR))

#define _MVCAMDLG_DLLNAME "MVCAMDLG.DLL\0" 



#ifdef __cplusplus
extern "C" {
#endif

#ifndef MVCAMDLG_DLL
#define _MVCAMDLG_IMPEXP  __declspec(dllimport)
#else /* only for building the DLL */
#define _MVCAMDLG_IMPEXP  __declspec(dllexport) 
#endif

_MVCAMDLG_IMPEXP VOID __stdcall mvOpenCamtypeDialog( HWND hWnd ) ;

#ifdef __cplusplus
}
#endif

#endif //  __MVCAMDLG_H__
