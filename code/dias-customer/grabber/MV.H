
/* Include-file for matrix-vision image-processing-library
        file: mv.h
        date: 7.10.92
        wb. : UH
        update: 22.2.93 UF  - new mode for SelDisplaySource -> DS_MEM2 for PCgrab-G2
                                                          - new function: MultipleSnap
        update: 10.1.94 UF  - several functions for PCprocess
*/


#ifndef __MV_H              /* Already included ? */
#define __MV_H              /* No !               */


/******************************************************************************
*               Compiler and Operating System dependend definitions           *
*******************************************************************************/


#ifdef __HIGHC__
/* ------------------------------------------------------------- HIGHC-DOS32 */
#ifdef _MSDOS
#define __mvSystem__ "HIGHC-DOS32"
#define __mv32BIT__
#define __mvDOS__

#define _MVEXPCALL
#define _MVDFKT

#endif
#endif /* __HIGHC__ */


#ifdef __WATCOMC__

#if !( defined(__mvDOS__) || defined(__mvWIN__) )
/* ------------------------------------------------------------- WATCOM-DOS32 */
#if defined(__386__) && !defined(__WINDOWS__) && !defined(__WINDOWS_386__) && !defined(__NT__)
#define __mvSystem__ "WATCOM-DOS32"
#define __mv32BIT__
#define __mvDOS__
#endif

/* ------------------------------------------------------------- WATCOM-WIN16 */
#if defined(__WINDOWS__) && !defined(__WINDOWS_386__)
#define __mvSystem__ "WATCOM-WIN16"
#define __mv16BIT__
#define __mvWIN__
#endif

/* ------------------------------------------------------------- WATCOM-WIN32 */
#if defined(__WINDOWS_386__) || defined(__NT__)
#define __mvSystem__ "WATCOM-WIN32"
#define __mv32BIT__
#define __mvWIN__

#ifndef _STDCALL_SUPPORTED
#define __mvWIN32s__
#else
#define _declspec __declspec
#endif

#ifdef _INC_WINDOWS
#define NODWORD
#define NOWORD
#define NOBYTE
#define NOBOOL
#endif

#endif

#endif  /* !( defined(__mvDOS__) || defined(__mvWIN__) ) */
#endif /* __WATCOMC__ */


#ifdef __BORLANDC__
/* ------------------------------------------------------------ BORLAND-DOS16 */
#if !defined(__DPMI32__) && !defined(_Windows)
#define __mvSystem__ "BORLAND-DOS16"
#define __mv16BIT__
#define __mvDOS__
#endif

/* ------------------------------------------------------------ BORLAND-DOS32 */
#if defined(__DPMI32__) && !defined(_WIN32)
#define __mvSystem__ "BORLAND-DOS32"
#define __mv32BIT__
#define __mvDOS__
#endif

/* ------------------------------------------------------------ BORLAND-WIN16 */
#if defined(_Windows) && !defined(__mvDOS__) && !defined(__WIN32__)
#define __mvSystem__ "BORLAND-WIN16"
#define __mv16BIT__
#define __mvWIN__
#endif


/* ------------------------------------------------------------ BORLAND-WIN32 */
#if !defined(__mvDOS__) && defined(__WIN32__)
#define __mvSystem__ "BORLAND-WIN32"
#define __mv32BIT__
#define __mvWIN__

#ifdef _INC_WINDOWS
#define NOCHAR
#endif

#define _declspec(impexp)

#endif

#endif /* __BORLANDC__ */

#ifdef _MSC_VER
/* ---------------------------------------------------------- MICROSOFT-DOS16 */
#if defined(_MSDOS) && !defined(_Windows) && !defined(_WINDOWS)
#define __mvSystem__ "MICROSOFT-DOS16"
#define __mv16BIT__
#define __mvDOS__
#endif


/* ---------------------------------------------------------- MICROSOFT-WIN16 */
#if defined(_WINDOWS) && !defined(__mvDOS__)  && !defined(_WIN32)
#define __mvSystem__ "MICROSOFT-WIN16"
#define __mv16BIT__
#define __mvWIN__
#ifdef _INC_WINDOWS
#define NODWORD
#define NOWORD
#define NOBYTE
#define NOBOOL
#endif
#endif


/* ---------------------------------------------------------- MICROSOFT-WIN32 */
#if defined(_WIN32)
#define __mvSystem__ "MICROSOFT-WIN32"
#define __mv32BIT__
#define __mvWIN__

#ifdef _INC_WINDOWS
#define NOCHAR
#endif
                                                          
#endif

#endif /* _MSC_VER */

/* ----------------------------------------------------------------- OS-Modes */
#if defined(__mv16BIT__) && defined(__mvDOS__)
#define __mvDOS16__
#endif

#if defined(__mv16BIT__) && defined(__mvWIN__)
#define __mvWIN16__
#endif

#if defined(__mv32BIT__) && defined(__mvDOS__)
#define __mvDOS32__
#endif

#if defined(__mv32BIT__) && defined(__mvWIN__)
#define __mvWIN32__
#endif


/******************************************************************************
*                              compiler switches                              *
*******************************************************************************/

/* ------------------------------------------------- struct alignment = 1BYTE */
#if defined(_MSC_VER)
#pragma pack(1)
#endif

#if defined(__BORLANDC__)
#if __BORLANDC__ > 0x410
#pragma option -a1
#endif
#endif

#if defined(__HIGHC__)
#pragma push_align_members(1);
#endif

#if defined(__WATCOMC__)
#pragma pack ( push, 1 );
#endif


/******************************************************************************
*                              OS dependend defines                           *
*******************************************************************************/


/* ---------------------------------------------------------------------- DOS */
#ifdef __mvDOS__

#ifndef _MVEXPCALL
#define _MVEXPCALL _cdecl 
#endif

#define _MVAPI(fkttype)  fkttype _MVEXPCALL
#ifdef _export
#undef _export
#endif
#define _export
#endif


/* -------------------------------------------------------------------- WIN16 */
#ifdef __mvWIN16__

#ifndef _MVEXPCALL
#define _MVEXPCALL _cdecl
#endif

#define _MVAPI(fkttype)  fkttype _export _MVEXPCALL

#endif


/* -------------------------------------------------------------------- WIN32 */
#ifdef __mvWIN32__

#ifndef _MVEXPCALL
#ifdef __mvWIN32s__
#define _MVEXPCALL _cdecl
#define _MVAPI(fkttype)  fkttype _MVEXPCALL 
#else
#define _MVEXPCALL _stdcall
#endif

#ifndef _MVAPI
#ifndef MV_DLL
#define _MVAPI(fkttype)  _declspec(dllimport) fkttype _MVEXPCALL
#else /* only for building the driver DLL */
#define _MVAPI(fkttype)  _declspec(dllexport) fkttype _MVEXPCALL
#endif
#endif

#ifdef _export
#undef _export
#endif
#define _export

#endif


#endif


/* ------------------------------------------------------------ DOS16 & WIN16 */
#ifdef __mv16BIT__

#ifndef FAR
#define FAR   _far
#endif

#ifndef HUGE
#define HUGE  _huge
#endif

#endif

/* ------------------------------------------------------------ DOS32 & WIN32 */
#ifdef __mv32BIT__

#ifdef FAR
#undef FAR
#endif
#define FAR

#ifdef far
#undef far
#endif
#define far

#ifdef _far
#undef _far
#endif
#define _far

#ifdef __mvWIN32__
#ifdef __far
#undef __far
#endif
#define __far
#endif

#ifdef HUGE
#undef HUGE
#endif
#define HUGE

#ifdef interrupt
#undef interrupt
#endif
#define interrupt

#ifdef _interrupt
#undef _interrupt
#endif
#define _interrupt

#ifdef __mvWIN32__
#ifdef __interrupt
#undef __interrupt
#endif
#define __interrupt
#endif

#undef outp
#undef inp
#undef outpw
#undef inpw

#endif


/******************************************************************************
*                              General defines & macros                       *
*******************************************************************************/

/* function type of driver function pointers */
#ifndef _MVDFKT
#define _MVDFKT _cdecl FAR
#endif


/* types for driver user */
#ifndef LPPRIVATE
#define LPPRIVATE VOID FAR*
#endif


#ifndef LPPIPE_T
#define LPPIPE_T VOID FAR*
#endif

/* macros */
#define XSIZE(dev)    ((dev)->Glob->XSize)
#define YSIZE(dev)    ((dev)->Glob->YSize)
#define XVISI(dev)    ((dev)->Glob->XVisi)
#define YVISI(dev)    ((dev)->Glob->YVisi)
#define XAOISIZE(dev) ((dev)->XAOISize)
#define YAOISIZE(dev) ((dev)->YAOISize)
#define XAOIPOS(dev)  ((dev)->XAOILeft)
#define YAOIPOS(dev)  ((dev)->YAOIUpper)

#define VCNT_START(xsize,vstart)  ( ((xsize)>0)? ((vstart)+1024/(xsize)+2) : (vstart) )
#define STRUCT_OFFS(p,Var)        (((long)&((p)->##Var)) - ((long)(p)))      /* Berechnet Addressoffset einer Komponente von einer Struktur */

#define MV_WARNING      0x4000
#define MV_FATAL_ERROR  0x8000
#define MV_ERROR_MASK   0x0FFF

#define SetErr(dev,err)      { if ((dev)->ErrorCode == 0) { (dev)->ErrorCode = err; if ((dev)->ErrorString) *(dev)->ErrorString = '\0'; }}
#define SetFatalErr(dev,err) { SetErr((dev),(err)); (dev)->ErrorCode |= MV_FATAL_ERROR; }
#define SetWarning(dev,err)  SetErr((dev),(err) | MV_WARNING )
#define SetFatalErrFlag(dev) { (dev)->ErrorCode |= MV_FATAL_ERROR; }
#define SetWarningFlag(dev)  { (dev)->ErrorCode |= MV_WARNING; }

#define GetErr(dev)       ((dev)->ErrorCode & MV_ERROR_MASK)
#define IsWarning(dev)    ((dev)->ErrorCode & MV_WARNING)
#define IsFatalError(dev) ((dev)->ErrorCode & MV_FATAL_ERROR)
#define GetErrStr(dev)    ((dev)->ErrorString)
#define ClearErr(dev)     {(dev)->ErrorCode = NO_ERR;  if ((dev)->ErrorString) *(dev)->ErrorString = '\0';}

