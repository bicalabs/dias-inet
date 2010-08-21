#ifndef	_MATRIX_DEFINITION
#define	_MATRIX_DEFINITION

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Matrix.h : header file
//

rtbyte _math_power (rtbyte a, UINT p);

/////////////////////////////////////////////////////////////////////////////
// CMatrix

template <class T>
class CMatrix : public CObject {
public:

  BOOL	blOverflow;
  UINT	nRows, nCols;
  T**	pMatrix;

public:
  enum  ETypes { mtxNormal, mtxDiag, mtxTrigon, };

  CMatrix ( UINT cc = 3, UINT rc = 3, T v = (T)(0) );
  CMatrix ( const CMatrix& mtx );
  CMatrix ( ETypes Type, UINT dim = 3, T v = (T)(1) );
  ~CMatrix ( );

  virtual T Sp ( );
  virtual T Det ( );
  virtual void Minor ( UINT c, UINT r );
  virtual void MinorCol ( UINT c );
  virtual void MinorRow ( UINT r );
  virtual T Max ( );
  virtual T Min ( );
  virtual T ArMean ( );			// Среднее арифметическое
//  virtual T HeMean ( );			// Среднее геометрическое
 // virtual T HaMean ( );			// Среднее гармоническое
  virtual T SqMean ( );			// Среднее квадратичное

  virtual T& operator() ( UINT c, UINT r );

  virtual CMatrix& operator= ( const T v );
  virtual CMatrix& operator= ( const CMatrix& mtx );

/*  virtual BOOL operator== ( const CMatrix& mtx );
  virtual BOOL operator!= ( const CMatrix& mtx );
  virtual BOOL operator== ( const rtbyte& v );
  virtual BOOL operator!= ( const rtbyte& v );*/

  virtual CMatrix operator+ ( const T& v );
  virtual CMatrix operator- ( const T& v );
  virtual CMatrix operator* ( const T& v );
  virtual CMatrix operator/ ( const T& v );
  virtual CMatrix operator+ ( const CMatrix& mtx );
  virtual CMatrix operator- ( const CMatrix& mtx );
  virtual CMatrix operator* ( const CMatrix& mtx );
  virtual CMatrix& operator+= ( const CMatrix& mtx );
  virtual CMatrix& operator-= ( const CMatrix& mtx );

  virtual CMatrix& operator! ( );  // M = M(-1)		// Reverting
  virtual CMatrix& operator~ ( );  // M = M(T)		// Transponing
//  virtual CMatrix& operator^ ( );  // M = M(+)		// Hermite

	virtual CMatrix& operator|= ( const CMatrix& mtx ); // Logical or
};

/////////////////////////////////////////////////////////////////////////////
// CMatrix

// ***
// ***  Constructor and destructor
// ***

template <class T>
CMatrix<T>::CMatrix ( UINT cc, UINT rc, T v ) {
  nCols = cc;
  nRows = rc;
  pMatrix = new T*[cc];
  for ( UINT ci = 0 , ri ; ci < cc ; ++ci ) {
    pMatrix[ci] = new T[rc];
    for ( ri = 0 ; ri < rc ; ++ri )
      pMatrix[ci][ri] = v;
  }
}

template <class T>
CMatrix<T>::CMatrix ( const CMatrix& mtx ) {
  nCols = mtx.nCols;
  nRows = mtx.nRows;
  pMatrix = new T*[nCols];
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci ) {
    pMatrix[ci] = new T[nRows];
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] = mtx.pMatrix[ci][ri];
  }
}

template <class T>
CMatrix<T>::CMatrix ( ETypes Type, UINT dim, T v ) {
  blOverflowMode = FALSE;
  nCols = dim;
  nRows = dim;
  pMatrix = new T*[nCols];
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci ) {
    pMatrix[ci] = new T[nRows];
    for ( ri = 0 ; ri < nRows ; ++ri ) {
      if ( Type != mtxDiag )
				pMatrix[ci][ri] = v;
      else if ( ri == ci )
				pMatrix[ci][ri] = v;
      else
				pMatrix[ci][ri] = 0;
    }
  }
}

