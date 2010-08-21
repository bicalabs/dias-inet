/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service objects broker sybsystem definitions and constants      *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

#define	WRITE_REAL(r)	\
			itoa ((int)(r), (char*)&(buff[0]), 10); \
			write (f_src, (char*)&(buff[0]), strlen ((char*)&(buff[0]))); \
			write (f_src, ".", 1); \
			itoa ((int)((r - (int)r) * 1000.0), (char*)&(buff[0]), 10); \
			write (f_src, (char*)&(buff[0]), strlen ((char*)&(buff[0]))); \
			write (f_src, "\t", 1);
#define	WRITE_INT(i)	\
			itoa (i, (char*)&(buff[0]), 10); \
			write (f_src, (char*)&(buff[0]), strlen ((char*)&(buff[0]))); \
			write (f_src, "\t", 1);


char _ob_str_surveyfields[] = "No\
	Image\
	Mask\
	GeomScale\
	DensScale\
	ObjectNo"

"	Apex-X\
	Apex-Y\
	Top\
	Bottom\
	Left\
	Right"

"	Area\
	Perim\
	CscArea\
	CscPerim\
	Fragments"

"	CenterByArea-X\
	CenterByArea-Y\
	CenterByPerim-X\
	CenterByPerim-Y\
	CenterByGrey-X\
	CenterByGrey-Y"

"	DiamMax\
	DiamMin\
	DiamMean\
	DiamVar\
	DiamOrt\
	DiamX\
	DiamY\
	AngleX-DiamMax\
	AngleX-DiamMin\
	ChordsNo\
	ChordsLenMean\
	ChordsLenVar\
	ChordMin\
	ChordMax"

"	Pixels\
	GreySum\
	GreySum2\
	GreyMean\
	GreyStdDev\
	GreyMin\
	GreyMax\
	GreyMod\
	GreyMed\x0D\x0A";

sbyte	_ob_parse_path (string origin, dks_query* query);

bool	_ob_create_storage (client* c, dksobj* data_ptr, packet* pack);
bool	_ob_create_image (client* c, dksobj* data_ptr, packet* pack);
bool	_ob_create_mask (client* c, dksobj* data_ptr, packet* pack);
bool	_ob_create_survey (client* c, dksobj* data_ptr, packet* pack);

bool	_ob_free_storage (client* c, void* data_ptr);
bool	_ob_free_image (client* c, void* data_ptr);
bool	_ob_free_mask (client* c, void* data_ptr);
bool	_ob_free_survey (client* c, void* data_ptr);

bool	_ob_link_storage (client* c, udword child_type, void* parent_data, string child);
bool	_ob_link_image (client* c, udword child_type, void* parent_data, string child);

bool	_ob_unlink_storage (client* c, udword child_type, void* parent_data, string child);
bool	_ob_unlink_image (client* c, udword child_type, void* parent_data, string child);

bool	_ob_load_storage (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_image (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_mask (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_mark (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_label (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_metric (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
bool	_ob_load_survey (client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);

bool	_ob_extract_storage (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_image (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_mask (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_mark (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_label (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_metric (client* c, udword format, void* data_ptr, int f_src, string obj_file);
bool	_ob_extract_survey (client* c, udword format, void* data_ptr, int f_src, string obj_file);

bool	_ob_read_storage (client* c, void* data_ptr, int f_obj);
bool	_ob_read_image (client* c, void* data_ptr, int f_obj);
bool	_ob_read_mask (client* c, void* data_ptr, int f_obj);
bool	_ob_read_mark (client* c, void* data_ptr, int f_obj);
bool	_ob_read_label (client* c, void* data_ptr, int f_obj);
bool	_ob_read_metric (client* c, void* data_ptr, int f_obj);
bool	_ob_read_survey (client* c, void* data_ptr, int f_obj);

bool	_ob_write_storage (client* c, void* data_ptr, int f_obj);
bool	_ob_write_image (client* c, void* data_ptr, int f_obj);
bool	_ob_write_mask (client* c, void* data_ptr, int f_obj);
bool	_ob_write_mark (client* c, void* data_ptr, int f_obj);
bool	_ob_write_label (client* c, void* data_ptr, int f_obj);
bool	_ob_write_metric (client* c, void* data_ptr, int f_obj);
bool	_ob_write_survey (client* c, void* data_ptr, int f_obj);

typedef bool (*diasob_create_proc)(client* c, dksobj* data_ptr, packet* pack);
typedef bool (*diasob_free_proc)(client* c, void* data_ptr);
typedef bool (*diasob_link_proc)(client* c, udword child_type, void* parent_data, string child);
typedef bool (*diasob_unlink_proc)(client* c, udword child_type, void* parent_data, string child);
typedef bool (*diasob_load_proc)(client* c, udword format, void* data_ptr, int f_src, string obj_file, uword perms);
typedef bool (*diasob_extract_proc)(client* c, udword format, void* data_ptr, int f_src, string obj_file);
typedef bool (*diasob_read_proc)(client* c, void* data_ptr, int f_obj);
typedef bool (*diasob_write_proc)(client* c, void* data_ptr, int f_obj);

diasob_create_proc _ob_create_table[] = {
	_ob_create_storage,
	_ob_create_image,
	_ob_create_mask,
	NULL,
	NULL,
	NULL,
	_ob_create_survey,
};
diasob_free_proc _ob_free_table[] = {
	_ob_free_storage,
	_ob_free_image,
	_ob_free_mask,
	NULL,
	NULL,
	NULL,
	_ob_free_survey,
};
diasob_link_proc _ob_link_table[] = {
	_ob_link_storage,
	_ob_link_image,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
diasob_unlink_proc _ob_unlink_table[] = {
	_ob_unlink_storage,
	_ob_unlink_image,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
diasob_load_proc _ob_load_table[] = {
	_ob_load_storage,
	_ob_load_image,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
diasob_extract_proc _ob_extract_table[] = {
	_ob_extract_storage,
	_ob_extract_image,
	NULL,
	NULL,
	NULL,
	NULL,
	_ob_extract_survey,
};
diasob_read_proc _ob_read_table[] = {
	_ob_read_storage,
	_ob_read_image,
	_ob_read_mask,
	NULL,
	NULL,
	NULL,
	_ob_read_survey,
};
diasob_write_proc _ob_write_table[] = {
	_ob_write_storage,
	_ob_write_image,
	_ob_write_mask,
	NULL,
	NULL,
	NULL,
	_ob_write_survey,
};