#define BankXYAdr(pBankDef,x,y)  (pBankDef->lStartAddr + (DWORD) (y) * pBankDef->lLineInc + (DWORD) (x) * pBankDef->lPixInc)

#define IsFunctionAvailable(dev,name)  ( ((void FAR*)(dev)->name != (void FAR*)(dev)->First)  )


/* define constants */

#define FALSE               0
#define TRUE                1

#define NO_CHANGE           0x4000                                      
#define RESTORE             0x4001
#define AUTO_SET            0x4002


#define FRQ_50HZ            0
#define FRQ_60HZ            1


#define RM_NOROW            0
#define RM_HARDWARE         1
#define RM_SOFTWARE         2



/* define devicecodes */

#define DEV_NODEV     0x00      /* no board */
#define DEV_FILE      0x01      /* file I/O, no board */
#define DEV_EPP       0x02      /* epp frame grabber */
#define DEV_PCGRABG   0x10      /* PCgrab-G1/G2 */
#define DEV_PCGRABC   0x20      /* PCgrab-C */
#define DEV_MVLC      0x30      /* MV-LC */
#define DEV_PCPM      0x40      /* PCprocess */
#define DEV_PCIB      0x50      /* PCimage-GVS */
#define DEV_PCISC     0x60      /* PCimage-SC */
#define DEV_PCISG     0x70      /* PCimage-SG/SGVS */
#define DEV_PCISRGB   0x80      /* PCimage-SRGB */
#define DEV_PCISDIG   0x90      /* PCimage-SDIG */
#define DEV_MVSLx     0xA0      /* MVsigma family code */
#define DEV_MVSLC     0xA1      /* MVsigma-SLC  bit 0: Color features */
#define DEV_MVSLG     0xA2      /* MVsigma-SLG  bit 1: Grey features */
#define DEV_MVSLD     0xA4      /* MVsigma-SLD  bit 2: Digital features*/
#define DEV_MVDISP    0xB0      /* MVdisplay family code */
#define DEV_PCCAM     0xC0      /* PC-Camera family */
#define DEV_PCMOS     0xC1      /* CMos version of PC-Camera */
#define DEV_PCCD      0xC2      /* CCD  version of PC-Camera */



/* define colorcodes */

#define COL_GREY      0         /* standard grey */
#define COL_GREY1     0         /* standard grey */
#define COL_GREY2     1         /* standard grey on bank 2 (only PCgrab) */
#define COL_CHROM     2         /* chrominance component */
#define COL_RED       3         /* red component */
#define COL_GREEN     4         /* green component */
#define COL_BLUE      5         /* blue component */
#define COL_RGB332    6         /* pixelformat 3 bit red, 3 bit green, 2 bit blue */
#define COL_RGB555    7         /* pixelformat 5 bit red, 5 bit green, 5 bit blue */
#define COL_RGB888    8         /* 32 bit true color mode */
#define COL_GREY8     9         /* 8 bit grey mode */
#define COL_CLASSCOLO 10        /* color component of realtime color classification */
#define COL_CLASSRESO 11        /* class results of realtime color classification */
#define COL_BGR888    12        /* 32 bit true color mode with exchanged colors */

#define COL_YC_PK     13        /* luminance and chrominance in packed order */
#define COL_YC_PL     14        /* luminance and chrominance in seperate planes */

#define COL_RGB565    15        /* pixelformat 5 bit red, 6 bit green, 5 bit blue */
#define COL_RGB24		  16        /* 24 bit true color mode */
#define COL_RGB32     17        /* 32 bit true color mode */

#define COL_GREY16    18        /* 16 bit grey mode */

#define COL_max       18        /* maximal defined color modes */

/* colormode flags */
#define COL_VGA         0x8000    /* change colormode for hardlive */
#define COL_HW_SPECIAL  0x2000    /* secial hardware dependend modes */
#define COL_MODE_MASK   0x00ff


/* define display-constants */

#define DS_OFF                   0
#define DS_LIVE                  1
#define DS_MEM                   2
#define DS_VGA                   3
#define DS_MEM2                  4
#define DS_ON                    5
#define DS_VGA_PASSTHRU          6
#define DS_VGA_PASSTHRU_INTCLK   7
#define DS_ANALOG_PASSTHRU       8
#define DS_VGATC_PASSTHRU       10
#define DS_VGAHC_PASSTHRU       11
#define DS_VGAWHC_PASSTHRU      12

#define DS_AUTO_VGA  0x0000
#define DS_DIG_VGA   0x2000
#define DS_ANA_VGA   0x4000

/* define LUT-codes */

#define INP_LUT1       0
#define INP_LUT2       1
#define INP_LUT3       2
#define OUT_LUT1       3
#define OUT_LUT2       4
#define OUT_LUT3       5
#define OVL_LUT1       6
#define OVL_LUT2       7
#define OVL_LUT3       8
#define OUT_LUT_ALL    9
#define OVL_LUT_ALL    10
#define INP_LUT15      15

#define OUT_LUT_RED    3
#define OUT_LUT_GREEN  4
#define OUT_LUT_BLUE   5
#define OVL_LUT_RED    6
#define OVL_LUT_GREEN  7
#define OVL_LUT_BLUE   8

/* define input-channel */

#define INP_AUTO       0


/* define videomodes */

#define VM_AUTO        0
#define VM_CCIR        1
#define VM_RS170       2
#define VM_PAL         3
#define VM_NTSC        4
#define VM_SECAM       5
#define VM_SVHS        6
#define VM_VSCAN       7
#define VM_CCIR576     8
#define VM_PAL576      9
#define VM_RGB         10
#define VM_DIG8        14
#define VM_DIG10       15
#define VM_DIG12       16
#define VM_DIG16       17
#define VM_DIG24       18
#define VM_DIG32       19
#define VM_TVES        20
#define VM_TVES60Hz    21

#define VM_NTSC_SVHS   21
#define VM_SECAM_SVHS  22
#define VM_max         22

#define VM_INVVSCAN    0x100

#define VM_INVHSYNC    0x200
#define VM_INVVSYNC    0x400
/* define saveimage modes */

#define IMAGE_NO_HEADER   0
#define IMAGE_MV_HEADER   1
#define IMAGE_MV_BMP      2
#define IMAGE_MV_MAXMODE  4

/* define bank select modes */

#define BS_MEM1    0
#define BS_MEM2    1
#define BS_MEM3    2
#define BS_OVERL   3
#define BS_LUMIN   0
#define BS_CHROM   1
#define BS_TC24    6
#define BS_YC      8

/* define bank select modes for MVcam-PCMOS*/

#define BS_SENIMG       0
#define BS_SENREFIMG    5
#define BS_IMG         10
#define BS_REFIMG      15
#define BS_SEN         20


/*... PCimageS : predefined bank numbers ................................*/
                               /* direct access to PCimageSC dma buffers */
#define BS_CHAN_E_0      0     /* even / both : RGB, YUVpacked, Y of YCplanar */
#define BS_CHAN_O_0      1     /* odd                                    */
#define BS_CHAN_E_1      2     /* even / both : V planar of YCplanar     */
#define BS_CHAN_O_1      3     /* odd                                    */
#define BS_CHAN_E_2      4     /* even / both : U planar of YCplanar     */
#define BS_CHAN_O_2      5     /* odd                                    */

                               /* pixel format       -   result       */
                               /*                    - ( bits/pixel ) */
#define BS_RGB16         6     /* RGB 16 pixel       -      16        */

#define BS_RGB32         7     /* RGB 24 pixel, 32 b -     (32)       */
#define BS_RGB24_R       8     /*   red component    -       8        */
#define BS_RGB24_G       9     /*   green component  -       8        */
#define BS_RGB24_B      10     /*   blue component   -       8        */
#define BS_RGB24        11     /*   24 bpp packed    -      24        */

#define BS_YC_PK        12     /* YC 16 packed pixel -      16        */
#define BS_YC_PK_Y      13     /*     Y component    -       8        */
#define BS_YC_PK_C      14     /*     C component    -       8        */

#define BS_YC_PL_Y      15     /* YC 16 planar Y comp -      8        */
#define BS_YC_PL_U      16     /*     U component    -       8        */
#define BS_YC_PL_V      17     /*     V component    -       8        */

#define BS_RGB15			18		   /* RGB 555 pixel			-		 15		 */

#define BS_PREDEF_MAX   19
                               /* 19..28 reserved                     */
#define BS_INTERN       29     /* internal use / bank buffer          */

#define BS_USER_MIN     30
#define BS_USER_MAX     49

#define DISPLAY_ADDR_FLAG   0x80000000UL    /* address flag for display access */

/* SRGB : individual color component selection for gain and offset setting  */

#define SGO_SET_R       0x100
#define SGO_SET_G       0x200
#define SGO_SET_B       0x400


/* MVcam: defines */

#define MVCAM_LED_FLAG  0x80000000UL


/* defines for Videotiming */

#define BIT_SET    1
#define BIT_RESET  0

#define HVT_CLAMP     0x01
#define HVT_ZERO      0x02
#define HVT_OUT0      0x02
#define HVT_OUT1      0x04
#define HVT_FIELD     0x08
#define HVT_BANK      0x10
#define HVT_ACQUIRE   0x20
#define HVT_LINT      0x40
#define HVT_SYNC      0x80
#define HVT_ALL       0xff

 // ram parameter masks
#define VRAM      1
#define HRAM      0

#define SCALING   2

 // helpful macros
#define mvSetVTRange(dev,ram,mask,start,stop)   mvSetVideoTiming(dev,ram,0,4095,1,mask,BIT_RESET); mvSetVideoTiming(dev,ram,start,stop,1,mask,BIT_SET)

/* defines for trig_mode from AcquireDef  */

#define TM_FIELD0     (1<<0)    /* Field 0 should be digitized                */
#define TM_FIELD1     (1<<1)    /* Field 1 should be digitized                */
#define TM_SHOWDIGI   (1<<2)    /* Show digitize-marker                       */

