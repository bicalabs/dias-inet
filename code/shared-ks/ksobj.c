/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions: objects broker subsystem                    *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 07.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Revised 11.01.2003 Maxim A. Orlovsky: added strict function arguments       *
*                     control and any kind of possible checks                  *
*  Extended 25.01.2003 Maxim A. Orlovsky: upgraded loading routine,            *
*                      added extract, delete, link and unlink routines         *
*  Modified 26.10.2004 Maxim A. Orlovsky: removed MySQL database and           *
*                      writen code for store and relation objects              *
*                                                                              *
*******************************************************************************/

#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <winsock2.h>

#include <sysconf.h>
#include <strsafex.h>
#include <libnodes.h>
#include "aimkstp.h"

#include "ks.h"
#include "ksobj.h"

store*		store_head = NULL;
relation*	relations_head = NULL;
object*		objects_head = NULL;
extern client*	clients_head;

bool ksobj_create (client* c, udword type, string path, uword perms, packet* pack) {
	void *data;
	store *stor;

	isclientf (c);
	notnullf (path, c);

	/* TODO: Temporary code. Relpace with apopriate one */
	data = ob_create (c, type, pack);
	ob_write (c, type, data, path, true);

	iffailedf (stor = (store*)ksmem_commit (c, sizeof (store)), c, "ksobj_open");
	if (!store_head)
		store_head = stor;
	else
		bdlist_insert (store_head, stor);
	stor->type = type;
	stor->perms = perms;
	stor->owner = c->uid;
	memcpy (stor->path, strsafe_ptr (path), min (strsafe_len (path), KS_OBJPATH_LEN - 2));
	stor->path[KS_OBJPATH_LEN - 1] = 0;

	return (true);
}

HOBJ ksobj_temp (client* c, udword type, packet* pack) {
	object* obj;
	hobject* hobj;

	isclientn (c);

	/* TODO: Temporary code. Relpace with apopriate one */
	iffailedn (obj = (object*)ksmem_commit (c, sizeof (object)), c, "ksobj_open");
	/* If object is not already opened by any of the clients */
	if (!objects_head)
		objects_head = obj;
	else
		bdlist_insert (objects_head, obj);

	/* Create object handle */
	ifn (!(hobj = (hobject*)ksmem_commit (c, sizeof (hobject))));
	hobj->owner = c;
	hobj->obj = obj;
	hobj->access = 0777;
	if (!c->hobj_head)
		c->hobj_head = hobj;
	else
		bdlist_insert (c->hobj_head, hobj);
	obj->refs = 1;

	obj->data = ob_create (c, type, pack);
	return (hobj);
}

bool ksobj_delete (client* c, string path) {
	store *stor;
	relation *rel;
	string str;

	isclientf (c);
	notnullf (path, c);

	/* If object is opened */
	isvalidf (!ksobj_objfrompath (c, path), c, KSERR_OBJOPENED);

	/* Reading object record from database */
	iffailedf (stor = _ksobj_store_find (c, path, true), c, "ksobj_delete");

	/* Checking all kind of permissions */
	isvalidf (kssec_access (stor->perms, c, OA_EXECUTE), c, KSERR_NOACCESS);
	for (rel = relations_head; rel; rel = rel->next)
		isvalidf (rel->child != stor, c, KSERR_OBJHASPARENTS);

	/* Removing childs */
	for (rel = relations_head; rel; ) {
		if (rel->parent == stor) {
			if (rel == relations_head)
				relations_head = rel->next;
			bdlist_remove (rel);

			ksobj_delete (c, str = strsafe_fromstr (rel->child->path, KS_OBJPATH_LEN));
			strsafe_free (str);

			ksmem_release (c, rel);
			rel = relations_head;
		} else
			rel = rel->next;
	}

	/* Removing object and file */
	if (stor == store_head)
		store_head = stor->next;
	bdlist_remove (stor);
	ksmem_release (c, stor);
	iffailedf (!remove (strsafe_ptr (path)), c, "ksobj_delete: remove syscall error");

	return (true);
}

bool ksobj_link (client* c, HOBJ parent, string child) {
	store* stor;
	relation* rel;

	isclientf (c);
	notnullf (parent, c);
	iffailedf (kssec_validate_hobj (c, parent, OA_EXECUTE), c, "unable to link object");
	isownerf (c, parent);
	notnullf (child, c);

	/* If object is opened */
	isvalidf (!ksobj_objfrompath (c, child), c, KSERR_OBJOPENED);

	/* Reading object record from database */
	iffailedf (stor = _ksobj_store_find (c, child, true), c, "ksobj_link");

	/* Checking all kind of permissions */
	isvalidf (kssec_access (stor->perms, c, OA_EXECUTE), c, KSERR_NOACCESS);

	/* Adding reference on parent */
	iffailedf (ob_link (c, parent->obj->type, stor->type, parent->obj->data, child), c, "ksobj_link");

	/* Updating objects database */
	iffailedf (rel = (relation*)ksmem_commit (c, sizeof (relation)), c, "ksobj_link");
	rel->child = stor;
	rel->parent = parent->obj->stor;
	if (!relations_head)
		relations_head = rel;
	else
		bdlist_add (relations_head, rel);

	return (true);
}

