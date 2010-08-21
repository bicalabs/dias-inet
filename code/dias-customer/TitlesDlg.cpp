// TitlesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dias.h"
#include "TitlesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitlesDlg dialog


CTitlesDlg::CTitlesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTitlesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitlesDlg)
	m_strTitle = _T("");
	m_strDescr = _T("");
	//}}AFX_DATA_INIT
}


void CTitlesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitlesDlg)
	DDX_Text(pDX, IDC_TITLES_TITLE, m_strTitle);
	DDV_MaxChars(pDX, m_strTitle, 64);
	DDX_Text(pDX, IDC_TITLES_DESCR, m_strDescr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitlesDlg, CDialog)
	//{{AFX_MSG_MAP(CTitlesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitlesDlg message handlers