#define TM_SYNCFIELD  (1<<6)    /* Digitize Fieldsynchron                     */
#define TM_SKIPFIELD  (1<<7)    /* Skip the aktual Field one time (internal)  */
#define TM_CONTSNAP   (1<<8)    /* Enable ContinousSnap (internal)            */
#define TM_EXTTRIG    (1<<9)    /* Enable Extern Trigger Mode (internal)      */
#define TM_SYNCHRON   (1<<10)   /* Read synchron from FiFo                    */
#define TM_DOUBLEACC  (1<<11)   /* Read with 'Double Access' from FiFo (int.) */
#define TM_DISABLE_FIELD_DETECT  (1<<12)   /*  */
#define TM_TOGGEL_FIELD          (1<<13)   /*  */


/* defines for Color-Decoder */

#define AV_CRCB_ADJ      1
#define AV_SUBC_BAL      2
#define AV_BLUE_BIAS     6
#define AV_RED_BIAS      7
#define AV_BLUE_CONT     9
#define AV_MAIN_CONT    10
#define AV_RED_CONT     11
#define AV_BLUE_BRIGHT  12
#define AV_MAIN_BRIGHT  13
#define AV_RED_BRIGHT   14
#define AV_MAIN_SAT     15
#define AV_CRCB_SAT_BAL 16
#define AV_HUE          17
#define AV_LUMA         18
#define AV_CHROMA       19
#define AV_SATURATION   20              /* SLC not implemented */
#define AV_CONTRAST     21
#define AV_CHROMA_U             22
#define AV_CHROMA_V             23
#define AV_MAXColDec            30


/* Codes for PCP-Memory-Board */

#define ME_4M_MOD        5
#define ME_8M_MOD        9

#define ME_PAR_WRT       1
#define ME_STD_WRT       0

#define ME_DBL_ACC       1
#define ME_STD_WRT       0

#define ME_INTERL        1
#define ME_BANKED        0

#define ME_32K_MODE      1
#define ME_2M_MODE       0


/* defines for PCimage */

/* #define GRABBER_STANDALONE  0 */
/* #define GRABBER_DUALMODE    1 */

/* define device info types */

#define DI_DEVNAME             1     /* Pointer to Devicename */
#define DI_DIGSTATUS           2     /* Status information    */
#define DI_DIGVCOUNT           3     /* actual vertical counter */
#define DI_DIGTRIGGERFF        6     /* Trigger Flip-Flop 1: waiting on Trigger */
#define DI_DIGTRIGGERINTR      7     /* Trigger Interrupt Line */
#define DI_DIGTIMINGINTR       8     /* Timing Interrupt Line */
#define DI_DIGVSYNC            9     /* vertical sync aktiv   */
#define DI_DIGFIELD           10     /* actual field          */
#define DI_DIGBUSY            11     /* busy flag             */
#define DI_DIGSNAPREADY       12     /* Snap finished         */
#define DI_RiscPC             13     /* Risc ProgrammCounter  */
#define DI_DIGLIVE            14     /* live state of digitizer (0,1) */  

#define DI_BASEADDR           16     /* Get Base-Addr         */
#define DI_GSPDEV             17     /* Get GSP-Device-Handle */
#define DI_ACTFIELDNR         18     /* Get actual-field-nr   */
#define DI_LASTFIELDNR        19     /* Get last digitized-field-nr   */

#define DI_GETISRINFO_SAVEDCOUNTER 30 /* Get ISR Counter from last user handled interrupt */
#define DI_GETISRINFO_RUNCOUNTER   31 /* Get ISR Counter from last interrupt */
#define DI_GETISRINFO_SYS          32 /* Get ISR system information */

#define DI_GETTRACEFUNCTION     40 /* Get 'printf' function ptr to log file */

#define DI_DIGVCOUNT_ADDR      (DI_DIGVCOUNT | 0x100)
#define DI_DIGSTATUS_ADDR      (DI_DIGSTATUS | 0x100)


#define DI_ERROR   0xFFFFFFFFL


/* Module-Number */

#define MOD_PCP        0
#define MOD_MGVS       1
#define MOD_MGVSEXT    2
#define MOD_MRGB       3
#define MOD_MDIG       4

#define MOD_LAST       4


/* define errorcodes */

#define NO_ERR                   0      /* no error */
#define ERR_NO_MEM               1      /* not enough memory */
#define ERR_CREATE_FILE          2      /* cannot create a file */
#define ERR_INVALID_FILE         3      /* invalid file */
#define ERR_WRITE_FILE           4      /* error writing a file */
#define ERR_OPEN_FILE            5      /* error opening a file */
#define ERR_READ_FILE            6      /* error reading a file */
#define ERR_LOAD_LIB             7      /* error loading a library */
#define ERR_NO_LUT_BUFF          8      /* not enough memory for LUT buffer */

#define ERR_DEVICE_USED          9      /* device is already open */
#define ERR_CFG_NOT_FOUND       10      /* cannot find config file (*.INI) */
#define ERR_DPMI                11      /* error in the DOS Protected Mode Interface */

#define ERR_NO_MEMADR           12      /* cannot read memory base address from file */
#define ERR_NO_IOADR            13      /* cannot read I/O base address from file */

#define ERR_WRONG_BOARD         15      /* cannot find the expected board */
#define ERR_DUP_DEV             16      /* cannot duplicate device-handle because module already initialized */

#define ERR_ILLEGAL_PARAM       20      /* wrong argument */
#define ERR_ILLEGAL_IOADDR      21      /* wrong I/O base address */
#define ERR_ILLEGAL_MEMADDR     22      /* wrong memory base address */
#define ERR_ILLEGAL_ARG         23      /* wrong argument */
#define ERR_ILLEGAL_HEAD        24      /* wrong image header */
#define ERR_ILLEGAL_ARG_TYPE    25      /* wrong argument type */

#define ERR_MEMADDR_USED        32      /* memory space is already used */
#define ERR_NO_MEM_ACC          33      /* no memory access possible */
#define ERR_PORT_ACC            34      /* no port access possible */
#define ERR_DMA                 35      /* error on DMA transfer */

#define ERR_INI_SECTION         40      /* section in INI-file not found */
#define ERR_INI_FUNCTION        41      /* function from INI-File not found */

#define ERR_NOT_IMPLEMENTED     50      /* function not implemented */

#define ERR_I2CBUS              51      /* no access to the I2C-bus possible */

#define ERR_OUT_LUT             52      /* no access to the output LUT */
#define ERR_OVL_LUT             53      /* no access to the overlay LUT */
#define ERR_IN_LUT              54      /* no access to the input LUT */

/* Errorcode for Testfunctions */

#define ERR_TEST_MEM            60      /* memory test failed */
#define ERR_TEST_PIO            61      /* port access failed */
#define ERR_TEST_CNT            62      /* counter test failed */
#define ERR_TEST_GSP            63      /* GSP test failed */
#define ERR_TEST_MSK            64      /* mask test failed */
#define ERR_TEST_DISP           65      /* dispaly test failed */

/* Errorcode for Displaymodes */

#define ERR_DISP_MODES_OVERFLOW 71      /* too many display modes defined */
#define ERR_UNDEF_DISP_MODE     72      /* unkown display mode */

/* Digitizer-Errors */

#define ERR_NO_CAMERA          100      /* no camera conected */
#define ERR_TIMING_RAM         110      /* no access to the timing-RAM posible */
#define ERR_LCA_ACC            111      /* no access to the LCA posible */
#define ERR_LCA_LOAD           112      /* error on loading LCA file */

#define ERR_GSP_COMMUN         201      /* communications error with GSP */
#define ERR_GSP_EXECUTE        202      /* error on starting the GSP-Kernel */
#define ERR_GSP_TYPE           203      /* wrong GSP-kernel type */
#define ERR_GSP_VERSION        204      /* wrong GSP-Kernel version */

/* Memory-Board-Errors */

#define ERR_LOAD_FPGA          150      /* FPGA-file not found */
#define ERR_ACC_FPGA           151      /* no access to the FPGA posible */

/* DSP-Errors */

#define ERR_DSP_CODE_OPEN      220      /* can't open DSP code file */
#define ERR_DSP_CODE_HEADR     221      /* line read error at header of DSP code file */
#define ERR_DSP_UNKOWN_COMMAND 222      /* unkown DSP command */
#define ERR_DSP_CODE_ERROR     223      /* error on starting DSP-vode-command */
#define ERR_DSP_COMM_TIMEOUT   224      /* DSP communication timeout */
#define ERR_DSP_RUN            225      /* error  in executing DSP-Code */
#define ERR_DSP_CODE_LOAD      226      /* erreor by download of DSP-code */
#define ERR_DSP_NOT_FOUND      227      /* DSP can't be found */
#define ERR_DSP_WRONG_MODULE   228      /* wrong or no module is connected*/
#define ERR_DSP_DPRAM          229      /* error on Data-Prarameter-RAM */

/*io-errs*/
#define ERR_USER_ABORT         240      /* operation was aborted by user */

/* OS-Errors */
#define ERR_WRONG_OS_VERSION   250      /* wrong operating system detected */

#define MAXERR                 251      /* maximum number of error codes */

#define MAX_ERRSTRING_LENGTH   256      /* maximum length of an error string */

/* define warncodes */

#define WARN_RANGE_ADJUSTED   1000


/* define Code for ChipWriteData - Access to Module */

#define MOD_OFFS            100


/* - PCimageS - SetScalerMode ( dev, mode = .. ) -                               */
#define SCM_SWITCH           0      /* switch scaler on / off                    */
#define SCM_FILTER           1      /* select h/v interpolation filter           */
#define SCM_LUT              2      /* switch anti-gamma / limiting / invers lut */
#define SCM_VERTICAL_BYPASS  3      /* set vertical scaler/filter bypass region  */
#define SCM_OUT_FIELD        4      /* set non standard interlace field handling */
#define SCM_CHROMA_KEY       5      /* set U/V chroma keying window              */


/* Mask bit macros */
#define BIT(i)    (1<<(i))
#define BITL(i)   (1UL<<((DWORD)i))

/* Interrupt mask bits */

