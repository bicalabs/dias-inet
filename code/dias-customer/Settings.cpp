// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "Settings.h"
#include "Resources.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning ( disable : 4239 )

CSettings theSettings;

/////////////////////////////////////////////////////////////////////////////
// CSettings

CSettings::CSettings () {
}

CSettings::~CSettings () {
}


/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers

void CSettings::Serialize (CArchive& ar) {
	if (ar.IsStoring ()) {
		// storing code
	}
	else {
		// loading code
	}
}

bool CSettings::Load () {
	CFile fSet, fRsc;
	CString strSet, strRsc;
	ulong type, len = 0x1000;
	char dataS[0x1000], dataR[0x1000];
	HKEY reg;

	// Determing files location

	strSet = theApp.LoadString (IDS_REG_ROOT);
	if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T(theApp.LoadString (IDS_REG_ROOT)), 0, KEY_READ | KEY_QUERY_VALUE, &reg) != ERROR_SUCCESS ||
	    RegQueryValueEx (reg, "Settings", NULL, &type, (uchar *)&dataS, &len) != ERROR_SUCCESS ||
	    RegQueryValueEx (reg, "Resources", NULL, &type, (uchar *)&dataR, &len) != ERROR_SUCCESS) {
		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_REGERR), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONSTOP);
		exit (1);
	}
	strSet = dataS;
	strRsc = dataR;
	RegCloseKey (reg);

	// Opening files
	if (!fSet.Open (strSet, CFile::shareDenyWrite | CFile::modeRead)) {
		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_FSETERR), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONSTOP);
		exit (1);
	}
	CArchive arSet (&fSet, CArchive::load);

	// Loading scales
	theApp.DensScales.Serialize (arSet);
	theApp.GeomScales.Serialize (arSet);
	theApp.MarkStyles.Serialize (arSet);

	// Loading resources
	if (!fRsc.Open (strRsc, CFile::shareDenyWrite | CFile::modeRead)) {
		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_FSETERR), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONSTOP);
		exit (1);
	}
	CArchive arRsc (&fRsc, CArchive::load);

/*	m_prootFieldInfo = new CTree<CFieldInfo,CFieldInfo&>;
	m_prootFieldInfo->Serialize (arRsc);

	CFieldInfo *pField;
	for (uvar32_64 n = 0, cnt = theSettings.m_prootFieldInfo->GetCount (); n < cnt; ++n) {
		CString strName = theSettings.m_prootFieldInfo->GetDataAt(n).GetName ();
		if (theSettings.m_prootFieldInfo->GetAt(n)->GetChild())
			for (uvar32_64 n2 = 0, cnt2 = theSettings.m_prootFieldInfo->GetAt(n)->GetChild()->GetCount (); n2 < cnt2; ++n2) {
				if (theSettings.m_prootFieldInfo->GetAt(n)->GetChild()->GetAt(n2)->GetChild())
					for (uvar32_64 n3 = 0, cnt3 = theSettings.m_prootFieldInfo->GetAt(n)->GetChild()->GetAt(n2)->GetChild()->GetCount (); n3 < cnt3; ++n3) {
						pField = &(theSettings.m_prootFieldInfo->GetAt(n)->GetChild()->GetAt(n2)->GetChild()->GetDataAt(n3));
						CVariable *var = NULL;
						if (pField->GetType () == fieldString)
							var = new CVariable (&theTesla, strName + "." + pField->GetName (), "", pField->GetUnits (), pField->GetDescr ());
						else if (pField->GetType () == fieldInt)
							var = new CVariable (&theTesla, strName + "." + pField->GetName (), uvar32_64 (0), pField->GetUnits (), pField->GetDescr ());
						else if (pField->GetType () == fieldReal)
							var = new CVariable (&theTesla, strName + "." + pField->GetName (), rtbyte (0), pField->GetUnits (), pField->GetDescr ());
						theTesla.Variables.Add (*var);
					}
			}
	}*/

	// @@@@ BEGIN TEMP SECTION
	m_strImageName = "Image";
	m_strPatchName = "Patch";
	m_strSurveyName = "Survey";

	m_nPatchColor = RGB (0xFF, 0x00, 0x00);
	m_nPatchColorMask = RGB (0xFF, 0xFF, 0xFF);
	// @@@@ END TEMP SECTION

	return (true);
}

