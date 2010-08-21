#include "stdafx.h"
#include "Dias.h"
#include "Settings.h"
#include "MainFrame.h"

#include "Image.h"
#include "Survey.h"
#include "Report.h"

#include "StorageView.h"
#include "StorageFrame.h"
#include "StorageDoc.h"
#include ".\storagedoc.h"

// Dialogs

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStorageDoc

IMPLEMENT_DYNCREATE(CStorageDoc, CDocument)

BEGIN_MESSAGE_MAP(CStorageDoc, CDocument)
	//{{AFX_MSG_MAP(CStorageDoc)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_TOOLS_BUILD_EXTRACTPATCHESBYLAYER, OnToolsBuildExtractpatchesbylayer)
	ON_COMMAND(ID_TOOLS_BUILD_FINDOUTBGMASK, OnToolsBuildFindoutbgmask)
	ON_COMMAND(ID_TOOLS_BUILD_REMOVEBACKGROUNDMASK, OnToolsBuildRemovebackgroundmask)
	ON_COMMAND(ID_BUILD_WHITENOISECOMPUTATIOS, OnBuildWhitenoisecomputatios)
	ON_COMMAND(ID_TOOLS_BUILD_MEDIANFILTERING, OnToolsBuildMedianfiltering)
	ON_COMMAND(ID_TOOLS_BUILD_REMOVENOISE, OnToolsBuildRemovenoise)
	ON_COMMAND(ID_TOOLS_BUILD_NEWNOISEREMOVEPROC4, OnToolsBuildNewnoiseremoveproc4)
	ON_COMMAND(ID_TOOLS_BUILD_NEWNOISEREMOVEPROC8, OnToolsBuildNewnoiseremoveproc8)
	ON_COMMAND(ID_TOOLS_BUILD_LASTMINIMUMSEGMENTATION, OnToolsBuildLastminimumsegmentation)
	ON_COMMAND(ID_STORAGE_HISTOGRAMS, OnStorageHistograms)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStorageDoc construction/destruction

CStorageDoc::CStorageDoc () {
	Images.m_pDoc = this;
	Surveis.m_pDoc = this;
	Frames.m_pDoc = this;

	m_blAcceptFromView = true;
	m_blForceSettings = false;
	m_blShowImages = true;
	m_blShowPatches = true;
	m_blShowMarks = true;
	m_blShowMeters = true;
	m_blImgVRuler = true;
	m_blImgHRuler = true;
	m_blImgGrid = false;
	m_blRptVRuler = true;
	m_blRptHRuler = true;
	m_blRptGrid = true;
	m_nImgZoom = 1;
	m_nRptZoom = 1;
	m_rectPlacement = CRect (0, 0, 500, 700);
	m_nPanelWidth = 130;
	m_nPanelActive = 1;
	m_blShowNames = true;
	m_nImgNo = 0;
	m_nDataNo = 0;
	m_nRptNo = 0;
}

CStorageDoc::~CStorageDoc () {
	DeleteContents ();
}

BOOL CStorageDoc::OnNewDocument () {
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle ("Storage");
	m_strName = "Storage";
	m_strDescript = "";
	m_nColorModel = cmGrayscale; //(ColorModel) dlg.m_nModel;
	m_nDimX = 768; //atoi (dlg.m_strDimX);
	m_nDimY = 576; //atoi (dlg.m_strDimY);
	m_nBPP = (m_nColorModel == cmMono) ? 1 : 8;
	m_nChannels = (m_nColorModel == cmCMYK) ? 4 : (m_nColorModel == cmMono || m_nColorModel == cmGrayscale) ? 1 : (m_nColorModel == cmMulty) ? 0 : 3;

	m_pImgTmp = new CPicture (this, "TemporaryImage", "System image for calculations");
	m_pCnlTmp = new CChannel (this, "TemporaryChannel", "System channel for calculations");

	// #### TODO: Call apopriate dialog here if the color model is set to multy

	if (!theApp.Documents.m_pHashHead)
		theApp.Documents.m_pHashHead = new CHash<CStorageDoc*,CStorageDoc>(this);
	else
		theApp.Documents.m_pHashHead->AddTail (new CHash<CStorageDoc*,CStorageDoc>(this));

//	CButton *btn = (CButton*)(dlg.GetDlgItem (IDC_DN_CREATE_ARCUIRE));
//	if (btn->GetCheck () == 1)
//		OnImageAcquireCamera ();
//	else
//		Images.Add (dlg.m_nCount);

	return TRUE;
}