#define IRQ_FEND_E      0               /* grab even field end interrupt / index */
#define IRQ_FEND_O      1               /* grab odd field end interrupt / index  */
#define IRQ_FBEGIN      2               /* any frame started interrupt / index   */
#define IRQ_TRIGGER     3               /* on trigger event  interrupt / index   */
#define IRQ_TIMING      4               /* on spez. acquire pixel position       */

#define IRQ_ABORT       31              /* indicates abort of isr-services after close-isr */

/* masks for interrupt enabling */

#define IRQ_FEND_E_MSK   BIT(IRQ_FEND_E)
#define IRQ_FEND_O_MSK   BIT(IRQ_FEND_O)
#define IRQ_FBEGIN_MSK   BIT(IRQ_FBEGIN) 
#define IRQ_TRIGGER_MSK  BIT(IRQ_TRIGGER) 
#define IRQ_TIMING_MSK   BIT(IRQ_TIMING) 

#define IRQ_ABORT_MSK    BIT(IRQ_ABORT) 

/* typedefs */

#define VOID           void


#ifdef __cplusplus
extern "C" {
#endif

#ifndef NOTYPES

#ifndef NODWORD
typedef unsigned long  DWORD;
#endif

#ifndef NOWORD
typedef unsigned short WORD;
#endif

#ifndef NOINT
#ifdef __mv16BIT__
typedef signed short   INT;
#else
typedef signed int   INT;
#endif
#endif

#ifndef NOCHAR
typedef unsigned char  CHAR;
#endif

#ifndef NOBYTE
typedef unsigned char  BYTE;
#endif

#ifndef NOBOOL
typedef int                BOOL;
#endif

#ifndef NOFLOAT
typedef float          FLOAT;
#endif

#endif /* #ifndef NOTYPES */

typedef int (__cdecl FAR *PRINTFKT)(const char FAR *__format, ...);
typedef char FAR * (FAR *GETSFKT)(char FAR *s );


typedef struct disp_mode_t
{
   WORD mode;
   CHAR name[16];
   WORD type;
   WORD xvisi;
   WORD yvisi;
   WORD xtotal;
   WORD ytotal;
   WORD xstart;
   WORD ystart;
   WORD hsync;
   WORD vsync;
   WORD pol;
   WORD vid;
	WORD gain;
   WORD hFreq;
} DISP_MODE_T;


typedef struct     /* PCprocess and PCimage only !! */
{
   WORD  trig_mode;        /* trigger mode */
   WORD  psize;            /* pixel size */
   WORD  pincr;            /* pixel incr (= psize) */
   WORD  vcnt_start;       /* value of video-line-counter on starting transfer */
   WORD  vcnt_incr;        /* number of video-lines per display-line */
   WORD pipeline_delay;    /* number of dummy pixel */
   WORD vcnt_fielden;      /* */
   WORD  fields_per_frame;
   DWORD start_addr[4];    /* adress of acquire memory */
   DWORD line_inc[4];      /* line increment (pitch) */
   WORD  xofs[4];          /* horizontal digitize start position (pixel) not used! */
   WORD  xsize[4];         /* number of visible pixels */
   WORD  yofs[4];          /* vertikal digitize start position (line's) not used! */
   WORD  ysize[4];         /* number of visible lines */
   WORD  xpos[4];          /* horizontal start pixel position */
   WORD  ypos[4];          /* vertical start line position */
   /* WORD vcnt_fielden; */  /* max number of video-lines to accept the aktual field */
} ACQUIRE_DEF_T;


typedef struct
{
	 DWORD	lStartAddr;
    DWORD	lLineInc;
    WORD		lPixInc;
    WORD		lPixSize;
	 DWORD	CMaske[3];			/* Masks for changing the colorformat 15.05.97 UG */
	 INT	   Shift[3];		   /* 15.05.97 UG */
} BANK_DEF_T;


typedef struct
{
    WORD  InpSelect;                  /* channel-No (1..3)                */
    WORD  MSelect;                    /* mode select color: COL_GREY ...  */
    DWORD ACQMemAddr;                 /* Memory-Address of acquire buffer */
} SNAP_TABLE_T;

typedef struct _mvdev_t DEV_T ;          /* forward-declaration of DEV_T     */


typedef struct ini_t                  /* contains info to init. device    */
{
  INT XSize, YSize;                   /* X-Size and Y-Size of Pixelbuffer */
} INI_T;

typedef INT (_MVEXPCALL *DRVINI_T)   (DEV_T FAR*, CHAR FAR*, CHAR FAR* FAR*);
typedef INT (_MVEXPCALL *MODULINI_T) (DEV_T FAR*, CHAR FAR*, CHAR FAR* FAR*, WORD);
typedef void (_MVDFKT *INTRHANDLER)(DEV_T FAR*, BYTE, WORD, VOID FAR*);

typedef struct vec_t               /* Struct to define vect. of points */
{                                  /* use in fillpoly... */
  INT XPos;
  INT YPos;
} VEC_T;


typedef struct info_t
{
  WORD CheckSum;
  WORD StructSize;
  CHAR SerialNumber[20];
  CHAR Revision[6];
  CHAR BoardType[20];
  WORD DispMem;
  WORD SysMem;
  WORD AqrMem;
  WORD Color;
  CHAR DriverName[10];
  CHAR DriverRevision[15];
  BYTE DeviceInitDataCount;
  BYTE DeviceInitData[32];
} INFO_T;


#define OLD_INFO_SIZE 83

typedef struct imhd_t              /* header for image-file */
{
  CHAR Ident[16];                  /* identification-string */
  CHAR Driver[16];                 /* driver name */
  WORD ColorMode;                  /* color-model (RGB, HSI, Grey,...) */
  WORD XSize;                      /* X-Size of image */
  WORD YSize;                      /* Y-Size of image */
  WORD Compress;                   /* Compression, 0 = NO */
  WORD PixLen;                     /* length of a pixel in byte */
  BYTE Free[200];                  /* for expansion */
} IMHD_T;


typedef struct lut_t               /* discribes one Look-Up-Table */
{
// uf/28/8/98 changed from WORD to DWORD
  DWORD      Size;                 /* count of LUT-entries */
  DWORD      Resolution;           /* maximum Value in LUT */
  WORD FAR* LUTPtr;                /* pointer to LUT-Buffer */
} LUT_T;


typedef struct csc_lut_t
{
  CHAR  Ident[16];                 /* identification-string  */
  WORD  HeaderSize;                /* size of this header    */
  DWORD BlockSize;                 /* size of one LUT-Block  */
  WORD  BlockCount;                /* number of LUT-Blocks   */
  WORD  BlockType;                 /* type of these blocks   */
  BYTE  free[38];                  /* reserved for later use */
} CSC_LUT_T;


typedef struct glob_t              /* contain global device-information */
{
  BYTE      StructVersion;         /* version of this struct */
  BYTE      DevType;               /* type of device */
  CHAR FAR* DevName;               /* name of the device */

  WORD      XSize;                 /* x-size of screen-buffer */
  WORD      YSize;                 /* y-size of screen-buffer */

  WORD      XVisi;                 /* visible x-area */
  WORD      YVisi;                 /* visible y-area */

  WORD      LockPage;              /* actual Page */

  WORD      ColorOffset;           /* offset between two colour-banks */
  WORD      LineOffset;            /* offset between two rows */

  WORD      RowMode;               /* mapping of rows in framegrabber */

  WORD      VideoMode;             /* Video Mode ( CCIR, PAL, NTSC,..) */
  WORD      DisplaySource;         /* display livevideo, stored image ... */
  WORD      OverlayActive;         /* display overlay ? */
  WORD      Input;
  WORD      InputClk;
  WORD      ExtTrig;
  WORD      Intr;
  WORD      OutputSyncPol;
  WORD      OutputSyncLine;
  WORD      ColorMode;             /* reportes color-model (RGB, HSI, Grey,...) */

  INT       InstCount;             /* count instances of device-handels */

  BYTE FAR* FAR* LineAdrTbl;       /* pointer to an array of pointers to lines */
  LPPRIVATE PrivateData;

  LUT_T     LUT[16];               /* array of lut-structures */

  VOID FAR* ModME;                 /* Pointer to Memory-Modul */

  VOID FAR* PropertyTable;           /* 6.5.97 Pointer to PropertyTable */

} GLOB_T;


typedef struct _mvdev_t               /* contains information about a device */
{
  GLOB_T FAR* Glob;                /* pointer to global data-block */
  VOID   FAR* Modul;               /* pointer to a modul-dependent data-block */

  WORD   IOAddress;                /* base-address of registers in io-space */
  DWORD  MemAddress[4];            /* linear base-address of the memory window */
  DWORD  MemWndSize[4];            /* size of the memory window */
  DWORD  PhysAddress[4];           /* physical base-address of the memory window */

  BYTE FAR* SegMemAddress[4];      /* segmented base-address of the memory window */

  WORD   NoTest;                   /* Bypass test for free mem-address */

  WORD   AHInc[4];                 /* Distance between 2 Selectors */
  WORD   Selectors[4];             /* Count of Selectors */

  LPPIPE_T FAR* PipeList;          /* Pointer to Array of all Pointer to Pipeline-Module */
  LPPIPE_T  Pipe;                  /* Pointer to Pipeline-Module */

  WORD   free[14];

  CHAR FAR* ErrorString;           /* Pointer to an Error-String */

  WORD   XAOILeft;                 /* left x-position of Area-Of-Interest  */
  WORD   YAOIUpper;                /* upper y-position of Area-Of-Interest */

  WORD   XAOISize;                 /* x-size of Area-Of-Interest */
  WORD   YAOISize;                 /* y-size of Area-Of-Interest */

  WORD   XSnapStart;               /* left x-position of snap-start */
  WORD   YSnapStart;               /* upper y-position of snap-start */

  WORD   ErrorCode;                /* last occured error-code */

  VOID      (_MVDFKT *First)();           /* dummy-ptr 1 */

  VOID      (_MVDFKT *CloseBoard)         /* free board and release memory */
              (DEV_T FAR* dev);           /* pointer to device-key */

  WORD      (_MVDFKT *SelInput)           /* select input and report active input */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD input);               /* determine input */

  WORD      (_MVDFKT *SelOverlay)         /* switch overlay on and off and report state */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD active);              /* determine on or off */

  WORD      (_MVDFKT *SelVideoMode)       /* select videomode and report old mode */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode);                /* determine mode ( CCIR, PAL, NTSC,..) */

  WORD      (_MVDFKT *SelDisplaySource)   /* select display source and report old source */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD source);              /* determine source ( live, stored, VGA ) */

  WORD      (_MVDFKT *SelColorMode)       /* select snap-color-model and report  old state */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode);                /* determine model (Lum., Chrom, RGB, ...) */

  WORD      (_MVDFKT *SelSnapStart)       /* */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT XLeft,                 /* left x-position */
               INT YUp);                  /* upper y-position */

  WORD      (_MVDFKT *SelInputLUT)        /* Select active Input-LUT */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Nr);                  /* Number of LUT (0 = OFF) */

  WORD      (_MVDFKT *SelInputClk)        /* Select inp-clock, (int,ext) */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Nr);                  /* Source (0 = intern, 1 = extern) */

  WORD      (_MVDFKT *SelExtTrig)         /* Select Trigger, (int,ext) */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Nr);                  /* 0 = disable, 1 = enable */

  WORD      (_MVDFKT *SelIntr)            /* Select interrupt (off,on) */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Nr);                  /* 0 = OFF */

  WORD      (_MVDFKT *SelOutputSyncPol)   /* */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD syncpol);             /* 0 = pos, 1 = neg */

  WORD      (_MVDFKT *SelOutputSyncLine)  /* Sync on RGB or external lines */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD sync);                /* 0 = Syn on RGB, 1 = sync on ext lines */

  VOID      (_MVDFKT *SetIOBase)          /* SetIOBase Addr */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD cnt);                 /* IOBase-Address */

  VOID      (_MVDFKT* SetMemBase)         /* SetMemBase Addr */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD WndNr,                /* Window number */
               DWORD MemAddr,             /* Memory Address */
               DWORD MemSize );           /* Size of the Memorywindow */

  VOID      (_MVDFKT *MultipleSnap)       /* multible snap */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD cnt,                  /* number of pictures    */
               SNAP_TABLE_T FAR* p);      /* pointer to snap table */

  VOID      (_MVDFKT *SingleSnap)         /* snap one image */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD wait);                /* wait until image is complete */

  VOID      (_MVDFKT *ContinousSnap)      /* snap all images */
              (DEV_T FAR* dev);           /* pointer to device-key */

  VOID      (_MVDFKT *StopContinousSnap)  /* end snap of all images */
              (DEV_T FAR* dev);           /* pointer to device-key */

  VOID      (_MVDFKT *WriteLUTLin)        /* load desired LUT's with a linear ramp */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr);               /* which LUT to set */

  VOID      (_MVDFKT *WriteLUTLinInv)     /* load desired LUT's with a inverted ramp */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr);               /* which LUT to set */

  VOID      (_MVDFKT *WriteLUTBin)        /* load desired LUT's with a binary slope */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr,                /* which LUT to set */
					WORD Treshold);            /* treshold for binary image */

  VOID      (_MVDFKT *WriteLUTBinInv)     /* load desired LUT's with an inverted binary slope */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr,                /* which LUT to set */
               WORD Treshold);            /* treshold for inv. binary image */

  VOID      (_MVDFKT *WriteLUTRamp)       /* load desired LUT's with variable ramp */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr,                /* which LUT to set */
               WORD XStart,               /* x-position of ramp-start */
               WORD YStart,               /* y-position of ramp-start */
               WORD XStop,                /* x-position of ramp-stop */
               WORD YStop);               /* y-position of ramp-stop */

  VOID      (_MVDFKT *WriteLUT332)        /* load desired LUT with value for RGB332-Mode */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr);               /* which LUT to set */

  VOID      (_MVDFKT *WriteLUTEGACol)     /* load desired LUT with value EGA-Color */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr);               /* which LUT to set */

  VOID      (_MVDFKT *WriteLUTVGACol)     /* load desired LUT with value of a VGA-Pal. */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr);               /* which LUT to set */

  VOID      (_MVDFKT *WriteLUTEntrys)     /* load desired LUT's with user-defined values */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr,                /* which LUT to set */
               WORD Start,                /* first entry to load */
               WORD Count,                /* count of entyes to load */
               WORD FAR* LUTData);        /* pointer to word-array of entrys */

  VOID      (_MVDFKT *ReadLUTEntrys)      /* get entrys of a desired LUT */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD LUTNr,                /* which LUT to read */
               WORD Start,                /* first entry to load */
               WORD Count,                /* count of entrys to load */
               WORD FAR* LUTData);        /* pointer to word-array for entrys */

  VOID      (_MVDFKT *SetAOI)             /* set Area Of Interest, valid  for all device actions */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XSize,                 /* x-size of AOI */
               INT YSize);                /* y-size of AOI */

  WORD      (_MVDFKT *ReadPixel)          /* read value of the desired pixel */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
					INT YPos);                 /* y-position */

  VOID      (_MVDFKT *WritePixel)         /* write value of the desired pixel */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               WORD Data);                /* data to write */

  VOID      (_MVDFKT *ReadPixelArray)     /* read values of mult. pixels in array */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               VEC_T FAR* vec,            /* array of positions to read */
               INT Count,                 /* count of pixels */
               INT inc,                   /* Size of Data-element in bytes */
               BYTE FAR* data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *WritePixelArray)    /* write values of mult. pixels from array */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               VEC_T FAR* vec,            /* array of positions to write */
               INT Count,                 /* count of pixels */
               INT inc,                   /* Size of Data-element in bytes */
               BYTE FAR* data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *ReadRow)            /* read value of the desired row */
				  (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               INT Count,                 /* count of pixels */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *WriteRow)           /* write value of the desired row */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               INT Count,                 /* count of Pixels */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *ReadCol)            /* read value of the desired column */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               INT Count,                 /* count of Pixels */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *WriteCol)           /* write value of the desired column */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               INT Count,                 /* count of Pixels */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *ReadLine)           /* read value of the desired line */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XStop,                 /* x-stop-position */
               INT YStop,                 /* y-stop-position */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *WriteLine)          /* write value of the desired line */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XStop,                 /* x-stop-position */
               INT YStop,                 /* y-stop-position */
					INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *ReadBlock)          /* read value of the desired block */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XSize,                 /* x-size */
               INT YSize,                 /* y-size */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* Pointer to byte-array for data */

  VOID      (_MVDFKT *WriteBlock)         /* write value of the desired block */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XSize,                 /* x-size */
               INT YSize,                 /* y-size */
               INT Inc,                   /* Size of Data-element in bytes */
               BYTE FAR* Data);           /* pointer to byte-array for data */

  VOID      (_MVDFKT *LoadFromFile)       /* read value of the desired block */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
					INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XSize,                 /* x-size */
               INT YSize,                 /* y-size */
               CHAR FAR* Name,            /* pointer to string with filename */
               INT Mode);                 /* Savemode */

  VOID      (_MVDFKT *SaveToFile)         /* write value of the desired block */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XStart,                /* x-start-position */
               INT YStart,                /* y-start-position */
               INT XSize,                 /* x-size */
               INT YSize,                 /* y-size */
               CHAR FAR* Name,            /* pointer to string with filename */
               INT Mode);                 /* SaveMode */

  CHAR FAR* (_MVDFKT *GetRowAdr)          /* lock and return row-address of image-buffer */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT y);                    /* y-position */

  VOID      (_MVDFKT *ReleaseRowAdr)      /* release buffer */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT y,                     /* y-position */
					VOID FAR* ptr);            /* Pointer from GetRowAdr */

  VOID      (_MVDFKT *CopyDev)            /* Copy image between devices */
              (DEV_T FAR* SourceDev,      /* source-device */
               DEV_T FAR* DestDev);       /* destination-device */

  INT       (_MVDFKT *SetZPS)             /* Zoom-Pan-Scroll */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT pan,
               INT scroll,
               INT zoom);

  WORD      (_MVDFKT* InterpretIni)       /* Interpret INI */
              (DEV_T FAR* dev,            /* pointer to device-key */
               CHAR FAR* filename,        /* name of INI file */
               CHAR FAR* FAR* ini_table,
               CHAR FAR* section);        /* name of section */

  VOID      (_MVDFKT* SetLogDevice)
              (DEV_T FAR* dev,            /* pointer to device-key */
               CHAR FAR* strlog);

  VOID      (_MVDFKT* MemTest)
              (DEV_T FAR* dev,            /* pointer to device-key */
              CHAR FAR* strMode);

  CHAR FAR* (_MVDFKT *GetMemAdr)          /* moves the memwindows and returns an address of image-buffer */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD  WndNr,               /* number of Memory Window */
               DWORD fg_addr,             /* Address of frame-grabber-memory */
               DWORD FAR* psize );        /* number of bytes to the window buttom */

  VOID      (_MVDFKT *ChipWriteData)      /* Write Chip Data */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD ChipNr,               /* chip number */
               WORD RegNr,                /* register number */
               WORD Mask,                 /* write mask */
               WORD Shift,                /* shift size */
               WORD Count, ...);          /* number of word-data */


  /* Hardware functions */

  WORD      (_MVDFKT *ReadRegisterWord)   /* return value of the specified register */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD offset);              /* offset to iobase of register */

  VOID      (_MVDFKT *WriteRegisterWord)  /* modify the specified register */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD offset,               /* offset to iobase of register */
               WORD data);                /* data to write to register */

  VOID      (_MVDFKT *ReadEEPROM)         /* Read contents of EEPROM into buffer */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD start,                /* Startaddress in EEPROM */
               WORD length,               /* Count of Bytes */
               BYTE FAR* Data);           /* Pointer to buffer */

  VOID      (_MVDFKT *WriteEEPROM)        /* Write contents of buffer into EEPROM */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD start,                /* Startaddress in EEPROM */
               WORD length,               /* Count of Bytes */
               BYTE FAR* Data);           /* Pointer to Buffer */

  DWORD     (_MVDFKT *ReadDigIO)          /* Read the digital IO-Port */
              (DEV_T FAR* dev);           /* pointer to device-key */

  VOID      (_MVDFKT *WriteDigIO)         /* Set the digital IO-Port */
              (DEV_T FAR* dev,            /* pointer to device-key */
               DWORD Data);               /* Data to writze */

  VOID      (_MVDFKT *GetInfo)            /* Read Info about Driver and Board */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INFO_T FAR* info);         /* info-struc */

  VOID      (_MVDFKT *InitBoard)
              (DEV_T FAR* dev);           /* pointer to device-key */

  VOID      (_MVDFKT *SetDisplaySize)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD xSize,
               WORD ySize,
               WORD HTotal,
               WORD VTotal,
               WORD VidClk,
               double HFreq);

  VOID      (_MVDFKT *SetDisplayPos)
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT HPos,                  /* horizontal display position */
               INT VPos);                 /* vertikal display position */

  VOID      (_MVDFKT *GetDisplayPos)
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT FAR* pHPos,            /* pointer to horizontal display position */
               INT FAR* pVPos);           /* pointer to vertikal display position */

  WORD      (_MVDFKT *SetKeyCol)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD keycolor);            /* keycolor */

  WORD      (_MVDFKT *SetMixDelay)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mixdelay);            /* mixdelay */

  VOID      (_MVDFKT* InitModul)          /* initialize device and return device-key */
              (DEV_T FAR* dev,            /* pointer to device-key */
               CHAR FAR* name,            /* name of config-file */
               CHAR FAR* FAR* ini_table,  /* init-struct */
               WORD modulnr,              /* Modul Number */
               MODULINI_T ModulInit);     /* init-func of driver */

  WORD      (_MVDFKT* SelIRQNr)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD IRQ);

  WORD      (_MVDFKT* SetIntrHandler)
              (DEV_T FAR* dev,            /* pointer to device-key */
               BYTE IntrType,
               INTRHANDLER INTRHandler);

  WORD      (_MVDFKT *SetDispMemXSize)    /* changes the VRAM X-Resolution */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD XRes);                /* horizontal VRAM - XSize */

  WORD      (_MVDFKT *SetIntrMask)        /* change the Mask for Hostinterrupts */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD intr_mask);           /* new interrupt mask */

  DWORD     (_MVDFKT *GetDeviceInfo)
				  (DEV_T FAR* dev,            /* pointer to device-key */
               WORD info_type,
               WORD wParam,
               DWORD lParam);

  VOID      (_MVDFKT *GetModuleInfo)
              (DEV_T FAR* dev,            /* pointer to device-key */
               BYTE Modules,              /* how many modules should be checked */
               INFO_T FAR* info);         /* info-structures */

  DWORD     (_MVDFKT* LoadCoff)
              (DEV_T FAR* dev,            /* pointer to device-key */
               CHAR FAR* filename);

  VOID      (_MVDFKT* GspExecute)
              (DEV_T FAR* dev,            /* pointer to device-key */
              DWORD entry_point);

  VOID      (_MVDFKT* ExecuteKernel)
              (DEV_T FAR* dev,            /* pointer to device-key */
              CHAR FAR* filename);

  VOID      (_MVDFKT *LCAFilename)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD lcanr,
               CHAR FAR* lcafile);

  INT       (_MVDFKT* SetAnalogValue)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD func,
               INT value);

  WORD      (_MVDFKT *TestFkt) ();        /* for extension */

  VOID      (_MVDFKT* LoadTimingRAM)
              (DEV_T FAR *dev);           /* pointer to device-key */

  VOID      (_MVDFKT *SetVideoTiming)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD ram,
               WORD start,
               WORD stop,
               WORD inc,
               WORD mask,
               WORD akt);

  WORD      (_MVDFKT *SelSyncInput)       /* select sync input and report active input */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD input);               /* determine input */

  VOID      (_MVDFKT *SetAcquireDef)
              (DEV_T FAR* dev,            /* pointer to device-key */
					ACQUIRE_DEF_T FAR* pAcqDef);

  VOID      (_MVDFKT *GetAcquireDef)
              (DEV_T FAR* dev,            /* pointer to device-key */
              ACQUIRE_DEF_T FAR* pAcqDef);

  VOID      (_MVDFKT *SetCSCMatrix)
              (DEV_T FAR* dev,            /* pointer to device-key */
               float FAR* csc_matrix);

  WORD      (_MVDFKT *SetGain)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD channel,
               WORD gain);                /* gain */

  WORD      (_MVDFKT *SetOffset)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD channel,
               WORD offset);              /* offset */

  WORD      (_MVDFKT *SetReference)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD channel,
               WORD nr,
               WORD val);                 /* reference */

  double    (_MVDFKT *SetPixelFreq)
              (DEV_T FAR* dev,            /* pointer to device-key */
               double freq);              /* pixel frequency */

  WORD      (_MVDFKT *SelChrominanceFilter)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD filter);

  VOID      (_MVDFKT *RAMTest)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Mode,
               DWORD addr1,
               DWORD addr2,
               DWORD mask);

  VOID      (_MVDFKT *DefineBank)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD bank,                 /* bank number */
               DWORD start_addr,          /* start address */
               DWORD line_inc,            /* line increment */
               WORD pix_incr,             /* pixel increment */
               WORD pix_size);            /* pixel size */

  WORD      (_MVDFKT *SelDisplayMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode);                /* display mode */

  VOID      (_MVDFKT* DefDisplayMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode,
               CHAR* name,
               WORD type,
               WORD xvisi,
               WORD yvisi,
               WORD xtotal,
               WORD ytotal,
               WORD xstart,
               WORD ystart,
               WORD hsync,
               WORD vsync,
               WORD pol,
               WORD vid,
               WORD gain,
               WORD hFreq);

  DISP_MODE_T FAR* (_MVDFKT *GetDisplayModePtr)
                     (DEV_T FAR* dev,     /* pointer to device-key */
                      WORD mode);

  DISP_MODE_T FAR* (_MVDFKT *GetFirstDisplayModePtr)
                     (DEV_T FAR* dev);    /* pointer to device-key */

  WORD      (_MVDFKT *GetActDisplayMode)
              (DEV_T FAR* dev);           /* pointer to device-key */

  WORD      (_MVDFKT * SearchDisplayMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD xvisi,
               WORD yvisi);

  BANK_DEF_T FAR* (_MVDFKT *GetBankDefPtr)
                     (DEV_T FAR* dev,     /* pointer to device-key */
                      WORD bank);

  WORD      (_MVDFKT *SetAcquireBank)
              (DEV_T FAR* dev,            /* pointer to device-key */
              WORD bank);     

  WORD      (_MVDFKT *SelCSCMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode);

  VOID      (_MVDFKT *LoadCSCLut)
              (DEV_T FAR* dev,            /* pointer to device-key */
               CHAR* file,
               WORD lut0,
               WORD lut1,
               WORD lut2,
					WORD lut3);

  WORD FAR* (_MVDFKT *SelCSCToPipeMux)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Pipe0,
               WORD Pipe1,
               WORD Pipe2);

  WORD FAR* (_MVDFKT *SelPipeToBusMux)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Byte0,
               WORD Byte1,
               WORD Byte2,
               WORD Byte3);

  WORD      (_MVDFKT* SetPaletteMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Mode);

  VOID      (_MVDFKT *AddPCIChip)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Vendor,
               WORD Chip,
               WORD Typ,
               CHAR FAR* Description);

  WORD      (_MVDFKT *SelBoardMode)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD Mode);

  DWORD     (_MVDFKT *SetPixelMask)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD mode,
               DWORD mask);

  DWORD     (_MVDFKT *AbortSnap)          /* aborts the aktiv snap */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD wParam,
               DWORD lParam);

  WORD      (_MVDFKT *InitDigIO)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD config); 

  DWORD     (_MVDFKT *SetZoom)            /* set vertical and horizontal zoom */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT XZoom,                 /* horizontal zoom */
               INT YZoom);                /* vertical zoom */

  DWORD     (_MVDFKT *SetTimeout)
              (DEV_T FAR* dev,            /* pointer to device-key */
               DWORD Timeout);            /* Timeout-Value */

  DWORD     (_MVDFKT *SetScalerMode)
              (DEV_T FAR* dev,          /* pointer to device-key */
               WORD mode,
               DWORD p0,
               DWORD p1 );

  double      (_MVDFKT *SetLowPassFreq)
              (DEV_T FAR*   dev,
               double       freq);

