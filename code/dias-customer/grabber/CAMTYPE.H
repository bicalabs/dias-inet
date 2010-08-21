/* Camera Definitions 

   Handling camera dependent properties:
   -------------------------------------
   The camera definition could also define a DLL which includes a dialogbox 
   to modify specific camera properties. The DLL name an the function name are
   defined by the function mvDefPropDialog(). It could also be called in the 
   camdefs.ini file.
   The property dialog function could connect to the camera, read out actual 
   setting and modify those setting. In some cases it is neccessary to make
   change in the cameradefintion stucture, for example when changing the binning 
   mode of the camera. The property dialog function makes this changes and calls 
   an callback function to update the frame grabber settings.
   The property dialog function prototype is defined as CPDLG_FUNCTION below.
   The callback function is defined as CPDLG_CALLBACK. 
   It's possible to pass an application specific pointer to that callback 
   function. That could be used to pass the C++ object pointer.

   The camera definition module only holds the DLL and function name. It doesn't
   call this functions. An application could call the dialog function if it is
   defined for the actual camera.

   The camera dialog stores its data into the  CAMTYPE_T::PropData

*/

#ifndef __CAMTYPE_H__
#define __CAMTYPE_H__

#define CT_NAME_LENGTH  40
#define CT_VO_RS170

#define STRUCT_ALIGNMENT 8
#include <align.h>

typedef struct property_data_s
{
    DWORD count;                  /* number of properties in the data block       */
    char  FAR* FAR* names;        /* pointer to a string list with property names */
    DWORD FAR* data;              /* memory holder for property data              */
} CAMPROP_T, FAR* LPCAMPROP_T;


typedef struct camtype_t
{
    /* CAMERA DATA */
    char Name[CT_NAME_LENGTH];   /* name of the camera                             */
    WORD VideoMode;              /*                                                */
    WORD Interlaced;             /* 0: NONINTERLACED 1:INTERLACED 2:INV_INTERLACED */
    WORD VerticalFreq;           /* Vertical scan frequency in Hz                  */
    WORD HorizontalFreq;         /* Horizontal scan frequency in Hz                */
    WORD ClockFreq;              /* Clock frequency in kHz                         */
    WORD ClockMode;              /* 0: internal; 1: external                       */

    /* ACQUIRE SETUP */
    WORD AcquireMode;            /* 0: Composite Sync; 1: Variable Scan            */
    WORD InvertSync;             /* Bit 0: pixelclock invert;
                                    Bit 1: line sync invert;
                                    Bit 2: frame sync invert;                      */
    WORD StartMode;              /* 1: Odd; 2: Even; 3: Next                       */
    WORD ChrominanceFilter;      /* 0: AC coupled; 1: 50Hz; 2: 60Hz; 3: DC coupled */
    WORD Gain[32];               /* 0: 0.5; 1: 1; 2: 2; 3: 4;                      */
    INT  Offset[32];             /* in mV                                          */
    WORD LowReference[32];       /* in mV                                          */
    WORD HighReference[32];      /* in mV                                          */

    /* TIMING RAM SETUP */
    WORD HTParameterMode;        /* horizontal parameter 0: in pixel 1: in ns;     */
    DWORD HTAcquireStart;        /* acquire start position                         */
    DWORD HTAcquireWidth;        /* acquire width                                  */
    WORD HTAcquireIncrement;     /* acquire increment                              */

    DWORD HTClampStart;          /* clamp start position                           */
    DWORD HTClampWidth;          /* clamp width 0: set to default 5                */
    DWORD HTZeroStart;           /* zero start position                            */
    DWORD HTZeroWidth;           /* zero width 0: set to default 5                 */
    DWORD HTFieldStart;          /* field gate start position 0: auto              */
    DWORD HTFieldWidth;          /* field gate width 0: auto                       */

    WORD  VTParameterMode;       /* vertical parameter 0: in lines 1: in us;       */
    DWORD VTAcquireStart;        /* acquire start position                         */
    DWORD VTAcquireWidth;        /* acquire width                                  */
    WORD  VTAcquireIncrement;    /* acquire increment                              */

    WORD  GenerateVSync;         /* 0 : no; 1 : yes                                */
    DWORD GenerateVSyncStart;    /* Startposition of vsync                         */
    DWORD GenerateVSyncWidth;    /* Width of Sync pulse                            */
    WORD  GenerateVSyncPol;      /* Sync polarity: 0:Sync is low; 1:Sync is high   */

    /* new elements from 9.9.99 */
    DWORD HTAcquireTotal;        /* time or pixel between two hsync's              */
    WORD  PhysCamType;           /* bit0: digital, bit1: linescan                  */
    char *Manufacturer;          /* name of the camera manufactor                  */
    char *PropDlgDLL;            /* DLL filename with properties dialog            */
    char *PropDlgFuncName;       /* DLL function name of property dialog           */
    CAMPROP_T PropData;          /* Property data, camera dependent                */                                 
    char *Comment;               /* additional information text                    */

} CAMTYPE_T, FAR* LPCAMTYPE_T;