bool CSettings::Save () {
	CFile fSet, fRsc;
	CString strSet, strRsc, strClass, strFormat;
	ulong type, len = 0x1000;
	char dataS[0x1000], dataR[0x1000];
	HKEY reg;

	// Determing files location
	if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T(theApp.LoadString (IDS_REG_ROOT)), 0, KEY_READ | KEY_QUERY_VALUE, &reg) != ERROR_SUCCESS ||
	    RegQueryValueEx (reg, "Settings", NULL, &type, (uchar *)&dataS, &len) != ERROR_SUCCESS ||
	    RegQueryValueEx (reg, "Resources", NULL, &type, (uchar *)&dataR, &len) != ERROR_SUCCESS) {
		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_REGWERR), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONSTOP);
		exit (1);
	}
	strSet = dataS;
	strRsc = dataR;
	RegCloseKey (reg);

	// Opening files
	if (!fSet.Open (strSet, CFile::shareExclusive | CFile::modeWrite | CFile::modeCreate) ||
	    !fRsc.Open (strRsc, CFile::shareExclusive | CFile::modeWrite | CFile::modeCreate)) {
		MessageBox (NULL, theApp.LoadString (IDS_MBTEXT_FSETWERR), theApp.LoadString (IDS_MBTITLE_APP), MB_OK | MB_ICONSTOP);
		exit (1);
	}
	CArchive arSet (&fSet, CArchive::store);
	CArchive arRsc (&fRsc, CArchive::store);

	// Saving scales
	theApp.DensScales.Serialize (arSet);
	theApp.GeomScales.Serialize (arSet);
	theApp.MarkStyles.Serialize (arSet);

	// Saving resources
	CTree<CFieldInfo,CFieldInfo&> *tree;
	CTree<CFieldInfo,CFieldInfo&> *root = new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (fieldString, "Storage document", "Document parametrs", ""));
	root->AddTail (new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (fieldString, "Image", "Image parametrs", "")));
	root->AddTail (new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (fieldString, "Channel", "Channel parametrs", "")));
	root->AddTail (new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (fieldString, "Patch", "Patch parametrs", "")));
	root->AddTail (new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (fieldString, "Mark", "Mark parametrs", "")));