#define SCS_VIDEO   1    /* mvSetSignalCoupling ( .. signal .. ) */
#define SCS_CLOCK   2
#define SCS_SYNC    3

#define SCM_DC      1    /* mvSetSignalCoupling ( .. mode .. ) */
#define SCM_AC      2

  WORD      (_MVDFKT *SetSignalCoupling)
              (DEV_T FAR*   dev,
               WORD         signal,
               WORD         mode);

  WORD      (_MVDFKT *SetClockDelay)
              (DEV_T FAR*   dev,
               WORD         steps);

  DWORD    (_MVDFKT *GetDeviceProperty)
              (DEV_T FAR*   dev,
               char FAR     *property,
               char FAR     *field,
               WORD         index,
               DWORD        *result);

  DWORD    (_MVDFKT *SelSyncPolarity)
               (DEV_T FAR* dev,
                WORD polarity );

  DWORD     (_MVDFKT *ReadPixelLong)      /* read value of the desired pixel */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos);                 /* y-position */

  VOID      (_MVDFKT *WritePixelLong)     /* write value of the desired pixel */
              (DEV_T FAR* dev,            /* pointer to device-key */
               INT rgb,                   /* select color ( Red, Green, Blue,.. ) */
               INT XPos,                  /* x-position */
               INT YPos,                  /* y-position */
               DWORD Data);               /* data to write */

  DWORD (_MVDFKT *WaitForInterrupt )
						(DEV_T FAR* dev,
                           DWORD intr_mask, 
                           DWORD Timeout, 
                           DWORD options );

  DWORD    (_MVDFKT *ReadRegisterDWord)   /* return value of the specified register */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD offset);              /* offset to iobase of register */

  VOID     (_MVDFKT *WriteRegisterDWord)  /* modify the specified register */
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD offset,               /* offset to iobase of register */
               DWORD data);                /* data to write to register */

  VOID     (_MVDFKT *SetAcquireClipList  ) /* Setup run-length-coded acquire mask */
                  ( DEV_T FAR* dev,                       /* pointer to device-key */
                    VOID  FAR* pClipList);        /* pointer to ClipList */

  VOID     (_MVDFKT *DefineMaskedBank)
              (DEV_T FAR* dev,            /* pointer to device-key */
               WORD bank,                 /* bank number */
               DWORD start_addr,          /* start address */
               DWORD line_inc,            /* line increment */
               WORD pix_incr,             /* pixel increment */
               WORD pix_size,             /* pixel size */
               DWORD* plColMask, 
					INT*   pShift );

  BOOL      (_MVDFKT *AllocFrameBuffer) 
              ( DEV_T FAR* dev,           /* pointer to device-key */
              DWORD BufferSize );         /* frame buffer size */

  DWORD      (_MVDFKT *GetRegisterInfo) 
              ( DEV_T FAR* dev,           /* pointer to device-key */
				WORD  RegNr,			  /* register number */
				WORD  InfoType,			  /* what dow we want to know about it */
				DWORD Param ) ;

  DWORD      (_MVDFKT *SetExpose)
              (DEV_T FAR* dev, 
               WORD  mode,
               DWORD low, 
               DWORD total );


  DWORD       (_MVDFKT *SetShutter)
               (DEV_T FAR* dev,
                WORD  mode,
                DWORD shutter );

  WORD        (_MVDFKT *SetMSBDataPos) 
                (DEV_T* dev, /* pointer to device-key */
					  WORD MSBPos); /*Position of the MSB*/

  VOID      (_MVDFKT *SingleSnapAsync)    /* snap one image asynchron (WIN32 only!) */
              (DEV_T FAR* dev,            /* pointer to device-key */
               void * hEvent );           /* Eventhandle to signal after image is finish */

  VOID      (_MVDFKT *MultipleSnapAsync)       /* multible snap asychron (WIN32 only!) */
              (DEV_T FAR* dev,            /* pointer to device-key */    
               WORD cnt,                  /* number of pictures    */
               SNAP_TABLE_T FAR* p,      /* pointer to snap table */
               void * hEvent );          /* Eventhandle to signal after each image */

  VOID      (_MVDFKT *Last)();            /* dummy-ptr 2 */

