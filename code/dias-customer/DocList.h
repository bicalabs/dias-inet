#if !defined(AFX_DOCLIST_H__6A6186FE_0916_4C39_827A_27BB4E679920__INCLUDED_)
#define AFX_DOCLIST_H__6A6186FE_0916_4C39_827A_27BB4E679920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DocList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDocList window

class CStorageView;

class CDocList : public CListCtrl {
friend class CStorageFrame;
// Construction
public:
	CDocList ();

// Attributes
public:
	CStorageView	*m_pView;

protected:
	UINT		m_nType;
	CImageList	m_imgBig, m_imgSmall;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocList)
	public:
	virtual BOOL Create(CWnd* pParentWnd, CStorageView *pView, UINT type);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDocList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDocList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditRename();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditDelete();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCLIST_H__6A6186FE_0916_4C39_827A_27BB4E679920__INCLUDED_)
