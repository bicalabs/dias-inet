#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <aimgui/aimbuttoncolor.h>

// CPaneToolMark dialog

class CPaneToolMark : public CDialog {
	DECLARE_DYNAMIC(CPaneToolMark)

public:
	CPaneToolMark(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaneToolMark();

// Dialog Data
	enum { IDD = IDD_PANE_MARKERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CImageList	m_ilKernels;
	CImageList	m_ilShapes;

	CListCtrl	m_ctrlList;

	CComboBoxEx	m_ctrlKernel;
	CComboBoxEx	m_ctrlShape;
	CComboBoxEx	m_ctrlOutStyle;
	CComboBoxEx	m_ctrlOutWidth;

	CaImButtonColor	m_btnKrnlColor;
	CaImButtonColor	m_btnShpColor;
	CaImButtonColor	m_btnOutColor;

	CSliderCtrl	m_sldKrnlSize;
	CSliderCtrl	m_sldKrnlTransp;
	CSliderCtrl	m_sldShpTransp;
	CSliderCtrl	m_sldShpForm;

	CStatic		m_edtKrnlSize;
	CStatic		m_edtKrnlTransp;
	CStatic		m_edtShpTransp;
	CStatic		m_edtShpForm;
	CStatic		m_edtKrnlOffX;
	CStatic		m_edtKrnlOffY;
	CStatic		m_edtShpSizeX;
	CStatic		m_edtShpSizeY;
	CStatic		m_edtShpAngle;

	CStatic		m_lckKrnlOffs;
	CStatic		m_lckShpSize;
	CStatic		m_lckShpAngle;

	CStatic		m_stKrnl;
	CStatic		m_stKrnlLine;
	CStatic		m_stShp;
	CStatic		m_stShpLine;
	CStatic		m_stSize;
	CStatic		m_stSizeLine;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Update ();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedPtmKrnlColor();
	afx_msg void OnLvnItemchangedPtmList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
