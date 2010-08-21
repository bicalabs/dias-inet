#pragma once

template<class TYPE, class ARG>
class CHash {
// Construction
public:
	CHash ();
	CHash (TYPE d);
	CHash (const CHash<TYPE,ARG>& lst);
	~CHash () {;}

// Operators
public:
	CHash<TYPE,ARG>& operator= (const CHash<TYPE,ARG>& lst);
	CHash<TYPE,ARG>* operator[] (uvar32_64 n);
	ARG operator() (uvar32_64 n) { return ((*this)[n]->m_hData); };

// Attributes
protected:
	CHash<TYPE,ARG> *m_pPrev, *m_pNext;
public:
	TYPE m_hData;

// Navigation
public:
	uvar32_64		GetPos ();
	uvar32_64		GetCount ();
	bool			IfMember (CHash<TYPE,ARG>* pMember);
	CHash<TYPE,ARG>*	GetAt (uvar32_64 n);
	ARG			GetDataAt (uvar32_64 n) { return (GetAt(n)->m_hData); };
	CHash<TYPE,ARG>*	GetNext () { return (m_pNext); };
	CHash<TYPE,ARG>*	GetPrev () { return (m_pPrev); };

// Creation, removing at al.
public:
	CHash<TYPE,ARG>*	GetReplica ();
	CHash<TYPE,ARG>*	GetCopy ();
	CHash<TYPE,ARG>*	GetHead ();
	CHash<TYPE,ARG>*	GetTail ();

	uvar32_64		AddHead (CHash<TYPE,ARG> *el) { return (GetHead()->InsertBefore(el)); };
	uvar32_64		AddTail (CHash<TYPE,ARG> *el) { return (GetTail()->InsertAfter(el)); };;
	uvar32_64		InsertAfter (CHash<TYPE,ARG> *el);
	uvar32_64		InsertBefore (CHash<TYPE,ARG> *el);
	uvar32_64		CopyBefore (CHash<TYPE,ARG> *el);
	uvar32_64		CopyAfter (CHash<TYPE,ARG> *el);
	CHash<TYPE,ARG>*	Cut ();
	void			Remove ();
	uvar32_64		RemoveBefore (uvar32_64 n);
	uvar32_64		RemoveAfter (uvar32_64 n);
	uvar32_64		RemoveHead ();
	uvar32_64		RemoveTail ();
	void			Delete () {
						delete ((TYPE)m_hData);
						Remove ();
					};
	uvar32_64		DeleteBefore (uvar32_64 n);
	uvar32_64		DeleteAfter (uvar32_64 n);
	uvar32_64		DeleteHead ();
	uvar32_64		DeleteTail ();

protected:
	void			SetNext (CHash<TYPE,ARG> *p) { m_pNext = p; };
	void			SetPrev (CHash<TYPE,ARG> *p) { m_pPrev = p; };

// Serialization
public:
	void			Serialize (CArchive& ar);
};

