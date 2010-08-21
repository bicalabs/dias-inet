/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service general internal definitions and constants              *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

#pragma once

#include "..\shared-ks\aim.h"

#include "cmddef.h"

#define	DARGST_PURE		0x0000

#define	DARGST_BOOL		(0x0010 + KSARGT_INT)
#define	DARGST_ENUM		(0x0020 + KSARGT_INT)
#define	DARGST_GREYVAL		(0x0040 + KSARGT_INT)
#define	DARGST_COORD		(0x0080 + KSARGT_INT)
#define	DARGST_HOBJ		(0x0100 + KSARGT_INT)

#define	DARGST_FILENAME		(0x0010 + KSARGT_STRING)
#define	DARGST_OBJECT		(0x0020 + KSARGT_STRING)
#define	DARGST_OBJFILE		(0x0040 + KSARGT_STRING)
#define	DARGST_MTX		(0x0080 + KSARGT_STRING)
#define	DARGST_GRAPH		(0x0100 + KSARGT_STRING)

#define	DARGST_COORD_LEFT	(DARGST_COORD + 0x0000)
#define	DARGST_COORD_TOP	(DARGST_COORD + 0x1000)
#define	DARGST_COORD_RIGHT	(DARGST_COORD + 0x2000)
#define	DARGST_COORD_BOTTOM	(DARGST_COORD + 0x4000)
#define	DARGST_MTX_OFFS		(DARGST_MTX + 0x0000)
#define	DARGST_MTX_FACTORS	(DARGST_MTX + 0x1000)
#define	DARGST_PLOT		(DARGST_GRAPH + 0x1000)
#define	DARGST_SHAPE		(DARGST_GRAPH + 0x2000)


#define	DOBJT_STORAGE	0x0000
#define	DOBJT_IMAGE	0x0001
#define	DOBJT_MASK	0x0002
#define	DOBJT_MARK	0x0003
#define	DOBJT_LABEL	0x0004
#define	DOBJT_METRIC	0x0005
#define	DOBJT_SURVEY	0x0006
#define	DOBJT__MAX	DOBJT_SURVEY

#define	DOBJF_STORAGE_KONTRON	0x00000001
#define	DOBJF_STORAGE_DIAS2	0x00000002
#define	DOBJF_STORAGE_DIAS3	0x00000003
#define	DOBJF_STORAGE_TIFF	0x00000004

#define	DOBJF_IMAGE_KONTRON	0x00010001
#define	DOBJF_IMAGE_DIAS2	0x00010002
#define	DOBJF_IMAGE_DIAS3	0x00010003
#define	DOBJF_IMAGE_BMP		0x00010004
#define	DOBJF_IMAGE_JPEG	0x00010005
#define	DOBJF_IMAGE_PNG		0x00010006
#define	DOBJF_IMAGE_TIFF	0x00010007
#define	DOBJF_IMAGE_TGA		0x00010008

#define	DOBJF_SURVEY_TAB	0x00020001
#define	DOBJF_SURVEY_DBF	0x00020002
#define	DOBJF_SURVEY_EXCEL	0x00020003


#define	DCM_BW		0x00
#define	DCM_GREYVAL	0x01
#define	DCM_RGB		0x02
#define	DCM_CMY		0x03
#define	DCM_CMYK	0x04
#define	DCM_HLS 	0x05


#define	DIASKS_OBJNAME_LENGTH	0x40
#define	DIASKS_OBJDESCR_LENGTH	0x40
#define	DIASKS_HISTORY_LENGTH	0x10
#define	DIASKS_HISTTITLE_LENGTH	0x60
#define	DIASKS_HISTUSER_LENGTH	0x30
#define	DIASKS_ORIGFILE_LENGTH	0x100
#define	DIASKS_ORIGQUERY_LENGTH	0x08
#define	DIASKS_ORIGPARAM_LENGTH	0x10
#define	DIASKS_OBJMAX_LENGTH	0x1000
#define	DIASKS_OBJSPEC_LENGTH	(DIASKS_OBJMAX_LENGTH - \
				 (DIASKS_OBJNAME_LENGTH + DIASKS_OBJDESCR_LENGTH + sizeof(clock_t) + 12 +\
				  DIASKS_ORIGFILE_LENGTH + sizeof(dks_query) * DIASKS_ORIGQUERY_LENGTH + \
				  (sizeof (history) * DIASKS_HISTORY_LENGTH)))