#if defined(__cplusplus)
} DEV_T;
#else
};
#endif



/* resident functions */


_MVAPI(DEV_T FAR*) mvOpenDevice   /* initialize device and return device-key */
           (CHAR FAR* name,               /* name of config-file */
            CHAR FAR* FAR* ini_table,     /* init-struct */
            INT FAR* result,              /* reports error-code */
            DRVINI_T DriverInit);         /* init-func of driver */

_MVAPI(DEV_T FAR*) mvDuplicateDeviceHandle   /* Create a new Handle for opened device */
           (DEV_T FAR* dev);              /* pointer to device-key */

_MVAPI(INT) mvCloseDevice  /* release board and free memory */
			  (DEV_T FAR* dev);              /* pointer to device-key */

typedef VOID (_MVDFKT *USER_FUNC_T)();

_MVAPI(WORD) mvAddUserFunction
           (const char FAR* FktName,
            USER_FUNC_T FktPtr);

_MVAPI(WORD) mvDelAllUserFunctions (VOID);

_MVAPI(VOID)  mvInterpreter
           (DEV_T FAR* dev,
            char FAR* promtstr,
            char FAR* exitstr,
            PRINTFKT prnt,
            GETSFKT gts);

_MVAPI(BOOL) mvInterpretString (DEV_T FAR* dev, CHAR FAR* str);

