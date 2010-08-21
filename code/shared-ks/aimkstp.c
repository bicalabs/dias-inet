/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services public include file with communicaction protocol definitions*
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 06.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

#include <malloc.h>
#include <memory.h>
#include <winsock2.h>

#include <sysconf.h>
#include <strsafex.h>
#include "aim.h"
#include "aimkstp.h"

#define	KSSOCK_BUFF_SIZE	0x100000

udword kstp_errno;

struct errordef_st kserr_messages[] = {
	KSERR_SUCCESS,		"Operation completed successfully",
	KSERR_NOACCESS,		"Access denied",
	KSERR_SHARECONFLICT,	"Object sharing conflict",
	KSERR_NOTHOBJ,		"Invalid object handle",
	KSERR_NOTEXISTS,	"Object not exists",
	KSERR_OPENED,		"Object is already opened",
	KSERR_SOCKET,		"Error during creating server socket",
	KSERR_BIND,		"Error during binding socket",
	KSERR_LISTEN,		"Error during listen socket",
	KSERR_COULDNTSTART,	"Kernel service couldn't start",
	KSERR_OBJOPENED,	"Object is successfully opened",
	KSERR_WAITFORCLIENT,	"Kernel service thread is waiting for client",
	KSERR_CLIENTCONNECTED,	"Client has been successfully connected",
	KSERR_CLIENTDISCONN,	"Client session was completed successfully",
	KSERR_ACCEPTSOCKET,	"Error while accepting socket connection",
	KSERR_SOCKRECV,		"Socket read error",
	KSERR_SOCKRECVDATA,	"Wrong request format",
	KSERR_ARGCOUNT,		"Wrong arguments count in service procedure query",
	KSERR_ARGTYPE,		"Wrong argument type in service procedure query",
	KSERR_NULLPTR,		"NULL pointer",
	KSERR_NEWCLIENT,	"New cliend has been logged in",
	KSERR_BYECLIENT,	"Client has been loged out",
	KSERR_UNKNOWNCMD,	"Unknown service call",
	KSERR_OUTOFMEM,		"Out of memory",
	KSERR_NOTKSMEM,		"Not a kernel service memory object",
	KSERR_NOTOWNER,		"Non-owner access denied",
	KSERR_MEMINT,		"Internal kernel service memory manager error",
	KSERR_SOCKRECVFMT,	"Wrong request format",
	KSERR_STARTED,		"Dias Kernel Service was successfully started",
	KSERR_SRCFILEOPEN,	"Unable to open source file",
	KSERR_OBJFILEOPEN,	"Unable to create object's file",
	KSERR_NOTIMPLEMENTED,	"Service is not implemented",
	KSERR_WRONGSRCPATH,	"Wrong source file path specified",
	KSERR_WRONGSRCFILE,	"Wrong source file format",
	KSERR_UNKNOWNDOBJF,	"Unknown object file format specified",
	KSERR_UNKNOWNDOBJT,	"Unknown object type specified",
	KSERR_OBJDBINIT,	"Unable to initialize database connection",
	KSERR_OBJDBCONN,	"Unable to connect objects' database",
	KSERR_MAXCLIENTS,	"Unable to connect client: maximum clients number reached",
	KSERR_SQL,		"SQL error",
	KSERR_OPFAILED,		"Operation failed",
	KSERR_OBJEXISTS,	"Object already exists in database",
	KSERR_OBJHASPARENTS,	"Object has parents",
	KSERR_OBJNOTPARENT,	"Object is not a parents",
	KSERR_QUOT,		"Quota limit",
	KSERR_CRCLENGTH,	"Packet length CRC error",
	KSERR_CRCPACK,		"Packet data CRC error",
	KSERR_MEMTOLARGE,	"The requested memory size is too large",
	0xFFFFFFFF, "Unknown internal error"
};

#define VALIDATE_A(cond,err,a)	if (cond) { \
					kstp_errno = err; \
					a; \
					return (false); \
				}
#define VALIDATE(cond,err)	if (cond) { \
					kstp_errno = err; \
					return (false); \
				}

