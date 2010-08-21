#ifndef	_VECTOR_DEFINITION
#define	_VECTOR_DEFINITION

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Vector.h : header file
//

#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// CVector

template <class T>
class CVector : public CMatrix<T> {
public:
  CVector ( int dim = 2, T value = (T)(0) );
  CVector ( CMatrix<T>& mtx );

  virtual T& operator() ( UINT c );

  virtual CVector& operator= ( const CVector& vect );
  virtual CVector& operator+= ( const CVector& vect );
  virtual CVector& operator-= ( const CVector& vect );
  virtual CVector operator+ ( const CVector& vect );
  virtual CVector operator- ( const CVector& vect );

  virtual BOOL operator<< ( CVector<T> vect );

  rtbyte Module ( );
};

template <class T>
CVector<T>::CVector ( CMatrix<T>& mtx ) {
  nCols = mtx.nCols;
  nRows = 1;
  pMatrix = new T*[mtx.nCols];
  for ( UINT ci = 0 ; ci < mtx.nCols ; ++ci ) {
    pMatrix[ci] = new T[1];
    pMatrix[ci][0] = mtx.pMatrix[ci][0];
  }
}

template <class T>
CVector<T>::CVector ( int dim, T value ) {
  nCols = dim;
  nRows = 1;
  pMatrix = new T*[dim];
  for ( UINT ci = 0 ; ci < dim ; ++ci ) {
    pMatrix[ci] = new T[1];
    pMatrix[ci][0] = ( T )( value );
  }
}

template <class T>
inline T& CVector<T>::operator ( ) ( UINT c ) {
  return ( pMatrix[c - 1][0] );
}


template <class T>
inline CVector<T>& CVector<T>::operator= ( const CVector& vect ) {
  CMatrix<T> (*this) = CMatrix<T> (vect);
  return ( *this );
}

template <class T>
inline CVector<T>& CVector<T>::operator+= ( const CVector& vect ) {
  CMatrix<T> (*this) += CMatrix<T> (vect);
  return ( *this );
}

template <class T>
inline CVector<T>& CVector<T>::operator-= ( const CVector& vect ) {
  CMatrix<T> (*this) -= CMatrix<T> (vect);
  return ( *this );
}

template <class T>
inline CVector<T> CVector<T>::operator+ ( const CVector& vect ) {
  CMatrix<T> vectAdd = CMatrix<T> (*this) + CMatrix<T> (vect);
  return ( (CVector<T>)(vectAdd) );
}

template <class T>
inline CVector<T> CVector<T>::operator- ( const CVector& vect ) {
  CMatrix<T> vectAdd = CMatrix<T> (*this) - CMatrix<T> (vect);
  return ( (CVector<T>)(vectAdd) );
}


template <class T>
inline BOOL CVector<T>::operator<< ( CVector<T> vect ) {
  return ( pMatrix[0][0] < vect.pMatrix[0][0] && pMatrix[1][0] < vect.pMatrix[1][0] );
}

template <class T>
rtbyte CVector<T>::Module ( ) {
  return ( sqrtl ( pMatrix[0][0] * pMatrix[0][0] + pMatrix[1][0] * pMatrix[1][0] ) );
}

#endif
