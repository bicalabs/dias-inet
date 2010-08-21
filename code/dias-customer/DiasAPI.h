
#include <aimapi/aimapi.h>
#include <aimgui/aimgui.h>
#include <tesla/tesla.h>

enum ColorModel { cmMono = 0, cmGrayscale = 1, cmRGB = 2, cmCMY = 3, cmCMYK = 4, cmLab = 5, cmMulty = 5 };
enum FieldType { fieldString = 0, fieldInt = 1, fieldReal = 2 };
enum DensScale { scaleLinear = 0, scalePolynom = 1, scaleExp = 2, scaleLog = 3 };
enum MarkKernel { krnlNone = 0, krnlRound = 1, krnlPlus = 2, krnlX = 3 };
enum MarkShape { shpNone = 0, shpEllipse = 1, shpTriangle = 2, shpRect = 3 };
enum MarkPoint { markpntNone = 0, markpntCenter = 1, markpntAxisX = 2, markpntAxisY = 3, markpntAxisXY = 4 };
enum Parameter {
	// for Segmentate
	segLowLimit, segHighLimit,
	// for ErodeSegs, DilateSegs, OpenSegs, CloseSegs
	segHorzLine, segVertLine, segDiag45, segDiag135, segCross, segSquare, segOctagon,
	// for FillHoles, ExtractHoles, FilterSegs, Segregate
	segInside, segOutside,
};

struct CDiasAPIFilterFn {
public:
	CString			m_strFnName;
//	void			(*FnCall)(CPicture *pIn1, CPicture *pIn2, CPicture *pIn3, void *pParam);
	void			(*AboutCall)();

//	MenuCategory		m_nMenuCat;
	CString			m_strMenuName;
	void			(*MenuCall)();
	void			(*MenuUpdateUI)(CCmdUI *pCmdUI);

//	DialogCategory		m_nDlgCat;
	udword			m_dwDlgFlags;
	CString			m_strDlgName;
//	CDiasAPIFilterDlg	*m_pDialog;
	void			*m_pDefaultParam;
	// #### TODO: Add help support
};

class CDiasApp;
class CColor;

class CStorageDoc;
class CSotrageFrame;
class CPicture;
class CChannel;
class CMark;
class CPatch;
class CSurvey;
class CField;

class CFieldInfo;
class CDensScale;
class CGeomScale;
class CMarkStyle;


class CDiasAPIFrames;
class CDiasAPIFrmRange;
class CDiasAPIChannels;
class CDiasAPIChnlRange;
class CDiasAPIMarks;
class CDiasAPIMrkRange;
class CDiasAPIPatches;
class CDiasAPIPtchRange;
class CDiasAPIImages;
class CDiasAPIImgRange;
class CDiasAPIFields;
class CDiasAPISurveis;
class CDiasAPISrvRange;
class CDiasAPIDocs;
class CDiasAPIDocRange;

class CDiasAPIDensScales;
class CDiasAPIGeomScales;
class CDiasAPIMarkStyles;


#include "Helpers.h"

#include "DiasStatusBar.h"

#include "StorageDoc.h"
#include "StorageFrame.h"

#include "Image.h"
#include "Channel.h"
#include "Mark.h"
#include "Patch.h"
#include "Survey.h"
#include "Field.h"

#include "Resources.h"
#include "DensScale.h"
#include "GeomScale.h"
#include "MarkStyle.h"

#include "DiasAPIFrames.h"
#include "DiasAPIChannels.h"
#include "DiasAPIChnlRange.h"
#include "DiasAPIMarks.h"
#include "DiasAPIMrkRange.h"
#include "DiasAPIPatches.h"
#include "DiasAPIPtchRange.h"
#include "DiasAPIImages.h"
#include "DiasAPIImgRange.h"
#include "DiasAPIFields.h"
#include "DiasAPISurveis.h"
#include "DiasAPISrvRange.h"
#include "DiasAPIDocs.h"
#include "DiasAPIDocRange.h"

#include "DiasAPIDensScales.h"
#include "DiasAPIGeomScales.h"
#include "DiasAPIMarkStyles.h"
