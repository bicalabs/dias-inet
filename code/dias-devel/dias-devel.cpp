// dias-devel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "dias-devel.h"
#include "MainDlg.h"
#include <initguid.h>
#include "dias-devel_i.c"
#include ".\dias-devel.h"
#include "..\dias-ks\cmddef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
  FILE *fp_kstp_log;
#endif


// CDiasDevelApp


BEGIN_MESSAGE_MAP(CDiasDevelApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_COMMAND(ID_FILE_CONNECT, Connect)
	ON_COMMAND(ID_FILE_DISCONNECT, Disconnect)
	ON_COMMAND(ID_FILE_DOWNLOAD, Download)
	ON_COMMAND(ID_FILE_UPLOAD, Upload)
	ON_COMMAND(ID_FILE_SELECTSTORAGE, SelectStorage)
	ON_COMMAND(ID_FILE_DELETESTORAGE, DeleteStorage)
	ON_COMMAND(ID_PLUG_HTHALAMAVP, Plugin_Hthalam_AVP_Mask)
	ON_COMMAND(ID_PLUG_MEMORYTEST1, OnPlugMemorytest1)
END_MESSAGE_MAP()


// CDiasDevelApp construction

ulong CDiasDevelApp::m_nMemUsed = 0;

CDiasDevelApp::CDiasDevelApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDiasDevelApp object

CDiasDevelApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x320D5912, 0xDB68, 0x4A93, { 0xB5, 0x7B, 0x98, 0x56, 0x50, 0x4B, 0xCA, 0x4B } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CDiasDevelApp initialization

BOOL CDiasDevelApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
/*	WSADATA wsdata;
	WSAStartup(0x0101,&wsdata);*/

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// Parse command line for automation or reg/unreg switches.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register class factories via CoRegisterClassObject().
		COleTemplateServer::RegisterAll();
	}
	// App was launched with /Unregserver or /Unregister switch.  Remove
	// entries from the registry.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;
	}

	m_hobj = NULL;
#ifdef _DEBUG
	fp_kstp_log = fopen ("c:/log/aim/dias-dc.log", "w");
