/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions: security controller subsystem               *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 07.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Revised 11.01.2003 Maxim A. Orlovsky: added strict function arguments       *
*                     control and any kind of possible checks                  *
*                                                                              *
*******************************************************************************/

#include <winsock2.h>

#include <sysconf.h>
#include <time.h>
#include <strsafex.h>
#include <libnodes.h>
#include "aimkstp.h"

#include "ks.h"
#include "kssec.h"

uvar32_64 _kssec_uid = 100;

bool kssec_authentificate (string user, string passwd, client* c) {
	return (true);
}

client* kssec_identify (uchar* key) {
	client* c;
	for (c = clients_head; c != NULL ; c = c->next)
		if (memcmp (c->key, key, sizeof (c->key)) != 0) break;
	if (c)
		time (&c->last_query_tm);
	return (c);
/*	if (clients_head && clients_head->next) return (clients_head->next);
	if (clients_head) return (clients_head);
	return (CLIENT_ROOT);*/
}

bool kssec_client_init (client* c) {
	notnullf (c, CLIENT_ROOT);

	memset (c, 0, sizeof (client));
	/* TODO: Read quottas from user database */
	if (_kssec_uid == 0) _kssec_uid = 100;
	c->uid = _kssec_uid++;
	c->mem_quot = (uvar32_64)(-1);
	c->cpu_quot = (uvar32_64)(-1);
	if (!clients_head)
		clients_head = c;
	else
		bdlist_insert (clients_head, c);
	kserr_fire (CLIENT_ROOT, KSERR_NEWCLIENT, "kssec_client_init", __FILE__, __LINE__);
	kserr_fire (c, KSERR_NEWCLIENT, "kssec_client_init", __FILE__, __LINE__);
	return (true);
}

void kssec_client_release (client* c) {
	hobject* hobj;
	notnull (c, CLIENT_ROOT) fail
		return;
	otherwise;
	if (c == CLIENT_ROOT)
		return;
	bdlist_remove (c);
	for (hobj = c->hobj_head; hobj; hobj = hobj->next)
		ksobj_close (c, hobj);
	ksmem_release_all (c);
	return;
}

uchar* kssec_generatekey (client* c) {
	int rnd = rand ();
	time_t tm = time (NULL);
	notnulln (c, CLIENT_ROOT);
	memset (c->key, 0, sizeof (c->key));
	memcpy (c->key, &tm, sizeof (time_t));
	memcpy (c->key + sizeof (time_t), &rnd, sizeof (int));
	return (c->key);
}

bool kssec_acl_read (object* obj, uchar* aclptr) {
	return (true);
}

bool kssec_access (uword perms, client* c, flags_t access) {
	return (true);
}

bool kssec_validate_rights (client* c, secacl* acl) {
	return (true);
}

bool kssec_validate_hobj (client* c, hobject* hobj, flags_t access) {
	hobject* hop = NULL;

	isclientf (c);
	notnullf (hobj, c);

	for (hop = c->hobj_head; hop; hop = hop->next)
		if (hop == hobj) {
			isvalidf (hop->owner == c, c, KSERR_NOTOWNER);
			isvalidf ((hop->access & access) == access, c, KSERR_NOACCESS);
			return (true);
		}
	kserr_fire (c, KSERR_NOTHOBJ, "kssec_validate_hobj", __FILE__, __LINE__);
	return (false);
}

void kssec_cleanup () {
	client* c;
	time_t tm;
	time (&tm);
	for (c = clients_head; c != NULL ; c = c->next)
		if (difftime (tm, c->last_query_tm) > conf_client_disc_int)
			kssec_client_release (c);
}