bool kstp_allocate (packet* data, address_t argsno) {
	address_t i;
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);

	data->argc = argsno;
	VALIDATE (!(data->argt = (ubyte*)malloc (argsno)), KSTPERR_OUTOFMEM);
	VALIDATE (!(data->argv = (void**)malloc (argsno * sizeof (void*))), KSTPERR_OUTOFMEM);
	for (i = 0 ; i < data->argc ; i++)
		data->argv[i] = NULL;
	return (true);
}

void kstp_release (packet* data) {
	address_t i;
	kstp_errno = 0;
	if (!data)	return;
	for (i = 0 ; data->argv && i < data->argc ; i++) {
//		if (data->argv[i] && data->argt[i] == 0x00)
//			strsafe_free ((string)data->argv[i]);
//		else
			free (data->argv[i]);
	}
	free (data->argv);
	free (data->argt);
	data->argv = NULL;
	data->argt = NULL;
}

bool kstp_send (socket_t s, packet* data) {
	udword	i;
	ubyte	crc8;
	address_t len, buff_len;
	uchar	*sendbuff, *ptr;

#ifdef _DEBUG
	udword j;
	ubyte *p;
#endif

	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);

	len = 76 + sizeof (address_t) + data->argc * (1 + 4);
	for (i = 0 ; i < data->argc ; ++i) {
		if (data->argt[i] == 0x00)	len += strsafe_len ((string)(data->argv[i]));
		if (data->argt[i] == 0x01)	len += sizeof (udword);
		if (data->argt[i] == 0x02)	len += sizeof (rtbyte);
	}
	VALIDATE (len > KSSOCK_BUFF_SIZE, KSTPERR_LARGENETPACK);
	len &= 0xFFFFFF;

	VALIDATE (!(sendbuff = (uchar*)malloc (buff_len = len)), KSTPERR_OUTOFMEM);
	memset (ptr = sendbuff, 0, buff_len);
	len |= (((len&0x0000FF) ^ ((len&0x00FF00)>>8) ^ ((len&0xFF0000)>>16)) << 24);
	memcpy (ptr, &(len), 4);
	ptr += 4;
	memcpy (ptr += 4, &(data->cmd), 4);
	memcpy (ptr += 4, &(data->argc), sizeof (address_t));
	memcpy (ptr += sizeof (address_t), data->key, 64);
	memcpy (ptr += 64, data->argt, data->argc);
	ptr += data->argc;

#ifdef _DEBUG
	fprintf (fp_kstp_log, "Sending packet: cmd=0x%08X, argc=%d\n", data->cmd, data->argc);
#endif

	for (i = 0 ; i < data->argc ; ++i) {
		switch (data->argt[i]) {
			case 0x00:
				len = *(udword*)(ptr) = strsafe_len ((string)(data->argv[i]));
				memcpy (ptr += 4, strsafe_ptr ((string)(data->argv[i])), len);
				ptr += *(udword*)(ptr - 4);
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\tstring: length=0x%08X content=\"", len);
				p = (ubyte*)strsafe_ptr ((string)(data->argv[i]));
				if (len >= 0xFF)
					fputs ("<object is too large>", fp_kstp_log);
				else
					for (j = 0; j < len; ++j) {
						if (p[j] < 0x20 || p[j] > 0x7E) {
							if (j == 0 || (p[j-1] > 0x20 && p[j-1] < 0x7E))
								fprintf (fp_kstp_log, "\", %02X", p[j]);
							else
								fprintf (fp_kstp_log, ", %02X", p[j]);
						} else {
							if (j > 0 && (p[j-1] < 0x20 || p[j-1] > 0x7E))
								fprintf (fp_kstp_log, ", \"%c", p[j]);
							else
								fputc (p[j], fp_kstp_log);
						}
					}
				fputs ("\n", fp_kstp_log);
#endif
				break;
			case 0x01:
				*(udword*)(ptr) = 4;
				memcpy (ptr += 4, data->argv[i], sizeof (sdword));
				ptr += 4;
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\tint: %08X\n", *(sdword*)(data->argv[i]));
#endif
				break;
			case 0x02:
				*(udword*)(ptr) = sizeof (rtbyte);
				memcpy (ptr += 4, data->argv[i], sizeof (rtbyte));
				ptr += sizeof (rtbyte);
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\treal: %f\n", *(rtbyte*)(data->argv[i]));
#endif
				break;
		}
	}

	for (ptr = sendbuff + 8, crc8 = 0; (address_t)(ptr - sendbuff) < buff_len; ++ptr)
		crc8 ^= *ptr;
	*((ubyte*)sendbuff + 4) = crc8;

