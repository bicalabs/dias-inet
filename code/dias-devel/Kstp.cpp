#include "StdAfx.h"
#include ".\kstp.h"

struct errs {
	udword	code;
	char*	msg;
};

errs errors[] = {
{KSERR_STARTED, "Started"},
{KSERR_COULDNTSTART, "Could not start"},
{KSERR_SUCCESS, "Command completed successfully"},
{KSERR_NOACCESS, "Access denied"},
{KSERR_SHARECONFLICT, "Sharing conflict"},
{KSERR_NOTHOBJ, "Not an object handle"},
{KSERR_NOTEXISTS, "Object does not exists"},
{KSERR_OPENED, "Opened"},
{KSERR_SOCKET, "Socket creation error"},
{KSERR_BIND, "Socket binding error"},
{KSERR_LISTEN, "Socket listen error"},
{KSERR_OBJOPENED, "Object is already open"},
{KSERR_WAITFORCLIENT, ""},
{KSERR_CLIENTCONNECTED, ""},
{KSERR_CLIENTDISCONN, ""},
{KSERR_ACCEPTSOCKET, "Socket accepting error"},
{KSERR_SOCKRECV, "Socket receive error"},
{KSERR_SOCKRECVDATA, "Socket data receiving error"},
{KSERR_ARGCOUNT, "Wrong command arguments count"},
{KSERR_ARGTYPE, "Wrong command argument type"},
{KSERR_NULLPTR, "Null pointer"},
{KSERR_NEWCLIENT, "Client is successfully connected"},
{KSERR_BYECLIENT, "Client is successfully disconected"},
{KSERR_UNKNOWNCMD, "Unknown command"},
{KSERR_SOCKRECVFMT, "Socket data reciving format error"},
{KSERR_SRCFILEOPEN, "Error opening source file"},
{KSERR_OBJFILEOPEN, "Error opening object's file"},
{KSERR_NOTIMPLEMENTED, "Not implemented yet"},
{KSERR_WRONGSRCPATH, "Wrong source path"},
{KSERR_WRONGSRCFILE, "Wrong source file"},
{KSERR_UNKNOWNDOBJF, "Unknown source file format"},
{KSERR_UNKNOWNDOBJT, "Unknown object type"},
{KSERR_OBJDBINIT, "Error initializing objects database"},
{KSERR_OBJDBCONN, "Error connection objects database"},
{KSERR_MAXCLIENTS, "Maximum clients"},
{KSERR_SQL, "Objects database SQL error"},
{KSERR_OPFAILED, "Operation failed"},
{KSERR_OBJEXISTS, "Object is already exists"},
{KSERR_OBJHASPARENTS, "Object is already encapsulated"},
{KSERR_OBJNOTPARENT, "Object is not a parent"},
{KSERR_QUOT, "Not enough quotas"},
{KSERR_NOCLIENT, "Client is not connected"},
{KSERR_QUOTAEXC, "Quota exlusion"},
{KSERR_NOTOWNER, "Not owner"},
{KSERR_CLIENTKEY, "Wrong client key"},
{KSERR_OUTOFMEM, "Out of system memory"},
{KSERR_NOTKSMEM, "Not a dias-ks memory object"},
{KSERR_MEMINT, "Memory initialization error"},
{KSERR_MEMTAILNOTNULL, "Memory tail not null error"},
{KSERR_MEMZEROSIZE, "Memory zero size error"},
{KSERR_MEMSTAMP1, "Memory stamp1 error"},
{KSERR_MEMSTAMP2, "Memory stamp2 error"},
{KSERR_LARGENETPACK, "Too large network packet"},
{KSERR_CRCLENGTH, "CRC error in packet length"},
{KSERR_CRCPACK, "CRC error in packet data"},
{KSERR_PACKRECVLEN, "Receved packet length not equal to actual packet length"},
{NULL, NULL}
};

ubyte CKstp::m_pbKey[64] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

ulong CKstp::m_nMemUsed = 0;

CKstp::CKstp (void) {
	m_nCmd = 0;
	m_nSize = 0;
	m_pValues = NULL;
}

CKstp::~CKstp (void) {
	Free ();
}

