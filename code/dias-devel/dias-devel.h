// dias-devel.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "dias-devel_i.h"

// CDiasDevelApp:
// See dias-devel.cpp for the implementation of this class
//

class CDiasDevelApp : public CWinApp
{
public:
	CDiasDevelApp();

public:
	sdword		m_hobj;
	CKstp		m_kstp;

protected:
	static ulong m_nMemUsed;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	afx_msg void Connect();

	void Plugin_Hthalam_AVP_Mask ();

public:
	afx_msg void Disconnect();
	BOOL ExitInstance(void);
	afx_msg void Download();
	afx_msg void Upload();
	afx_msg void SelectStorage();
	afx_msg void DeleteStorage();

	void* New (size_t s) {
		void *p = new ubyte[s];
		if (!p) {
			_CrtMemState state;
			_CrtMemCheckpoint (&state);
			_CrtDumpMemoryLeaks ();
		} else
			CDiasDevelApp::m_nMemUsed += s;
		return p;
	};
	afx_msg void OnPlugMemorytest1();
};

extern CDiasDevelApp theApp;