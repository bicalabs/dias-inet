// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxdisp.h>

// This macro is the same as IMPLEMENT_OLECREATE, except it passes TRUE
// for the bMultiInstance parameter to the COleObjectFactory constructor.
// We want a separate instance of this application to be launched for
// each automation proxy object requested by automation controllers.
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
		RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
	const AFX_DATADEF GUID class_name::guid = \
		{ l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
#endif // IMPLEMENT_OLECREATE2

#import <msxml4.dll>
using namespace MSXML2;

#include <sysconf.h>
#include <libnodes.h>
#include "..\dias-ks\dias.h"

#include "Kstp.h"

struct ksinfo {
	char*	about;
	char*	descr;
	char*	manual;
	char*	copyright;
	char*	source;
	char*	check;
};

struct ksval {
	udword	n;
	rtbyte	r;
	CString	str;
	void*	ptr;
	udword	size;
};

struct ksarg {
	ksarg*	prev;
	ksarg*	next;
	char*	title;
	char*	name;
	int	gentype;
	int	subtype;
	ksval	min;
	ksval	max;
	ksval	deflt;
	ksval	val;
	CString	str;
};

struct kscmd {
	kscmd*	prev;
	kscmd*	next;
	char*	title;
	char*	name;
	udword	code;
	ksinfo	info;
	bool	state;
	ksarg*	args;
	ksarg*	rets;
};

struct ksmtx_factors {
	ksmtx_factors*	prev;
	ksmtx_factors*	next;
	udword	dimx;
	udword	dimy;
	CString	name;
	rtbyte	*factors;
};

struct ksmtx_order {
	ksmtx_order*	prev;
	ksmtx_order*	next;
	udword	dimx;
	udword	dimy;
	CString	name;
	sdword	*order;
};
