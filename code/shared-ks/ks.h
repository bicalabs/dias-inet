/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions                                              *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 04.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

#pragma once

#include "aim.h"

#define	isvalid(v,c,e)	if (!(v)) { kserr_fire (c, e, #v , __FILE__, __LINE__);
#define isclient(c)     if (!(c)) { kserr_fire (CLIENT_ROOT, KSERR_NOCLIENT, #c , __FILE__, __LINE__);
#define isowner(c,o)    if (o->owner != c) { kserr_fire (c, KSERR_NOTOWNER, #o , __FILE__, __LINE__);
#define notnull(v,c)	if (!(v)) { kserr_fire (c, KSERR_NULLPTR, #v , __FILE__, __LINE__);
#define iffailed(v,c,m)	if (!(v)) { kserr_fire (c, KSERR_OPFAILED, m, __FILE__, __LINE__);
#define	otherwise	}
#define	fail

#define	ifn(v)		if (v) return (NULL);
#define isvalidn(v,c,e) isvalid(v,c,e) return  (NULL); }
#define isclientn(c)    isclient(c) return  (NULL); }
#define isownern(c,o)   isowner(c,o) return  (NULL); }
#define notnulln(v,c)   notnull(v,c) return  (NULL); }
#define iffailedn(v,c,m) iffailed(v,c,m) return  (NULL); }

#define	iff(v)		if (v) return (false);
#define isvalidf(v,c,e) isvalid(v,c,e) return  (false); }
#define isclientf(c)    isclient(c) return  (false); }
#define isownerf(c,o)   isowner(c,o) return  (false); }
#define notnullf(v,c)   notnull(v,c) return  (false); }
#define iffailedf(v,c,m) iffailed(v,c,m) return  (false); }

#define	ifm(v)		if (v) return (-1);
#define isvalidm(v,c,e) isvalid(v,c,e) return  (-1); }
#define isclientm(c)    isclient(c) return  (-1); }
#define isownerm(c,o)   isowner(c,o) return  (-1); }
#define notnullm(v,c)   notnull(v,c) return  (-1); }
#define iffailedm(v,c,m) iffailed(v,c,m) return  (-1); }

#define	ifj(v,j)	if (v) goto j;
#define isvalidj(v,c,e,j) isvalid(v,c,e) goto j; }
#define isclientj(c,j)	isclient(c) goto j; }
#define isownerj(c,o,j)	isowner(c,o) goto j; }
#define notnullj(v,c,j)	notnull(v,c) goto j; }
#define iffailedj(v,c,m,j) iffailed(v,c,m) goto j; }

#define	ifb(v)		if (v) break;
#define isvalidb(v,c,e)	isvalid(v,c,e) break; }
#define isclientb(c)	isclient(c) break; }
#define isownerb(c,o)	isowner(c,o) break; }
#define notnullb(v,c)	notnull(v,c) break; }
#define iffailedb(v,c,m) iffailed(v,c,m) break; }

#define atomic(a)
#define endatomic
#define atomicall

struct client_st;
struct memunit_st;
struct secacl_st;
struct secacl_list_st;
struct store_st;
struct relation_st;
struct object_st;
struct hobject_st;
struct hobject_list_st;
struct error_st;
typedef struct client_st client;
typedef struct memunit_st memunit;
typedef struct secacl_st secacl;
typedef struct secacl_list_st secacl_list;
typedef struct store_st store;
typedef struct relation_st relation;
typedef struct object_st object;
typedef struct hobject_st hobject;
typedef struct hobject_st* HOBJ;
typedef struct hobject_list_st hobject_list;
typedef struct error_st error;

extern char*		conf_socket_ip;
extern uword		conf_socket_port;
extern uvar32_64	conf_socket_clients;
extern uvar32_64	conf_socket_threads;
extern char*		conf_log_error;
extern char*		conf_log_access;
extern char*		conf_log_clients;
extern char*		conf_log_memory;
extern char*		conf_log_kstp;
extern uvar32_64	conf_mem_pages_maxfree;
extern uvar32_64	conf_mem_pages_minfree;
extern uvar32_64	conf_client_disc_int;


/*** SECURITY ***/

bool	kssec_authentificate (string user, string passwd, client* c);
client*	kssec_identify (uchar* key);
bool	kssec_client_init (client* c);
void	kssec_client_release (client* c);
uchar*	kssec_generatekey (client* c);
bool	kssec_acl_read (object* obj, uchar* aclptr);
bool	kssec_access (uword perms, client* c, flags_t access);
bool	kssec_validate_rights (client* c, secacl* acl);
bool    kssec_validate_hobj (client* c, hobject* hobj, flags_t access);
void	kssec_cleanup ();

/*** OBJECTS ***/

/* Object types */
#define OT_STORAGE      0x00
#define   OT_IMAGE      0x01
#define     OT_MARKERS  0x03
#define     OT_OVERLAYS 0x05
#define     OT_LAYERS   0x09
#define       OT_MASKS  0x19
#define   OT_DATA       0x10
#define OT_GEOMSCALE    0x20
#define OT_DENSSCALE    0x40

/* Object access rights */
#define	OA_READ		0x01
#define	OA_EXECUTE	0x02
#define	OA_WRITE	0x04
#define	OA_RDWR		(OA_READ | OA_WRITE)
#define	OA_RDEX		(OA_READ | OA_EXECUTE)
#define	OA_ALL		(OA_READ | OA_WRITE | OA_EXECUTE)

