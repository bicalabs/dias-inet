
/*******************************************************************************
*  This is a part of DIAS ver3.00, aIm SUITE.Internet                          *
*                                                                              *
*  DIAS kernel service-specific publuc functions code definitions              *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  File is automatically generated from dias_cmdsys.xml                        *
*                                                                              *
*******************************************************************************/

#pragma once

#include "..\shared-ks\aim.h"


/*******************************************************************************
  DIAS command system
*******************************************************************************/

#define	DCMD	0x10000000


#define DCMD_OBJINFO		DCMD + 0x10000
#define	DCMD_OBJINFO_STORAGE	DCMD_OBJINFO + DOBJT_STORAGE
#define	DCMD_OBJINFO_IMAGE	DCMD_OBJINFO + DOBJT_IMAGE
#define DCMD_OBJGET		DCMD + 0x20000
#define	DCMD_OBJGET_STORAGE	DCMD_OBJGET + DOBJT_STORAGE
#define	DCMD_OBJGET_IMAGE	DCMD_OBJGET + DOBJT_IMAGE

#define	DCMD_MEASURE		DCMD + 0x0000
					
//
// Image analysis commands
//

#define	DCMD_img	DCMD + 0x1000

// ** Floating window operationing
#define DCMD_img_mtx	DCMD_img + 0x400
		
//  * Arythmetic operations
#define	DCMD_img_mtx_sum	DCMD_img_mtx + 0x00	// Summ operation
#define	DCMD_img_mtx_prd	DCMD_img_mtx + 0x01	// Productuion operation
		
//  * Statistical operations
#define	DCMD_img_mtx_max	DCMD_img_mtx + 0x02	// Maximum operation
#define	DCMD_img_mtx_min	DCMD_img_mtx + 0x03	// Minimum operation
#define	DCMD_img_mtx_avg	DCMD_img_mtx + 0x04	// Average filrer
#define	DCMD_img_mtx_med	DCMD_img_mtx + 0x05	// Median filrer
#define	DCMD_img_mtx_gvg	DCMD_img_mtx + 0x06	// Geometric average operation
#define	DCMD_img_mtx_hvg	DCMD_img_mtx + 0x07	// Harmonic average operation
#define	DCMD_img_mtx_std	DCMD_img_mtx + 0x08	// Standart deviation operation
		
//  * Boolean logic operations
#define	DCMD_img_mtx_and	DCMD_img_mtx + 0x09	// Logical AND operation
#define	DCMD_img_mtx_or	DCMD_img_mtx + 0x0a	// Logical OR operation
#define	DCMD_img_mtx_xor	DCMD_img_mtx + 0x0b	// Logical XOR operation
		
//  * Geometry distance operations
#define	DCMD_img_mtx_eds	DCMD_img_mtx + 0x0c	// Euclidian distance operation
#define	DCMD_img_mtx_mds	DCMD_img_mtx + 0x0d	// Manhattan distance operation
		
//  * Diffirentiating operations
#define	DCMD_img_mtx_grad	DCMD_img_mtx + 0x0e	// Gradient (nabla) operation
#define	DCMD_img_mtx_div	DCMD_img_mtx + 0x0f	// Divergence operation
#define	DCMD_img_mtx_rot	DCMD_img_mtx + 0x10	// Rotor operation
#define	DCMD_img_mtx_lapl	DCMD_img_mtx + 0x11	// Laplacian operation
		
//  * Special operations
#define	DCMD_img_mtx_filter	DCMD_img_mtx + 0x12	// Custom operation
#define	DCMD_img_mtx_logic	DCMD_img_mtx + 0x13	// Content-dependent logic

// ** Whole image processing
#define DCMD_img_whl	DCMD_img + 0x600
		
//  * Information
#define	DCMD_img_whl_stat	DCMD_img_whl + 0x01	// Statistical information
		
//  * Transofrmation
#define	DCMD_img_whl_resample	DCMD_img_whl + 0x02	// Resample image
#define	DCMD_img_whl_product	DCMD_img_whl + 0x03	// Matrix pruduction of two images

// ** Trace image operationing
#define DCMD_img_trc	DCMD_img + 0x500
		
//  * Objects separation/aggregation
#define	DCMD_img_trc_erode	DCMD_img_trc + 0x01	// Erode objects
#define	DCMD_img_trc_dilate	DCMD_img_trc + 0x02	// Dilate objects
#define	DCMD_img_trc_open	DCMD_img_trc + 0x03	// Open objects
#define	DCMD_img_trc_close	DCMD_img_trc + 0x04	// Close objects
		
//  * Objects transformation
#define	DCMD_img_trc_holes	DCMD_img_trc + 0x05	// Fill holes in objects
#define	DCMD_img_trc_link	DCMD_img_trc + 0x06	// Link lines
		
//  * Objects selection
#define	DCMD_img_trc_scrap	DCMD_img_trc + 0x07	// Scrap objects
#define	DCMD_img_trc_mark	DCMD_img_trc + 0x08	// Mark objects
#define	DCMD_img_trc_mask	DCMD_img_trc + 0x09	// Mask objects
		
//  * Objects identification
#define	DCMD_img_trc_ident	DCMD_img_trc + 0x0a	// Identificate objects
#define	DCMD_img_trc_window	DCMD_img_trc + 0x0b	// Identificate objects in window

