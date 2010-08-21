#pragma once



// CCmdLine command target

class CCmdLine : public CCommandLineInfo {
protected:
	uint	m_nWaiting;
	bool	m_bPrg;
	CString	m_strPrg;
public:
	CCmdLine();
	virtual ~CCmdLine();
	virtual void ParseParam (const char* pszParam, BOOL bFlag, BOOL bLast);
	void ExecuteParams ();
};


