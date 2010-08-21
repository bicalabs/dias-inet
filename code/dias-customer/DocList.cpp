// DocList.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "Dias.h"
#include "StorageView.h"
#include "DocList.h"
#include ".\doclist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocList

CDocList::CDocList () {
}

CDocList::~CDocList () {
}


BEGIN_MESSAGE_MAP(CDocList, CListCtrl)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_RENAME, OnEditRename)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocList message handlers

BOOL CDocList::Create (CWnd* pParentWnd, CStorageView *pView, UINT type) {
	m_nType = type;
	m_pView = pView;

	m_imgBig.Create (20, 21, ILC_COLOR8 | ILC_MASK, 3, 1);
	m_imgSmall.Create (20, 21, ILC_COLOR8 | ILC_MASK, 3, 1);
	m_imgBig.Add (theApp.LoadBitmap (IDB_IL_DOCTYPES), 0xFFFFFF);
	m_imgSmall.Add (theApp.LoadBitmap (IDB_IL_DOCTYPES), 0xFFFFFF);

	CListCtrl::Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | LVS_ALIGNLEFT | LVS_AUTOARRANGE | LVS_EDITLABELS | LVS_REPORT | LVS_SHAREIMAGELISTS | 
	                    LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER, CRect (0, 0, 200, 200), pParentWnd, 0);

	InsertColumn (0, "", LVCFMT_LEFT, 100);
	SetImageList (&m_imgSmall, LVSIL_SMALL);
	SetBkColor (::GetSysColor (COLOR_BTNSHADOW));
	SetTextBkColor (::GetSysColor (COLOR_BTNSHADOW));
	SetTextColor (0xFFFFFF);
	return (true);
}

void CDocList::OnSize (UINT nType, int cx, int cy) {
	CRect rect;
	CListCtrl::OnSize(nType, cx, cy);
	GetClientRect (&rect);
	SetColumnWidth (0, rect.Width() - 5);
}

void CDocList::OnContextMenu (CWnd* pWnd, CPoint point) {
	CMenu menu;
	if (GetSelectedCount () == 0)
		menu.LoadMenu (IDR_POPUP_DOC_EMPTY);
	else
		menu.LoadMenu (IDR_POPUP_DOC_ITEM);
	CMenu *pSub = menu.GetSubMenu (0);
	pSub->TrackPopupMenu (TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDocList::OnItemChanged (NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	if (!(pNMListView->uChanged & LVIF_STATE))
		return;
	if (m_nType == 0)
		m_pView->OnChangeImage (aimGetSelListItem(*this, false));
	else if (m_nType == 1)
		m_pView->OnChangeSurvey (aimGetSelListItem(*this, false));
	// #### TODO: Decomment this lines when Reports will be done
//	else if (m_nType == 2)
//		m_pView->OnChangeImage (aimGetSelListItem(*this, false));
}

void CDocList::OnEndLabelEdit (NMHDR* pNMHDR, LRESULT* pResult) {
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ( ( pDispInfo->item.mask & LVIF_TEXT ) == 0 )
		return;

	uvar32_64 selected = aimGetSelListItem (*this, false);
	SetItemText (selected, 0, pDispInfo->item.pszText);
	if (m_nType == 0)
		m_pView->m_pDoc->Images[selected].SetName (pDispInfo->item.pszText);
	else if (m_nType == 1)
		m_pView->m_pDoc->Surveis[selected].SetName (pDispInfo->item.pszText);
	// #### TODO: Decomment this lines when Reports will be done
//	else if (m_nTYpe == 2)
//		m_pView->m_pDoc->Reports[selected].SetName (pDispInfo->item.pszText);
	
	*pResult = 0;
}

BOOL CDocList::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt) {
	int sel;
	if ((sel = aimGetSelListItem (*this, true)) != -1)
		SetItemState (sel, 0, LVIS_SELECTED | LVIS_FOCUSED);
	sel -= zDelta / abs (zDelta);
	if (sel < 0 || sel >= GetItemCount ())
		sel = (-zDelta / abs (zDelta) < 0) ? GetItemCount () - 1 : 0;
	SetItemState (sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	EnsureVisible (sel, false);
	return true;
}

void CDocList::OnEditRename () {
	int selected = aimGetSelListItem (*this, false);
	if (selected != -1)
		EditLabel (selected);
}

void CDocList::OnEditDelete () {
	UINT n = m_pView->m_pDoc->Frames[aimActive].GetActiveImageNo ();
	m_pView->m_pDoc->Images(aimSelected).Remove ();
	if (m_pView->m_pDoc->Images.GetCount() <= n)
		m_pView->m_pDoc->Frames[aimActive].SetActiveImageNo (m_pView->m_pDoc->Images.GetCount() - 1);
	else
		m_pView->m_pDoc->Frames[aimActive].SetActiveImageNo (n);
}