#define STRUCT_ALIGNMENT 0
#include <align.h>

/* ============ Property definitions ================== */
typedef void (_MVEXPCALL *CPDLG_CALLBACK) 
                     ( DEV_T FAR* dev,        /* device handle */
                       void * UserPointer,    /* user pointer which was passed to 
                                                 the dialog function */
                       LPCAMTYPE_T pCamera ); /* camera definition struct */

#define CPDLGM_SHOWDIALOG       0  /* The function presents the dialogbox and calls the 
                                      callback function after the Apply or OK button 
                                      is pressed and it had to change the 
                                      camera definition                             */
#define CPDLGM_SHOWAUTODIALOG   1  /* The function presents the dialogbox and calls the 
                                      callback function each time when it had to change  
                                      the camera definition                          */
#define CPDLGM_IN_PROPERTIES    2  /* The funciton reads the actual camera settings 
                                      without showing the dialog, it calls the callback
                                      if it had to change the camera definition     */
#define CPDLGM_OUT_PROPERTIES   3  /* The function writes the camera setting to the
                                      camera without showing the dialog             */ 

typedef BOOL (_MVEXPCALL * CPDLG_FUNCTION) /* returns true if the dialog is closed 
                                              with OK, every parameter could be 0 */
                     ( void * hwnd,        /* parent window handle */
                     CPDLG_CALLBACK callbackfkt,
                     DEV_T FAR* dev,       /* device handle */
                     void * UserPointer,   /* user pointer which will be passed 
                                              to the callback function */
                     LPCAMTYPE_T pCamera,  /* camera definition struct */
                     DWORD fmode  );       /* mode flag */

/* =============== General defintions ==================== */

#define CT_CAM_DEF_ENTRIES  (VM_max+1)


#define CAM_VIDEO_MODES   {"VM_AUTO",   "VM_CCIR",      "VM_RS170",     "VM_PAL",       "VM_NTSC",\
                           "VM_SECAM",  "VM_SVHS",      "VM_VSCAN",     "VM_CCIR576",   "VM_PAL576",\
                           "VM_RGB",    "#11",          "#12",          "#13",          "VM_DIG8",\
                           "VM_DIG10",  "VM_DIG12",     "VM_DIG16",     "VM_DIG24",     "VM_DIG32",\
                           "VM_TVES",   "VM_NTSC_SVHS", "VM_SECAM_SVHS",\
                           NULL}

#define CAM_INTERL_MODES   {"NONINTERLACED","INTERLACED","INV_INTERLACED",NULL}
#define CAM_ACQUIRE_MODES  {"STANDARD","VSCAN",NULL}
#define CAM_START_MODES    {"","ODD","EVEN","NEXT_FIELD",NULL}
#define CAM_FILTER         {"AC","PAL","NTSC","DC",NULL}
#define CAM_UNIT_MODES     {"PIXEL","TIME","LINES",NULL}
#define CAM_CLOCK_MODES    {"PCLK_INTERN","PCLK_EXTERN",NULL}
#define CAM_INVERT_MODES   {"NOT_INV","INV_PCLK","INV_SYNC","INV_SYNC&PCLK","INV_HSYNC","INV_HSYNC&PCLK","", "", "INV_VSYNC","INV_VSYNC&PCLK", NULL}
#define CAM_PHYS_CAM_TYPES {"ANALOG","DIGITAL","ANALOG_LINESCAN","DIGITAL_LINESCAN", NULL}

/* Bitmasks for Invert options */
#define PICINV        1
#define SYNCINV_HV    2
#define SYNCINV_H     4
#define SYNCINV_V     8


extern CAMTYPE_T default_camdef [CT_CAM_DEF_ENTRIES] ;

/* Macros : */

#define _PClk(p)    (((double)((p)->ClockFreq))/1000.0)       /* in MHz */
#define _HClk(p)    (((double)((p)->HorizontalFreq))/1000.0)  /* in kHz */
#define _VClk(p)    ((p)->VerticalFreq)                       /* in Hz  */

