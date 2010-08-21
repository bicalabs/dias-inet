//---------------------------------------------------------------------------
#ifndef ISignalsH
#define ISignalsH

#ifndef _IFKT
#define _IFKT __stdcall
#endif

/*------------------Predefined Signals-------------------------------*/
#define SIG_INTEG_START  0
#define SIG_INTEG_STOP   1
#define SIG_TRIGGER      2
#define SIG_FLUSH        3

#define SIG_USER         8

#define MAX_SIGNAL_NAME 30
#define MAX_SIGNAL_MASK 16


class ISignals
{
    public:
        virtual void _IFKT Signal( int SignalNr ) = 0;
        virtual BOOL _IFKT IsPulse( int SignalNr ) = 0;
        virtual char* _IFKT GetSignalName( int SignalNr ) = 0;
        virtual BOOL _IFKT SetSignalDefinition( int SignalNr, char* name, char* mask, int time ) = 0;
        virtual BOOL _IFKT Save() = 0;
        virtual BOOL _IFKT Load() = 0;
};

#ifndef GRABBER_DLL
#define _GRABBER_IMPEXP  __declspec(dllimport)
#else /* only for building the DLL */
#define _GRABBER_IMPEXP  __declspec(dllexport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
_GRABBER_IMPEXP ISignals* __stdcall GetISignals();


typedef ISignals* (__stdcall * TGetISignals )( void );


#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
#endif