#ifdef _DEBUG
	fprintf (fp_kstp_log, "\tpacket: length=0x%08X, crc8=0x%02X\n", buff_len, (int)crc8);
	fflush (fp_kstp_log);
#endif

	VALIDATE_A (send (s, (const char *)sendbuff, buff_len, 0) == -1, KSTPERR_SOCKRECV, free (sendbuff));
	kstp_release (data);
	free (sendbuff);
	return (true);
}

bool kstp_recv (socket_t s, packet* data) {
	udword	i;
	ubyte	crc8;
	udword	len, tmplen;
	ubyte	*recvbuff, *ptr;

#ifdef _DEBUG
	uvar32_64 j;
	ubyte *p;
#endif

	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);

	recv (s, (char *)&tmplen, 4, MSG_PEEK);
	len = tmplen & 0xFFFFFF;
	VALIDATE ((tmplen >> 24) != ((len&0x0000FF) ^ ((len&0x00FF00)>>8) ^ ((len&0xFF0000)>>16)), KSTPERR_CRCLENGTH);

	VALIDATE (!(recvbuff = (uchar*)malloc (len)), KSTPERR_OUTOFMEM);
	memset (recvbuff, 0, len);
	tmplen = 0;
	do {
		VALIDATE_A ((i = recv (s, (char *)(recvbuff + tmplen), len - tmplen, 0)) == -1, KSTPERR_SOCKRECV, free (recvbuff));
		tmplen += i;
	} while (len > tmplen);
	VALIDATE_A (len != tmplen, KSTPERR_PACKRECVLEN, free (recvbuff));
	for (ptr = recvbuff + 8, crc8 = 0; (address_t)(ptr - recvbuff) < len; ++ptr)
		crc8 ^= *ptr;
	VALIDATE_A (crc8 != *(ubyte*)(recvbuff + 4), KSTPERR_CRCPACK, free (recvbuff));

	data->cmd = *(udword*)(ptr = recvbuff + 8);
	data->argc = *(udword*)(ptr += 4);
	memcpy (data->key, ptr += 4, 64);
	VALIDATE_A (!(data->argt = (ubyte *)malloc (data->argc)), KSTPERR_OUTOFMEM, free (recvbuff));
	VALIDATE_A (!(data->argv = (void **)malloc (sizeof (void *) * data->argc)), KSTPERR_OUTOFMEM,
	            free (recvbuff); free (data->argt));
	memset (data->argv, 0, sizeof (void *) * data->argc);

	memcpy (data->argt, ptr += 64, data->argc);
	ptr += data->argc;

#ifdef _DEBUG
	fprintf (fp_kstp_log, "Receiving packet: cmd=0x%08X, argc=%d, length=0x%08X, crc8=0x%02X\n", data->cmd, data->argc, len, (int)crc8);
