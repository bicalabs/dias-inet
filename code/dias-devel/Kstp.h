#pragma once

enum EKstpTypes {
	e_kstp_bin = 0, e_kstp_int = 1, e_kstp_real = 2, e_kstp_none = -1,
};

class CKstpValue {
friend class CKstp;
protected:
	EKstpTypes	m_nType;
	ulong	m_nSize;
	struct {
		sdword		dw;
		rtbyte		r;
		ubyte*		bin;
	}		m_uData;

public:
	CKstpValue () {
		m_nType = e_kstp_none;
		m_nSize = 0;
		m_uData.bin = NULL;
	};
	CKstpValue (udword dw) {
		m_nType = e_kstp_int;
		m_uData.dw = dw;
		m_nSize = 0;
	};
	CKstpValue (rtbyte r) { 
		m_nType = e_kstp_real; 
		m_uData.r = r;
		m_nSize = 0;
	};
	CKstpValue (ubyte* pb, ulong size) {
		m_nType = e_kstp_bin;
		m_uData.bin = new ubyte[m_nSize = size];
		memcpy (m_uData.bin, pb, m_nSize);
	};
	CKstpValue (char* pb) { 
		m_nType = e_kstp_bin; 
		m_uData.bin = new ubyte[m_nSize = strlen (pb)];
		memcpy (m_uData.bin, pb, m_nSize);
	};
	CKstpValue (CKstpValue&) {
		throw ("Copy constructor is not allowed for CKstpValue class");
	};
	~CKstpValue () {
		Reset ();
	};

	void Reset () {
		if (m_nType == e_kstp_bin && m_nSize > 0) delete m_uData.bin;
		m_nType = e_kstp_none;
		m_nSize = 0;
		m_uData.bin = NULL;
	};
	ulong Size () { return (m_nSize); };
	EKstpTypes Type () { return (m_nType); };

	void SetInt (sdword dw) { Reset (); m_nType = e_kstp_int; m_uData.dw = dw; };
	void SetReal(rtbyte r) { Reset (); m_nType = e_kstp_real; m_uData.r = r; };
	void SetBin (char* pb) {
		Reset (); 
		m_nType = e_kstp_bin; 
		m_uData.bin = new ubyte[m_nSize = strlen (pb)];
		memcpy (m_uData.bin, pb, m_nSize);
	};
	void SetBin (ubyte* pb, ulong size) {
		Reset (); 
		m_nType = e_kstp_bin;
		m_uData.bin = new ubyte[m_nSize = size];
		memcpy (m_uData.bin, pb, m_nSize);
	};

	sdword operator() (void) {
		if (m_nType == e_kstp_int)
			return (m_uData.dw);
		else
			throw ("Wrong CKstpValue type in operator() expression");
	};

	rtbyte operator() (rtbyte) {
		if (m_nType == e_kstp_real)
			return (m_uData.r);
		else
			throw ("Wrong CKstpValue type in operator() expression");
	};

	ubyte* operator() (char*) {
		if (m_nType == e_kstp_bin)
			return (m_uData.bin);
		else
			throw ("Wrong CKstpValue type in operator() expression");
	};
};

class CKstp {
protected:
	static ubyte m_pbKey[64];
	static ulong m_nMemUsed;

	socket_t	m_nSocket;

	udword			m_nCmd;
	ulong		m_nSize;
	CKstpValue*		m_pValues;

	bool Send ();
	bool Recv ();

public:
	CKstp(void);
	~CKstp(void);

	void SetCmd (udword cmd) { m_nCmd = cmd; };

	bool Alloc (ulong size);
	void Free ();
	bool Communicate (int cmd, char* form, ...);
	bool Process ();

	CKstpValue& operator[] (ulong argno) { return (m_pValues[argno]); };
	EKstpTypes operator() (ulong argno) { return (m_pValues[argno].Type ()); };
	ulong operator() () { return (m_nSize); };

	void* New (size_t s) {
		void *p;
		TRY {
			p = new ubyte[s];
		} CATCH_ALL (e) {
			_CrtMemState state;
			_CrtMemCheckpoint (&state);
			_CrtDumpMemoryLeaks ();
		} END_CATCH_ALL;
		if (!p) {
			_CrtMemState state;
			_CrtMemCheckpoint (&state);
			_CrtDumpMemoryLeaks ();
		} else
			CKstp::m_nMemUsed += s;
		return p;
	};
};
