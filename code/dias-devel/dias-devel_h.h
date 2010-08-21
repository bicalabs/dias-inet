

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sun Jul 25 00:58:22 2004
 */
/* Compiler settings for .\dias-devel.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __dias2Ddevel_h_h__
#define __dias2Ddevel_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Idiasdevel_FWD_DEFINED__
#define __Idiasdevel_FWD_DEFINED__
typedef interface Idiasdevel Idiasdevel;
#endif 	/* __Idiasdevel_FWD_DEFINED__ */


#ifndef __diasdevel_FWD_DEFINED__
#define __diasdevel_FWD_DEFINED__

#ifdef __cplusplus
typedef class diasdevel diasdevel;
#else
typedef struct diasdevel diasdevel;
#endif /* __cplusplus */

#endif 	/* __diasdevel_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __diasdevel_LIBRARY_DEFINED__
#define __diasdevel_LIBRARY_DEFINED__

/* library diasdevel */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_diasdevel;

#ifndef __Idiasdevel_DISPINTERFACE_DEFINED__
#define __Idiasdevel_DISPINTERFACE_DEFINED__

/* dispinterface Idiasdevel */
/* [uuid] */ 


EXTERN_C const IID DIID_Idiasdevel;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3910DA92-03F4-4B8D-8BFE-439ECA15D7F1")
    Idiasdevel : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IdiasdevelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Idiasdevel * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Idiasdevel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Idiasdevel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Idiasdevel * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Idiasdevel * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Idiasdevel * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Idiasdevel * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IdiasdevelVtbl;

    interface Idiasdevel
    {
        CONST_VTBL struct IdiasdevelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Idiasdevel_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Idiasdevel_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Idiasdevel_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Idiasdevel_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Idiasdevel_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Idiasdevel_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Idiasdevel_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Idiasdevel_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_diasdevel;

#ifdef __cplusplus

class DECLSPEC_UUID("7AFF2E3E-5185-47FB-81FA-B8C36830C497")
diasdevel;
#endif
#endif /* __diasdevel_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