bool ksobj_unlink (client* c, HOBJ parent, string child) {
	store* stor;
	relation* rel;

	isclientf (c);
	notnullf (parent, c);
	iffailedf (kssec_validate_hobj (c, parent, OA_EXECUTE), c, "unable to unlink object");
	isownerf (c, parent);
	notnullf (child, c);

	/* If object is opened */
	isvalidf (!ksobj_objfrompath (c, child), c, KSERR_OBJOPENED);

	/* Reading object record from database */
	iffailedf (stor = _ksobj_store_find (c, child, true), c, "ksobj_unlink");

	/* Checking all kind of permissions */
	isvalidf (kssec_access (stor->perms, c, OA_EXECUTE), c, KSERR_NOACCESS);
// ???	isvalidf (stor->parents != 0, c, KSERR_OBJNOTPARENT);

	/* Removing reference on parent */
	iffailedf (ob_unlink (c, parent->obj->type, stor->type, parent->obj->data, child), c, "ksobj_unlink");

	/* Updating objects database */
	for (rel = relations_head; rel; rel = rel->next) {
		if (rel->child == stor && rel->parent == parent->obj->stor) {
			if (rel == relations_head)
				relations_head = rel->next;
			bdlist_remove (rel);
			ksmem_release (c, rel);
		}
	}

	return (true);
}

HOBJ ksobj_open (client* c, string path, flags_t access, flags_t share) {
	store* stor;
	object* obj;
	hobject* hobj;
	flags_t	old_share, old_access;

	isclientn (c);
	notnulln (path, c);

	/* TODO: modify all object structure fields (acces, creating time, parents et al.) */
	/* If object is not already opened by any of the clients */
	if (!(obj = ksobj_objfrompath (c, path))) {
		/* TODO: add all fields to query */
		iffailedn (obj = (object*)ksmem_commit (c, sizeof (object)), c, "ksobj_open");
		memset (obj, 0, sizeof (object));
		iffailed (stor = _ksobj_store_find (c, path, true), c, "ksobj_open") fail
			ksmem_release (c, obj);
			return (NULL);
		otherwise;
		memcpy (obj->path, strsafe_ptr (path), strsafe_len (path));
		obj->type	= stor->type;
		obj->attr	= stor->attr;
		obj->perms	= stor->perms;
		obj->owner	= stor->owner;
		obj->created	= stor->created;
		obj->modified	= stor->modified;
		obj->accessed	= time (NULL);
		obj->stor	= stor;

		/* Checking permissions */
		isvalidn (kssec_access (obj->perms, c, access), c, KSERR_NOACCESS);

		/* Query and read ACLs table, open object's file and read object's data */
		if (!(obj->data = ob_alloc (c, obj->type)) ||
		    !ob_read (c, obj->type, obj->data, path)) {
			ksmem_release (c, obj);
			return (NULL);
		}
		obj->refs = 0;
		obj->share = share;
		if (!objects_head)
			objects_head = obj;
		else
			bdlist_insert (objects_head, obj);
	} else {
		/* If this client can't access object by object ACL */
		isvalidn (kssec_access (obj->perms, c, access), c, KSERR_NOACCESS);
		/* If object share doesn't permit to open object for requested access */
		isvalidn ((obj->share & access) <= 0, c, KSERR_SHARECONFLICT);
		/* If object share is less strong that the sugested one try to update it */
		old_share = obj->share;
		if (obj->share & share ^ share) /* TODO: revise */
			obj->share |= share;
		/* If the handle to object is already opened by client */
		isvalid (hobj = ksobj_hfromobj (c, obj), c, KSERR_OPENED) fail
			/* If handle access flags is less strong than the suggested ones try to update it */
			old_access = hobj->access;
			if (hobj->access & access ^ access) /* TODO: revise */
				hobj->access |= access;
			obj->refs++;
			return (hobj); /* SUCCEED */
		otherwise;
	}
	/* Create object handle */
	ifn (!(hobj = (hobject*)ksmem_commit (c, sizeof (hobject))));
	hobj->owner = c;
	hobj->obj = obj;
	hobj->access = access;
	if (!c->hobj_head)
		c->hobj_head = hobj;
	else
		bdlist_insert (c->hobj_head, hobj);
	obj->refs++;
	return (hobj);
}

