#if !defined(AFX_AIMAPICOLLECTION_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
#define AFX_AIMAPICOLLECTION_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_

#pragma once

template <class T>
class _DECLSPEC CaImAPICollection : public CaImAPIObject {
// Construction
public:
	CaImAPICollection () { m_pHashHead = NULL; };
	~CaImAPICollection () {
		if (!m_pHashHead)	return;
		m_pHashHead->DeleteTail ();
		m_pHashHead->Delete ();
		m_pHashHead = NULL;
	};

	CaImAPICollection& operator= (CaImAPICollection& coll) {
		if (m_pHashHead) {
			m_pHashHead->DeleteTail ();
			m_pHashHead->Delete ();
			m_pHashHead = NULL;
		}
		if (coll.m_pHashHead)
			m_pHashHead = coll.m_pHashHead->GetCopy ();
		return (*this);
	};

	CaImAPICollection& operator+= (CaImAPICollection& coll) {
		m_pHashHead.AddTail (coll.m_pHashHead->GetCopy ());
		return (*this);
	};

// Attributes
public:
	CHash<T*,T>* m_pHashHead;

// Operations
public:
	uvar32_64 NameLookup (CString name) {
		uvar32_64 pos = 0;
		if (!m_pHashHead)	return (uvar32_64(-1));
		for (CHash<T*,T>* p = m_pHashHead; p; p = p->GetNext (), ++pos)
			if (p->m_hData->m_strName == name)
				return (pos);
		return (uvar32_64(-1));
	};

	T& operator[] (uvar32_64 pos) {
		if (!m_pHashHead)
			throw (0);
		return (*(m_pHashHead->GetAt(pos)->m_hData));
	};

	T& operator[] (CString name) {
		if (!m_pHashHead)
			throw (0);
		return (*(m_pHashHead->GetAt(NameLookup(name))->m_hData));
	};

	uvar32_64 GetCount () {
		if (!m_pHashHead)	return (0);
		else		return (m_pHashHead->GetCount ());
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaImAPICollection)
	void Serialize (CArchive& ar) {
		CaImAPIObject::Serialize (ar);
		uvar32_64 n, size;
		if (ar.IsStoring ()) {
			if (m_pHashHead == NULL || (size = m_pHashHead->GetCount ()) == 0) {
				ar << uvar32_64 (0);
				return;
			}
			ar << size;
			for (n = 0; n < size; ++n)
				m_pHashHead->GetAt(n)->m_hData->Serialize (ar);
		} else {
			if (m_pHashHead) {
				m_pHashHead->DeleteTail ();
				m_pHashHead->Delete ();
				m_pHashHead = NULL;
			}
		}
	}
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_AIMAPICOLLECTION_H__454E1FAC_03C4_487D_8E76_E7C37D00FFD7__INCLUDED_)