bool CKstp::Alloc (ulong size) {
	udword cmd = m_nCmd;
	Free ();
	m_nCmd = cmd;
	m_nSize = size;
	if (m_nSize > 0)
		m_pValues = (CKstpValue*) new CKstpValue[m_nSize];
	return (true);
}

void CKstp::Free () {
	if (m_pValues && m_nSize > 0)
		delete [] m_pValues;
	m_nCmd = 0;
	m_nSize = 0;
	m_pValues = NULL;
}

bool CKstp::Communicate (int cmd, char* form, ...) {
	int n = 0;
	char *pc;
	ubyte* pb;
	va_list al;
	va_start (al, form);

	Free ();
	m_nCmd = cmd;
	m_nSize = strlen (form);
	if (m_nSize > 0)
		m_pValues = (CKstpValue*) new CKstpValue[m_nSize];
	while (*form) {
		if (*form == 'i') {
			m_pValues[n].m_nType = e_kstp_int;
			m_pValues[n].m_uData.dw = va_arg(al, int);
		} else if (*form == 'r') {
			m_pValues[n].m_nType = e_kstp_real;
			m_pValues[n].m_uData.r = va_arg(al, double);
		} else if (*form == 's') {
			m_pValues[n].m_nType = e_kstp_bin;
			pc = va_arg(al, char*);
			m_pValues[n].m_nSize = strlen (pc);
			m_pValues[n].m_uData.bin = (ubyte*) new ubyte[(m_pValues[n].m_nSize + 1)];
			strcpy ((char*)(m_pValues[n].m_uData.bin), pc);
		} else if (*form == 'b') {
			m_pValues[n].m_nType = e_kstp_bin;
			pb = va_arg(al, ubyte*);
			m_pValues[n].m_uData.bin = (ubyte*) new ubyte[(m_pValues[n].m_nSize = *(ulong*)pb)];
			memcpy (m_pValues[n].m_uData.bin, pb + sizeof (ulong), m_pValues[n].m_nSize);
		}
		form++;
		n++;
	}
	va_end (al);

	return (Process ());
}