struct names_st;
struct history_st;
struct record_st;
typedef struct names_st names;
typedef struct history_st history;
typedef struct record_st record;

struct names_st {
	names*		prev;
	names*		next;
	char		name[DIASKS_OBJNAME_LENGTH];
};

struct history_st {
	udword		code;
	char		title[DIASKS_HISTTITLE_LENGTH];
	char		user[DIASKS_HISTUSER_LENGTH];
	time_t		performed;
	clock_t		elapsed;
};

struct record_st {
	record*		prev;
	record*		next;

	char		src_image[KS_OBJPATH_LEN];
	char		src_mask[KS_OBJPATH_LEN];
	char		src_geom[KS_OBJPATH_LEN];
	char		src_dens[KS_OBJPATH_LEN];
	uvar32_64	src_object;

	rtbyte		apexx;
	rtbyte		apexy;
	rtbyte		top;
	rtbyte		bottom;
	rtbyte		left;
	rtbyte		right;

	rtbyte		area;
	rtbyte		perim;
	rtbyte		csc_area;
	rtbyte		csc_perim;
	uvar32_64	fragments;

	rtbyte		grav_areax;
	rtbyte		grav_areay;
	rtbyte		grav_perimx;
	rtbyte		grav_perimy;
	rtbyte		grav_greyx;
	rtbyte		grav_greyy;

	rtbyte		diam_max;
	rtbyte		diam_min;
	rtbyte		diam_mean;
	rtbyte		diam_var;
	rtbyte		diam_ort;
	rtbyte		diam_x;
	rtbyte		diam_y;
	rtbyte		angle_max;
	rtbyte		angle_min;
	uvar32_64	chords_count;
	rtbyte		chords_mean;
	rtbyte		chords_var;
	rtbyte		chords_min;
	rtbyte		chords_max;

	rtbyte		pixels;
	rtbyte		sum;
	rtbyte		sum2;
	rtbyte		mean;
	rtbyte		stddev;
	rtbyte		min;
	rtbyte		max;
	rtbyte		mod;
	rtbyte		med;
};

typedef struct {
	char		name[DIASKS_ORIGPARAM_LENGTH];
	svar32_64	value;
} dks_query;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	ubyte		object_specific_data[DIASKS_OBJSPEC_LENGTH];
} dksobj;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	uvar32_64	dimx;
	uvar32_64	dimy;
	uvar32_64	dimz;
	uvar32_64	img_count;
	uvar32_64	svr_count;

	names*		img_head;
	names*		svr_head;
} dksobj_storage;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	udword		orig_colors;

	uvar32_64	dimx;
	uvar32_64	dimy;
	uvar32_64	dimz;
	uvar32_64	msk_count;
	uvar32_64	mrk_count;
	uvar32_64	ovl_count;
	uvar32_64	lbl_count;
	uvar32_64	mtr_count;

	ubyte**		layers;
	names*		msk_head;
	names*		mrk_head;
	names*		ovl_head;
	names*		lbl_head;
	names*		mtr_head;
} dksobj_image;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	uvar32_64	dimx;
	uvar32_64	dimy;
	uvar32_64	count;

	uvar32_64**	cont;
	uvar32_64**	perim;
	uvar32_64*	cont_sizes;
	uvar32_64*	perim_sizes;
} dksobj_mask;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	udword		orig_kernel;
	udword		orig_shape;

	uvar32_64	count;

//	mark_kernel*	kernel;
//	mark_shape*	shape;
	uvar32_64*	sizex;
	uvar32_64*	sizey;
	rtbyte*		angle;
} dksobj_marks;

typedef struct {
	char		name[DIASKS_OBJNAME_LENGTH];
	char		descr[DIASKS_OBJDESCR_LENGTH];

	clock_t		cpu_time;
	uword		hist_start;
	uword		hist_finish;
	history		hist_cont[DIASKS_HISTORY_LENGTH];

	char		orig_file[DIASKS_ORIGFILE_LENGTH];
	ubyte		orig_qlen;
	dks_query	orig_query[DIASKS_ORIGQUERY_LENGTH];
	udword		orig_format;

	uvar32_64	rec_count;
	record*		rec_head;
} dksobj_survey;