#endif

	CMainDlg dlg;
	m_pMainWnd = &dlg;

	try {
		dlg.DoModal();
	} catch (CMemoryException* e) {
		e->ReportError ();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CDiasDevelApp::ExitInstance (void) {
	Disconnect ();
	return CWinApp::ExitInstance ();
}

void CDiasDevelApp::Connect () {
	m_kstp.Communicate (KSCMD_HELLO, "ss", "dias2", "pass");
	m_hobj = NULL;
}

void CDiasDevelApp::Disconnect () {
	m_kstp.Communicate (KSCMD_BYE, "");
	m_hobj = NULL;
}

void CDiasDevelApp::Upload () {
	CString strVarFile, strSrcFile;

	CFileDialog dlgOpen (true);
	dlgOpen.m_ofn.lpstrTitle = "Select file to upload";
	dlgOpen.m_ofn.lpstrInitialDir = "u:\\sci\\topics\\nie_iddm\\pept_adm";
	dlgOpen.m_ofn.lpstrFilter = "Kontron Elektronik(C) VIDAS(R) Sequences""\0""*.IMG""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	if (dlgOpen.DoModal () != IDOK)
		return;
	if (dlgOpen.m_ofn.nFilterIndex == 1)
		strSrcFile = dlgOpen.GetPathName ();
	else	return;

	CFileDialog dlgSave (false);
	dlgSave.m_ofn.lpstrTitle = "Select uploading dir";
	dlgSave.m_ofn.lpstrInitialDir = "c:\\var\\aim\\dias";
	dlgSave.m_ofn.lpstrFilter = "All files""\0""*.*""\0";
	dlgSave.m_ofn.nFilterIndex = 1;
	if (dlgSave.DoModal () != IDOK)
		return;
	strVarFile = dlgSave.GetPathName ();

	strSrcFile.Replace ('\\', '/');
	strVarFile.Replace ('\\', '/');

	m_kstp.Communicate (KSCMD_LOAD, "iissi", DOBJT_STORAGE, DOBJF_STORAGE_KONTRON, LPCTSTR(strSrcFile), LPCTSTR(strVarFile), 0777);
//	m_hobj = m_kstp[0]();
}

void CDiasDevelApp::Download () {
	CString strVarFile, strSrcFile;

	CFileDialog dlgOpen (true);
	dlgOpen.m_ofn.lpstrTitle = "Select source DIAS sequence";
	dlgOpen.m_ofn.lpstrInitialDir = "c:\\var\\aim\\dias";
	dlgOpen.m_ofn.lpstrFilter = "All files""\0""*.*""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	if (dlgOpen.DoModal () != IDOK)
		return;
	strVarFile = dlgOpen.GetPathName ();

	CFileDialog dlgSave (false);
	dlgSave.m_ofn.lpstrTitle = "Select file to download";
	dlgSave.m_ofn.lpstrInitialDir = "u:\\sci\\topics\\nie_iddm\\pept_adm";
	dlgSave.m_ofn.lpstrFilter = "Kontron Elektronik(C) VIDAS(R) Sequences""\0""*.IMG""\0";
	dlgSave.m_ofn.nFilterIndex = 1;
	if (dlgSave.DoModal () != IDOK)
		return;
	strSrcFile = dlgSave.GetPathName ();

	strSrcFile.Replace ('\\', '/');
	strVarFile.Replace ('\\', '/');

	m_kstp.Communicate (KSCMD_EXTRACT, "iss", DOBJF_STORAGE_KONTRON, LPCTSTR(strSrcFile), LPCTSTR(strVarFile));
}

void CDiasDevelApp::SelectStorage () {
	CString strVarFile;
	CFileDialog dlgOpen (true);
	dlgOpen.m_ofn.lpstrTitle = "Select source DIAS sequence";
	dlgOpen.m_ofn.lpstrInitialDir = "c:\\var\\aim\\dias";
	dlgOpen.m_ofn.lpstrFilter = "All files""\0""*.*""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	if (dlgOpen.DoModal () != IDOK)
		return;
	strVarFile = dlgOpen.GetPathName ();

	if (m_hobj)
		m_kstp.Communicate (KSCMD_CLOSE, "i", m_hobj);
	strVarFile.Replace ('\\', '/');

	if (!m_kstp.Communicate (KSCMD_OPEN, "sii", LPCTSTR(strVarFile), 0777, 0777))
		return;
	m_hobj = m_kstp[0]();

	((CMainDlg*)m_pMainWnd)->OnRefresh ();
}

void CDiasDevelApp::DeleteStorage () {
	CString strVarFile;
	CFileDialog dlgOpen (true);
	dlgOpen.m_ofn.lpstrTitle = "Select source DIAS sequence";
	dlgOpen.m_ofn.lpstrInitialDir = "c:\\var\\aim\\dias";
	dlgOpen.m_ofn.lpstrFilter = "All files""\0""*.*""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	if (dlgOpen.DoModal () != IDOK)
		return;
	strVarFile = dlgOpen.GetPathName ();

	strVarFile.Replace ('\\', '/');

	m_kstp.Communicate (KSCMD_DELETE, "s", LPCTSTR(strVarFile));
}

void CDiasDevelApp::Plugin_Hthalam_AVP_Mask () {

	// * Selecting files - source files and destination dir

	CString strVarFile, strSrcFile;

	CFileDialog dlgOpen (true);
	dlgOpen.m_ofn.lpstrTitle = "Select files to compute";
	dlgOpen.m_ofn.lpstrInitialDir = "l:\\sci\\topics\\prenatal_stress\\hthalam.avp.if\\images.3-vidas.src";
	dlgOpen.m_ofn.lpstrFilter = "Kontron Elektronik(C) VIDAS(R) Sequences""\0""*.IMG""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	dlgOpen.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (dlgOpen.DoModal () != IDOK)
		return;

	// * Initializing interface with DIAS

	POSITION pos;
	CString str;
	udword imgcnt, szgauss, szround;
	udword htmp1, htmp2, hmem1, hmem2, hfibr, hnrns, hcurr, hrslt, hbgtrk, hhalo, hstd, hgausn;
	ksmtx_factors *fcgauss;
	ksmtx_order *orround;
	
	for (fcgauss = ((CMainDlg*)m_pMainWnd)->dias_fctsys; fcgauss ; fcgauss = fcgauss->next) { 
		if (fcgauss->name && fcgauss->name == "Gaussian function factors")
			break;
	}
	szgauss = fcgauss->dimx * fcgauss->dimy * sizeof(rtbyte);
	ubyte *gauss = new ubyte[szgauss + sizeof (ulong)];
	memcpy (gauss, &szgauss, sizeof (ulong));
	memcpy (gauss + sizeof (ulong), fcgauss->factors, szgauss);

	for (orround = ((CMainDlg*)m_pMainWnd)->dias_ordsys; orround ; orround = orround->next) { 
		if (orround->name && orround->name == "Round with dot: standart order")
			break;
	}
	szround = orround->dimx * orround->dimy * sizeof(sdword);
	ubyte *round = new ubyte[szround + sizeof (ulong)];
	memcpy (round, &szround, sizeof (ulong));
	memcpy (round + sizeof (ulong), orround->order, szround);

	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	htmp1 = m_kstp[0]();
	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	htmp2 = m_kstp[0]();
	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	hmem1 = m_kstp[0]();
	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	hmem2 = m_kstp[0]();
	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	hfibr = m_kstp[0]();
	m_kstp.Communicate (KSCMD_TEMP, "iiiiiii", DOBJT_IMAGE, 0, 0, 768, 576, 1, 0);
	hnrns = m_kstp[0]();

	// * Iterating files
	pos = dlgOpen.GetStartPosition ();
	while (strSrcFile = dlgOpen.GetNextPathName (pos)) {
		// Opening storage
		strSrcFile.Replace ('\\', '/');
		if (m_hobj)
			m_kstp.Communicate (KSCMD_CLOSE, "i", m_hobj);

		m_kstp.Communicate (KSCMD_LOAD, "iissi", DOBJT_STORAGE, DOBJF_STORAGE_KONTRON, LPCTSTR(strSrcFile), "c:/var/aim/dias/plugins/avp_hthalam/source", 0777);
		m_kstp.Communicate (KSCMD_OPEN, "sii", "c:/var/aim/dias/plugins/avp_hthalam/source", 0777, 0777);
		m_hobj = m_kstp[0]();

		m_kstp.Communicate (KSCMD_CREATE, "isiiii", DOBJT_STORAGE, "c:/var/aim/dias/plugins/avp_hthalam/result", 0777, 768, 576, 1);
		m_kstp.Communicate (KSCMD_OPEN, "sii", "c:/var/aim/dias/plugins/avp_hthalam/result", 0777, 0777);
		hrslt = m_kstp[0]();

		((CMainDlg*)m_pMainWnd)->OnRefresh ();

		m_kstp.Communicate (DCMD_OBJGET_STORAGE, "i", m_hobj);
		dksobj_storage* stor = (dksobj_storage*)New (sizeof (dksobj_storage));
		memcpy (stor, m_kstp[0](""), theApp.m_kstp[0].Size ());
		imgcnt = stor->img_count;

		for (int i = 0; i < imgcnt; ++i) {
			hcurr = ((CMainDlg*)m_pMainWnd)->m_ctrlI1N.GetItemData (i);
			((CMainDlg*)m_pMainWnd)->m_ctrlI1N.SetSelectionMark (i);
			((CMainDlg*)m_pMainWnd)->OnImg1Changed ();
			m_pMainWnd->UpdateWindow ();

			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-bgtrk%03d", i);
			m_kstp.Communicate (KSCMD_CREATE, "isiiiii", DOBJT_IMAGE, str, 0777, 768, 576, 1, 0);
			m_kstp.Communicate (KSCMD_OPEN, "sii", str, 0777, 0777);
			hbgtrk = m_kstp[0]();

			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-std%03d", i);
			m_kstp.Communicate (KSCMD_CREATE, "isiiiii", DOBJT_IMAGE, str, 0777, 768, 576, 1, 0);
			m_kstp.Communicate (KSCMD_OPEN, "sii", str, 0777, 0777);
			hstd = m_kstp[0]();

			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-halo%03d", i);
			m_kstp.Communicate (KSCMD_CREATE, "isiiiii", DOBJT_IMAGE, str, 0777, 768, 576, 1, 0);
			m_kstp.Communicate (KSCMD_OPEN, "sii", str, 0777, 0777);
			hhalo = m_kstp[0]();

			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-gausn%03d", i);
			m_kstp.Communicate (KSCMD_CREATE, "isiiiii", DOBJT_IMAGE, str, 0777, 768, 576, 1, 0);
			m_kstp.Communicate (KSCMD_OPEN, "sii", str, 0777, 0777);
			hgausn = m_kstp[0]();

			m_kstp.Communicate (DCMD_img_mtx_filter, "ibii", 2, gauss, hgausn, hcurr);
			m_kstp.Communicate (DCMD_img_mtx_min, "iii", 6, hmem1, hgausn);
			m_kstp.Communicate (DCMD_img_mtx_avg, "iii", 4, htmp1, hmem1);
			m_kstp.Communicate (DCMD_img_pxw_sub, "riiii", 1.0, 0, hbgtrk, hcurr, htmp1);
			m_kstp.Communicate (DCMD_img_pxw_thr, "iiiii", 0x30, 0xFF, 0xFF, hmem2, hbgtrk);

			m_kstp.Communicate (DCMD_img_mtx_std, "iiii", 2, 16, hstd, hcurr);
//			m_kstp.Communicate (DCMD_img_pxw_thr, "iiiii", 0x30, 0xFF, 0xFF, hcurr, htmp2);
//			m_kstp.Communicate (DCMD_img_mtx_logic, "iiiiibii", 0xFF, false, 0xFF, 5, 6, round, hcurr, htmp2);

			m_kstp.Communicate (DCMD_img_pxw_sub, "riiii", 1.0, 0, htmp1, hgausn, hmem2);
			m_kstp.Communicate (DCMD_img_pxw_sub, "riiii", 0.25, 0, hhalo, htmp1, hmem1);

//			((CMainDlg*)m_pMainWnd)->OnImg1Changed ();
//			m_pMainWnd->UpdateWindow ();

			m_kstp.Communicate (KSCMD_SAVE, "i", hgausn);
			m_kstp.Communicate (KSCMD_CLOSE, "i", hgausn);
			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-gausn%03d", i);
			m_kstp.Communicate (KSCMD_LINK, "is", hrslt, str);

			m_kstp.Communicate (KSCMD_SAVE, "i", hbgtrk);
			m_kstp.Communicate (KSCMD_CLOSE, "i", hbgtrk);
			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-bgtrk%03d", i);
			m_kstp.Communicate (KSCMD_LINK, "is", hrslt, str);

			m_kstp.Communicate (KSCMD_SAVE, "i", hstd);
			m_kstp.Communicate (KSCMD_CLOSE, "i", hstd);
			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-std%03d", i);
			m_kstp.Communicate (KSCMD_LINK, "is", hrslt, str);

			m_kstp.Communicate (KSCMD_SAVE, "i", hhalo);
			m_kstp.Communicate (KSCMD_CLOSE, "i", hhalo);
			str.Format ("c:/var/aim/dias/plugins/avp_hthalam/result-halo%03d", i);
			m_kstp.Communicate (KSCMD_LINK, "is", hrslt, str);

			m_kstp.Communicate (KSCMD_CLOSE, "i", hcurr);
		}

		m_kstp.Communicate (KSCMD_CLOSE, "i", m_hobj);
		m_hobj = 0;
		m_kstp.Communicate (KSCMD_SAVE, "i", hrslt);
		m_kstp.Communicate (KSCMD_CLOSE, "i", hrslt);
		m_kstp.Communicate (KSCMD_EXTRACT, "iss", DOBJF_STORAGE_KONTRON, LPCTSTR(strSrcFile+".proc"), "c:/var/aim/dias/plugins/avp_hthalam/result");
		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/result");
//		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/result-gausn");
//		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/result-bgtrk");
//		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/result-std");
//		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/result-halo");
		m_kstp.Communicate (KSCMD_DELETE, "s", "c:/var/aim/dias/plugins/avp_hthalam/source");
	}


	// * Finalizing interface with DIAS
	m_kstp.Communicate (KSCMD_CLOSE, "i", htmp1);
	m_kstp.Communicate (KSCMD_CLOSE, "i", htmp2);
	m_kstp.Communicate (KSCMD_CLOSE, "i", hmem1);
	m_kstp.Communicate (KSCMD_CLOSE, "i", hmem2);
	m_kstp.Communicate (KSCMD_CLOSE, "i", hfibr);
	m_kstp.Communicate (KSCMD_CLOSE, "i", hnrns);

}

void CDiasDevelApp::OnPlugMemorytest1 () {
	CString str;
	m_kstp.Communicate (KSCMD_TEST_MEM, "ii", KSTEST_MEM_HASH, 300);
	str.Format ("Process have been taken %d ticks", m_kstp[0]());
	MessageBox (NULL, str, "Test results", MB_OK | MB_ICONINFORMATION);
}