#	define	ADD(X)		tree->AddTail ((X));
#	define	NEW(T,A,B,C)	new CTree<CFieldInfo,CFieldInfo&> (CFieldInfo (T, A, B, C))
	
	// 1. document
	root->GetAt(0)->AddChild (CFieldInfo (fieldString, "General", "General document parameters", ""));
	tree = root->GetAt(0)->GetChild();
	ADD (NEW (fieldString, "Components", "Amount of different document components", ""));
	ADD (NEW (fieldString, "Images", "Common image parameters", ""));

	root->GetAt(0)->GetChild()->GetAt(0)->AddChild (CFieldInfo (fieldString, "Name", "Document name", "-"));
	tree = root->GetAt(0)->GetChild()->GetAt(0)->GetChild();
	ADD (NEW (fieldString, "Description", "Document description", "-"));

	root->GetAt(0)->GetChild()->GetAt(1)->AddChild (CFieldInfo (fieldInt, "ImagesNo", "Amount of images in document", "-"));
	tree = root->GetAt(0)->GetChild()->GetAt(1)->GetChild();
	ADD (NEW (fieldInt, "SurveisNo", "Amount of surveis in document", "-"));
	ADD (NEW (fieldInt, "ReportsNo", "Amount of reports in document", "-"));

	root->GetAt(0)->GetChild()->GetAt(2)->AddChild (CFieldInfo (fieldInt, "ChannelsNo", "Amount of used channels", "-"));
	tree = root->GetAt(0)->GetChild()->GetAt(2)->GetChild();
	ADD (NEW (fieldInt, "BPP", "Bits per pixel", "-"));
	ADD (NEW (fieldInt, "DimX", "Document images X-size", "px"));
	ADD (NEW (fieldInt, "DimY", "Document images Y-size", "px"));
	ADD (NEW (fieldString, "ColorModel", "Used color model", "-"));

	// 2. image
	root->GetAt(1)->AddChild (CFieldInfo (fieldString, "General", "General image parameters", ""));
	tree = root->GetAt(1)->GetChild();
	ADD (NEW (fieldString, "Components", "Amount of different image components", ""));

	root->GetAt(1)->GetChild()->GetAt(0)->AddChild (CFieldInfo (fieldString, "Name", "Image name", "-"));
	tree = root->GetAt(1)->GetChild()->GetAt(0)->GetChild();
	ADD (NEW (fieldString, "Description", "Image description", "-"));
	ADD (NEW (fieldInt, "No", "Image number in sequence", "-"));

	root->GetAt(1)->GetChild()->GetAt(1)->AddChild (CFieldInfo (fieldInt, "PatchesNo", "Amount of patches in image", "-"));
	tree = root->GetAt(1)->GetChild()->GetAt(1)->GetChild();
	ADD (NEW (fieldInt, "MarksCnt[aimAll]", "Amount of marks in image", "-"));
	ADD (NEW (fieldInt, "MarksCnt[aimCurrent]", "Amount of marks with current style in image", "-"));
	for (uvar32_64 n = 1; n < theApp.MarkStyles.GetCount (); n++) {
		strFormat.Format ("MarkStyleNo['%s']", theApp.MarkStyles[n].GetName ());
		ADD (NEW (fieldInt, strFormat, "Amount of marks with specific style in image", "-"));
	}
	ADD (NEW (fieldInt, "MarksCnt[aimUndef]", "Amount of marks with undefined style in image", "-"));

	ADD (NEW (fieldInt, "MetersCnt", "Amount of meters in image", "-"));
	ADD (NEW (fieldInt, "TextCnt", "Amount of text strings in image", "-"));

	// 3. channel
	root->GetAt(2)->AddChild (CFieldInfo (fieldString, "General", "General channel parameters", ""));

	root->GetAt(2)->GetChild()->GetAt(0)->AddChild (CFieldInfo (fieldString, "Name", "Channel name", "-"));
	tree = root->GetAt(2)->GetChild()->GetAt(0)->GetChild();
	ADD (NEW (fieldString, "Description", "Channel description", "-"));
	ADD (NEW (fieldInt, "No", "The number of channel", "-"));

	// 4. patch
	root->GetAt(3)->AddChild (CFieldInfo (fieldString, "General", "General patch parameters", ""));
	tree = root->GetAt(3)->GetChild();
	ADD (NEW (fieldString, "Native geometric", "Nonscaled geometric parameters", ""));
	ADD (NEW (fieldString, "Scaled geometric", "Geometric parameters with scale current correction", ""));
	ADD (NEW (fieldString, "Native densitometric", "Nonscaled densitometric parameters", ""));
	ADD (NEW (fieldString, "Scaled densitometric", "Densitometric parameters with scale current correction", ""));

	root->GetAt(3)->GetChild()->GetAt(0)->AddChild (CFieldInfo (fieldString, "Name", "Patch name", "-"));
	tree = root->GetAt(3)->GetChild()->GetAt(0)->GetChild();
	ADD (NEW (fieldString, "Description", "Patch description", "-"));
	ADD (NEW (fieldInt, "No", "Patch number in image", "-"));

	root->GetAt(3)->GetChild()->GetAt(1)->AddChild (CFieldInfo (fieldInt, "Left", "The left X coord of the patch bounding rectangle", "px"));
	tree = root->GetAt(3)->GetChild()->GetAt(1)->GetChild();
	ADD (NEW (fieldInt, "Top", "The top Y coord of the patch bounding rectangle", "px"));
	ADD (NEW (fieldInt, "Right", "The right X coord of the patch bounding rectangle", "px"));
	ADD (NEW (fieldInt, "Bottom", "The bottom Y coord of the patch bounding rectangle", "px"));
	ADD (NEW (fieldInt, "ApexX", "The X-coord of the patch apex", "px"));
	ADD (NEW (fieldInt, "ApexY", "The Y-coord of the patch apex", "px"));
	ADD (NEW (fieldInt, "Area", "The patch area", "px(2)"));
	ADD (NEW (fieldInt, "Perimeter", "The patch perimeter", "px"));
	ADD (NEW (fieldInt, "PerimeterX", "Patch X-axis component of the perimeter", "px"));
	ADD (NEW (fieldInt, "PerimeterY", "Patch Y-axis component of the perimeter", "px"));
	ADD (NEW (fieldInt, "PerimeterXY", "Patch diagonal XY component of the perimeter", "px"));
	ADD (NEW (fieldInt, "CscArea", "Circumstate patch figure area ", "px(2)"));
	ADD (NEW (fieldInt, "CscPerimeter", "Circumstate patch figure perimeter ", "px"));
	ADD (NEW (fieldInt, "Fragments", "Patch fragments count", "amount"));
	ADD (NEW (fieldInt, "DiamMax", "The patch maximum diameter", "px"));
	ADD (NEW (fieldInt, "DiamMin", "The patch minimum diameter", "px"));
	ADD (NEW (fieldReal, "DiamMean", "The mean value fo path diameters being measured in 32 directions", "px"));
	ADD (NEW (fieldReal, "DiamSigma", "The sigma value fo path diameters being measured in 32 directions", "px"));
	ADD (NEW (fieldReal, "DiamMaxAngle", "The angle to the X-axis of the patch maximum diameter", "px"));
	ADD (NEW (fieldReal, "DiamMinAngle", "The angle to the X-axis of the patch minimum diameter", "px"));
	ADD (NEW (fieldReal, "GravX", "X component of the patch gravity center", "px"));
	ADD (NEW (fieldReal, "GravY", "Y component of the patch gravity center", "px"));

	root->GetAt(3)->GetChild()->GetAt(2)->AddChild (CFieldInfo (fieldReal, "$Left", "The left X coord of the patch bounding rectangle", "GU"));
	tree = root->GetAt(3)->GetChild()->GetAt(2)->GetChild();
	ADD (NEW (fieldReal, "$Top", "The top Y coord of the patch bounding rectangle", "GU"));
	ADD (NEW (fieldReal, "$Right", "The right X coord of the patch bounding rectangle", "GU"));
	ADD (NEW (fieldReal, "$Bottom", "The bottom Y coord of the patch bounding rectangle", "GU"));
	ADD (NEW (fieldReal, "$ApexX", "The X-coord of the patch apex", "GU"));
	ADD (NEW (fieldReal, "$ApexY", "The Y-coord of the patch apex", "GU"));
	ADD (NEW (fieldReal, "$Area", "The patch area", "GU(2)"));
	ADD (NEW (fieldReal, "$Perimeter", "The patch perimeter", "GU"));
	ADD (NEW (fieldReal, "$PerimeterX", "Patch X-axis component of the perimeter", "GU"));
	ADD (NEW (fieldReal, "$PerimeterY", "Patch Y-axis component of the perimeter", "GU"));
	ADD (NEW (fieldReal, "$PerimeterXY", "Patch diagonal XY component of the perimeter", "GU"));
	ADD (NEW (fieldReal, "$CscArea", "Circumstate patch figure area ", "GU(2)"));
	ADD (NEW (fieldReal, "$CscPerimeter", "Circumstate patch figure perimeter ", "GU"));
	ADD (NEW (fieldReal, "$Fragments", "Patch fragments count", "amount"));
	ADD (NEW (fieldReal, "$DiamMax", "The patch maximum diameter", "px"));
	ADD (NEW (fieldReal, "$DiamMin", "The patch minimum diameter", "px"));
	ADD (NEW (fieldReal, "$DiamMean", "The mean value fo path diameters being measured in 32 directions", "px"));
	ADD (NEW (fieldReal, "$DiamSigma", "The sigma value fo path diameters being measured in 32 directions", "px"));
	ADD (NEW (fieldReal, "$DiamMaxAngle", "The angle to the X-axis of the patch maximum diameter", "px"));
	ADD (NEW (fieldReal, "$DiamMinAngle", "The angle to the X-axis of the patch minimum diameter", "px"));
	ADD (NEW (fieldReal, "$GravX", "X component of the patch gravity center", "GU"));
	ADD (NEW (fieldReal, "$GravY", "Y component of the patch gravity center", "GU"));

	root->GetAt(3)->GetChild()->GetAt(3)->AddChild (CFieldInfo (fieldReal, "Sum", "The sum of patche's pixels grey level", "grayvalue"));
	tree = root->GetAt(3)->GetChild()->GetAt(3)->GetChild ();
	ADD (NEW (fieldReal, "Sum2", "The squared sum of patche's pixels grey level", "grayvalue(2)"));
	ADD (NEW (fieldInt, "Min", "The minimum greyvalue in patch", "grayvalue"));
	ADD (NEW (fieldInt, "Max", "The maximum greyvalue in patch", "grayvalue"));
	ADD (NEW (fieldReal, "Mean", "The mean greylevel in patch", "grayvalue"));
	ADD (NEW (fieldReal, "StdDev", "The standart deviation of graylevel values in a patch", "grayvalue"));

	root->GetAt(3)->GetChild()->GetAt(4)->AddChild (CFieldInfo (fieldReal, "$Sum", "The sum of patche's pixels grey level", "DU"));
	tree = root->GetAt(3)->GetChild()->GetAt(4)->GetChild ();
	ADD (NEW (fieldReal, "$Sum2", "The squared sum of patche's pixels grey level", "DU(2)"));
	ADD (NEW (fieldReal, "$Min", "The minimum greyvalue in patch", "DU"));
	ADD (NEW (fieldReal, "$Max", "The maximum greyvalue in patch", "DU"));
	ADD (NEW (fieldReal, "$Mean", "The mean greylevel in patch", "DU"));
	ADD (NEW (fieldReal, "$StdDev", "The standart deviation of graylevel values in a patch", "DU"));

	// 5. mark
	root->GetAt(4)->AddChild (CFieldInfo (fieldString, "Position", "Mark position", ""));
	tree = root->GetAt(4)->GetChild();
	ADD (NEW (fieldString, "Kernel", "Kernel parameters", ""));
	ADD (NEW (fieldString, "Shape", "Shape parameters", ""));

	root->GetAt(4)->GetChild()->GetAt(0)->AddChild (CFieldInfo (fieldInt, "PosX", "The native X-cood of mark center", "px"));
	tree = root->GetAt(4)->GetChild()->GetAt(0)->GetChild();
	ADD (NEW (fieldInt, "No", "Mark number in image", "-"));
	ADD (NEW (fieldInt, "PosY", "The native Y-cood of mark center", "px"));
	ADD (NEW (fieldReal, "$PosX", "The scaled X-cood of mark center", "GU"));
	ADD (NEW (fieldReal, "$PosY", "The scaled Y-cood of mark center", "GU"));

	root->GetAt(4)->GetChild()->GetAt(1)->AddChild (CFieldInfo (fieldString, "KrnlType", "The kernel type", "-"));
	tree = root->GetAt(4)->GetChild()->GetAt(1)->GetChild();
	ADD (NEW (fieldInt, "KrnlSize", "The kernel size", "px"));

	root->GetAt(4)->GetChild()->GetAt(2)->AddChild (CFieldInfo (fieldString, "ShpType", "The shape type", "-"));
	tree = root->GetAt(4)->GetChild()->GetAt(2)->GetChild();
	ADD (NEW (fieldInt, "ShpSizeX", "The shape size in X-axis", "px"));
	ADD (NEW (fieldInt, "ShpSizeY", "The shape size in Y-axis", "px"));
	ADD (NEW (fieldInt, "ShpSizeAngle", "The shape orientation angle", "°"));

#	undef	ADD
#	undef	NEW

	root->Serialize (arRsc);

	return (true);
}

int CSettings::GetSetting (UINT nID, SettingQuery nQueryType, bool bl, UINT nSubNo) {
	return (-1);
}

CString CSettings::GetSetting (UINT nID, SettingQuery nQueryType, UINT nSubNo) {
	switch (nID) {
		case ID_IMAGE_NAME:
			return (m_strImageName);
		case ID_PATCH_NAME:
			return (m_strPatchName);
		case ID_SURVEY_NAME:
			return (m_strSurveyName);
	}
	return ("");
}

void CSettings::SetSetting (UINT nID, int iVal) {
	switch (nID) {
	}
}

void CSettings::SetSetting (UINT nID, CString strVal) {
	switch (nID) {
		case ID_IMAGE_NAME:
			m_strImageName = strVal;
			break;
		case ID_PATCH_NAME:
			m_strPatchName = strVal;
			break;
		case ID_SURVEY_NAME:
			m_strSurveyName = strVal;
			break;
	}
}

