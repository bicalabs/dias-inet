#pragma once

class CSaxContent : public ISAXContentHandler {
public:
	CSaxContent ();
	CSaxContent (CTreeCtrl* tree, kscmd* cmdsys, ksmtx_factors* fctsys, ksmtx_order* ordsys);
	~CSaxContent ();

public:
	static ulong references;

protected:
	int	cls;
	char	buff[0x100];
	CString	chars;
	kscmd*	cmds;
	ksmtx_factors*	factors;
	ksmtx_order*	orders;
	HTREEITEM m_tviCurr;
	CTreeCtrl* m_pTree;

public:
	long __stdcall QueryInterface (REFIID riid, void **ppObject) { return S_OK; }
	ulong __stdcall AddRef () { return ++references; }
	ulong __stdcall Release () { return --references; }

	long __stdcall raw_putDocumentLocator (struct ISAXLocator *pLocator);
	long __stdcall raw_startDocument ();
	long __stdcall raw_endDocument ();
	long __stdcall raw_startPrefixMapping (ushort *wsPrefix, int nPrefix, ushort *wsUri, int nUri);
	long __stdcall raw_endPrefixMapping (ushort *wsPrefix, int nPrefix);
	long __stdcall raw_startElement (ushort *wsNsUri, int nNsUri, ushort *wsName, int nName, ushort *wsQName, int nQName, struct ISAXAttributes *pAttr);
	long __stdcall raw_endElement (ushort *wsNsUri, int nNsUri, ushort *wsName, int nName, ushort *wsQName, int nQName);
	long __stdcall raw_characters (ushort *wsChars, int nChars);
	long __stdcall raw_ignorableWhitespace (ushort *wsChars, int nChars);
	long __stdcall raw_processingInstruction (ushort *wsTarget, int nTarget, ushort *wsData, int nData);
	long __stdcall raw_skippedEntity (ushort *wsName, int nName);
};

class CSaxError : public ISAXErrorHandler {
public:
	CSaxError ();
	CSaxError (CTreeCtrl* tree);
	~CSaxError ();

public:
	static ulong references;

protected:
	char buff[0x100];

public:
	long __stdcall QueryInterface (REFIID riid, void **ppObject) { return S_OK; }
	ulong __stdcall AddRef () { return ++references; }
	ulong __stdcall Release () { return --references; }

	long __stdcall raw_error (ISAXLocator *pLocator, ushort *wsMessage, long hrCode);
	long __stdcall raw_fatalError (ISAXLocator *pLocator, ushort *wsMessage, long hrCode);
	long __stdcall raw_ignorableWarning (ISAXLocator *pLocator, ushort *wsMessage, long hrCode);
};
