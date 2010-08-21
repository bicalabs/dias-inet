// StringFile.cpp: implementation of the CStringFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::CStringFile()
// 
// Description: This is the contructor for this class. It is possible
//	the set the size of the internal buffer in the constructor.
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 17:21:52]
// 
// Returns	: - nothing -  
// Params	: 
//		  [int nBufSize] - size of the used buffer
//
// ==================================================================
CStringFile::CStringFile(int nBufSize)
{
	m_dwRead = nBufSize;
	m_nBufferSize = nBufSize;
	m_pBuffer = new BYTE[nBufSize];
	m_dwMasterIndex = 0;
	m_dwIndex = 0;
	m_dwLine = 0;
}

CStringFile::~CStringFile()
{
	delete m_pBuffer;
}

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::Open()
// 
// Description:This function opens a file for reading. The file is 
//			opened with readonly reads...and shared only when read
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 17:21:52]
// 
// Returns	: [BOOL] - FALSE on open failure (you may check 
//							optional feError)
//					   TRUE on successfull opening of this file
// Params	: 
//			  [LPCSTR szFile] - The wanted file
//			  [CFileException *feError] -(optional) this provides for
//						means of extended error detection
// ==================================================================
BOOL CStringFile::Open(LPCSTR szFile, CFileException *feError)
{
	CFileStatus		rStatus;
	if(!m_fFile.Open(szFile, CFile::modeRead| CFile::shareDenyWrite,feError))
		return FALSE;
	m_fFile.GetStatus(rStatus);
	m_nMaxSize = rStatus.m_size;
	return TRUE;
}

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::Close()
// 
// Description:Closes our previously opened file
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 17:26:53]
// 
// Returns	: [void] - nothing
// 
// Params	: - none - 
// ==================================================================
void CStringFile::Close()
{
	m_fFile.Close();
	m_dwMasterIndex = 0;
	m_dwIndex = 0;
	m_dwLine = 0;
}

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::GetNextLine()
// 
// Description	: Read the next line from a file
// 
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 12:57:50]
// 
// Returns	: [DWORD] - 0 nothing read because of an error or EOF
//						non 0...index of line first line has line 
//						number 1
// Params	: 
//			  [CString &szLine] - The read line return in a CString
// 
// ==================================================================
DWORD CStringFile::GetNextLine(CString &szLine)
{
	char	*szBuffer;
	DWORD	dwReturn;
	szBuffer = new char[m_nBufferSize];
	dwReturn = this->GetNextLine(szBuffer,m_nBufferSize);
	if(dwReturn != 0)
		szLine = szBuffer;
	else szLine = "";	//Empty
	delete szBuffer;
	return dwReturn;
}

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::GetNextLine()
// 
// Description	: Read the next line from a file
// 
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 12:53:50]
// 
// Returns	: [DWORD] - 0 nothing read because of an error or EOF
//						non 0...index of line first line has line 
//						number 1
// Params	: 
//			  [LPSTR szLine] - Pointer to a buffer which recieves the 
//								read line
//			  [int iLineSize] - Size of the supplied line buffer
// ==================================================================
DWORD CStringFile::GetNextLine(LPSTR szLine, int iLineSize)
{
	BYTE	*chTemp;
	BOOL	bStop=FALSE;
	int		nOut;

	chTemp = (LPBYTE) szLine;
	*chTemp = 0;
	nOut = 0;
	while(!bStop)
	{
		if(!m_dwLine || m_dwIndex==m_dwRead)
		{
			m_dwMasterIndex = m_fFile.Seek(0,CFile::current);
			m_dwRead=m_fFile.Read((LPVOID) m_pBuffer,m_nBufferSize);
			m_dwIndex = 0;
			if(m_dwRead == 0)
			{
				bStop = TRUE; //Error during readfile or END-OF-FILE encountered
				if(nOut>0)
				{
					chTemp[nOut++] = (char) 0;
					return m_dwLine;	
				}
				else return m_dwLine = 0; //nix gelezen
			}
			else
			{
				if(m_dwRead != (DWORD) m_nBufferSize)
					bStop = TRUE;	//END-OF-FILE
			}
		}
		for(;m_dwIndex < m_dwRead;m_dwIndex++)
		{
			if((nOut+1) == iLineSize)
			{
				m_szError.Format("m_pBuffer overflow in line %u (line length over %d chars)",++m_dwLine,iLineSize);
				::MessageBox(NULL,m_szError,"Warning !",MB_ICONEXCLAMATION|MB_OK);
				chTemp[nOut] = '\0';
				return m_dwLine;
			}
			switch(m_pBuffer[m_dwIndex])
			{
			case 0x0d://End of Line encountered
			case 0x0a:
				if((m_dwIndex+1) < m_dwRead) // Check we're not on end of m_pBuffer ???
					if(m_pBuffer[m_dwIndex+1] == '\n' || m_pBuffer[m_dwIndex+1] == '\r')
					{
						if(!*chTemp)
							m_dwLine++;
						m_dwIndex++;
					}
				if(*chTemp)
				{
					chTemp[nOut++] = '\0';
					m_dwLine++;
					return m_dwLine;
				}
				break;
			default: chTemp[nOut++] = m_pBuffer[m_dwIndex];
			}
		}
	}
	if(nOut>0)
	{
		chTemp[nOut++] = '\0';
		return m_dwLine;	
	}
	return m_dwLine = 0; //nix gelezen
}

void CStringFile::GetErrorText(CString &szMsg)
{
	szMsg = m_szError;
}

// =======================Function Description=======================
// 
// FUNCTION	:  CStringFile::Reset()
// 
// Description:Reset the linecounter....this function effectively sets
//			the reading pointer to 0, so when the GetNextLine is 
//			executed it will start reading the first line
// ------------------------------------------------------------------
// Author	: [Frank Driesens], Created : [30-03-2000 17:28:49]
// 
// Returns	: [void] - none
// 
// Params	: - none -
// 
// ==================================================================
void CStringFile::Reset()
{
	m_dwIndex = 0;
	m_dwLine = 0;
	m_fFile.SeekToBegin();
}
