/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service object broker subsystem                                 *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Rewrited 25.01.2003 Maxim A. Orlovsky: added stepwise object processing     *
*                      algorithm, function lookup tables,                      *
*                      link, unlink, extract routine, storage object           *
*                      KONTRON sequences support                               *
*  Extended 05.03.2003 Maxim A. Orlovsky: added mask and survey creating,      *
*		       writing, reading, linking and unlinking procs                   *
*                                                                              *
*******************************************************************************/

#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sysconf.h>
#include <libnodes.h>
#include <strsafex.h>
#include "dias-ks.h"
#include "broker.h"

extern relation* relations_head;

void* ob_alloc (client* c, udword type) {
	dksobj* data;

	isclientn (c);
	iffailedn (data = ksmem_commit (c, sizeof(dksobj)), c, "ob_alloc");
	return (data);
}

void* ob_create (client* c, udword type, packet* pack) {
	dksobj* data;

	isclientn (c);
	iffailedn (data = ksmem_commit (c, sizeof(dksobj)), c, "ob_create");
	isvalidn (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidn (_ob_create_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	iffailedn (_ob_create_table[type] (c, data, pack), c, "ob_free");
	return (data);
}

bool ob_free (client* c, udword type, void* ptr) {
	isclientf (c);
	notnullf (ptr, c);
	isvalidf (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidf (_ob_free_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	iffailedf (_ob_free_table[type] (c, ptr), c, "ob_free");
	return (ksmem_release(c, ptr));
}

bool ob_load (client* c, udword type, udword format, string src_file, string obj_file, uword perms) {
	int f_src;
	dksobj* data;
	bool retval = true;

	isclientf (c);
	notnullf (src_file, c);
	notnullf (obj_file, c);

	isvalidf (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidf (_ob_load_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	iffailedf (data = ksmem_commit (c, sizeof(dksobj)), c, "ob_load");

	data->orig_format = format;
	data->orig_qlen = _ob_parse_path (src_file, data->orig_query);
	iffailedf (data->orig_qlen != -1, c, "ob_load");
	if (strsafe_len (src_file) >= DIASKS_ORIGFILE_LENGTH) {
		memcpy (data->orig_file, strsafe_ptr (src_file), DIASKS_ORIGFILE_LENGTH - 1);
		data->orig_file[DIASKS_ORIGFILE_LENGTH - 1] = '\0';
	} else
		strcpy (data->orig_file, strsafe_ptr (src_file)); 

	isvalidf ((f_src = open (strsafe_ptr (src_file), _O_RDONLY | _O_BINARY)) != -1, c, KSERR_SRCFILEOPEN);
	retval &= _ob_load_table[type] (c, format, data, f_src, obj_file, perms);
	close (f_src);

	if (retval)
		retval |= ob_write (c, type, data, obj_file, true);
	else
		ob_free (c, type, data);

	return (retval);
}

bool ob_extract (client* c, udword type, udword format, string src_file, string obj_file) {
	int f_src;
	dksobj* data;
	bool retval = true;

	isclientf (c);
	notnullf (src_file, c);
	notnullf (obj_file, c);

	isvalidf (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidf (_ob_extract_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);

	iffailedf (data = ksmem_commit (c, sizeof(dksobj)), c, "ob_extract");
	iffailedf (ob_read (c, type, data, obj_file), c, "ob_extract");

	isvalidf ((f_src = open (strsafe_ptr (src_file), _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY, S_IWRITE)) != -1, c, KSERR_SRCFILEOPEN);
	retval &= _ob_extract_table[type] (c, format, data, f_src, obj_file);
	close (f_src);

	ob_free (c, type, data);
	return (retval);
}

bool ob_link (client* c, udword parent_type, udword child_type, void* parent_data, string child) {
	dksobj* data = (dksobj*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	isvalidf (parent_type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidf (child_type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);

	isvalidf (_ob_link_table[parent_type] != NULL, c, KSERR_NOTIMPLEMENTED);
	return (_ob_link_table[parent_type] (c, child_type, data, child));
}

bool ob_unlink (client* c, udword parent_type, udword child_type, void* parent_data, string child) {
	dksobj* data = (dksobj*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	isvalidf (parent_type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);
	isvalidf (child_type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);

	isvalidf (_ob_unlink_table[parent_type] != NULL, c, KSERR_NOTIMPLEMENTED);
	return (_ob_unlink_table[parent_type] (c, child_type, data, child));
}

bool ob_read (client* c, udword type, void* data_ptr, string obj_file) {
	int	f_obj;
	bool	retval = true;
	dksobj*	data = (dksobj*)data_ptr;

	isclientf (c);
	notnullf (data && obj_file, c);
	isvalidf (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);

	isvalidf (_ob_free_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	isvalidf (_ob_read_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	_ob_free_table[type] (c, data);
	isvalidf ((f_obj = open (strsafe_ptr(obj_file), _O_RDONLY | _O_BINARY)) != -1, c, KSERR_OBJFILEOPEN);
	iffailedf (read (f_obj, data, sizeof (dksobj)) == sizeof (dksobj), c, "ob_read");
	retval &= _ob_read_table[type] (c, data, f_obj);

	close (f_obj);
	return (retval);
}

bool ob_write (client* c, udword type, void* data_ptr, string obj_file, bool release) {
	int	f_obj;
	bool	retval = true;
	dksobj*	data = (dksobj*)data_ptr;

	isclientf (c);
	notnullf (data && obj_file, c);
	isvalidf (type <= DOBJT__MAX, c, KSERR_UNKNOWNDOBJT);

	isvalidf (_ob_write_table[type] != NULL, c, KSERR_NOTIMPLEMENTED);
	/* TODO: create path to object file if nessesary */
	isvalidf ((f_obj = open (strsafe_ptr (obj_file), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, S_IWRITE)) != -1, c, KSERR_OBJFILEOPEN);
	iffailedf (write (f_obj, data, sizeof (dksobj)) == sizeof (dksobj), c, "ob_write");
	retval &= _ob_write_table[type] (c, data, f_obj);
	if (release)
		ob_free (c, type, data);

	close (f_obj);
	return (retval);
}

/* Parsing file name
   File name is represented in format
     /path/compound_file.ext?component=no&component=no
   example:
     n:/storages/hlth-colh-ic10#01@003.hthal.cck8.icc.dis?image=5&overlay=-1 */
sbyte _ob_parse_path (string origin, dks_query* query) {
	sbyte comp_cnt = 0;
	uvar32_64 file_len;
	char *file_ptr, *comp_ptr, *comp_item, *comp_div;

	if (origin == NULL || query == NULL)
		return (-1);

	/* Dividing path on two components: proper file name and query */
	file_ptr = strsafe_ptr (origin);
	file_len = strsafe_len (origin);
	if ((comp_ptr = strchr (file_ptr, '?')) != NULL &&
	    (address_t)comp_ptr < (address_t)file_ptr + file_len)
		*comp_ptr++ = '\0';
	else
		comp_ptr = NULL;

	/* Extracting query components */
	while (comp_ptr && comp_cnt < DIASKS_ORIGQUERY_LENGTH &&
	       (address_t)comp_ptr < (address_t)file_ptr + file_len) {
		if (comp_item = strchr (comp_ptr, '&'))
			*comp_item++ = '\0';
		if (comp_div = strchr (comp_ptr, '=')) {
			*comp_div++ = '\0';
			query[comp_cnt].value = atoi (comp_div);
		} else
			query[comp_cnt].value = -1;

		/* Checking for buffer overflow attack */
		if (strlen (comp_ptr) >= DIASKS_ORIGPARAM_LENGTH) {
			memcpy (query[comp_cnt].name, comp_ptr, DIASKS_ORIGPARAM_LENGTH - 1);
			query[comp_cnt].name[DIASKS_ORIGPARAM_LENGTH - 1] = '\0';
		} else
			strcpy (query[comp_cnt++].name, comp_ptr);
		comp_ptr = comp_item;
	}
	return (comp_cnt);
}

bool _ob_create_storage (client* c, dksobj* data_ptr, packet* pack) {
	dksobj_storage* data = (dksobj_storage*)data_ptr;
	udword dimx, dimy, dimz;

	isclientf (c);
	notnullf (data_ptr, c);
	notnullf (pack, c);

	iffailedf (kstp_get_int (pack, 3 + 0, &dimx), c, "_ob_create_storage: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 1, &dimy), c, "_ob_create_storage: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 2, &dimz), c, "_ob_create_storage: wrong arguments count");

	data->dimx = dimx;
	data->dimy = dimy;
	data->dimz = dimz;

	return (true);
}

bool _ob_create_image (client* c, dksobj* data_ptr, packet* pack) {
	dksobj_image* data = (dksobj_image*)data_ptr;
	udword dimx, dimy, dimz, color, l;
	uvar32_64 size;

	isclientf (c);
	notnullf (data_ptr, c);
	notnullf (pack, c);

	iffailedf (kstp_get_int (pack, 3 + 0, &dimx), c, "_ob_create_image: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 1, &dimy), c, "_ob_create_image: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 2, &dimz), c, "_ob_create_image: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 3, &color), c, "_ob_create_image: wrong arguments count");

	data->dimx = dimx;
	data->dimy = dimy;
	data->dimz = dimz;
	data->layers = ksmem_commit (c, sizeof(ubyte*) * dimz);
	size = dimx * dimy;
	for (l = 0; l < dimz; ++l) {
		data->layers[l] = ksmem_commit (c, size);
		memset (data->layers[l], color, size);
	}

	return (true);
}

bool _ob_create_mask (client* c, dksobj* data_ptr, packet* pack) {
	dksobj_mask* data = (dksobj_mask*)data_ptr;
	udword img, layer;
	ubyte *p, *colmap;
	uvar32_64 *sizemap, cols, i, n, d, size;
	dksobj_image* image;

	isclientf (c);
	notnullf (data_ptr, c);
	notnullf (pack, c);

	iffailedf (kstp_get_int (pack, 3 + 0, &img), c, "_ob_create_mask: wrong arguments count");
	iffailedf (kstp_get_int (pack, 3 + 1, &layer), c, "_ob_create_mask: wrong arguments count");
	notnullf (img, c);
	iff (!kssec_validate_hobj (c, (HOBJ)img, OA_READ));
	image = (dksobj_image*)((HOBJ)img)->obj->data;
	iffailedf (layer < image->dimz, c, "_ob_create_mask: wrong layer no");

	data->dimx = image->dimx;
	data->dimy = image->dimy;
	size = data->dimx * data->dimy;

	iffailedf (colmap = ksmem_commit (c, min (size, 0xFFFF) * sizeof (ubyte)), c, "_ob_create_mask");
	iffailedf (sizemap = ksmem_commit (c, min (size, 0xFFFF) * sizeof (uvar32_64)), c, "_ob_create_mask");

	cols = 0;
	p = image->layers[layer];
	for (i = 0; i < size; ++i, ++p) {
		for (n = 0; n < cols; ++n)
			if (*p == colmap[n]) {
				sizemap[n]++;
				break;
			}
		if (n == cols && *p != 0x00) {
			colmap[cols] = *p;
			sizemap[cols++] = 1;
		}
	}

	iffailedf (data->cont = ksmem_commit (c, cols * sizeof (uvar32_64*)), c, "_ob_create_mask");
	iffailedf (data->perim = ksmem_commit (c, cols * sizeof (uvar32_64*)), c, "_ob_create_mask");
	iffailedf (data->cont_sizes = ksmem_commit (c, cols * sizeof (uvar32_64*)), c, "_ob_create_mask");
	iffailedf (data->perim_sizes = ksmem_commit (c, cols * sizeof (uvar32_64*)), c, "_ob_create_mask");
	for (n = 0; n < cols; ++n) {
		iffailedf (data->cont[n] = ksmem_commit (c, sizemap[n] * sizeof (uvar32_64)), c, "_ob_create_mask");
		sizemap[n] = 0;
	}

	p = image->layers[layer];
	for (i = 0; i < size; ++i, ++p) {
		for (n = 0; n < cols; ++n)
			if (*p == colmap[n]) {
				data->cont[n][data->cont_sizes[n]++] = i;
				d = (uvar32_64)p;
				if (d % data->dimx == 0 || d % data->dimx == data->dimx - 1 ||
				    d / data->dimx == 0 || d / data->dimx == data->dimy - 1 ||
				    *(p - data->dimx) != colmap[n] || *(p + data->dimx) != colmap[n] ||
				    *(p - 1) != colmap[n] || *(p + 1) != colmap[n])
					sizemap[n]++;
				break;
			}
	}

	memcpy (data->perim_sizes, sizemap, cols * sizeof(uvar32_64));
	for (n = 0; n < cols; ++n) {
		iffailedf (data->perim[n] = ksmem_commit (c, sizemap[n] * sizeof (uvar32_64)), c, "_ob_create_mask");
		sizemap[n] = 0;
	}

	p = image->layers[layer];
	for (i = 0; i < size; ++i, ++p) {
		for (n = 0; n < cols; ++n) {
			d = (uvar32_64)p;
			if (*p == colmap[n] && (
			     d % data->dimx == 0 || d % data->dimx == data->dimx - 1 ||
			     d / data->dimx == 0 || d / data->dimx == data->dimy - 1 ||
			     *(p - data->dimx) != colmap[n] || *(p + data->dimx) != colmap[n] ||
			     *(p - 1) != colmap[n] || *(p + 1) != colmap[n])) {
				data->perim[n][sizemap[n]++] = i;
				break;
			}
		}
	}

	data->count = cols;

	ksmem_release (c, colmap);
	ksmem_release (c, sizemap);
	return (true);
}

bool _ob_create_survey (client* c, dksobj* data_ptr, packet* pack) {
	dksobj_survey* data = (dksobj_survey*)data_ptr;

	isclientf (c);
	notnullf (data_ptr, c);
	notnullf (pack, c);

	return (true);
}


bool _ob_free_storage (client* c, void* data_ptr) {
	names *img, *next;
	dksobj_storage* data = (dksobj_storage*)data_ptr;

	isclientf (c);
	if (!data) return (true);

	for (img = data->img_head; img; img = next) {
		next = img->next;
		bdlist_remove (img);
		ksmem_release (c, img);
	}	
	memset (data, 0, sizeof(dksobj));
	return (true);
}

bool _ob_free_image (client* c, void* data_ptr) {
	uvar32_64 l;
	dksobj_image* data = (dksobj_image*)data_ptr;

	isclientf (c);
	if (!data) return (true);
	if (!data->layers)  return (true);

	for (l = 0; l < data->dimz; ++l)
		ksmem_release (c, data->layers[l]);
	ksmem_release (c, data->layers);
	memset (data, 0, sizeof(dksobj));
	return (true);
}

bool _ob_free_mask (client* c, void* data_ptr) {
	uvar32_64 n;
	dksobj_mask* data = (dksobj_mask*)data_ptr;

	isclientf (c);
	if (!data) return (true);

	ksmem_release (c, data->cont_sizes);
	ksmem_release (c, data->perim_sizes);
	if (data->cont)
		for (n = 0; n < data->count; ++n) {
			ksmem_release (c, data->cont[n]);
			ksmem_release (c, data->perim[n]);
		}
	ksmem_release (c, data->cont);
	ksmem_release (c, data->perim);
	memset (data, 0, sizeof(dksobj));
	return (true);
}

bool _ob_free_survey (client* c, void* data_ptr) {
	dksobj_survey* data = (dksobj_survey*)data_ptr;
	record *rec, *next;

	isclientf (c);
	if (!data) return (true);

	for (rec = data->rec_head; rec; rec = next) {
		next = rec->next;
		bdlist_remove (rec);
		ksmem_release (c, rec);
	}
	memset (data, 0, sizeof(dksobj));
	return (true);
}

bool _ob_link_storage (client* c, udword child_type, void* parent_data, string child) {
	names *item;
	dksobj_storage* data = (dksobj_storage*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	iffailedf (strsafe_len (child) < DIASKS_OBJNAME_LENGTH, c, "_ob_link_storage: child name is too long");
	isvalidf (child_type == DOBJT_IMAGE || child_type == DOBJT_SURVEY, c, KSERR_UNKNOWNDOBJT);

	iffailedf (item = ksmem_commit (c, sizeof (names)), c, "_ob_link_storage");
	bdlist_init (item);

	strcpy (item->name, strsafe_ptr(child));
	if (child_type == DOBJT_IMAGE) {
		if (data->img_head == NULL)
			data->img_head = item;
		else
			bdlist_add (data->img_head, item);
		data->img_count++;
	}
	else if (child_type == DOBJT_STORAGE) {
		if (data->svr_head == NULL)
			data->svr_head = item;
		else
			bdlist_add (data->svr_head, item);
		data->svr_count++;
	}

	return (true);
}

bool _ob_link_image (client* c, udword child_type, void* parent_data, string child) {
	names *item;
	dksobj_image* data = (dksobj_image*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	iffailedf (strsafe_len (child) < DIASKS_OBJNAME_LENGTH, c, "_ob_link_image: child name is too long");
	/* TODO: Change next line when object will be ready */
	isvalidf (child_type == DOBJT_MASK, c, KSERR_UNKNOWNDOBJT);

	iffailedf (item = ksmem_commit (c, sizeof (names)), c, "_ob_link_image");
	bdlist_init (item);

	strcpy (item->name, strsafe_ptr(child));
	if (child_type == DOBJT_MASK) {
		if (data->msk_head == NULL)
			data->msk_head = item;
		else
			bdlist_add (data->msk_head, item);
		data->msk_count++;
	}

	return (true);
}

bool _ob_unlink_storage (client* c, udword child_type, void* parent_data, string child) {
	names* item;
	dksobj_storage* data = (dksobj_storage*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	isvalidf (child_type == DOBJT_IMAGE || child_type == DOBJT_SURVEY, c, KSERR_UNKNOWNDOBJT);

	if (child_type == DOBJT_IMAGE) {
		for (item = data->img_head; item; item = item->next)
			if (!strcmp (item->name, strsafe_ptr(child))) {
				if (data->img_head == item)
					data->img_head = item->next;
				bdlist_remove (item);
				data->img_count--;
				return (true);
			}
	}
	if (child_type == DOBJT_STORAGE) {
		for (item = data->svr_head; item; item = item->next)
			if (!strcmp (item->name, strsafe_ptr(child))) {
				if (data->svr_head == item)
					data->svr_head = item->next;
				bdlist_remove (item);
				data->svr_count--;
				return (true);
			}
	}

	return (false);
}

bool _ob_unlink_image (client* c, udword child_type, void* parent_data, string child) {
	names* item;
	dksobj_image* data = (dksobj_image*)parent_data;

	isclientf (c);
	notnullf (data && child, c);
	/* TODO: Change next line when object will be ready */
	isvalidf (child_type == DOBJT_MASK, c, KSERR_UNKNOWNDOBJT);

	if (child_type == DOBJT_MASK) {
		for (item = data->msk_head; item; item = item->next)
			if (!strcmp (item->name, strsafe_ptr(child))) {
				if (data->msk_head == item)
					data->msk_head = item->next;
				bdlist_remove (item);
				data->msk_count--;
				return (true);
			}
	}

	return (true);
}

bool _ob_extract_storage (client* c, udword format, dksobj* data_ptr, int f_src, string obj_file) {
	string str;
	names *item, *next;
	dksobj_image *image;
	dksobj_storage *data = (dksobj_storage*)data_ptr;

	uword header[0x40];

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_src != -1, c, "_ob_extract_storage");

	switch (format) {
		case DOBJF_STORAGE_KONTRON:
			memset (header, 0, 0x80);
			header[0] = 0x0001;
			header[1] = 0x1247;
			header[2] = 0xB06D;
			header[3] = (uword)(data->dimx);
			header[4] = (uword)(data->dimy);
			header[5] = 0x4321;
			header[6] = 0;
			header[10] = 0x0000;
			if (write (f_src, header, 0x80) != 0x80)
				return (false);
			for (item = data->img_head; item; item = next) {
				iffailedf (image = ksmem_commit (c, sizeof(dksobj)), c,
			                   "unable to extract nested object");
				iffailedf (ob_read (c, DOBJT_IMAGE, image,
				           str = strsafe_fromstr (item->name, DIASKS_OBJNAME_LENGTH)), c,
				           "unable to extract nested object");
				strsafe_free (str);
				iffailedf (
				  _ob_extract_image (c, DOBJF_IMAGE_KONTRON, image, f_src,
				                     str = strsafe_fromstr (item->name, DIASKS_OBJNAME_LENGTH)),
				  c, "error occured while extracting nested image");
				ob_free (c, DOBJT_IMAGE, image);
				strsafe_free (str);

				next = item->next;
				bdlist_remove (item);
				ksmem_release (c, item);
				data->img_count--;
			}
			data->img_head = NULL;
			return (true);
		case DOBJF_STORAGE_DIAS2:
		case DOBJF_STORAGE_DIAS3:
		case DOBJF_STORAGE_TIFF:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "_ob_extract_storage", __FILE__, __LINE__);
			return (false);
		default:
			kserr_fire (c, KSERR_UNKNOWNDOBJF, "_ob_extract_storage", __FILE__, __LINE__);
			return (false);
	}
	return (false);
}

bool _ob_extract_image (client* c, udword format, dksobj* data_ptr, int f_src, string obj_file) {
	dksobj_image *data = (dksobj_image*)data_ptr;

	uword header[0x20];

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_src != -1, c, "_ob_extract_image");

	switch (format) {
		case DOBJF_IMAGE_KONTRON:
			/* reading KONTRON .img file header header */
			isvalidf (lseek (f_src, 0, SEEK_SET) != -1, c, KSERR_WRONGSRCFILE);
			isvalidf (read (f_src, header, 0x20) == 0x20, c, KSERR_WRONGSRCFILE);
			/* checking KONTRON .img file header stamp */
			isvalidf (header[0] == 0x0001 &&
			          header[1] == 0x1247 &&
			          header[2] == 0xB06D &&
			          header[5] == 0x4321, c, KSERR_WRONGSRCFILE);
			iffailedf (header[3] == data->dimx && header[4] == data->dimy,
			           c, "_ob_extract_image: size mismatch");
			header[6]++;
			isvalidf (lseek (f_src, 0, SEEK_SET) != -1, c, KSERR_WRONGSRCFILE);
			isvalidf (write (f_src, header, 0x20) == 0x20, c, KSERR_WRONGSRCFILE);
			isvalidf (lseek (f_src, 0, SEEK_END) != -1, c, KSERR_WRONGSRCFILE);
			iffailedf (data->dimz != 0, c, "_ob_extract_image: size mismatch");
			isvalidf (write (f_src, data->layers[0], data->dimx * data->dimy),
			          c, KSERR_WRONGSRCFILE);
			//ksmem_release (c, data->layers[0]);
			//ksmem_release (c, data->layers);
			return (true);
		case DOBJF_IMAGE_DIAS2:
		case DOBJF_IMAGE_DIAS3:
		case DOBJF_IMAGE_BMP:
		case DOBJF_IMAGE_JPEG:
		case DOBJF_IMAGE_PNG:
		case DOBJF_IMAGE_TIFF:
		case DOBJF_IMAGE_TGA:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "_ob_extract_image", __FILE__, __LINE__);
			return (false);
		default:
			kserr_fire (c, KSERR_UNKNOWNDOBJF, "_ob_extract_image", __FILE__, __LINE__);
			return (false);
	}
	return (false);
}

bool _ob_extract_survey (client* c, udword format, dksobj* data_ptr, int f_src, string obj_file) {
	uvar32_64 n;
	record* rec;
	uword buff[0x100];
	dksobj_survey *data = (dksobj_survey*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_src != -1, c, "_ob_extract_survey");

	switch (format) {
		case DOBJF_SURVEY_TAB:
			write (f_src, _ob_str_surveyfields, strlen (_ob_str_surveyfields));
			for (n = 1, rec = data->rec_head; rec; rec = rec->next, ++n) {
				WRITE_INT (n);
				write (f_src, rec->src_image, strlen (rec->src_image));
				write (f_src, "\t", 1);
				write (f_src, rec->src_mask, strlen (rec->src_mask));
				write (f_src, "\t", 1);
				write (f_src, rec->src_geom, strlen (rec->src_geom));
				write (f_src, "\t", 1);
				write (f_src, rec->src_dens, strlen (rec->src_dens));
				write (f_src, "\t", 1);
				WRITE_INT (rec->src_object);

				WRITE_REAL (rec->apexx);
				WRITE_REAL (rec->apexy);
				WRITE_REAL (rec->top);
				WRITE_REAL (rec->bottom);
				WRITE_REAL (rec->left);
				WRITE_REAL (rec->right);

				WRITE_REAL (rec->area);
				WRITE_REAL (rec->perim);
				WRITE_REAL (rec->csc_area);
				WRITE_REAL (rec->csc_perim);
				WRITE_INT (rec->fragments);

				WRITE_REAL (rec->grav_areax);
				WRITE_REAL (rec->grav_areay);
				WRITE_REAL (rec->grav_perimx);
				WRITE_REAL (rec->grav_perimy);
				WRITE_REAL (rec->grav_greyx);
				WRITE_REAL (rec->grav_greyy);

				WRITE_REAL (rec->diam_max);
				WRITE_REAL (rec->diam_min);
				WRITE_REAL (rec->diam_mean);
				WRITE_REAL (rec->diam_var);
				WRITE_REAL (rec->diam_ort);
				WRITE_REAL (rec->diam_x);
				WRITE_REAL (rec->diam_y);
				WRITE_REAL (rec->angle_max);
				WRITE_REAL (rec->angle_min);
				WRITE_INT (rec->chords_count);
				WRITE_REAL (rec->chords_mean);
				WRITE_REAL (rec->chords_var);
				WRITE_REAL (rec->chords_min);
				WRITE_REAL (rec->chords_max);

				WRITE_REAL (rec->pixels);
				WRITE_REAL (rec->sum);
				WRITE_REAL (rec->sum2);
				WRITE_REAL (rec->mean);
				WRITE_REAL (rec->stddev);
				WRITE_REAL (rec->min);
				WRITE_REAL (rec->max);
				WRITE_REAL (rec->mod);
				WRITE_REAL (rec->med);
				write (f_src, "\x0D\x0A", 2);
			}
			return (true);
		case DOBJF_SURVEY_DBF:
		case DOBJF_SURVEY_EXCEL:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "_ob_extract_survey", __FILE__, __LINE__);
			return (false);
		default:
			kserr_fire (c, KSERR_UNKNOWNDOBJF, "_ob_extract_survey", __FILE__, __LINE__);
			return (false);
	}
	return (false);
}

bool _ob_load_storage (client* c, udword format, dksobj* data_ptr, int f_src, string obj_file, uword perms) {
	store* stor;
	relation* rel;
	object *pobj;
	hobject *phobj;
	string s1, s2;
	dksobj_storage *data = (dksobj_storage*)data_ptr;

	uvar32_64 l;
	uword header[0x10];
	string name, source;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_src != -1, c, "_ob_read_image");

	switch (format) {
		case DOBJF_STORAGE_KONTRON:
			isvalidf (data->orig_qlen == 0, c, KSERR_WRONGSRCPATH);
			/* reading KONTRON .img file header header */
			isvalidf (read (f_src, header, 0x20) == 0x20, c, KSERR_WRONGSRCFILE);
			/* checking KONTRON .img file header stamp */
			isvalidf (header[0] == 0x0001 &&
			          header[1] == 0x1247 &&
			          header[2] == 0xB06D &&
			          header[5] == 0x4321, c, KSERR_WRONGSRCFILE);
			/* filling structure fields */
			strcpy (data->name, "Sequence");
			strcpy (data->descr, "Imported KONTRON .img sequence");
			data->dimx = header[3];
			data->dimy = header[4];
			data->dimz = 1;
			/* reading source file */
			for (l = 0; l < header[6]; ++l) {
				source = strsafe_format2 ("%s?image=%s",
					  s1 = strsafe_fromstr(data->orig_file, DIASKS_ORIGFILE_LENGTH),
					  s2 = strsafe_fromint (l));
				strsafe_free (s1);
				strsafe_free (s2);
				name = strsafe_format2 ("%s-img%s", obj_file, s1 = strsafe_fromint (l));
				strsafe_free (s1);
				iffailed (ksobj_load (c, DOBJT_IMAGE, DOBJF_IMAGE_KONTRON, source, name, perms), c, "error loading nested objects") fail
					strsafe_free (source);
					strsafe_free (name);
					return (false);
				otherwise
				strsafe_free (source);

				iffailedf (ob_link (c, DOBJT_STORAGE, DOBJT_IMAGE, data, name), c,
					   "unable to link nested object");
				iffailedf (stor = _ksobj_store_find (c, name, true), c, "ksobj_link");
				iffailedf (rel = (relation*)ksmem_commit (c, sizeof (relation)), c, "ksobj_link");
				rel->child = stor;
				rel->parent = _ksobj_store_find (c, obj_file, true);
				if (!relations_head)
					relations_head = rel;
				else
					bdlist_add (relations_head, rel);
			}
			return (true);
		case DOBJF_STORAGE_DIAS2:
		case DOBJF_STORAGE_DIAS3:
		case DOBJF_STORAGE_TIFF:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "_ob_load_storage", __FILE__, __LINE__);
			return (false);
		default:
			kserr_fire (c, KSERR_UNKNOWNDOBJF, "_ob_load_storage", __FILE__, __LINE__);
			return (false);
	}
	return (false);
}

bool _ob_load_image (client* c, udword format, dksobj* data_ptr, int f_src, string obj_file, uword perms) {
	dksobj_image *data = (dksobj_image*)data_ptr;

	uvar32_64 size;
	uword header[0x10];

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_src != -1, c, "_ob_read_image");

	switch (format) {
		case DOBJF_IMAGE_KONTRON:
			isvalidf (data->orig_qlen == 1 && 
			          !strcmp (data->orig_query[0].name, "image"), c, KSERR_WRONGSRCPATH);
			/* reading KONTRON .img file header header */
			isvalidf (read (f_src, header, 0x20) == 0x20, c, KSERR_WRONGSRCFILE);
			/* checking KONTRON .img file header stamp */
			isvalidf (header[0] == 0x0001 &&
			          header[1] == 0x1247 &&
			          header[2] == 0xB06D &&
			          header[5] == 0x4321, c, KSERR_WRONGSRCFILE);
			/* checking if specified number of image does not exists in file */
			isvalidf (data->orig_query[0].value != -1 && 
			          data->orig_query[0].value < (svar32_64)header[6], c, KSERR_WRONGSRCPATH);
			/* filling structure fields */
			strcpy (data->name, "Image");
			strcpy (data->descr, "Imported from KONTRON .img sequence");
			data->dimx = header[3];
			data->dimy = header[4];
			data->dimz = 1;
			data->orig_colors = DCM_GREYVAL;
			/* reading source file */
			size = data->dimx * data->dimy;
			isvalidf (lseek (f_src, data->orig_query[0].value * size + 0x80, SEEK_SET) != -1, 
			          c, KSERR_WRONGSRCPATH);
			iffailedf ((data->layers = ksmem_commit (c, sizeof (ubyte*))) && 
			           (data->layers[0] = ksmem_commit (c, size)), c, "_ob_load_image");
			isvalid (read (f_src, data->layers[0], size) == (signed)size,
			         c, KSERR_WRONGSRCFILE) fail
				ksmem_release (c, data->layers[0]);
			otherwise
			return (true);
		case DOBJF_IMAGE_DIAS2:
		case DOBJF_IMAGE_DIAS3:
		case DOBJF_IMAGE_BMP:
		case DOBJF_IMAGE_JPEG:
		case DOBJF_IMAGE_PNG:
		case DOBJF_IMAGE_TIFF:
		case DOBJF_IMAGE_TGA:
			kserr_fire (c, KSERR_NOTIMPLEMENTED, "_ob_load_image", __FILE__, __LINE__);
			return (false);
		default:
			kserr_fire (c, KSERR_UNKNOWNDOBJF, "_ob_load_image", __FILE__, __LINE__);
			return (false);
	}
	return (false);
}

bool _ob_read_storage (client* c, void* data_ptr, int f_obj) {
	names *img, *svr;
	uvar32_64 n;
	dksobj_storage* data = (dksobj_storage*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_write_sequence");

	data->img_head = NULL;
	for (n = 0; n < data->img_count; ++n) {
		iffailedf (img = ksmem_commit (c, sizeof(names)), c, "_ob_read_sequence");
		bdlist_init (img);
		iffailedf (read (f_obj, img->name, DIASKS_OBJNAME_LENGTH) == DIASKS_OBJNAME_LENGTH,
		           c, "_ob_read_sequence");
		if (!data->img_head)
			data->img_head = img;
		else
			bdlist_add (data->img_head, img);
	}

	data->svr_head = NULL;
	for (n = 0; n < data->svr_count; ++n) {
		iffailedf (svr = ksmem_commit (c, sizeof(names)), c, "_ob_read_sequence");
		bdlist_init (svr);
		iffailedf (read (f_obj, svr->name, DIASKS_OBJNAME_LENGTH) == DIASKS_OBJNAME_LENGTH,
		           c, "_ob_read_sequence");
		if (!data->svr_head)
			data->svr_head = svr;
		else
			bdlist_add (data->svr_head, svr);
	}

	return (true);
}

bool _ob_read_image (client* c, void* data_ptr, int f_obj) {
	uvar32_64 l, size;
	dksobj_image* data = (dksobj_image*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_read_image");

	size = data->dimx * data->dimy;
	iffailedf (data->layers = ksmem_commit (c, data->dimz * sizeof (uvar32_64)), c, "_ob_read_image");
	for (l = 0; l < data->dimz; ++l) {
		iffailedf (data->layers[l] = ksmem_commit (c, size), c, "_ob_read_image");
		iffailed (read (f_obj, data->layers[l], size) == (signed)size, c, "_ob_read_image") fail
			while (l--)
				ksmem_release (c, data->layers[l]);
			ksmem_release (c, data->layers);
			data->layers = 0;
			return (false);
		otherwise;
	}

	return (true);
}

bool _ob_read_mask (client* c, void* data_ptr, int f_obj) {
	uvar32_64 size, n;
	dksobj_mask* data = (dksobj_mask*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_read_mask");

	size = data->count * sizeof (uvar32_64);
	iffailedf (data->cont = ksmem_commit (c, data->count * sizeof (uvar32_64*)), c, "_ob_read_mask");
	iffailedf (data->perim = ksmem_commit (c, data->count * sizeof (uvar32_64*)), c, "_ob_read_mask");
	iffailedf (data->cont_sizes = ksmem_commit (c, size), c, "_ob_read_mask");
	iffailedf (data->perim_sizes = ksmem_commit (c, size), c, "_ob_read_mask");
	iffailedf (read (f_obj, data->cont_sizes, size) == (signed)size, c, "_ob_read_mask");
	iffailedf (read (f_obj, data->perim_sizes, size) == (signed)size, c, "_ob_read_mask");
	for (n = 0; n < data->count; ++n) {
		size = data->cont_sizes[n] * sizeof (uvar32_64);
		iffailedf (data->cont[n] = ksmem_commit (c, size), c, "_ob_read_mask");
		iffailedf (read (f_obj, data->cont[n], size) == (signed)size, c, "_ob_write_mask");
		size = data->perim_sizes[n] * sizeof (uvar32_64);
		iffailedf (data->perim[n] = ksmem_commit (c, size), c, "_ob_read_mask");
		iffailedf (read (f_obj, data->perim[n], size) == (signed)size, c, "_ob_write_mask");
	}

	return (true);
}

bool _ob_read_survey (client* c, void* data_ptr, int f_obj) {
	record *rec;
	uvar32_64 n;
	dksobj_survey* data = (dksobj_survey*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_read_survey");

	data->rec_head = NULL;
	for (n = 0; n < data->rec_count; ++n) {
		iffailedf (rec = ksmem_commit (c, sizeof(record)), c, "_ob_read_survey");
		iffailedf (read (f_obj, rec, sizeof(record)) == sizeof(record),
		           c, "_ob_read_survey");
		bdlist_init (rec);
		if (!data->rec_head)
			data->rec_head = rec;
		else
			bdlist_add (data->rec_head, rec);
	}

	return (true);
}

bool _ob_write_storage (client* c, void* data_ptr, int f_obj) {
	names *img, *svr;
	dksobj_storage* data = (dksobj_storage*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_write_sequence");

	for (img = data->img_head; img; img = img->next)
		iffailedf (write (f_obj, img->name, DIASKS_OBJNAME_LENGTH) == DIASKS_OBJNAME_LENGTH,
		           c, "_ob_write_sequence");

	for (svr = data->svr_head; svr; svr = svr->next)
		iffailedf (write (f_obj, svr->name, DIASKS_OBJNAME_LENGTH) == DIASKS_OBJNAME_LENGTH,
		           c, "_ob_write_sequence");

	return (true);
}

bool _ob_write_image (client* c, void* data_ptr, int f_obj) {
	uvar32_64 l, size;
	dksobj_image* data = (dksobj_image*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_write_image");

	size = data->dimx * data->dimy;
	for (l = 0; l < data->dimz; ++l)
		iffailedf (write (f_obj, data->layers[l], size) == (signed)size, c, "_ob_write_image");

	return (true);
}

bool _ob_write_mask (client* c, void* data_ptr, int f_obj) {
	uvar32_64 n, size;
	dksobj_mask* data = (dksobj_mask*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_write_mask");

	size = data->count * sizeof (uvar32_64);
	iffailedf (write (f_obj, data->cont_sizes, size) == (signed)size, c, "_ob_write_mask");
	iffailedf (write (f_obj, data->perim_sizes, size) == (signed)size, c, "_ob_write_mask");
	for (n = 0; n < data->count; ++n) {
		size = data->cont_sizes[n] * sizeof (uvar32_64);
		iffailedf (write (f_obj, data->cont[n], size) == (signed)size, c, "_ob_write_mask");
		size = data->perim_sizes[n] * sizeof (uvar32_64);
		iffailedf (write (f_obj, data->perim[n], size) == (signed)size, c, "_ob_write_mask");
	}

	return (true);
}

bool _ob_write_survey (client* c, void* data_ptr, int f_obj) {
	record *rec;
	dksobj_survey* data = (dksobj_survey*)data_ptr;

	isclientf (c);
	notnullf (data, c)
	iffailedf (f_obj != -1, c, "_ob_write_survey");

	for (rec = data->rec_head; rec; rec = rec->next)
		iffailedf (write (f_obj, rec, sizeof(record)) == sizeof(record),
		           c, "_ob_write_survey");

	return (true);
}
