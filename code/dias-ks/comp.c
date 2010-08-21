/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service computations engine subsystem                           *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directody     *
*                                                                              *
*  Created 28.12.2002 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/

/* TODO: Image processing functions to implement in ver1.0:
         gradients, median, mask, link;
	 use 16-bit color depth for evedy channel */

/* TODO: Evedy ce_* function should be build on the following shemee:
         a) function-specific variables declaration
	 b) standart macro call
	    [standart variables declaration, parameters validation,
	     image hobj, obj, access, and dimension validation]
	 c) update following client fields: cpu_used, comp_used, accessed
	 d) set response structure fields, even if there is nothing to respond
	 e) read and validate function-specific data from the query structure.
	    all
	 f) update the following fileds in the destination images:
	    cpu_used, comp_used, accessed, modified, histody
	 g) update the following fields in the source images: accessed
	 h) lock source images on write access and destination images
	    on read and write access from the beginning till the end of function */

#include <math.h>
#include <time.h>
#include <winsock2.h>
#include <sys/timeb.h>

#include <sysconf.h>
#include <strsafex.h>
#include "dias-ks.h"
#include "comp.h"

#include "comp/floodfill.cm"
#include "comp/measure.cm"
#include "cmdimpl.cm"