_MVAPI(char FAR*) mvGetIniFilename(DEV_T FAR* dev);


_MVAPI(INT) pcgLibInit
           (DEV_T FAR* dev,
            CHAR FAR *filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) lccdLibInit
           (DEV_T FAR* dev,
            CHAR FAR *filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pcpLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pvhsLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table,
            WORD modulnr);

_MVAPI(INT) mgvsLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table,
            WORD modulnr);

_MVAPI(INT) mrgbLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
				CHAR FAR* FAR* ini_table,
            WORD modulnr);

_MVAPI(INT) mdigLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table,
            WORD modulnr);

_MVAPI(INT) pcibLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pcigmLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table,
            WORD modulnr);

_MVAPI(INT) pciycLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pciscLibInit
			  (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pcisgLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) pcisrgbLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
            CHAR FAR* FAR* ini_table);

_MVAPI(INT) sdigdrvLibInit
           (DEV_T FAR* dev,
            CHAR FAR* filename,
				CHAR FAR* FAR* ini_table);

#define slcLibInit slLibInit
#define slgLibInit slLibInit
#define sldLibInit slLibInit

_MVAPI(INT) slLibInit
			  (DEV_T FAR* dev,
				CHAR FAR* filename,
				CHAR FAR* FAR* ini_table);

_MVAPI(INT) mxLibInit
			  (DEV_T FAR* dev,
				CHAR FAR* filename,
				CHAR FAR* FAR* ini_table);

_MVAPI(INT) cmosdrvLibInit
			  (DEV_T FAR* dev,
				CHAR FAR* filename,
				CHAR FAR* FAR* ini_table);

_MVAPI(INT) pccddrvLibInit
			  (DEV_T FAR* dev,
				CHAR FAR* filename,
				CHAR FAR* FAR* ini_table);


_MVAPI(WORD) GSPReady (DEV_T FAR* dev);

extern VOID* pDOSMem_g;
extern DWORD hDOSMem_g;

_MVAPI(VOID) mvSetErrString
			  (DEV_T FAR* dev,
				CHAR FAR* errstring);
#define SetErrStr(dev,err,str)  { if (dev->ErrorCode == 0) { dev->ErrorCode = err; mvSetErrString (dev, (CHAR FAR*) str); }}

#define mvSelInput(dev,a)                       dev->SelInput(dev,a)
#define mvSelOverlay(dev,a)                     dev->SelOverlay(dev,a)
#define mvSelVideoMode(dev,a)                   dev->SelVideoMode(dev,a)
#define mvSelDisplaySource(dev,a)               dev->SelDisplaySource(dev,a)
#define mvSelColorMode(dev,a)                   dev->SelColorMode(dev,a)
#define mvSelInputLUT(dev,a)                    dev->SelInputLUT(dev,a)
#define mvSelInputClk(dev,a)                    dev->SelInputClk(dev,a)
#define mvSelExtTrig(dev,a)                     dev->SelExtTrig(dev,a)
#define mvSelIntr(dev,a)                        dev->SelIntr(dev,a)
#define mvSelOutputSyncPol(dev,a)               dev->SelOutputSyncPol(dev,a)
#define mvSelOutputSyncLine(dev,a)              dev->SelOutputSyncLine(dev,a)
#define mvSingleSnap(dev,a)                     dev->SingleSnap(dev,a)
#define mvContinousSnap(dev)                    dev->ContinousSnap(dev)
#define mvStopContinousSnap(dev)                dev->StopContinousSnap(dev)
#define mvWriteLUTLin(dev,a)                    dev->WriteLUTLin(dev,a)
#define mvWriteLUTLinInv(dev,a)                 dev->WriteLUTLinInv(dev,a)
#define mvWriteLUTBin(dev,a,t)                  dev->WriteLUTBin(dev,a,t)
#define mvWriteLUTBinInv(dev,a,t)               dev->WriteLUTBinInv(dev,a,t)
#define mvWriteLUTStretch(dev,a,b,c,d,e)        dev->WriteLUTRamp(dev,a,b,c,d,e)
#define mvWriteLUT332(dev,a)                    dev->WriteLUT332(dev,a)
#define mvWriteLUTEGACol(dev,a)                 dev->WriteLUTEGACol(dev,a)
#define mvWriteLUTVGACol(dev,a)                 dev->WriteLUTVGACol(dev,a)
#define mvWriteLUTEntries(dev,a,b,c,d)          dev->WriteLUTEntrys(dev,a,b,c,d)
#define mvReadLUTEntries(dev,a,b,c,d)           dev->ReadLUTEntrys(dev,a,b,c,d)
#define mvWriteLUTEntrys(dev,a,b,c,d)           dev->WriteLUTEntrys(dev,a,b,c,d)
#define mvReadLUTEntrys(dev,a,b,c,d)            dev->ReadLUTEntrys(dev,a,b,c,d)
#define mvSetAOI(dev,a,b,c,d)                   dev->SetAOI(dev,a,b,c,d)
#define mvReadPixel(dev,a,b,c)                  dev->ReadPixel(dev,a,b,c)
#define mvWritePixel(dev,a,b,c,d)               dev->WritePixel(dev,a,b,c,d)
#define mvReadPixelArray(dev,a,b,c,d,e)         dev->ReadPixelArray(dev,a,b,c,d,e)
#define mvWritePixelArray(dev,a,b,c,d,e)        dev->WritePixelArray(dev,a,b,c,d,e)
#define mvReadRow(dev,a,b,c,d,e,f)              dev->ReadRow(dev,a,b,c,d,e,f)
#define mvWriteRow(dev,a,b,c,d,e,f)             dev->WriteRow(dev,a,b,c,d,e,f)
#define mvReadCol(dev,a,b,c,d,e,f)              dev->ReadCol(dev,a,b,c,d,e,f)
#define mvWriteCol(dev,a,b,c,d,e,f)             dev->WriteCol(dev,a,b,c,d,e,f)
#define mvReadLine(dev,a,b,c,d,e,f,g)           dev->ReadLine(dev,a,b,c,d,e,f,g)
#define mvWriteLine(dev,a,b,c,d,e,f,g)          dev->WriteLine(dev,a,b,c,d,e,f,g)
#define mvReadBlock(dev,a,b,c,d,e,f,g)          dev->ReadBlock(dev,a,b,c,d,e,f,g)
#define mvWriteBlock(dev,a,b,c,d,e,f,g)         dev->WriteBlock(dev,a,b,c,d,e,f,g)
#define mvLoadImage(dev,a,b,c,d,e,f,g)          dev->LoadFromFile(dev,a,b,c,d,e,f,g)
#define mvSaveImage(dev,a,b,c,d,e,f,g)          dev->SaveToFile(dev,a,b,c,d,e,f,g)
#define mvGetRowAdr(dev,a,b)                    dev->GetRowAdr(dev,a,b)
#define mvReleaseRowAdr(dev,a,b,c)              dev->ReleaseRowAdr(dev,a,b,c)
#define mvCopyDev(dev1,dev2)                    dev->CopyDev(dev1,dev2)
#define mvReadRegisterWord(dev,a)               dev->ReadRegisterWord(dev,a)
#define mvWriteRegisterWord(dev,a,b)            dev->WriteRegisterWord(dev,a,b)
#define mvReadEEPROM(dev,a,b,c)                 dev->ReadEEPROM(dev,a,b,c)
#define mvWriteEEPROM(dev,a,b,c)                dev->WriteEEPROM(dev,a,b,c)
#define mvReadDigIO(dev)                        dev->ReadDigIO(dev)
#define mvWriteDigIO(dev,a)                     dev->WriteDigIO(dev,a)
#define mvGetInfo(dev,a)                        dev->GetInfo(dev,a)

