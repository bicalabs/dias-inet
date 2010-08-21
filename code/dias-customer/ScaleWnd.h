#if !defined(AFX_SCALEWND_H__CB3ABF79_E414_41FC_B009_253261F69F11__INCLUDED_)
#define AFX_SCALEWND_H__CB3ABF79_E414_41FC_B009_253261F69F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScaleWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleWnd window

class CScaleWnd : public CWnd {
// Construction
public:
	enum EScaleType {scaleVert, scaleHoriz, scaleCentr};

	CScaleWnd();
	virtual ~CScaleWnd();

// Attributes
public:
	EScaleType m_nType;
	bool	m_blInner;
	UINT	m_nBorder;
	UINT	m_nIntBegin, m_nIntEnd;
	UINT	m_nActiveBegin, m_nActiveEnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleWnd)
	public:
	virtual BOOL Create(EScaleType nType, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CScaleWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEWND_H__CB3ABF79_E414_41FC_B009_253261F69F11__INCLUDED_)