template <class T>
CMatrix<T>::~CMatrix ( ) {
  for ( UINT ci = 0 ; ci < nCols ; ++ci )
    delete[] pMatrix[ci];
  delete[] pMatrix;
  nCols = nRows = 0;
}

// ***
// ***  Defining functions
// ***

template <class T>
T CMatrix<T>::Sp ( ) {
  T sp = T ( 0 );
  if ( nRows != nCols )
    return ( sp );
  for ( UINT idx = 0 ; idx < nCols ; ++idx )
    sp = sp + pMatrix[idx][idx];
  return ( sp );
}

template <class T>
T CMatrix<T>::Det ( ) {
  if ( nRows != nCols )
    return ( 0 );

  T rDet = 0;
  CMatrix  mtx ( *this );

  if ( nCols <= 1 )
    return ( pMatrix[0][0] );
  for ( UINT ci = 0 ; ci < nCols ; ++ci ) {
    mtx = CMatrix ( *this );
    mtx.Minor ( ci + 1, 1 );
    rDet += ( T )pow ( -1.0L, (int)(ci + 1) ) * pMatrix[ci][0] * mtx.Det ( );
  }
  return ( rDet );
}

template <class T>
void CMatrix<T>::Minor ( UINT c, UINT r ) {
  MinorCol ( c );
  MinorRow ( r );
}

template <class T>
void CMatrix<T>::MinorCol ( UINT c ) {
  for ( UINT ci = c , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci - 1][ri] = pMatrix[ci][ri];
  --nCols;
}

template <class T>
void CMatrix<T>::MinorRow ( UINT r ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = r ; ri < nRows ; ++ri )
      pMatrix[ci][ri - 1] = pMatrix[ci][ri];
  --nRows;
}

template <class T>
T CMatrix<T>::Max ( ) {
  T max = 0;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      max = ( pMatrix[ci][ri] > max ) ? pMatrix[ci][ri] : max;
  return ( max );
}

template <class T>
T CMatrix<T>::Min ( ) {
  T min = 0;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      min = ( pMatrix[ci][ri] < min ) ? pMatrix[ci][ri] : min;
  return ( min );
}

template <class T>
T CMatrix<T>::ArMean ( ) {
  T mean = 0;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mean += pMatrix[ci][ri];
  mean /= ( T ) ( ri * ci );
  return ( mean );
}

/*template <class T>
T CMatrix<T>::HeMean ( ) {
  T mean = 0;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mean += pMatrix[ci][ri];
  mean /= ri * ci;
  return ( mean );
}*/

/*template <class T>
T CMatrix<T>::HaMean ( ) {
  T mean = 0;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mean += 1 / pMatrix[ci][ri];
  mean /= ri * ci;
  return ( mean );
}*/

template <class T>
T CMatrix<T>::SqMean ( ) {
  T mean = 0;
/*  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mean += ( (pMatrix[ci][ri]) << 1 );
  mean /= ( ri * ri * ci * ci );*/
  return ( mean );
}

// ***
// ***  Overloading operators
// ***


template <class T>
inline T& CMatrix<T>::operator ( ) ( UINT c, UINT r ) {
  return ( pMatrix[c - 1][r - 1] );
}


template <class T>
CMatrix<T>& CMatrix<T>::operator= ( const T v ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] = v;
  return ( *this );
}


template <class T>
CMatrix<T>& CMatrix<T>::operator= ( const CMatrix& mtx ) {
  for ( UINT ci = 0 ; ci < nCols ; ++ci )
    delete[] pMatrix[ci];
  delete[] pMatrix;

  nCols = mtx.nCols;
  nRows = mtx.nRows;
  pMatrix = new T*[nCols];
  for ( ci = 0 ; ci < nCols ; ++ci )
    pMatrix[ci] = new T[nRows];

	UINT ri;
  for ( ci = 0 ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] = mtx.pMatrix[ci][ri];
  return ( *this );
}


