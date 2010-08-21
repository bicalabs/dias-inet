// CmdLine.cpp : implementation file
//

#include "stdafx.h"
#include "Dias.h"
#include "CmdLine.h"


// CCmdLine

CCmdLine::CCmdLine()
{
	m_bPrg = false;
	m_nWaiting = 0;
}

CCmdLine::~CCmdLine()
{
}

void CCmdLine::ParseParam (const char* pszParam, BOOL bFlag, BOOL bLast) {
	CCommandLineInfo::ParseParam (pszParam, bFlag, bLast);
	if (bFlag == TRUE && pszParam == CString ("prg")) {
		m_bPrg = true;
		m_nWaiting = 1;
	}
	if (bFlag == FALSE) {
		switch (m_nWaiting) {
			case 1:
				m_strPrg = pszParam;
				break;
		}
		m_nWaiting = 0;
	}
}

void CCmdLine::ExecuteParams () {
	if (m_bPrg == true && m_strPrg == "noise-4-hvg2med1") {
		theApp.Documents[aimActive].OnToolsBuildNewnoiseremoveproc4 ();
		theApp.CloseAllDocuments (TRUE);
		exit (0);
	}
	if (m_bPrg == true && m_strPrg == "noise-8-hvg2med1hvg") {
		theApp.Documents[aimActive].OnToolsBuildNewnoiseremoveproc8 ();
		theApp.CloseAllDocuments (TRUE);
		exit (0);
	}
}

// CCmdLine member functions