BOOL CStorageDoc::OnOpenDocument (LPCTSTR lpszPathName) {
	theApp.StatusState (IDI_INDICATOR_OPEN);

	if (!CDocument::OnOpenDocument(lpszPathName))
		return (false);

	if (!theApp.Documents.m_pHashHead)
		theApp.Documents.m_pHashHead = new CHash<CStorageDoc*,CStorageDoc>(this);
	else
		theApp.Documents.m_pHashHead->AddTail (new CHash<CStorageDoc*,CStorageDoc>(this));

	m_pImgTmp = new CPicture (this, "TemporaryImage", "System image for calculations");
	m_pCnlTmp = new CChannel (this, "TemporaryChannel", "System channel for calculations");

	theApp.StatusState (0);
	return (true);
}

BOOL CStorageDoc::OnSaveDocument (LPCTSTR lpszPathName) {
	BOOL ret;
	theApp.StatusState (IDI_INDICATOR_SAVE);
	ret = CDocument::OnSaveDocument(lpszPathName);
	UpdateAllViews (NULL, 0);
	theApp.StatusState (0);
	return (ret);
}

void CStorageDoc::OnCloseDocument () {
	if (theApp.Documents.GetCount () == 1) {
		theApp.Documents.m_pHashHead->GetAt(theApp.Documents.NameLookup (m_strName))->Remove ();
		theApp.Documents.m_pHashHead = NULL;
	} else if (theApp.Documents.NameLookup (m_strName) != uvar32_64 (-1) && theApp.Documents.NameLookup (m_strName) < theApp.Documents.GetCount ())
		theApp.Documents.m_pHashHead->GetAt(theApp.Documents.NameLookup (m_strName))->Remove ();
	CDocument::OnCloseDocument ();
}

void CStorageDoc::DeleteContents () {
/*	Images(aimAll).Remove ();
	CDocument::DeleteContents ();
	UpdateAllViews (NULL, 0);*/
}

bool CStorageDoc::GetSetting (CString name) throw (CaImAPIException) {
	if (name == "ShowImages")	return (m_blShowImages);
	else if (name == "ShowPatches")	return (m_blShowPatches);
	else if (name == "ShowMarks")	return (m_blShowMarks);
	else if (name == "ShowMeters")	return (m_blShowMeters);
	else if (name == "ImgVRuler")	return (m_blImgVRuler);
	else if (name == "ImgHRuler")	return (m_blImgHRuler);
	else if (name == "ImgGrid")	return (m_blImgGrid);
	else if (name == "RptVRuler")	return (m_blRptVRuler);
	else if (name == "RptHRuler")	return (m_blRptHRuler);
	else if (name == "RptGrid")	return (m_blRptGrid);
	else	throw (CaImAPIException (0));	// #### TODO: Put aImAPIException number
	return (false);
}

void CStorageDoc::SetSetting (CString name, bool value) {
	if (name == "ShowImages")	m_blShowImages = m_blShowImages;
	else if (name == "ShowPatches")	m_blShowPatches = m_blShowPatches;
	else if (name == "ShowMarks")	m_blShowMarks = m_blShowMarks;
	else if (name == "ShowMeters")	m_blShowMeters = m_blShowMeters;
	else if (name == "ImgVRuler")	m_blImgVRuler = m_blImgVRuler;
	else if (name == "ImgHRuler")	m_blImgHRuler = m_blImgHRuler;
	else if (name == "ImgGrid")	m_blImgGrid = m_blImgGrid;
	else if (name == "RptVRuler")	m_blRptVRuler = m_blRptVRuler;
	else if (name == "RptHRuler")	m_blRptHRuler = m_blRptHRuler;
	else if (name == "RptGrid")	m_blRptGrid = m_blRptGrid;
	else	throw (CaImAPIException (0));	// #### TODO: Put aImAPIException number
}