template <class T>
CMatrix<T> CMatrix<T>::operator+ ( const T& r ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] += r;
  return ( *this );
}


template <class T>
CMatrix<T> CMatrix<T>::operator- ( const T& r ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] -= r;
  return ( *this );
}


template <class T>
CMatrix<T> CMatrix<T>::operator* ( const T& r ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] *= r;
  return ( *this );
}


template <class T>
CMatrix<T> CMatrix<T>::operator/ ( const T& r ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] /= r;
  return ( *this );
}

template <class T>
CMatrix<T> CMatrix<T>::operator+ ( const CMatrix& mtx ) {
  CMatrix mtxAdd = *this;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mtxAdd.pMatrix[ci][ri] = pMatrix[ci][ri] + mtx.pMatrix[ci][ri];
  return ( mtxAdd );
}

template <class T>
CMatrix<T> CMatrix<T>::operator- ( const CMatrix& mtx ) {
  CMatrix mtxSub = *this;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      mtxSub.pMatrix[ci][ri] = pMatrix[ci][ri] - mtx.pMatrix[ci][ri];
  return ( mtxSub );
}


template <class T>
CMatrix<T> CMatrix<T>::operator* ( const CMatrix& mtx ) {
  if ( nCols != mtx.nRows )
    return ( *this );

  T	   rMtpl;
  CMatrix  mtxProtoT ( nCols, mtx.nRows );
  for ( UINT ci = 0 , ri , di ; ci < nCols ; ++ci ) {
    rMtpl = 0;
    for ( ri = 0 ; ri < nRows ; ++ri ) {
      for ( di = 0 ; di < nRows ; ++di )
        rMtpl += pMatrix[ri][di] * mtx.pMatrix[di][ci];
      mtxProtoT.pMatrix[ci][ri] = rMtpl;
    }
  }
  return ( mtxProtoT );
}


template <class T>
CMatrix<T>& CMatrix<T>::operator~ ( ) { // Transponing matrix
  if ( nCols != nRows )
    return ( *this );
  T  t;
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri ) {
      t = pMatrix[ci][ri];
      pMatrix[ci][ri] = pMatrix[ri][ci];
      pMatrix[ri][ci] = t;
    }
  return ( *this );
}


template <class T>
CMatrix<T>& CMatrix<T>::operator! ( ) { // Reverting matrix
  if ( nCols != nRows )
    return ( *this );

  T	  det = Det ( );
  CMatrix mtxMinor,
	  mtxProtoT ( *this );

  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri ) {
      mtxMinor = mtxProtoT;
      mtxMinor.Minor ( ci + 1, ri + 1 );
      pMatrix[ci][ri] = ( T )_math_power ( -1, ci + ri ) * mtxMinor.Det ( ) / det;
    }
  return ( *this );
}

template <class T>
CMatrix<T>& CMatrix<T>::operator+= ( const CMatrix& mtx ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] += mtx.pMatrix[ci][ri];
  return ( *this );
}

template <class T>
CMatrix<T>& CMatrix<T>::operator-= ( const CMatrix& mtx ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] -= mtx.pMatrix[ci][ri];
  return ( *this );
}

template <class T>
CMatrix<T>& CMatrix<T>::operator|= ( const CMatrix& mtx ) {
  for ( UINT ci = 0 , ri ; ci < nCols ; ++ci )
    for ( ri = 0 ; ri < nRows ; ++ri )
      pMatrix[ci][ri] = ( pMatrix[ci][ri] == 0 && mtx.pMatrix[ci][ri] == 0 ) ? 0 : pMatrix[ci][ri];
  return ( *this );
}

#endif
