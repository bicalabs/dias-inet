#include "stdafx.h"
#include "aImAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CaImAPIException

CaImAPIException::CaImAPIException (udword type) : m_nType (type) { ; };
CaImAPIException::~CaImAPIException () { ; };

