#if !defined(AFX_AIMAPICOLLECTIONEXT_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
#define AFX_AIMAPICOLLECTIONEXT_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_

#pragma once

#include "aImAPIRange.h"

template <class T>
class _DECLSPEC CaImAPICollectionExt : public CaImAPICollection<T> {
// Construction
public:
	~CaImAPICollectionExt () { ; };
	CaImAPICollectionExt& operator= (CaImAPICollectionExt& coll) {
		(CaImAPICollection<T>)(*this) = (CaImAPICollection<T>)coll;
		return (*this);
	}

// Operations
public:
	uvar32_64 Add (T& item) {
		CHash<T*,T>* p = new CHash<T*,T>(&item);
		if (!m_pHashHead)
			m_pHashHead = p;
		else
			m_pHashHead->GetTail()->InsertAfter (p);
		m_pHashHead = m_pHashHead->GetHead ();
		return ((uvar32_64)(p->GetPos()));
	};

	uvar32_64 Add (CaImAPIRange<T>& item) {
		uvar32_64 c;
		CHash<CHash<T*,T>*,CHash<T*,T>*>* p;
		for (p = item.m_pList, c = 0; !p; p = p->GetNext (), ++c)
			Add (*(p->m_hData->m_hData));
		m_pHashHead = m_pHashHead->GetHead ();
		return (c);
	};

	uvar32_64 Insert (uvar32_64 pos, T& item) {
		CHash<T*,T>* p = new CHash<T*,T>(&item);
		if (m_pHashHead && pos > m_pHashHead->GetCount ())
			throw (0);
		if (!m_pHashHead)
			m_pHashHead = p;
		else
			m_pHashHead->GetAt(pos)->InsertAfter (p);
		m_pHashHead = m_pHashHead->GetHead ();
		return ((uvar32_64)(p->GetPos ()));
	};

	uvar32_64 Insert (CString name, T& item) {
		int pos;
		if ((pos = NameLookup (name)) == uvar32_64(-1))
			throw (0);
		CHash<T*,T>* p = new CHash<T*,T>(&item);
		m_pHashHead->GetAt(pos)->InsertAfter (p);
		m_pHashHead = m_pHashHead->GetHead ();
		return ((uvar32_64)(p->GetPos ()));
	};

	uvar32_64 Insert (uvar32_64 pos, CaImAPIRange<T>& item) {
		uvar32_64 c;
		CHash<CHash<T*,T>*,CHash<T*,T>*>* p;
		for (p = item.m_pList, c = 0; !p; p = p->GetNext (), ++c)
			Insert (pos, *(p->m_hData->m_hData));
		return (c);
	};

	uvar32_64 Insert (CString name, CaImAPIRange<T>& item) {
		uvar32_64 c;
		CHash<CHash<T*,T>*,CHash<T*,T>*>* p;
		for (p = item.m_pList, c = 0; !p; p = p->GetNext (), ++c)
			Insert (name, *(p->m_hData->m_hData));
		return (c);
	};

	void Delete (uvar32_64 pos) {
		CHash<T*,T>* p = m_pHashHead;
		if (!m_pHashHead)	return;
		if (pos >= GetCount ())
			throw (CaImAPIException (0));
		if (m_pHashHead == m_pHashHead->GetAt(pos))
			p = m_pHashHead->GetNext ();
		m_pHashHead->GetAt(pos)->Delete();
		m_pHashHead = p;
	};

	uvar32_64 DeleteAll () {
		uvar32_64 c = GetCount ();
		if (!m_pHashHead)	return (0);
		m_pHashHead->DeleteTail ();
		m_pHashHead->Delete ();
		m_pHashHead = NULL;
		return (c);
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaImAPICollectionExt)
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_AIMAPICOLLECTIONEXT_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
