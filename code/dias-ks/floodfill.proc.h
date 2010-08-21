/*******************************************************************************
*  This is a part of DIAS ver3.0, aIm SUITE.Internet                           *
*                                                                              *
*  DIAS kernel service computation engine floodfill macros                     *
*       used in the most of tracing functions (TRC_HOLES, TRC_SCRAP ...)       *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Rewrited 31.01.2003 Maxim A. Orlovsky: recursive O(n!) algorithm is changed *
*                      on iterative O(2n)-O(8n) algorithm                      *
*                                                                              *
*******************************************************************************/

/*
 * Predefined variables:
 *   conn8 - connection mode (true for 8-points connected shapes support mode)
 *   edges - edges mode (true for interpretate image edges as opened)
 *   seed - initial offset
 *   dimx, dimy, size - set to image sizes
 *   fill - fill color, what - search color
 *   pos, ptr, free, area - declared
 *
 * Arguments:
 *   p - image to process
 *
 */

#define FLOODFILL(p)            area = 0; FLOODFILL_FW(p); FLOODFILL_BW(p);

#define FLOODFILL_FW(p)         \
        free = edges; \
        for (pos = 0, ptr = p; pos < size; ++pos, ++ptr) { \
		if (!edges && pos == seed) { \
			if (*ptr != what) \
				break; \
			free = true; \
		} \
                FLOODFILL_PROC(p, +, -, >=, + 0 ); \
        }
#define FLOODFILL_BW(p)         \
        free = edges; \
        for (pos = size - 1, ptr = p + size - 1; (sint)pos >= 0; --pos, --ptr) {\
		if (!edges && pos == seed && *ptr != what) \
			break; \
                FLOODFILL_PROC(p, -, +, <, + size ); \
        }

#define FLOODFILL_PROC(p,fw,bw,cmp,add) \
	if (edges && (pos / dimx == 0 || pos / dimx == dimy - 1 || \
	              pos % dimx == 0 || pos % dimx == dimx - 1)) \
		free = true; \
	if (*ptr == what) { \
		if (free) { *ptr = fill; area++; } \
		else \
		if ((ptr bw dimx cmp p add && *(ptr bw dimx) == fill) || \
		    ((conn8 || ((pos - 1) / dimx == pos / dimx && *(ptr - 1) == fill)) && \
		     ptr bw dimx - 1 cmp p add && *(ptr bw dimx - 1) == fill) || \
		    ((conn8 || ((pos + 1) / dimx == pos / dimx && *(ptr + 1) == fill)) && \
		     ptr bw dimx + 1 cmp p add && *(ptr bw dimx + 1) == fill)) { \
			*ptr = fill; \
			area++; \
			free = true; \
			while (bw##bw ptr cmp p add && *ptr == what) pos bw##bw; \
			ptr fw##fw; \
		} \
	} else if (*ptr != fill) { \
		if (!edges || (pos / dimx != 0 && pos / dimx != dimy - 1 && \
		               pos % dimx != 0 && pos % dimx != dimx - 1)) \
			free = false; \
	}