#define _PixToTime(p,v)  ((DWORD)(( 1000.0 * ((double)v) / _PClk(p)  )+0.5)) /* in ns */
#define _LineToTime(p,v) ((DWORD)(( 1000.0 * ((double)v) / _HClk(p)  )+0.5)) /* in us */

#define _TimeToPix(p,v)  ((DWORD)((  ((double)v) * _PClk(p) / 1000.0  )+0.5))/* in Pixel */
#define _TimeToLine(p,v) ((DWORD)((  ((double)v) * _HClk(p) / 1000.0  )+0.5))/* in Zeilen */

#define _GetHPix(p,v) (((p)->HTParameterMode  == 1) ? _TimeToPix(p,v) : (v) )
#define _GetHTime(p,v) (((p)->HTParameterMode != 1) ? _PixToTime(p,v) : (v) )

#define _GetVLine(p,v) (((p)->VTParameterMode == 1) ? _TimeToLine(p,v) : (v) )
#define _GetVTime(p,v) (((p)->VTParameterMode != 1) ? _LineToTime(p,v) : (v) )

#define _HP(v)  (_GetHPix(pCam,pCam->v))
#define _VL(v)  (_GetVLine(pCam,pCam->v))

/* Update definitions */

#define CAM_UPDATE_VIDEOMODE        0x00000001UL
#define CAM_UPDATE_INTERLACED       0x00000002UL
#define CAM_UPDATE_VERTICALFREQ     0x00000004UL
#define CAM_UPDATE_HORIZONTALFREQ   0x00000008UL
#define CAM_UPDATE_CLOCKFREQ        0x00000010UL
#define CAM_UPDATE_CLOCKMODE        0x00000020UL
#define CAM_UPDATE_ACQUIREMODE      0x00000040UL
#define CAM_UPDATE_INVERTSYNC       0x00000080UL
#define CAM_UPDATE_STARTMODE        0x00000100UL
#define CAM_UPDATE_CHROMFILTER      0x00000200UL
#define CAM_UPDATE_GAIN             0x00000400UL
#define CAM_UPDATE_OFFSET           0x00000800UL
#define CAM_UPDATE_LOWREF           0x00001000UL
#define CAM_UPDATE_HIREF            0x00002000UL
#define CAM_UPDATE_HPOS             0x00004000UL
#define CAM_UPDATE_HSIZE            0x00008000UL
#define CAM_UPDATE_VPOS             0x00010000UL
#define CAM_UPDATE_VSIZE            0x00020000UL
#define CAM_UPDATE_CLAMP            0x00040000UL
#define CAM_UPDATE_ZERO             0x00080000UL
#define CAM_UPDATE_FIELD            0x00100000UL

#define CAM_UPDATE_ALL              0xFFFFFFFFUL
#define CAM_UPDATE_VIDEO            ( CAM_UPDATE_ALL & ~CAM_UPDATE_CHROMFILTER & ~CAM_UPDATE_GAIN & ~CAM_UPDATE_OFFSET & ~CAM_UPDATE_LOWREF & ~CAM_UPDATE_HIREF )



/* Prototypes : */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) || defined(__WIN32__) || defined(__386__) || defined(__DPMI32__)
#define INTERPRETER_FKT
#else
#define INTERPRETER_FKT _loadds
#endif



#if defined(_WIN32)  && !defined(__DPMI32__)
 #ifdef __BORLANDC__
  #ifdef CAMDEF_DLL
   #define CAMDEFAPI(fkttype)  fkttype __export _stdcall
  #else
   #define CAMDEFAPI(fkttype) extern fkttype _stdcall
  #endif
 #else /* MS.. */
  #ifdef CAMDEF_DLL
   #define CAMDEFAPI(fkttype)  __declspec(dllexport) fkttype _stdcall
  #else
   #define CAMDEFAPI(fkttype)  fkttype _stdcall
  #endif
//#define CAMDEFAPI(fkttype)  fkttype
#endif
#else /* not _WIN32 */
#define CAMDEFAPI(fkttype)  fkttype
#endif



CAMDEFAPI(WORD) mvInitCamTypeDefinitions  ( WORD MaxCameraDefs );
CAMDEFAPI(VOID) mvFreeCamTypeDefinitions  ( VOID );

/* use together with dyncamic driver dll loading: */
CAMDEFAPI(WORD) mvInitCamTypeDefDynamic  ( char* dllname, WORD MaxCameraDefs );
CAMDEFAPI(VOID) mvFreeCamTypeDefDynamic  ( VOID );


