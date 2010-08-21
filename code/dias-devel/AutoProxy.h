// AutoProxy.h: header file
//

#pragma once

class CMainDlg;


// CAutoProxy command target

class CAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CAutoProxy)

	CAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CMainDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

