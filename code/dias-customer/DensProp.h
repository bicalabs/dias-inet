#if !defined(AFX_DENSPROP_H__3277694C_3564_4040_9322_6E1755322948__INCLUDED_)
#define AFX_DENSPROP_H__3277694C_3564_4040_9322_6E1755322948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DensProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDensProp dialog

class CDensProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CDensProp)

// Construction
public:
	CDensProp();
	~CDensProp();

// Dialog Data
	//{{AFX_DATA(CDensProp)
	enum { IDD = IDD_PREF_DENSSCALE };
	CEdit	m_ctrlUnits;
	CEdit	m_ctrlName;
	CEdit	m_ctrlDescr;
	CEdit	m_ctrlB;
	CEdit	m_ctrlA;
	CListCtrl	m_ctrlTypes;
	CButton	m_btnSelect;
	CButton	m_btnRemove;
	CListCtrl	m_ctrlList;
	double	m_rA;
	double	m_rB;
	CString	m_strdescr;
	CString	m_strName;
	CString	m_strUnits;
	//}}AFX_DATA

protected:
	CImageList m_ilTypes;
	CImageList m_ilScales;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDensProp)
	public:
	virtual void OnOK();
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnReset();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateList();
	// Generated message map functions
	//{{AFX_MSG(CDensProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeA();
	afx_msg void OnChangeB();
	afx_msg void OnChangeName();
	afx_msg void OnChangeDescr();
	afx_msg void OnChangeUnits();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelect();
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedTypes(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DENSPROP_H__3277694C_3564_4040_9322_6E1755322948__INCLUDED_)