WORD _MVDFKT INTERPRETER_FKT mvSelCamera               ( DEV_T FAR* dev, char FAR* Name);
VOID _MVDFKT INTERPRETER_FKT mvDefCamType              ( DEV_T FAR* dev, char FAR* Name, char FAR* VideoMode, char FAR* Interlaced, WORD VFreq, WORD HFreq, WORD ClockFreq, char FAR* ClockMode );
VOID _MVDFKT INTERPRETER_FKT mvDefCamAcquireSetup      ( DEV_T FAR* dev, char FAR* Name, char FAR* AcquireMode, char FAR* Invert, char FAR* StartMode );
VOID _MVDFKT INTERPRETER_FKT mvDefCamGenerateVSync     ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width );
VOID _MVDFKT INTERPRETER_FKT mvDefCamAnalogParam       ( DEV_T FAR* dev, char FAR* Name, char FAR* ChromFilter, WORD Gain, INT OffsetVol, WORD LowRefVol, WORD HighRefVol);
WORD _MVDFKT INTERPRETER_FKT mvDefHorizontalUnit       ( DEV_T FAR* dev, char FAR* Name, char FAR* Unit );
WORD _MVDFKT INTERPRETER_FKT mvDefVerticalUnit         ( DEV_T FAR* dev, char FAR* Name, char FAR* Unit );
VOID _MVDFKT INTERPRETER_FKT mvDefCamHorizontalAcquire ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width, WORD Increment );
VOID _MVDFKT INTERPRETER_FKT mvDefCamVerticalAcquire   ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width, WORD Increment );
VOID _MVDFKT INTERPRETER_FKT mvDefCamClamp             ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width );
VOID _MVDFKT INTERPRETER_FKT mvDefCamZero              ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width );
VOID _MVDFKT INTERPRETER_FKT mvDefCamFieldGate         ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width );
VOID _MVDFKT INTERPRETER_FKT mvDefCamGenerateVSync     ( DEV_T FAR* dev, char FAR* Name, DWORD Start, DWORD Width );

VOID _MVDFKT INTERPRETER_FKT mvDefChannelGain          ( DEV_T FAR* dev, char FAR* Name, WORD Channel, WORD Gain );
VOID _MVDFKT INTERPRETER_FKT mvDefChannelOffset        ( DEV_T FAR* dev, char FAR* Name, WORD Channel, INT OffsetVol );
VOID _MVDFKT INTERPRETER_FKT mvDefChannelReference     ( DEV_T FAR* dev, char FAR* Name, WORD Channel, WORD ReferenceNr, WORD RefVol );

/* 9.9.99 new functions */
VOID _MVDFKT INTERPRETER_FKT mvDefCamHorizontalTotal   ( DEV_T FAR* dev, char FAR* Name, DWORD Total );
VOID _MVDFKT INTERPRETER_FKT mvDefPhysCamType          ( DEV_T FAR* dev, char FAR* Name, char FAR* CameraType );
VOID _MVDFKT INTERPRETER_FKT mvDefManufacturer         ( DEV_T FAR* dev, char FAR* Name, char FAR* Manufacturer );
VOID _MVDFKT INTERPRETER_FKT mvDefPropDialog           ( DEV_T FAR* dev, char FAR* Name, char FAR* DLLfilename,char FAR* DLLfunctionname  );
VOID _MVDFKT INTERPRETER_FKT mvDefCamComment           ( DEV_T FAR* dev, char FAR* Name, char FAR* text  );


LPCAMTYPE_T _MVDFKT mvGetCameraTypePtr         ( DEV_T FAR* dev, char FAR* Name );
LPCAMTYPE_T _MVDFKT mvGetActCameraTypePtr      ( DEV_T FAR* dev );
LPCAMTYPE_T _MVDFKT mvGetFirstCameraTypePtr    ( DEV_T FAR* dev );
WORD        _MVDFKT mvGetNumCameraTypesDefined ( DEV_T FAR* dev );

VOID        _MVDFKT mvUpdateCamera  ( DEV_T FAR* dev, LPCAMTYPE_T pCam, DWORD lUpdateFlags );
VOID        _MVDFKT mvUpdateActCamera  ( DEV_T FAR* dev, DWORD lUpdateFlags );

VOID _MVDFKT mvOutputCamType ( LPCAMTYPE_T pCam, PRINTFKT prf ) ;

#ifdef __cplusplus
}
#endif

#endif