bool ksobj_close (client* c, HOBJ h) {
	store* stor;
	string str;

	isclientf (c);
	notnullf (h, c);
	iffailedf (kssec_validate_hobj (c, h, OA_READ), c, "ksobj_close");
	isownerf (c, h);

	if (!--h->obj->refs) {
		iffailedf (stor = _ksobj_store_find (c, str = strsafe_fromstr (h->obj->path, KS_OBJPATH_LEN), true), c, "ksobj_clode");
		strsafe_free (str);
		stor->accessed = h->obj->accessed;
		ob_free (c, h->obj->type, h->obj->data);
		if (objects_head == h->obj) {
			objects_head = objects_head->next;
			if (objects_head)
				objects_head->prev = NULL;
		} else
			bdlist_remove (h->obj);
		ksmem_release (c, h->obj);
	}
	if (c->hobj_head == h) {
		c->hobj_head = c->hobj_head->next;
		if (c->hobj_head)
			c->hobj_head->prev = NULL;
	} else
		bdlist_remove (h);
	ksmem_release (c, h);
	return (true);
}

bool ksobj_save (client* c, HOBJ h) {
	store* stor;
	string str;

	isclientf (c);
	notnullf (h, c);
	iffailedf (kssec_validate_hobj (c, h, OA_READ), c, "ksobj_save");
	isownerf (c, h);

	ob_write (c, h->obj->type, h->obj->data, str = strsafe_fromstr (h->obj->path, 0x100), false);
	strsafe_free (str);

	iffailedf (stor = _ksobj_store_find (c, str = strsafe_fromstr (h->obj->path, KS_OBJPATH_LEN), true), c, "ksobj_clode");
	strsafe_free (str);
	stor->modified = time (NULL);
	return (true);
}

bool ksobj_load (client* c, udword type, udword format, string file, string path, uword perms) {
	store* stor;

	isclientf (c);
	notnullf (path && file, c);

	isvalidf (!(stor = _ksobj_store_find (c, path, false)), c, KSERR_OBJEXISTS);

	iffailedf (stor = (store*)ksmem_commit (c, sizeof (store)), c, "ksobj_open");
	if (!store_head)
		store_head = stor;
	else
		bdlist_insert (store_head, stor);
	stor->type = type;
	stor->perms = perms;
	stor->owner = c->uid;
	memcpy (stor->path, strsafe_ptr (path), min (strsafe_len (path), KS_OBJPATH_LEN - 2));
	stor->path[KS_OBJPATH_LEN - 1] = 0;

	iffailed (ob_load (c, type, format, file, path, perms), c, "error loading hested objects") fail
		if (store_head == stor)
			store_head = NULL;
		else
			bdlist_remove (stor);
	otherwise;

	return (true);
}

bool ksobj_extract (client* c, udword format, string file, string path) {
	store* stor;

	isclientf (c);
	notnullf (path && file, c);

	iffailedf (stor = _ksobj_store_find (c, path, true), c, "ksobj_clode");
	isvalidf (kssec_access (stor->perms, c, OA_EXECUTE), c, KSERR_NOACCESS);

	iff (!ob_extract (c, stor->type, format, file, path));
	return (true);
}

bool ksobj_access (client* c, HOBJ h, flags_t new_access) {
	return (true);
}

bool ksobj_share (client* c, HOBJ h, flags_t new_share) {
	return (true);
}

object* ksobj_objfrompath (client* c, string path) {
	object* obj;

	isclientf (c);
	notnullf (path, c);

	for (obj = objects_head; obj; obj = obj->next)
		if (strsafe_len (path) == strlen (obj->path) && !memcmp (obj->path, strsafe_ptr (path), strsafe_len (path)))
			return (obj);
	return (NULL);
}

hobject* ksobj_hfromobj (client* c, object* obj) {
	hobject* hobj;

	isclientf (c);
	notnullf (obj, c);

	for (hobj = c->hobj_head; hobj; hobj = obj->next)
		if (hobj->obj == obj)
			return (hobj);
	return (NULL);
}

bool _ksobj_store_read () {
	// Add code here
	return (true);
}

bool _ksobj_store_write () {
	// Add code here
	return (true);
}

store* _ksobj_store_find (client* c, string path, bool neerr) {
	store* stor;

	isclientf (c);
	notnullf (path, c);

	for (stor = store_head; stor; stor = stor->next)
		if (strsafe_len (path) == strlen (stor->path) && !memcmp (stor->path, strsafe_ptr (path), strsafe_len (path)))
			return (stor);
	return (NULL);
}
