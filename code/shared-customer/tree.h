#pragma once

template<class TYPE, class ARG>
class CTree {
// Construction
public:
	CTree ();
	CTree (ARG d);
	CTree (const CTree<TYPE,ARG>& lst);
	~CTree () {;}

// Operators
public:
	CTree<TYPE,ARG>& operator= (const CTree<TYPE,ARG>& lst);
	CTree<TYPE,ARG>* operator[] (uvar32_64 n);
	ARG operator() (uvar32_64 n) { return ((*this)[n]->m_hData); };

// Attributes
protected:
	CTree<TYPE,ARG> *m_pPrev, *m_pNext, *m_pChild;
public:
	TYPE m_hData;

// Navigation
public:
	uvar32_64		GetPos ();
	uvar32_64		GetCount ();
	bool			IfMember (CTree<TYPE,ARG>* pMember);
	CTree<TYPE,ARG>*	GetAt (uvar32_64 n);
	ARG			GetDataAt (uvar32_64 n) { return (GetAt(n)->m_hData); };
	CTree<TYPE,ARG>*	GetNext () { return (m_pNext); };
	CTree<TYPE,ARG>*	GetPrev () { return (m_pPrev); };
	CTree<TYPE,ARG>*	GetChild () { return (m_pChild); };

// Creation, removing at al.
public:
	CTree<TYPE,ARG>*	GetCopy ();
	CTree<TYPE,ARG>*	GetHead ();
	CTree<TYPE,ARG>*	GetTail ();

	void			AddChild (TYPE d) { m_pChild = new CTree<TYPE,ARG> (d); };
	uvar32_64		AddHead (CTree<TYPE,ARG> *el) { return (GetHead()->InsertBefore(el)); };
	uvar32_64		AddTail (CTree<TYPE,ARG> *el) { return (GetTail()->InsertAfter(el)); };;
	uvar32_64		InsertAfter (CTree<TYPE,ARG> *el);
	uvar32_64		InsertBefore (CTree<TYPE,ARG> *el);
	uvar32_64		CopyBefore (CTree<TYPE,ARG> *el);
	uvar32_64		CopyAfter (CTree<TYPE,ARG> *el);
	CTree<TYPE,ARG>*	Cut ();
	void			Remove ();
	uvar32_64		RemoveBefore (uvar32_64 n);
	uvar32_64		RemoveAfter (uvar32_64 n);
	uvar32_64		RemoveHead ();
	uvar32_64		RemoveTail ();

protected:
	void			SetNext (CTree<TYPE,ARG> *p) { m_pNext = p; };
	void			SetPrev (CTree<TYPE,ARG> *p) { m_pPrev = p; };

// Serialization
public:
	void			Serialize (CArchive& ar);
};