// ** Pixelwise image operations
#define DCMD_img_pxw	DCMD_img + 0x300
		
//  * Arythmetic operations
#define	DCMD_img_pxw_add	DCMD_img_pxw + 0	// Summa operation
#define	DCMD_img_pxw_sub	DCMD_img_pxw + 1	// Substitute operation
#define	DCMD_img_pxw_mul	DCMD_img_pxw + 2	// Multiplication operation
#define	DCMD_img_pxw_div	DCMD_img_pxw + 3	// Division operation
#define	DCMD_img_pxw_pow	DCMD_img_pxw + 4	// Power operation
#define	DCMD_img_pxw_root	DCMD_img_pxw + 5	// Root operation
		
//  * Statistical operations
#define	DCMD_img_pxw_max	DCMD_img_pxw + 11	// Maximum operation
#define	DCMD_img_pxw_min	DCMD_img_pxw + 10	// Minimum operation
#define	DCMD_img_pxw_avg	DCMD_img_pxw + 13	// Average filrer
#define	DCMD_img_pxw_med	DCMD_img_pxw + 12	// Median filrer
#define	DCMD_img_pxw_gvg	DCMD_img_pxw + 14	// Geometric average operation
#define	DCMD_img_pxw_hvg	DCMD_img_pxw + 15	// Harmonic average operation
#define	DCMD_img_pxw_std	DCMD_img_pxw + 16	// Standart deviation operation
		
//  * Boolean logic operations
#define	DCMD_img_pxw_and	DCMD_img_pxw + 6	// Logical AND operation
#define	DCMD_img_pxw_or	DCMD_img_pxw + 7	// Logical OR operation
#define	DCMD_img_pxw_xor	DCMD_img_pxw + 8	// Logical XOR operation
#define	DCMD_img_pxw_not	DCMD_img_pxw + 9	// Logical NOT operation
		
//  * Geometry distance operations
#define	DCMD_img_pxw_eds	DCMD_img_pxw + 17	// Euclidian distance operation
#define	DCMD_img_pxw_mds	DCMD_img_pxw + 18	// Manhattan distance operation
		
//  * Other operations
#define	DCMD_img_pxw_thr	DCMD_img_pxw + 19	// Threshold operation

// ** Single value pixelwise operations
#define DCMD_img_pxn	DCMD_img + 0x200
		
//  * Arythmetic operations
#define	DCMD_img_pxn_add	DCMD_img_pxn + 0	// Summa operation
#define	DCMD_img_pxn_subl	DCMD_img_pxn + 1	// Substitute left operation
#define	DCMD_img_pxn_mul	DCMD_img_pxn + 3	// Multiplication operation
#define	DCMD_img_pxn_divl	DCMD_img_pxn + 4	// Division left operation
#define	DCMD_img_pxn_pow	DCMD_img_pxn + 6	// Power operation
#define	DCMD_img_pxn_root	DCMD_img_pxn + 7	// Root operation
		
//  * Statistical operations
#define	DCMD_img_pxn_max	DCMD_img_pxn + 12	// Maximum operation
#define	DCMD_img_pxn_min	DCMD_img_pxn + 11	// Minimum operation
#define	DCMD_img_pxn_avg	DCMD_img_pxn + 13	// Average filrer
#define	DCMD_img_pxn_gvg	DCMD_img_pxn + 14	// Geometric average operation
#define	DCMD_img_pxn_hvg	DCMD_img_pxn + 15	// Harmonic average operation
		
//  * Boolean logic operations
#define	DCMD_img_pxn_and	DCMD_img_pxn + 8	// Logical AND operation
#define	DCMD_img_pxn_or	DCMD_img_pxn + 9	// Logical OR operation
#define	DCMD_img_pxn_xor	DCMD_img_pxn + 10	// Logical XOR operation
		
//  * Geometry distance operations
#define	DCMD_img_pxn_eds	DCMD_img_pxn + 16	// Euclidian distance operation
#define	DCMD_img_pxn_mds	DCMD_img_pxn + 17	// Manhattan distance operation

// *** Substraction operations modes
enum ce_logic_comp { 
		
	ce_logc_eq = 0, // (=) Equal
	ce_logc_ne = 1, // (!=) Not equal
	ce_logc_lt = 2, // (<) Less then
	ce_logc_gt = 3, // (>) Greater than
	ce_logc_le = 4, // (<=) Less or equal
	ce_logc_ge = 5, // (>=) Greater or equal
};
	

// *** Substraction operations modes
enum ce_logic_reslt { 
		
	ce_logr_value = 1, // (Value) Set to value 'ResltVal'
	ce_logr_bool = 0, // (Source) Save source value
	ce_logr_count = 2, // (Count) Set to count of pixels
	ce_logr_sum = 3, // (Sum) Set to sum of pixels values
	ce_logr_mean = 4, // (Mean) Set to Mean of pixels values
	ce_logr_min = 5, // (Min) Set to minimal of pixels values
	ce_logr_max = 6, // (Max) Set to maximal of pixels values
};
	
								   
enum ce_sub_modes { ce_subm_zero = 0, ce_subm_absolute = 1, ce_subm_module = 2 };

				