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

#pragma once

#if defined _DEBUG
#	include <stdio.h>
	extern FILE* fp_kstp_log;
#endif

extern udword kstp_errno;

typedef udword error_t;

struct errordef_st {
	error_t		type;
	char		message[128];
};

extern struct errordef_st kserr_messages[];

#define	KSTPERR_SOCKRECV	0x2000000E
#define	KSTPERR_SOCKRECVDATA	0x2000000F
#define	KSTPERR_ARGCOUNT	0x20000010
#define	KSTPERR_ARGTYPE		0x20000011
#define	KSTPERR_NULLPTR		0x20000012
#define	KSTPERR_OUTOFMEM	0x20000017
#define	KSTPERR_SOCKRECVFMT	0x2000001B
#define	KSTPERR_LARGENETPACK	0x20000032
#define	KSTPERR_CRCLENGTH	0x20000033
#define	KSTPERR_CRCPACK		0x20000034
#define	KSTPERR_PACKRECVLEN	0x20000035


struct packet_st {
	ubyte		key[64];
	udword		cmd;
	address_t	argc;
	ubyte*		argt;
	void**		argv;
};

typedef struct packet_st packet;

bool	kstp_allocate (packet* data, address_t argno);
void	kstp_release (packet* data);
bool	kstp_send (socket_t s, packet* data);
bool	kstp_recv (socket_t s, packet* data);
bool	kstp_get_string (packet* data, address_t argno, struct string_st** val);
bool	kstp_get_int (packet* data, address_t argno, sdword* val);
bool	kstp_get_real (packet* data, address_t argno, rtbyte* val);
bool	kstp_set_string (packet* data, address_t argno, struct string_st* val);
bool	kstp_set_int (packet* data, address_t argno, sdword val);
bool	kstp_set_real (packet* data, address_t argno, rtbyte val);
char*	kstp_msg (error_t err);
