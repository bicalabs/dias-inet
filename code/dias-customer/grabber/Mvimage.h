#ifndef __MVIMAGE_H__
#define __MVIMAGE_H__


#define _MVIMAGE_VERSION_MAJOR 3
#define _MVIMAGE_VERSION_MINOR 2
#define _MVIMAGE_VERSION_STR   "3, 2, 0, 1\0"
#define _MVIMAGE_VERSION   ((_MVIMAGE_VERSION_MAJOR<<16) | (_MVIMAGE_VERSION_MINOR))

#define _MVIMAGE_DLLNAME "MVIMAGE.DLL\0" 



#ifndef _WINDOWS_
typedef void *HANDLE;
#endif


// flags
#define IMGF_MODIFIED  1     // image is modified and should by saved
#define IMGF_ALLOCATED 2     // 
#define IMGF_GREYSCALE 4    
#define IMGF_DIRTY     8     // image doesn't have legal image 

// macros
#define imgModify(pimg)   do { (pimg)->sFlags |= IMGF_MODIFIED; } while(0)
#define imgIsModified(pimg) { (pimg)->sFlags & IMGF_MODIFIED }
#define imgIsAllocated(pimg) ( (pimg)->sFlags & IMGF_ALLOCATED )
#define imgClean(pimg)    do { (pimg)->sFlags &= ~IMGF_MODIFIED; } while(0)
#define imgLine(pimg,y)   ((pimg)->pLine[y])

#define imgPitch(pimg)   ((pimg)->lPitch)
#define imgWidth(pimg)   ((pimg)->lWidth)
#define imgHeight(pimg)  ((pimg)->lHeight)
#define imgBPP(pimg)     ((pimg)->sBitsPerPixel)


#define STRUCT_ALIGNMENT 8
#include "align.h"

/* Image structure describes the hole image or an AOI */
typedef struct
{
   unsigned char   *pData ;            // pointer to image data (always start of AOI)
   unsigned char  **pLine ;            // pointer to array of linepointer (each linestart of AOI)
   unsigned long    lPitch ;           // number of bytes to next line
   unsigned long    lWidth ;           // image width in pixel
   unsigned long    lHeight ;          // image height in pixel
   unsigned long    lStartX ;          // image width in pixel
   unsigned long    lStartY ;          // image height in pixel
   unsigned short   sBytesPerPixel ;   // number of bytes to next pixel
   unsigned short   sFlags ;           // flags ( MODIFIED )
   unsigned short   sBitsPerPixel;     // number of bits per pixel
}  TIMAGE, * LPIMAGE ;


#define STRUCT_ALIGNMENT 0
#include "align.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MVIMAGE_DLL
#define _MVIMAGE_IMPEXP  __declspec(dllimport) 
#else /* only for building the DLL */
#define _MVIMAGE_IMPEXP  __declspec(dllexport)
#endif




_MVIMAGE_IMPEXP LPIMAGE __stdcall imgNew      ( void );
_MVIMAGE_IMPEXP LPIMAGE __stdcall imgDelete   ( LPIMAGE pimg );
_MVIMAGE_IMPEXP LPIMAGE __stdcall imgAOI      ( LPIMAGE pimg,
                      unsigned long x,
                      unsigned long y,
                      unsigned long Width,
                      unsigned long Height );
_MVIMAGE_IMPEXP void    __stdcall imgSetup    ( LPIMAGE pimg,
                      unsigned long Width,
                      unsigned long Height,
                      unsigned short bpp,
                      unsigned long pitch,
                      void * ptr,
                      unsigned short flags );
_MVIMAGE_IMPEXP LPIMAGE __stdcall imgCreate   ( unsigned long Width,
                      unsigned long Height,
                      unsigned short bpp,
                      unsigned short align,
                      void * ptr );
_MVIMAGE_IMPEXP LPIMAGE __stdcall imgDuplicate( LPIMAGE pimgSrc );
_MVIMAGE_IMPEXP void __stdcall imgCopyDescription( LPIMAGE pimgDest, LPIMAGE pimgSrc );

_MVIMAGE_IMPEXP void __stdcall imgMakeSameSize( LPIMAGE pimgSrc, LPIMAGE pimgDest );
_MVIMAGE_IMPEXP void __stdcall imgSetupLinePointers( LPIMAGE pimg );

_MVIMAGE_IMPEXP unsigned long __stdcall imgGetPixel( LPIMAGE pimg, unsigned long x, unsigned long y );
_MVIMAGE_IMPEXP unsigned long __stdcall imgSetPixel( LPIMAGE pimg, unsigned long x, unsigned long y, unsigned long value );

// ---------------------- DIB functions ----------------------------------------- */
_MVIMAGE_IMPEXP HANDLE __stdcall imgCreateDIB( LPIMAGE pimg, int boLineInverse );
_MVIMAGE_IMPEXP void * _stdcall imgGetDIBBits( HANDLE hDIB );


#ifdef __cplusplus
}
#endif

#endif // __MVIMAGE_H__