bool CKstp::Process () {
	socket_t s;
	struct sockaddr_in ks_inetserv;
	ks_inetserv.sin_family = AF_INET;
	ks_inetserv.sin_port = htons (5634);
	ks_inetserv.sin_addr.s_addr = inet_addr ("127.0.0.1");
	memset (&(ks_inetserv.sin_zero), 0, 8);
	if ((m_nSocket = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
		//
		return (false);
	}
	if ((s = connect (m_nSocket, (struct sockaddr*)&ks_inetserv, sizeof (struct sockaddr_in))) == -1) {
		//
		return (false);
	}

	ubyte key[64];
	memcpy (key, m_pbKey, 64);
	Send ();
	Free ();
	Recv ();
	closesocket (m_nSocket);


	int i;
	CString msg;
	if (m_nCmd != 0) {
		memcpy (m_pbKey, key, 64);
		for (i = 0; errors[i].msg && errors[i].code != m_nCmd; ++i) {;}
		msg.Format ("Error: 0x%08X, %s\nMessage: '%s'\nFile: %s, Line: %d", m_nCmd, errors[i].msg ? errors[i].msg : "Unknown error", m_pValues[0](""), m_pValues[1](""), m_pValues[2]);
		MessageBox (NULL, msg, "DIAS-KS returned error", MB_OK | MB_ICONSTOP);
		return (false);
	}
	return (true);
}

bool CKstp::Send () {
	udword	i;
	ubyte	crc8;
	address_t len, buff_len;
	uchar	*sendbuff, *ptr;
	ubyte	*argt = (ubyte*) new ubyte[m_nSize];

	len = 76 + sizeof (address_t) + m_nSize * (1 + 4);
	for (i = 0 ; i < m_nSize ; ++i) {
		argt[i] = (ubyte)m_pValues[i].Type ();
		if (argt[i] == e_kstp_bin)		len += m_pValues[i].Size ();
		if (argt[i] == e_kstp_int)		len += sizeof (udword);
		if (argt[i] == e_kstp_real)	len += sizeof (rtbyte);
	}
	len &= 0xFFFFFF;

	sendbuff = (uchar*) new uchar[(buff_len = len)];
	memset (ptr = sendbuff, 0, buff_len);
	len |= (((len&0x0000FF) ^ ((len&0x00FF00)>>8) ^ ((len&0xFF0000)>>16)) << 24);
	memcpy (ptr, &(len), 4);
	ptr += 4;
	memcpy (ptr += 4, &(m_nCmd), 4);
	memcpy (ptr += 4, &(m_nSize), sizeof (address_t));
	memcpy (ptr += sizeof (address_t), m_pbKey, 64);
	memcpy (ptr += 64, argt, m_nSize);
	ptr += m_nSize;

	for (i = 0 ; i < m_nSize ; ++i) {
		switch ((EKstpTypes)argt[i]) {
			case e_kstp_bin:
				len = *(udword*)(ptr) = m_pValues[i].Size ();
				memcpy (ptr += 4, m_pValues[i].m_uData.bin, m_pValues[i].Size ());
				ptr += *(udword*)(ptr - 4);
				break;
			case e_kstp_int:
				*(udword*)(ptr) = sizeof (sdword);
				memcpy (ptr += 4, &(m_pValues[i].m_uData.dw), sizeof (sdword));
				ptr += 4;
				break;
			case e_kstp_real:
				*(udword*)(ptr) = sizeof (rtbyte);
				memcpy (ptr += 4, &(m_pValues[i].m_uData.r), sizeof (rtbyte));
				ptr += sizeof (rtbyte);
				break;
		}
	}

	for (ptr = sendbuff + 8, crc8 = 0; (address_t)(ptr - sendbuff) < buff_len; ++ptr)
		crc8 ^= *ptr;
	*((ubyte*)sendbuff + 4) = crc8;

	send (m_nSocket, (const char *)sendbuff, buff_len, 0);

	delete [] sendbuff;
	delete [] argt;
	return (true);
}

bool CKstp::Recv () {
	udword	i;
	ubyte	crc8;
	udword	len, tmplen;
	ubyte	*recvbuff, *ptr;
	ubyte	*argt;

	Free ();

	recv (m_nSocket, (char *)&tmplen, 4, MSG_PEEK);
	len = tmplen & 0xFFFFFF;

	recvbuff = (uchar *) new uchar [len];
	memset (recvbuff, 0, len);
	tmplen = 0;
	do {
		i = recv (m_nSocket, (char *)(recvbuff + tmplen), len - tmplen, 0);
		tmplen += i;
	} while (len > tmplen);
	for (ptr = recvbuff + 8, crc8 = 0; (address_t)(ptr - recvbuff) < len; ++ptr)
		crc8 ^= *ptr;

	m_nCmd = *(udword*)(ptr = recvbuff + 8);
	m_nSize = *(udword*)(ptr += 4);
	memcpy (m_pbKey, ptr += 4, 64);
	argt = (ubyte*) new ubyte[m_nSize];
	m_pValues = (CKstpValue*) new CKstpValue[m_nSize];

	memcpy (argt, ptr += 64, m_nSize);
	ptr += m_nSize;

	for (i = 0 ; i < m_nSize ; ++i) {
		m_pValues[i].m_nType = (EKstpTypes) argt[i];
		switch ((EKstpTypes)argt[i]) {
			case e_kstp_bin:
				m_pValues[i].m_nSize = len = *(udword*)(ptr);
				m_pValues[i].m_uData.bin = (ubyte*) new ubyte[(len + 1)];
				memcpy (m_pValues[i].m_uData.bin, ptr + 4, len);
				m_pValues[i].m_uData.bin[len] = 0x00;
				ptr += len + 4;
				break;
			case e_kstp_int:
				m_pValues[i].m_uData.dw = *(sdword*)(ptr += 4);
				ptr += sizeof (sdword);
				break;
			case e_kstp_real:
				m_pValues[i].m_uData.r = *(rtbyte*)(ptr += 4);
				ptr += sizeof (rtbyte);
				break;
		}
	}

	delete [] recvbuff;
	delete [] argt;
	return (true);
}