#endif

	for (i = 0 ; i < data->argc ; ++i) {
		switch (data->argt[i]) {
			case 0x00:
				data->argv[i] = strsafe_frombin (ptr + 4, len = *(udword*)(ptr));
				ptr += len + 4;
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\tstring: length=0x%08X content=\"", len);
				p = (ubyte*)strsafe_ptr ((string)(data->argv[i]));
				if (len >= 0xFF)
					fputs ("<object is too large>", fp_kstp_log);
				else
					for (j = 0; j < len; ++j) {
						if (p[j] < 0x20 || p[j] > 0x7E) {
							if (j == 0 || (p[j-1] > 0x20 && p[j-1] < 0x7E))
								fprintf (fp_kstp_log, "\", %02X", p[j]);
							else
								fprintf (fp_kstp_log, ", %02X", p[j]);
						} else {
							if (j > 0 && (p[j-1] < 0x20 || p[j-1] > 0x7E))
								fprintf (fp_kstp_log, ", \"%c", p[j]);
							else
								fputc (p[j], fp_kstp_log);
						}
					}
				fputs ("\n", fp_kstp_log);
#endif
				break;
			case 0x01:
				VALIDATE_A ((len = *(udword*)ptr) != 4, KSTPERR_SOCKRECVFMT,
				            free (recvbuff); kstp_release (data));
				VALIDATE_A (!(data->argv[i] = malloc (len)), KSTPERR_OUTOFMEM,
				            free (recvbuff); kstp_release (data));
				memcpy (data->argv[i], ptr += 4, len);
				ptr += 4;
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\tint: %08X\n", *(sdword*)(data->argv[i]));
#endif
				break;
			case 0x02:
				VALIDATE_A (*(udword*)(ptr) != sizeof (rtbyte), KSTPERR_SOCKRECVFMT,
				            free (recvbuff); kstp_release (data));
				VALIDATE_A (!(data->argv[i] = malloc (len)), KSTPERR_OUTOFMEM,
				            free (recvbuff); kstp_release (data));
				memcpy (data->argv[i], ptr += 4, len);
				ptr += sizeof (rtbyte);
#ifdef _DEBUG
				fprintf (fp_kstp_log, "\treal: %f\n", *(rtbyte*)(data->argv[i]));
#endif
				break;
		}
	}
#ifdef _DEBUG
	fflush (fp_kstp_log);
#endif
	free (recvbuff);
	return (true);
}

bool kstp_get_string (packet* data, address_t argno, string* val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (data->argt[argno] != KSARGT_STRING, KSTPERR_ARGTYPE);
	VALIDATE (!val, KSTPERR_NULLPTR);
	*val = (string)(data->argv[argno]);
	return (true);
}

bool kstp_get_int (packet* data, address_t argno, sdword* val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (data->argt[argno] != KSARGT_INT, KSTPERR_ARGTYPE);
	VALIDATE (!val, KSTPERR_NULLPTR);
	*val = *(sdword *)(data->argv[argno]);
	return (true);
}

bool kstp_get_real (packet* data, address_t argno, rtbyte* val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (data->argt[argno] != KSARGT_REAL, KSTPERR_ARGTYPE);
	VALIDATE (!val, KSTPERR_NULLPTR);
	*val = *(rtbyte *)(data->argv[argno]);
	return (true);
}

bool kstp_set_string (packet* data, address_t argno, string val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (!data->argt || !data->argv, KSTPERR_NULLPTR);
	data->argt[argno] = KSARGT_STRING;
	if (!val)
		data->argv[argno] = strsafe_fromstr ("", 1);
	else {
		VALIDATE (!(data->argv[argno] = malloc (sizeof(struct string_st) + strsafe_len (val))), KSTPERR_OUTOFMEM);
		memcpy ((string)(data->argv[argno]), val, sizeof (struct string_st) + strsafe_len (val));
	}
	return (true);
}

bool kstp_set_int (packet* data, address_t argno, sdword val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (!data->argt || !data->argv, KSTPERR_NULLPTR);
	data->argt[argno] = KSARGT_INT;
	VALIDATE (!(data->argv[argno] = malloc (sizeof(sdword))), KSTPERR_OUTOFMEM);
	*(sdword *)(data->argv[argno]) = val;
	return (true);
}

bool kstp_set_real (packet* data, address_t argno, rtbyte val) {
	kstp_errno = 0;
	VALIDATE (!data, KSTPERR_NULLPTR);
	VALIDATE (argno >= data->argc, KSTPERR_ARGCOUNT);
	VALIDATE (!data->argt || !data->argv, KSTPERR_NULLPTR);
	data->argt[argno] = KSARGT_REAL;
	VALIDATE (!(data->argv[argno] = malloc (sizeof(rtbyte))), KSTPERR_OUTOFMEM);
	memcpy (data->argv[argno], &val, sizeof (rtbyte));
	return (true);
}

char* kstp_msg (error_t err) {
	address_t i = 0;
	struct errordef_st* e = kserr_messages;
	while (e[i].type != 0xFFFFFFFF && e[i].type != err) i++;
	return (e[i].message);
}