template<class TYPE,class ARG>
CHash<TYPE,ARG>::CHash () {
	m_pPrev = m_pNext = NULL;
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>::CHash (TYPE d) {
	m_hData = d;
	m_pPrev = m_pNext = NULL;
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>::CHash (const CHash<TYPE,ARG>& lst) {
	*this = lst;
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>& CHash<TYPE,ARG>::operator= (const CHash<TYPE,ARG>& lst) {
	m_hData = lst.m_hData;
	m_pPrev = lst.m_pPrev;
	m_pNext = lst.m_pNext;
	return (*this);
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>* CHash<TYPE,ARG>::operator[] (uvar32_64 n) {
	CHash<TYPE,ARG> *ptr = ((n < 0)?m_pPrev:m_pNext);
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
CHash<TYPE,ARG>* CHash<TYPE,ARG>::GetAt (uvar32_64 n) {
	return ((*this)[n]);
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>* CHash<TYPE,ARG>::GetCopy () {
	CHash<TYPE,ARG> *add, *curr, *el = this->GetHead (),
			*head = new CHash<TYPE,ARG>;
	if (!head)	return (NULL);
	for (curr = head; el; el = el->m_pNext) {
		curr->m_hData = new ARG (*(el->m_hData));
		add = new CHash<TYPE,ARG>;
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
CHash<TYPE,ARG>* CHash<TYPE,ARG>::GetReplica () {
	CHash<TYPE,ARG> *add, *curr, *el = this->GetHead (),
			*head = new CHash<TYPE,ARG>;
	if (!head)	return (NULL);
	for (curr = head; el; el = el->m_pNext) {
		curr->m_hData = el->m_hData;
		add = new CHash<TYPE,ARG>;
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
uvar32_64 CHash<TYPE,ARG>::GetCount () {
	uvar32_64 len = 1;
	CHash<TYPE,ARG> *ptr;
	for ( ptr = m_pPrev; ptr; ++len, ptr = ptr->m_pPrev ) {;}
	for ( ptr = m_pNext; ptr; ++len, ptr = ptr->m_pNext ) {;}
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::GetPos () {
	uvar32_64 len = 0;
	CHash<TYPE,ARG> *ptr;
	for ( ptr = m_pPrev; ptr; ++len, ptr = ptr->m_pPrev ) {;}
	return (len);
}

template<class TYPE,class ARG>
bool CHash<TYPE,ARG>::IfMember (CHash<TYPE,ARG>* pMember) {
	for (CHash<TYPE,ARG>* p = this; p; p = p->m_pPrev)
		if (pMember == p)
			return (true);
	for (p = m_pNext; p; p = p->m_pNext)
		if (pMember == p)
			return (true);
	return (false);
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>* CHash<TYPE,ARG>::GetHead () {
	CHash<TYPE,ARG> *ptr = m_pPrev;
	if (!ptr)
	    return (this);
	while (ptr->m_pPrev)
		ptr = ptr->m_pPrev;
	return (ptr);
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>* CHash<TYPE,ARG>::GetTail () {
	CHash<TYPE,ARG> *ptr = m_pNext;
	if (!ptr)
	    return (this);
	while (ptr->m_pNext)
		ptr = ptr->m_pNext;
	return (ptr);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::InsertAfter (CHash<TYPE,ARG> *el) {
	if (!el)	return (0);
	if (IfMember(el))	return (0);
	uvar32_64 len = el->GetCount ();
	CHash<TYPE,ARG> *head = el->GetHead ();
	head->m_pPrev = this;
	el = el->GetTail ();
	el->m_pNext = m_pNext;
	if (m_pNext)	m_pNext->m_pPrev = el;
	m_pNext = head;
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::InsertBefore (CHash<TYPE,ARG> *el) {
	if (!el)	return (0);
	if (IfMember(el))	return (0);
	uvar32_64 len = el->GetCount ();
	CHash<TYPE,ARG> *tail = el->GetTail ();
	tail->m_pNext = this;
	el = el->GetHead ();
	el->m_pPrev = m_pPrev;
	if (m_pPrev)	m_pPrev->m_pNext = el;
	m_pPrev = tail;
	return (len);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::CopyBefore (CHash<TYPE,ARG> *el) {
	if (!el)	return (0);
	return (InsertBefore (el->GetCopy ()));
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::CopyAfter (CHash<TYPE,ARG> *el) {
	if (!el)	return (0);
	return (InsertAfter (el->GetCopy ()));
}

template<class TYPE,class ARG>
CHash<TYPE,ARG>* CHash<TYPE,ARG>::Cut () {
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	return (this);
}

template<class TYPE,class ARG>
void CHash<TYPE,ARG>::Remove () {
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	delete this;
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::RemoveBefore (uvar32_64 n) {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pPrev, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pPrev;
		delete ptr;
	}
	m_pPrev = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::RemoveAfter (uvar32_64 n) {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pNext, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pNext;
		delete ptr;
	}
	m_pNext = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::RemoveHead () {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pPrev, *nptr;
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
uvar32_64 CHash<TYPE,ARG>::RemoveTail () {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pNext, *nptr;
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
uvar32_64 CHash<TYPE,ARG>::DeleteBefore (uvar32_64 n) {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pPrev, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pPrev;
		delete ptr->m_hData;
		delete ptr;
	}
	m_pPrev = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::DeleteAfter (uvar32_64 n) {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pNext, *nptr;
	for (c = 0; ptr && c < n; ++c) {
		nptr = ptr->m_pNext;
		delete ptr->m_hData;
		delete ptr;
	}
	m_pNext = ptr;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::DeleteHead () {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pPrev, *nptr;
	for (c = 0; ptr; ++c, ptr = nptr) {
		nptr = ptr->m_pPrev;
		delete ((TYPE)ptr->m_hData);
		delete ptr;
		if (nptr)
			nptr->m_pNext = NULL;
	}
	m_pPrev = NULL;
	return (c);
}

template<class TYPE,class ARG>
uvar32_64 CHash<TYPE,ARG>::DeleteTail () {
	uvar32_64 c;
	CHash<TYPE,ARG> *ptr = m_pNext, *nptr;
	for (c = 0; ptr; ++c, ptr = nptr) {
		nptr = ptr->m_pNext;
		delete ((TYPE)ptr->m_hData);
		delete ptr;
		if (nptr)
			nptr->m_pPrev = NULL;
	}
	m_pNext = NULL;
	return (c);
}

template<class TYPE,class ARG>
void CHash<TYPE,ARG>::Serialize (CArchive& ar) {
	uvar32_64 n, cnt = GetCount ();
	CHash<TYPE,ARG>* pHead = GetHead (), p;
	if (ar.IsLoading ()) {
		RemoveTail ();
		RemoveHead ();
		ar >> cnt;
		p = new CHash<TYPE,ARG>;
		if (cnt > 0)
			m_hData.Serialize (ar);
		for (n = 0; n < cnt; ++n) {
			p->m_hData.Serialize (ar);
			GetTail()->CopyAfter (p);
		}
		delete p;
	} else {
		ar << cnt;
		for (n = 0; n < cnt; ++n)
			pHead->GetDataAt(n).Serialize (ar);
	}
}