void CStorageDoc::Serialize (CArchive& ar) {
	CString strCaption = "aIm DIAS Storage v. 2.0";
	if (ar.IsStoring ()) {
		ar << strCaption;
		ar << m_nDimX << m_nDimY << m_nBPP << m_nChannels << (ubyte)m_nColorModel;
		ar << (ubyte)m_blAcceptFromView << (ubyte)m_blForceSettings;
		ar << (ubyte)m_blShowImages << (ubyte)m_blShowPatches << (ubyte)m_blShowMarks << (ubyte)m_blShowMeters;
		ar << (ubyte)m_blImgVRuler << (ubyte)m_blImgHRuler << (ubyte)m_blImgGrid << (ubyte)m_blRptVRuler << (ubyte)m_blRptHRuler << (ubyte)m_blRptGrid;
		ar << m_nImgZoom << m_nRptZoom;
		ar << m_rectPlacement << m_nPanelWidth << m_nPanelActive << (ubyte)m_blShowNames;
		ar << m_nImgNo << m_nDataNo << m_nRptNo;
		Images.Serialize (ar);
		Surveis.Serialize (ar);
		// #### TODO: Add Reports serialization here
	} else {
		DeleteContents ();
		m_strName = ar.GetFile()->GetFileTitle ();
		ar >> strCaption;
		if (strCaption != "aIm DIAS Storage v. 2.0") {
			MessageBox (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_ERR_DOC_VERSION), theApp.LoadString (IDS_ERR_DOC), MB_OK | MB_ICONERROR);
			return;
		}
		(int&)m_nColorModel = 0;
		ar >> m_nDimX >> m_nDimY >> m_nBPP >> m_nChannels >> (ubyte&)m_nColorModel;
		ar >> (ubyte&)m_blAcceptFromView >> (ubyte&)m_blForceSettings;
		ar >> (ubyte&)m_blShowImages >> (ubyte&)m_blShowPatches >> (ubyte&)m_blShowMarks >> (ubyte&)m_blShowMeters;
		ar >> (ubyte&)m_blImgVRuler >> (ubyte&)m_blImgHRuler >> (ubyte&)m_blImgGrid >> (ubyte&)m_blRptVRuler >> (ubyte&)m_blRptHRuler >> (ubyte&)m_blRptGrid;
		ar >> m_nImgZoom >> m_nRptZoom;
		ar >> m_rectPlacement >> m_nPanelWidth >> m_nPanelActive >> (ubyte&)m_blShowNames;
		ar >> m_nImgNo >> m_nDataNo >> m_nRptNo;
		Images.m_pDoc = this;
		Surveis.m_pDoc = this;
		Images.Serialize (ar);
		Surveis.Serialize (ar);
		UpdateAll ();
		// #### TODO: Add Reports serialization here
	}
}

void CStorageDoc::OnChangedViewList () {
	// #### TODO: Update Frames member

	CDocument::OnChangedViewList();
}

void CStorageDoc::Save () {
	OnSaveDocument (GetPathName ());
}

void CStorageDoc::SaveAs (CString name, bool verbal/* = true*/) {
	SetPathName (name);
	OnSaveDocument (verbal ? "" : GetPathName ());
}

void CStorageDoc::Close (bool blAsk/* = true*/, bool blSave/* = true*/) {
	if (blSave)
		OnSaveDocument (GetPathName ());
	if (!blAsk)
		SetModifiedFlag (false);
	OnCloseDocument ();
}

void CStorageDoc::Clear (bool blSave/* = false*/) {
	if (blSave)
		Save ();
	DeleteContents ();
	UpdateAll ();
}

uvar16_32 CStorageDoc::GetDimX () {
	return (m_nDimX);
}

uvar16_32 CStorageDoc::GetDimY () {
	return (m_nDimY);
}

bool CStorageDoc::SetDims (uvar16_32 nDim, bool blStretch, bool blVerbal) {
	// #### TODO: Add conversion code here
	return (true);
}

ColorModel CStorageDoc::GetColorModel () {
	return (m_nColorModel);
}

bool CStorageDoc::SetColorModel (ColorModel cm) {
	// #### TODO: Add conversion code here
	return (true);
}

ubyte CStorageDoc::GetChannelsCount () {
	return (m_nChannels);
}

CString CStorageDoc::GetInfo (const CString name) {
	return (m_mapInfo[CString(name)]);
}

void CStorageDoc::SetInfo (const CString name, const CString value) {
	m_mapInfo[CString(name)] = value;
}

void CStorageDoc::Update (CUpdateHint::UpdateTarget tgt, udword cmd, udword start, udword end) {
	// #### TODO: Chech for global falg of update mode
	CUpdateHint* pHint = new CUpdateHint (tgt, cmd, start, end);
	UpdateAllViews (NULL, NULL, pHint);
}

void CStorageDoc::UpdateAll () {
	// #### TODO: Chech for global falg of update mode
	UpdateAllViews (NULL);
}

