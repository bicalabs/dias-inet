#if !defined(AFX_AIMAPIRANGE_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
#define AFX_AIMAPIRANGE_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_

#pragma once

enum _DECLSPEC aImAPIRngType {
	aimAll		= 0x00000000,
	aimSelected	= 0x00000001,
	aimNonselected	= 0x00000002,
	aimActive	= 0x00000004,
	aimInactive	= 0x00000008,
	aimVisible	= 0x00000010,
	aimHidden	= 0x00000020,
	aimSaved	= 0x00000040,
	aimModified	= 0x00000080,
};

template<class T>
class CaImAPICollection;
template<class T>
class CaImAPICollectionExt;

template<class T>
class _DECLSPEC CaImAPIRange {
friend class CaImAPICollection<T>;
friend class CaImAPICollectionExt<T>;
public:
	CaImAPIRange () : m_ppHead (NULL), m_pList (NULL) { ; };
	~CaImAPIRange () { Reset (); };
protected:
	CHash<T*,T>** m_ppHead;
	CHash<CHash<T*,T>*,CHash<T*,T>*>* m_pList;

public:
	T& operator[] (uvar32_64 pos) throw (CaImAPIException) {
		if (!m_pList || !(m_pList->GetAt(pos)) || !(m_pList->GetAt(pos)->m_hData))
			// #### TODO: Exception
			throw (CaImAPIException (0));
		return (*(m_pList->GetAt(pos)->m_hData->m_hData));
	};
protected:
	void Include (CHash<T*,T>* pItem) {
		CHash<CHash<T*,T>*,CHash<T*,T>*>* pNew = new CHash<CHash<T*,T>*,CHash<T*,T>*>;
		pNew->m_hData = pItem;
		if (!m_pList) m_pList = pNew;
		else m_pList->AddTail (pNew);
	};
public:
	void Reset () {
		if (!m_pList)	return;
		m_pList->RemoveTail ();
		m_pList->Remove ();
		m_pList = NULL;
	};
	uvar32_64 Remove () {
		uvar32_64 c = 0;
		for (CHash<CHash<T*,T>*,CHash<T*,T>*>* p = m_pList; p; p = p->GetNext (), ++c) {
			if (p->m_hData == *m_ppHead && *m_ppHead)
				*m_ppHead = (*m_ppHead)->GetNext ();
			p->m_hData->Remove ();
		}
		return (c);
	};
	uvar32_64 GetCount () {
		if (!m_pList)	return (0);
		else		return (m_pList->GetCount ());
	};
	uvar32_64 SetName (CString strName) {
		uvar32_64 c = 0;
		for (CHash<CHash<T*,T>*,CHash<T*,T>*>* p = m_pList; p; p = p->GetNext (), ++c) {
			if (p->m_hData == *m_ppHead && *m_ppHead)
				*m_ppHead = (*m_ppHead)->GetNext ();
			p->m_hData->m_hData->SetName (strName);
		}
		return (c);
	};
	uvar32_64 SetDescription (CString strDescr) {
		uvar32_64 c = 0;
		for (CHash<CHash<T*,T>*,CHash<T*,T>*>* p = m_pList; p; p = p->GetNext (), ++c) {
			if (p->m_hData == *m_ppHead && *m_ppHead)
				*m_ppHead = (*m_ppHead)->GetNext ();
			p->m_hData->m_hData->SetDescription (strDescr);
		}
		return (c);
	};
};

#endif // !defined(AFX_AIMAPIRANGE_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