/* Object share access permissions */
#define	OSA_NOT_READ	0x01
#define	OSA_NOT_EXECUTE	0x02
#define	OSA_NOT_WRITE	0x04
#define	OSA_NOT_RW	(OSA_NOT_READ | OSA_NOT_WRITE)
#define	OSA_EXCLUSIVE	(OSA_NOT_READ | OSA_NOT_WRITE | OSA_NOT_EXECUTE)

/* Objects attributes */
#define	OAT_TEMPORARY	0x0001

/* TODO: Change created, modified and accessed fields every time when needed */
struct object_st {
	/* This part is not stored in objects database */
        object*		prev;
        object*		next;
	/* This part is stored in objects database */
        udword		type;
	uword		attr;
        uword		perms;
	udword		owner;
	time_t		created;
	time_t		modified;
	time_t		accessed;
	char		path[KS_OBJPATH_LEN];
	/* This part is not stored in objects database */
	uword		share;
	uvar32_64	refs;
	uvar32_64	parents;
	store*		stor;
	void*		data;
};

struct store_st {
        store*		prev;
        store*		next;
        udword		type;
	uword		attr;
        uword		perms;
	udword		owner;
	time_t		created;
	time_t		modified;
	time_t		accessed;
	char		path[KS_OBJPATH_LEN];
};

struct relation_st {
        relation*	prev;
        relation*	next;
	store*		parent;
	store*		child;
};

struct hobject_st {
	hobject*	prev;
	hobject*	next;
        client*		owner;
        object*		obj;
        flags_t		access;
};

struct hobject_list_st {
	hobject_list*	prev;
	hobject_list*	next;
	HOBJ		handle;
};

bool	ksobj_load (client* c, udword type, udword format, string file, string path, uword perms);
bool	ksobj_extract (client* c, udword format, string file, string path);
bool	ksobj_link (client* c, HOBJ parent, string child);
bool	ksobj_unlink (client* c, HOBJ parent, string child);
bool	ksobj_create (client* c, udword type, string path, uword perms, packet* pack);
bool	ksobj_delete (client* c, string path);
HOBJ	ksobj_open (client* c, string path, flags_t access, flags_t share);
HOBJ	ksobj_temp (client* c, udword type, packet* pack);
bool	ksobj_close (client* c, HOBJ h);
bool	ksobj_save (client* c, HOBJ h);

object*	ksobj_objfrompath (client* c, string path);
hobject* ksobj_hfromobj (client* c, object* obj);

void*	ob_alloc (client* c, udword type);
void*	ob_create (client* c, udword type, packet* pack);
bool    ob_free (client* c, udword type, void* ptr);
bool	ob_link (client* c, udword parent_type, udword child_type, void* parent_data, string child);
bool	ob_unlink (client* c, udword parent_type, udword child_type, void* parent_data, string child);
bool	ob_load (client* c, udword type, udword format, string src_file, string obj_file, uword perms);
bool	ob_extract (client* c, udword type, udword format, string src_file, string obj_file);
bool	ob_read (client* c, udword type, void* data_ptr, string obj_file);
bool	ob_write (client* c, udword type, void* data_ptr, string obj_file, bool release);

/*** ERRORS ***/

typedef udword error_t;

struct error_st {
	error_t		err;
	char		func[KS_ERRFUNC_LEN];
	char		file[KS_ERRFILE_LEN];
	int		line;
};

bool	kserr_init (uchar* error_log, uchar* std_log);
void	kserr_stop ();
void	kserr_fire (client* client, error_t error, char* func, char* file, int line);
error*	kserr_get (client* client, uword offset);
error*	kserr_last (client* client);
char*	kserr_msg (error_t error);

/*** CLIENT ***/

#define	CLIENT_ROOT &client_root
extern client client_root;
extern client* clients_head;

typedef struct client_st {
	client*		prev;
	client*		next;

	ubyte		key[64];
	uchar		ip[16];

	udword		uid;
	ubyte 		gcnt;
	udword*		gids;

	memunit*	mem_tail;
	uvar32_64	mem_used;
	uvar32_64	mem_quot;

	uvar32_64	cpu_time;
	uvar32_64	cpu_quot;

	uword		err_start;
	uword		err_finish;
	error		err_cont[KS_ERRCONT_LEN];

	hobject*	hobj_head;

	uvar32_64	last_query_tm;
} client;

/*** MEMORY ***/

bool	ksmem_init ();
void	ksmem_stop ();
void*	ksmem_commit (client* c, address_t size);
void*	ksmem_resize (client* c, void* ptr, address_t new_size);
bool	ksmem_release (client* c, void* ptr);
bool	ksmem_release_all (client* c);
int		ksmem_configure (client* c, int cmd, svar32_64 param);
int		ksmem_test (client *c, int cmd, svar32_64 param);

/*** SOCKETS ***/

bool	ks_open_socket ();
void	ks_close_socket ();
bool	ks_session ();
bool	ks_proc (packet* query, packet* resp);

bool	ks_allocate_resp_args (packet* data, address_t argsno);
void	ks_lasterr_resp (packet* resp, client* c);

bool	cm_proc (client* c, packet* query, packet* resp);