template<class TYPE,class ARG>
CTree<TYPE,ARG>::CTree () {
	m_pPrev = m_pNext = m_pChild = NULL;
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>::CTree (ARG d) {
	m_hData = d;
	m_pPrev = m_pNext = m_pChild = NULL;
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>::CTree (const CTree<TYPE,ARG>& lst) {
	*this = lst;
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>& CTree<TYPE,ARG>::operator= (const CTree<TYPE,ARG>& lst) {
	m_hData = lst.m_hData;
	m_pPrev = lst.m_pPrev;
	m_pNext = lst.m_pNext;
	m_pChild = lst.m_pChild;
	return (*this);
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::operator[] (uvar32_64 n) {
	CTree<TYPE,ARG> *ptr = ((n < 0)?m_pPrev:m_pNext);
	if (n == 0)
		return (this);
	else if ( n < 0 )
		for (uvar32_64 i = 1; i > n && ptr; --i)
			ptr = ptr->m_pPrev;
	else
		for (uvar32_64 i = 1; i < n && ptr; ++i)
			ptr = ptr->m_pNext;
	return (ptr);
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::GetAt (uvar32_64 n) {
	return ((*this)[n]);
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::GetCopy () {
	CTree<TYPE,ARG> 	*add, *curr, *el = this->GetHead (),
			*head = new CTree<TYPE,ARG>;
	if (!head)	return (NULL);
	for (curr = head; el; el = el->m_pNext) {
		curr->m_hData = el->m_hData;
		add = new CTree<TYPE,ARG>;
		curr->InsertAfter (add);
		curr = add;
	}
	curr = curr->m_pPrev;
	curr->m_pNext = NULL;
	add->m_pPrev = NULL;
	delete add;
	return (head);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::GetCount () {
	uvar32_64 len = 1;
	CTree<TYPE,ARG> *ptr;
	for ( ptr = m_pPrev; ptr; ++len, ptr = ptr->m_pPrev ) {;}
	for ( ptr = m_pNext; ptr; ++len, ptr = ptr->m_pNext ) {;}
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::GetPos () {
	uvar32_64 len = 0;
	CTree<TYPE,ARG> *ptr;
	for ( ptr = m_pPrev; ptr; ++len, ptr = ptr->m_pPrev ) {;}
	return (len);
}

template<class TYPE,class ARG>
bool CTree<TYPE,ARG>::IfMember (CTree<TYPE,ARG>* pMember) {
	for (CTree<TYPE,ARG>* p = this; p; p = p->m_pPrev)
		if (pMember == p)
			return (true);
	for (p = m_pNext; p; p = p->m_pNext)
		if (pMember == p)
			return (true);
	return (false);
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::GetHead () {
	CTree<TYPE,ARG> *ptr = m_pPrev;
	if (!ptr)
	    return (this);
	while (ptr->m_pPrev)
		ptr = ptr->m_pPrev;
	return (ptr);
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::GetTail () {
	CTree<TYPE,ARG> *ptr = m_pNext;
	if (!ptr)
	    return (this);
	while (ptr->m_pNext)
		ptr = ptr->m_pNext;
	return (ptr);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::InsertAfter (CTree<TYPE,ARG> *el) {
	if (!el)	return (0);
	if (IfMember(el))	return (0);
	uvar32_64 len = el->GetCount ();
	CTree<TYPE,ARG> *head = el->GetHead ();
	head->m_pPrev = this;
	el = el->GetTail ();
	el->m_pNext = m_pNext;
	if (m_pNext)	m_pNext->m_pPrev = el;
	m_pNext = head;
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::InsertBefore (CTree<TYPE,ARG> *el) {
	if (!el)	return (0);
	if (IfMember(el))	return (0);
	uvar32_64 len = el->GetCount ();
	CTree<TYPE,ARG> *tail = el->GetTail ();
	tail->m_pNext = this;
	el = el->GetHead ();
	el->m_pPrev = m_pPrev;
	if (m_pPrev)	m_pPrev->m_pNext = el;
	m_pPrev = tail;
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::CopyBefore (CTree<TYPE,ARG> *el) {
	if (!el)	return (0);
	return (InsertBefore (el->GetCopy ()));
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::CopyAfter (CTree<TYPE,ARG> *el) {
	if (!el)	return (0);
	return (InsertAfter (el->GetCopy ()));
}

template<class TYPE,class ARG>
CTree<TYPE,ARG>* CTree<TYPE,ARG>::Cut () {
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	return (this);
}

template<class TYPE,class ARG>
void CTree<TYPE,ARG>::Remove () {
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	delete this;
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::RemoveBefore (uvar32_64 n) {
	uvar32_64 c;
	CTree<TYPE,ARG> *ptr = m_pPrev, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pPrev;
		delete ptr;
	}
	m_pPrev = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::RemoveAfter (uvar32_64 n) {
	uvar32_64 c;
	CTree<TYPE,ARG> *ptr = m_pNext, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pNext;
		delete ptr;
	}
	m_pNext = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::RemoveHead () {
	uvar32_64 c;
	CTree<TYPE,ARG> *ptr = m_pPrev, *nptr;
	for (c = 0; ptr; ++c, ptr = nptr) {
		nptr = ptr->m_pPrev;
		delete ptr;
		if (nptr)
			nptr->m_pNext = NULL;
	}
	m_pPrev = NULL;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CTree<TYPE,ARG>::RemoveTail () {
	uvar32_64 c;
	CTree<TYPE,ARG> *ptr = m_pNext, *nptr;
	for (c = 0; ptr; ++c, ptr = nptr) {
		nptr = ptr->m_pNext;
		delete ptr;
		if (nptr)
			nptr->m_pPrev = NULL;
	}
	m_pNext = NULL;
	return (c);
}

template<class TYPE,class ARG>
void CTree<TYPE,ARG>::Serialize (CArchive& ar) {
	udword n, cnt = GetCount (), childs;
	CTree<TYPE,ARG> *pHead = GetHead (), *p;
	if (ar.IsLoading ()) {
		RemoveTail ();
		RemoveHead ();
		ar >> cnt;
		p = new CTree<TYPE,ARG>;
		m_hData.Serialize (ar);
		ar >> childs;
		if (childs) {
			m_pChild = new CTree<TYPE,ARG>;
			m_pChild->Serialize (ar);
		}
		for (n = 1; n < cnt; ++n) {
			p->m_hData.Serialize (ar);
			GetTail()->CopyAfter (p);
			ar >> childs;
			if (childs) {
				GetTail()->m_pChild = new CTree<TYPE,ARG>;
				GetTail()->m_pChild->Serialize (ar);
			}
		}
		delete p;
	} else {
		ar << cnt;
		for (n = 0; n < cnt; ++n) {
			pHead->GetDataAt(n).Serialize (ar);
			if (!pHead->GetAt(n)->m_pChild)
				ar << (udword)0;
			else {
				ar << (udword)1;
				pHead->GetAt(n)->m_pChild->Serialize (ar);
			}
		}
	}
}
