// MainDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "imagectrl.h"

class CAutoProxy;

// CMainDlg dialog
class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg);
	friend class CAutoProxy;
	friend class CDiasDevelApp;

// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMainDlg();

	void* New (size_t s) {
		void *p;
		TRY {
			p = new ubyte[s];
		} CATCH_ALL (e) {
			_CrtMemState state;
			_CrtMemCheckpoint (&state);
			_CrtDumpMemoryLeaks ();
		} END_CATCH_ALL;
		if (!p) {
			_CrtMemState state;
			_CrtMemCheckpoint (&state);
			_CrtDumpMemoryLeaks ();
		} else
			CMainDlg::m_nMemUsed += s;
		return p;
	};

// Dialog Data
	enum { IDD = IDD_DIASDEVEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	sdword m_hImg1, m_hImg2;
	static ulong m_nMemUsed;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	void OnImg1Changed ();
	void OnImg2Changed ();
	void RefreshArgs ();

	DECLARE_MESSAGE_MAP()

	CComboBox	m_cmbI1W;
	CListCtrl	m_ctrlI1N;
	CComboBox	m_cmbI2W;
	CListCtrl	m_ctrlI2N;
	CImageCtrl	m_ctrlImg1;
	CImageCtrl	m_ctrlImg2;
	CListCtrl	m_ctrlMacro;
	BOOL		m_blPreview;
	CTreeCtrl	m_ctrlFuncs;
	CListCtrl	m_ctrlArgs;
	CImageList	m_ctrlIl;
	CComboBox*	m_pCombo;
	CComboBox*	m_pComboFct;
	CComboBox*	m_pComboOrd;
	CEdit*		m_pEdit;
	int		m_iEditItem;
	kscmd		*dias_cmdsys;
	ksmtx_factors	*dias_fctsys;
	ksmtx_order	*dias_ordsys;

public:
	afx_msg void OnFuncs_SelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnArgs_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnArgs_Click(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnArgs_DblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefresh();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnImg1_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnImg2_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnImg1_ItemActivated(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnImg2_ItemActivated(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnExecute();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnImg1_Click(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnImg2_Click(NMHDR *pNMHDR, LRESULT *pResult);
};

#define DCMD_ARGST_OBJECT
#define	DCMD_ARGST_BOOL
#define	DCMD_ARGST_GREYVAL
#define	DCMD_ARGST_OFFSETS
#define	DCMD_ARGST_FACTORS