/* New Functions: (22.2.1993) */
#define mvMultipleSnap(dev,c,p)                 dev->MultipleSnap(dev,c,p)

/* New Functions: ( 8.1993 ) */

#define mvInterpretIni(dev,f,i,s)               dev->InterpretIni(dev,f,i,s)
#define mvGetMemAdr(dev,w,a,ps)                 dev->GetMemAdr(dev,w,a,ps)
#define mvChipWriteData                         dev->ChipWriteData
#define mvSetDisplaySize(dev,x,y,h,v,vc,f)      dev->SetDisplaySize(dev,x,y,h,v,vc,f)
#define mvSetDisplayPos(dev,h,v)                dev->SetDisplayPos(dev,h,v)
#define mvGetDisplayPos(dev,ph,pv)              dev->GetDisplayPos(dev,ph,pv)
#define mvSetKeyCol(dev,w)                      dev->SetKeyCol(dev,w)
#define mvSetMixDelay(dev,w)                    dev->SetMixDelay(dev,w)
#define mvSetDispMemXSize(dev,x)                dev->SetDispMemXSize(dev,x)

#define mvSetZPS(dev,z,p,s)                     dev->SetZPS(dev,z,p,s)
#define mvInitModule(dev,n,i,d,w)               dev->InitModul(dev,n,i,d,w)
#define mvSelIRQNo(dev,irq)                     dev->SelIRQNr(dev,irq)
#define mvInstISR(dev,t,ih)                     dev->SetIntrHandler(dev,t,ih)
#define mvSetIntrMask(dev,m)                    dev->SetIntrMask(dev,m)

#define mvLoadCoff(dev,f)                       dev->LoadCoff(dev,f)
#define mvGspExecute(dev,e)                     dev->GspExecute(dev,e)
#define mvExecuteKernel(dev,f)                  dev->ExecuteKernel(dev,f)
#define mvSetAcquireDef(dev,p)                  dev->SetAcquireDef(dev,p)
#define mvGetAcquireDef(dev,p)                  dev->GetAcquireDef(dev,p)
#define mvSetCSCMatrix(dev,m)                   dev->SetCSCMatrix(dev,m)

#define mvLoadTimingRAM(dev)                    dev->LoadTimingRAM(dev)
#define mvSetVideoTiming(dev,r,a,e,i,m,akt)     dev->SetVideoTiming(dev,r,a,e,i,m,akt)
#define mvSelSyncInput(dev,n)                   dev->SelSyncInput(dev,n)
#define mvLCAFilename(dev,n,f)                  dev->LoadLCAFile(dev,n,f)

#define mvSetGain(dev,c,v)                      dev->SetGain(dev,c,v)
#define mvSetOffset(dev,c,v)                    dev->SetOffset(dev,c,v)
#define mvSetReference(dev,c,m,v)               dev->SetReference(dev,c,m,v)
#define mvSetPixelFreq(dev,f)                   dev->SetPixelFreq(dev,f)
#define mvSelChrominanceFilter(dev,f)           dev->SelChrominanceFilter(dev,f)
#define mvRAMTest(dev,m,a1,a2)                  dev->RAMTest(dev,m,a1,a2)
#define mvDefineBank(dev,b,a,li,pi,ps)          dev->DefineBank(dev,b,a,li,pi,ps)
#define mvSetAnalogValue(dev,f,v)               dev->SetAnalogValue(dev,f,v)

#define mvSelDisplayMode(dev,mode)              dev->SelDisplayMode(dev,mode)
#define mvDefDisplayMode(dev,mode,name,type,xvisi,yvisi,xtotal,ytotal,xstart,ystart,hsync,vsync,pol,vid,gain,hFreq)  dev->DefDisplayMode(dev,mode,name,type,xvisi,yvisi,xtotal,ytotal,xstart,ystart,hsync,vsync,pol,vid,gain,hFreq)
#define mvGetDisplayModePtr(dev,mode)           dev->GetDisplayModePtr(dev,mode)
#define mvGetFirstDisplayModePtr(dev)           dev->GetFirstDisplayModePtr(dev)
#define mvGetActDisplayMode(dev)                dev->GetActDisplayMode(dev)
#define mvSearchDisplayMode(dev,x,y)            dev->SearchDisplayMode(dev,x,y)

#define mvSelCSCMode(dev,mode)                  dev->SelCSCMode(dev,mode)
#define mvLoadCSCLut(dev,file,l0,l1,l2,l3)      dev->LoadCSCLut(dev,file,l0,l1,l2,l3)
#define mvSelCSCToPipeMux(dev,p0,p1,p2)         dev->SelCSCToPipeMux(dev,p0,p1,p2)
#define mvSelPipeToBusMux(dev,b0,b1,b2,b3)      dev->SelPipeToBusMux(dev,b0,b1,b2,b3)

#define mvSetPaletteMode( dev, Mode )           dev->SetPaletteMode( dev,Mode )

/* New Functions: ( 10.1994 ) */

#define mvSelBoardMode(dev,Mode)                dev->SelBoardMode(dev,Mode)
#define mvSetPixelMask(dev,mode,mask)           dev->SetPixelMask(dev,mode,mask)

#define mvGetDeviceInfo(dev,info_type,w,l)      dev->GetDeviceInfo(dev,info_type,w,l)
#define mvAbortSnap(dev,w,l)                    dev->AbortSnap(dev,w,l)
#define mvInitDigIO(dev,config)                 dev->InitDigIO(dev,config)
#define mvSetZoom(dev,type,zoom)                dev->SetZoom(dev,type,zoom)

#define mvSetAcquireBank(dev, bank)             dev->SetAcquireBank(dev,bank)
#define mvGetModuleInfo(dev,mod,inf)            dev->GetModuleInfo(dev,mod,inf)

#define mvSetTimeout(dev, timeout)              dev->SetTimeout(dev,timeout)

#define mvGetBankDefPtr(dev,bank)               dev->GetBankDefPtr(dev,bank)
#define mvSetScalerMode(dev,mode,p0,p1)         dev->SetScalerMode(dev,mode,p0,p1)

/* New Functions: ( 20.2.96 ) */

#define mvSetLowPassFreq(dev,freq)              dev->SetLowPassFreq(dev,freq)
#define mvSetSignalCoupling(dev,signal,mode)    dev->SetSignalCoupling(dev,signal,mode)
#define mvSetClockDelay(dev,steps)              dev->SetClockDelay(dev,steps)
#define mvGetDeviceProperty(dev,pp,fld,ix,res)  dev->GetDeviceProperty(dev,pp,fld,ix,res)
#define mvSelSyncPolarity(dev,pol)              dev->SelSyncPolarity(dev,pol)


/* New Functions: ( 4.2.97 ) */

#define mvReadPixelLong(dev,a,b,c)              dev->ReadPixelLong(dev,a,b,c)
#define mvWritePixelLong(dev,a,b,c,d)           dev->WritePixelLong(dev,a,b,c,d)


/* New Functions: ( 17.2.97 ) */
#define mvWaitForInterrupt(dev,mask,t,o)        dev->WaitForInterrupt(dev,mask,t,o)

#define mvReadRegisterDWord(dev,a)              dev->ReadRegisterDWord(dev,a)
#define mvWriteRegisterDWord(dev,a,b)           dev->WriteRegisterDWord(dev,a,b)


/* New Function: ( 16.4.97 ) */
#define mvSetAcquireClipList(dev,p)             dev->SetAcquireClipList(dev,p)

/* New Function: ( 16.6.97 )  uf/15.2.99 Parameter changed */
#define mvDefineMaskedBank(dev,b,a,li,pi,ps,cm,sm)   dev->DefineMaskedBank(dev,b,a,li,pi,ps,cm,sm) 

/* New Function: ( 04.09.97 ) */
#define mvAllocFrameBuffer(dev,BufferSize)       dev->AllocFrameBuffer(dev, BufferSize)

/* New Function: ( 28.1.98 ) */
#define mvGetRegisterInfo(dev,reg,inf)			 dev->GetRegisterInfo(dev,reg,inf,0)
#define mvGetRegisterIndex(dev,name)          dev->GetRegisterInfo(dev,0,12,(DWORD)(name))


/* New Function: ( 16.3.98 ) */
#define mvSetExpose(dev,mode,low,total)       dev->SetExpose(dev,mode,low,total) 

/* New Function: ( 17.4.98 ) */
#define mvSetShutter(dev, mode, shutter)        dev->SetShutter(dev, mode, shutter) 


/* New Function: ( 31.8.98 ) */
#define mvSetMSBDataPos(dev, MSBPos)          dev->SetMSBDataPos(dev, MSBPos)

/* New Functions: ( 24.2.99 ) */
#define mvSingleSnapAsync(dev,event)          dev->SingleSnapAsync(dev,event)
#define mvMultipleSnapAsync(dev,cnt,p,event)  dev->MultipleSnapAsync(dev,cnt,p,event)


#ifdef __cplusplus
}
#endif




/******************************************************************************
*                              compiler switches                              *
*******************************************************************************/

/* ----------------------------------------------- struct alignment = default */
#if defined(_MSC_VER)
#pragma pack()
#endif

#if defined(__BORLANDC__)
#pragma option -a.
#endif


#if defined(__HIGHC__)
#pragma pop_align_members();
#endif

#if defined(__WATCOMC__)
#pragma pack ( pop );
#endif




#endif






