/*CStorageFrame& CStorageDoc::GetActiveFrame () {
	return (*((CStorageFrame*)(((CMainFrame*)(theApp.m_pMainWnd))->MDIGetActive())));
}

CStorageView& CStorageDoc::GetActiveView () {
	return (*(CStorageView*)(
		((CStorageFrame*)(((CMainFrame*)(theApp.m_pMainWnd))->MDIGetActive()))->GetActiveView()
		));
}*/

/////////////////////////////////////////////////////////////////////////////
// CStorageDoc diagnostics

#ifdef _DEBUG
void CStorageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStorageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CStorageDoc::OnFileImport () {
	CFile		fImport;
	CFileDialog	dlgOpen (true);
	// #### TODO: Place here initial directory name taken from configuration structures
	dlgOpen.m_ofn.lpstrInitialDir = ".";
	dlgOpen.m_ofn.lpstrFilter = "Kontron Elektronik Images (*.img)""\0""*.IMG""\0"
		"All supported signle image formats""\0""*.JPG;*.JPEG;*.GIF;*.PNG;*.BMP;*.DIB""\0"
		"JPEG Bitmap (*.jpg, *.jpeg)""\0""*.JPG;*.JPEG""\0"
		"CumpuServe GIF (*.gif)""\0""*.GIF""\0"
		"Prtable Network Graphics (*.png)""\0""*.PNG""\0"
		"MS Windows Bitmap (*.bmp, *.dib)""\0""*.BMP;*.DIB""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
//	dlgOpen.SetWindowText (theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION));
	if (dlgOpen.DoModal () != IDOK)
		return;

	// #### TODO: Add support for video formats
	// #### TODO: Add support for multi-page tif format
	// #### TODO: Create import dialog
	// #### TODO: Add progress control on the status bar

	uvar32_64	selected;
	svar32_64	nImages;
	uvar32_64	dwSize = m_nDimX * m_nDimY;
	CString		strTitle;
	BYTE		*pbPixels = (BYTE *)aimMemoryCommit (dwSize, "CStorageDoc::OnDocAddImportseq", "pbPixels");


	if ((selected = Frames[aimActive].GetActiveImageNo()) == -1)
		selected = Images.GetCount () - 1;

	theApp.StatusState (IDI_INDICATOR_IMPORT);
	if (dlgOpen.m_ofn.nFilterIndex == 1) {
		if (!fImport.Open (dlgOpen.GetPathName (), CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite)) {
			MessageBox (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_IMPORTSEQ_OPENERR), theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		BYTE	*pbHeader = new BYTE[0x80];
		fImport.Read ( pbHeader, 0x80 );
		if (((DWORD*)(pbHeader + 2))[0] != 0xB06D1247) {
			MessageBox  (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_IMPORTSEQ_TYPEERR), theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		if (((WORD*)pbHeader)[5] != 0x4321) {
			MessageBox  (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_IMPORTSEQ_TYPEERR), theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		if (((WORD*)pbHeader)[3] != m_nDimX || ((WORD*)pbHeader)[4] != m_nDimY || m_nBPP != 8) {
			MessageBox  (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_IMPORTSEQ_SIZEERR), theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		nImages = ((WORD*)pbHeader)[6];

		theApp.StatusState (IDI_INDICATOR_IMPORT);
		while (--nImages >= 0) {
			strTitle.Format ("%s #%03d", dlgOpen.GetFileTitle (), ((WORD*)pbHeader)[6] - nImages);
			CPicture *pImg = new CPicture (this, strTitle, "");
			fImport.Read (pbPixels, dwSize);
			pImg->Channels(aimAll).SetBits (pbPixels, dwSize);
			Images.Insert (selected++, *pImg);
		}
		nImages = ((WORD*)pbHeader)[6];

		fImport.Close ();
		delete [] pbHeader;
		aimMemoryRelease (pbPixels, "CStorage::OnDocAddImportseq", "pbPixels");
	} else if (dlgOpen.m_ofn.nFilterIndex >= 2) {
		CImage image;
		image.Load (dlgOpen.GetPathName ());
		if (image.GetBPP () != m_nBPP || image.GetHeight () != m_nDimY || image.GetWidth () != m_nDimX) {
			MessageBox  (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_IMPORTSEQ_SIZEERR), theApp.LoadString (IDS_IMPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		CPicture *pImg = new CPicture (this, dlgOpen.GetFileTitle (), "");
		ubyte *pBits;
		if (image.IsDIBSection () && image.GetPitch () < 0)
			pBits = (ubyte*)image.GetBits () - (m_nDimY - 1) * m_nDimX;
		else
			pBits = (ubyte*)image.GetBits ();
		pImg->Channels(aimAll).SetBits (pBits, m_nDimX * m_nDimY * m_nBPP / 8);
		Images.Insert (selected++, *pImg);
	}

	theApp.StatusState (0);
}

void CStorageDoc::OnFileExport () {
	CFileDialog	dlgSave (false);
	// #### TODO: Place here initial directory name taken from configuration structures
	dlgSave.m_ofn.lpstrInitialDir = ".";
	dlgSave.m_ofn.lpstrFilter = "Kontron Elektronik(C) VIDAS(R) Sequences""\0""*.IMG""\0";
	dlgSave.m_ofn.nFilterIndex = 1;
	if (dlgSave.DoModal () != IDOK)
		return;

	// #### TODO: Add support for video formats
	// #### TODO: Add support for multi-page tif format
	// #### TODO: Create import dialog
	// #### TODO: Add progress control on the status bar

	Export (dlgSave.GetPathName () + ".img", dlgSave.m_ofn.nFilterIndex);
}

void CStorageDoc::Export (CString strFile, uint nType) {
	CFile		fExport;
	uvar32_64	nPos, nImages = Images.GetCount ();
	uvar32_64	dwSize = m_nDimX * m_nDimY;
	ubyte		*pbPixels = (ubyte*)aimMemoryCommit (dwSize, "CStorageDoc::OnDocAddImportseq", "pbPixels");

	if (nType == 1) {
		ubyte	*pbHeader = new ubyte[0x80];
		memset (pbHeader, 0, 0x80);
		((uword*)(pbHeader))[0] = 0x0001;
		((uword*)(pbHeader))[1] = 0x1247;
		((uword*)(pbHeader))[2] = 0xB06D;
		((uword*)(pbHeader))[3] = m_nDimX;
		((uword*)(pbHeader))[4] = m_nDimY;
		((uword*)(pbHeader))[5] = 0x4321;
		((uword*)(pbHeader))[6] = nImages;

		if (!fExport.Open (strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareExclusive)) {
			MessageBox (theApp.m_pMainWnd->m_hWnd, theApp.LoadString (IDS_EXPORTSEQ_OPENERR), theApp.LoadString (IDS_EXPORTSEQ_DLGCAPTION), MB_OK | MB_ICONERROR);
			return;
		}
		theApp.StatusState (IDI_INDICATOR_EXPORT);
		fExport.Write (pbHeader, 0x80);
		for (nPos = 0; nPos < nImages; ++nPos) {
			Images[nPos].Channels(aimAll).GetBits (pbPixels, dwSize);
			fExport.Write (pbPixels, dwSize);
		}
		theApp.StatusState (0);

		delete [] pbHeader;
		aimMemoryRelease (pbPixels, "CStorage::OnDocAddImportseq", "pbPixels");
	} else
		return;
}

void CStorageDoc::OnToolsBuildExtractpatchesbylayer () {
	for (uvar32_64 curr = 0; curr < Images.GetCount (); curr += 6) {
		Images.Insert (curr, "Background");
		Images.Insert (curr + 1, "Neurons");
		Images.Insert (curr + 2, "Nuclei");
		Images.Insert (curr + 3, "Nucleoli");
		Images.Insert (curr + 4, "Glia");
		for (uword l = 0, col = 1; l < 0x06; ++l) {
			Images.Insert (curr + 5, "Temporary");
			for (uvar32_64 n = 0; n < Images[curr].Patches.GetCount (); ++n) {
				if (Images[curr].Patches[n].GetLayer () != l)
					continue;
				Images[curr].Patches[n].ToBinary (Images[curr + 6], 0, col++);
				if (n == 0)
					Images[curr + l + 1] = Images[curr + 6];
				else
					Images[curr + l + 1] ^= Images[curr + 6];
			}
			Images.Delete (curr + 6);
		}
	}
}

void CStorageDoc::OnToolsBuildFindoutbgmask () {
	Images.Insert (0, "Background mask");
	Images[1] = Images[0];
	Images[0].SetName ("Background mask");
	for (uvar32_64 curr = 1; curr < Images.GetCount (); curr++)
		Images[0].Min (Images[curr]);
}

void CStorageDoc::OnToolsBuildRemovebackgroundmask () {
	for (uvar32_64 curr = 1; curr < Images.GetCount (); curr++)
		Images[curr] -= Images[0];
}

int __cdecl _sortfn_ubyte (const void *b1, const void *b2) {
	if (*((ubyte*)b1) == *((ubyte*)b2))
		return 0;
	if (*((ubyte*)b1) < *((ubyte*)b2))
		return -1;
	if (*((ubyte*)b1) > *((ubyte*)b2))
		return +1;
	return 0;
}

void CStorageDoc::OnBuildWhitenoisecomputatios () {
	uvar32_64 n;
	uvar32_64 cnt = Images.GetCount ();
	uvar32_64 size = m_nDimX * m_nDimY;

	ubyte** ppBits = new ubyte*[cnt];
	ubyte** ppResults = new ubyte*[12];
	rtbyte* pValues = new rtbyte[12];
	ubyte*	pMedian = new ubyte[cnt];

	for (n = 0; n < cnt; ++n) {
		ppBits[n] = new ubyte[size];
		Images[n].Channels[0].GetBits (ppBits[n], size, 1, false);
	}
	for (n = 0; n < 12; ++n)
		ppResults[n] = new ubyte[size];

	Images.Insert (cnt - 1, "Minimum");
	Images.Insert (cnt, "Maximum");
	Images.Insert (cnt + 1, "Quartile1");
	Images.Insert (cnt + 2, "Median");
	Images.Insert (cnt + 3, "Quartile3");
	Images.Insert (cnt + 4, "Arth. Average");
	Images.Insert (cnt + 5, "Arth. Dispersion");
	Images.Insert (cnt + 6, "Geom. Average");
	Images.Insert (cnt + 7, "Geom. Dispersion");
	Images.Insert (cnt + 8, "Harm. Average");
	Images.Insert (cnt + 9, "Harm. Dispersion");
	Images.Insert (cnt + 10, "Temp");
	Images.Delete (cnt + 11);

	for (uvar32_64 p = 0; p < size; ++p) {
		memset (pValues, 0, 12 * sizeof (rtbyte));
		pValues[0] = 0xFF;
		pValues[7] = 1;
		for (n = 0; n < cnt; n++) {
			pValues[0] = min (pValues[0], ppBits[n][p]);
			pValues[1] = max (pValues[1], ppBits[n][p]);
			pValues[5] += ppBits[n][p];
			pValues[7] *= ppBits[n][p];
			if (ppBits[n][p] != 0)
				pValues[9] += 1. / ppBits[n][p];

			pMedian[n] = ppBits[n][p];
		}
		qsort (pMedian, cnt, sizeof (ubyte), _sortfn_ubyte);
		pValues[2] = pMedian[cnt/4];
		pValues[3] = pMedian[cnt/2];
		pValues[4] = pMedian[3*cnt/4];
		pValues[5] /= cnt;
		pValues[7] = pow (double(pValues[7]), 1. / cnt);
		if (pValues[9] != 0)
			pValues[9] = cnt / pValues[9];

		for (n = 0; n < cnt; n++) {
			pValues[6] += pow (double(pValues[5] - ppBits[n][p]), 2.0);
			pValues[8] *= pow (double(ppBits[n][p] / pValues[7]), 0.5);
			pValues[10] += pow (double(1. / pValues[9] - 1. / ppBits[n][p]), -2.0);
		}
		pValues[6] /= cnt;
		pValues[6] = pow (double(pValues[6]), 0.5);
		pValues[8] = pow (double(pValues[8]), 1. / cnt);
		pValues[8] = pow (double(pValues[8]), 0.5);
		if (pValues[10] != 0)
			pValues[10] = cnt / pValues[10];
		pValues[10] = pow (double(pValues[10]), 0.5);

		for (n = 0; n < 12; n++)
			ppResults[n][p] = pValues[n];
	}

	for (n = 0; n < 11; n++)
		Images[cnt + n].Channels[0].SetBits (ppResults[n], size, 1);

	delete [] pValues;
	for (n = 0; n < cnt; ++n)
		delete [] ppBits[n];
	delete [] ppBits;
	for (n = 0; n < 12; ++n)
		delete [] ppResults[n];
	delete [] ppResults;
}

void CStorageDoc::OnToolsBuildMedianfiltering () {
	uvar32_64 curr = Images[aimActive].GetPos ();
	Images.Insert (curr, "1 x 1 once");
	Images[curr+1] = Images[curr];
	Images[curr+1].Median (1, 1);

	Images.Insert (curr, "3 x 3 once");
	Images[curr+1] = Images[curr];
	Images[curr+1].Median (3, 3);

	Images.Insert (curr, "5 x 5 once");
	Images[curr+1] = Images[curr];
	Images[curr+1].Median (5, 5);
}

void CStorageDoc::OnToolsBuildRemovenoise () {
	uvar32_64 cnt = Images.GetCount () / 4;
	CString strText;
	theApp.MacroStart ("Remove Noise");
	if (Images.GetCount () % 4 - 1 != 0)
		if (MessageBox (Frames[aimActive], "Image count does not proportional to 4!\nDo you wish to continue?", "Remove Noise", MB_ICONWARNING | MB_YESNO) == IDNO)
			return;
	for (svar32_64 n = 0; (Images.GetCount () - n) / 4 > 0; n++) {
		Images[n].Min (Images[n + 1]);
		Images[n + 2].Min (Images[n + 3]);
		Images[n].Min (Images[n + 2]);
		Images[n] -= Images[Images.GetCount () - 1];
//		Images[n].Median (1, 1);
		Images.Delete (n + 1);
		Images.Delete (n + 1);
		Images.Delete (n + 1);
		strText.Format ("%d images processed (%d%%)", n, (n * 100)/cnt);
		theApp.StatusText (strText);
	}
	theApp.MacroStop ();
}

void CStorageDoc::OnToolsBuildNewnoiseremoveproc4 () {
/*	uint		nCount;
	POSITION	pos;
	CFile		fImport;
	CFileDialog	dlgOpen (true);
	dlgOpen.m_ofn.lpstrInitialDir = ".";
	dlgOpen.m_ofn.lpstrFilter = "DIAS Images Storages (*.dis)""\0""*.DIS""\0""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	dlgOpen.m_ofn.lpstrFile = new char[4096];
	dlgOpen.m_ofn.nMaxFile = 4096;
	dlgOpen.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (dlgOpen.DoModal () != IDOK)
		return;

	for (nCount = 0, pos = dlgOpen.GetStartPosition (); pos != NULL; ++nCount) {
		CString file = dlgOpen.GetNextPathName (pos);

		theApp.Documents.Open ((uchar *)file.GetBuffer ());
		CStorageDoc *pDoc = &(theApp.Documents[(uchar *)file.GetBuffer ()]);*/
		CStorageDoc *pDoc = this;

		uvar32_64 cnt = pDoc->Images.GetCount () / 4;
		CString strText;
		theApp.MacroStart ("Remove Noise");
		strText.Format ("0 images processed (0%%)");
		if (pDoc->Images.GetCount () % 4 != 0)
			if (MessageBox (Frames[aimActive], "Image count does not proportional to 4!\nDo you wish to continue?", "Remove Noise", MB_ICONWARNING | MB_YESNO) == IDNO)
				return;
		for (svar32_64 n = 0; (pDoc->Images.GetCount () - n) / 4 > 0; n+=1) {
			pDoc->Images[n].Hvg (pDoc->Images[n + 1]);
			pDoc->Images[n + 2].Hvg (pDoc->Images[n + 3]);
			pDoc->Images[n].Hvg (Images[n + 2]);
			pDoc->Images[n].Median (1, 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);

			strText.Format ("%d images processed (%d%%)", n, (n * 100)/cnt);
			theApp.StatusText (strText);

		}
		pDoc->Save ();
		Export (pDoc->GetPathName () + ".img", 1);
//		pDoc->Close ();
//	}

	theApp.MacroStop ();
}

void CStorageDoc::OnToolsBuildNewnoiseremoveproc8 () {
/*	uint		nCount;
	POSITION	pos;
	CFile		fImport;
	CFileDialog	dlgOpen (true);
	dlgOpen.m_ofn.lpstrInitialDir = ".";
	dlgOpen.m_ofn.lpstrFilter = "DIAS Images Storages (*.dis)""\0""*.DIS""\0""\0";
	dlgOpen.m_ofn.nFilterIndex = 1;
	dlgOpen.m_ofn.lpstrFile = new char[4096];
	dlgOpen.m_ofn.nMaxFile = 4096;
	dlgOpen.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (dlgOpen.DoModal () != IDOK)
		return;

	for (nCount = 0, pos = dlgOpen.GetStartPosition (); pos != NULL; ++nCount) {
		CString file = dlgOpen.GetNextPathName (pos);

		theApp.Documents.Open ((uchar *)file.GetBuffer ());
		CStorageDoc *pDoc = &(theApp.Documents[(uchar *)file.GetBuffer ()]);*/
		CStorageDoc *pDoc = this;

		uvar32_64 cnt = pDoc->Images.GetCount () / 4;
		CString strText;
		theApp.MacroStart ("Remove Noise");
		strText.Format ("0 images processed (0%%)");
		if (pDoc->Images.GetCount () % 84 != 0)
			if (MessageBox (Frames[aimActive], "Image count does not proportional to 4!\nDo you wish to continue?", "Remove Noise", MB_ICONWARNING | MB_YESNO) == IDNO)
				return;
		for (svar32_64 n = 0; (pDoc->Images.GetCount () - n) / 8 > 0; n+=1) {
			pDoc->Images[n].Hvg (pDoc->Images[n + 1]);
			pDoc->Images[n + 2].Hvg (pDoc->Images[n + 3]);
			pDoc->Images[n].Hvg (Images[n + 2]);
			pDoc->Images[n].Median (1, 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images[n + 1].Hvg (pDoc->Images[n + 2]);
			pDoc->Images[n + 3].Hvg (pDoc->Images[n + 4]);
			pDoc->Images[n + 1].Hvg (Images[n + 3]);
			pDoc->Images[n + 1].Median (1, 1);
			pDoc->Images[n].Hvg (Images[n + 1]);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);
			pDoc->Images.Delete (n + 1);

			strText.Format ("%d images processed (%d%%)", n, (n * 100)/cnt);
			theApp.StatusText (strText);

		}
		pDoc->Save ();
		Export (pDoc->GetPathName () + ".img", 1);
//		pDoc->Close ();
//	}

	theApp.MacroStop ();
}

void CStorageDoc::OnToolsBuildLastminimumsegmentation () {
	CStatistics stat;
	ubyte p, m, nRange, nThres = 0xFF, nThres2 = 0x00;
	rtbyte rMinL, rMinR;

	nRange = 5;
	for (uvar32_64 n = 0; n < Images.GetCount (); n += 7) {
		Images.Insert (n, "Median");
		Images[n + 1] = Images[n];
		Images[n + 1].Median (5, 5);

		Images[n + 1].Channels[0].Statistics (stat);
		for (p = 0xFF - nRange; p >= nRange; --p) {
			rMinL = rMinR = 0xFF;
			for (m = 1; m <= nRange; ++m) {
				rMinL = min (rMinL, stat.Freq[p - m]);
				rMinR = min (rMinR, stat.Freq[p + m]);
			}
			if (stat.Freq[p] < rMinL && stat.Freq[p] < rMinR && p != stat.Max && p <= 100) {
				nThres = p; //(stat.Max + 2 * p) / 3;
				break;
			}
		}
		Images.Insert (n, "Segments");
		Images[n + 1] = Images[n + 2];
		Images[n + 1].Channels[0].Segment (nThres, 0x00, 0xFF);

		Images.Insert (n, "Nucl1");
		Images[n + 1] = Images[n + 3];
		Images.Insert (n, "Nucl2");
		Images[n + 1] = Images[n + 4];
		Images.Insert (n, "Nucl3");
		Images[n + 1] = Images[n + 5];
		Images.Insert (n, "Nucl4");
		Images[n + 1] = Images[n + 6];

		for (p = nRange; p <= 0xFF - nRange; ++p) {
			rMinL = rMinR = 0xFF;
			for (m = 1; m <= nRange; ++m) {
				rMinL = min (rMinL, stat.Freq[p - m]);
				rMinR = min (rMinR, stat.Freq[p + m]);
			}
			if (stat.Freq[p] < rMinL && stat.Freq[p] < rMinR && p != stat.Max && p <= 100) {
				nThres2 = p;
				break;
			}
		}
		Images[n + 1].Channels[0].Segment (nThres2, 0x00, 0xFF);
		Images[n + 2].Channels[0].Segment ((1 * nThres + 3 * nThres2) / 4, 0x00, 0xFF);
		Images[n + 3].Channels[0].Segment ((2 * nThres + 2 * nThres2) / 4, 0x00, 0xFF);
		Images[n + 4].Channels[0].Segment ((3 * nThres + 1 * nThres2) / 4, 0x00, 0xFF);

		Images[n + 1].Xor (Images[n + 2]);
		Images[n + 2].Xor (Images[n + 3]);
		Images[n + 3].Xor (Images[n + 4]);
		Images[n + 4].Xor (Images[n + 5]);
	}
}

void CStorageDoc::OnStorageHistograms () {
	// TODO: Add your command handler code here
}
