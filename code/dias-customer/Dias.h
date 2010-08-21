#pragma once

#include "resource.h"       // main symbols
#include "DiasAPI.h"

#define _AIM_MAINMODULE

#define	UPDHA_MODE	0x00000001
#define	UPDHA_DENSSCALE	0x00000002
#define	UPDHA_GEOMSCALE	0x00000004
#define	UPDHA_MARKSTYLE	0x00000008
#define	UPDHA_IMAGE	0x00000010
#define	UPDHA_MARK	0x00000020
#define	UPDHA_PATCH	0x00000040
#define	UPDHA_TEXT	0x00000080
#define	UPDHA_METER	0x00000100
#define	UPDHA_SURVEY	0x00001000
#define	UPDHA_FIELD	0x00002000

#define	OBJT_IMAGE	0x00000001
#define	OBJT_CHANNEL	0x00000002
#define	OBJT_PATCH	0x00000003
#define	OBJT_MARK	0x00000004
#define	OBJT_TEXT	0x00000005
#define	OBJT_METER	0x00000006
#define	OBJT_SURVEY	0x00000007
#define	OBJT_FIELD	0x00000008

class CMainFrame;

class CDiasApp : public CWinApp {
// *** Constructors
public:
	CDiasApp();

public:
	CDiasAPIDocs		Documents;
	CDiasAPIDensScales	DensScales;
	CDiasAPIGeomScales	GeomScales;
	CDiasAPIMarkStyles	MarkStyles;

// *** Variables
protected:
	CArray<CBitmap*,CBitmap*>	m_bmpaLoad;

	CMainFrame*	m_pMainFrame;
	uvar32_64	m_nObjects[OBJT_FIELD];

// *** Functions
public:
	CString		LoadString (UINT nID);
	CBitmap*	LoadBitmap (UINT nID);
	void		PrepareMenu (CaImMenu* pMenu, UINT nID);

	void		Update (udword action);
	void		MacroStart (CString strName);
	void		MacroStop ();
	void		MacroPause ();
	void		StatusText (CString strText);
	void		StatusState (UINT nID);
	void		ObjectsAdd (udword type, CObject* obj);
	void		ObjectsRemove (udword type, CObject* obj);

// *** IDE Code
	//{{AFX_VIRTUAL(CDiasApp)
public:
	virtual BOOL	InitInstance();
	virtual int	ExitInstance();
protected:
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDiasApp)
protected:
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "util.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
