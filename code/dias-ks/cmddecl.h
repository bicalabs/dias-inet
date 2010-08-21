/*******************************************************************************
*  This is a part of DIAS ver3.00, aIm SUITE.Internet                          *
*                                                                              *
*  DIAS kernel service-specific publuc functions declarations                  *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  DIAS development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  File is automatically generated from dias_cmdsys.xml                        *
*                                                                              *
*******************************************************************************/

				
//***
// Image analysis commands
//***

					

/*
 *** Floating window operationing
 */

	// Arythmetic operations
bool ce_img_mtx_sum	(client* c, packet* query, packet* resp);		// Summ operation
bool ce_img_mtx_prd	(client* c, packet* query, packet* resp);		// Productuion operation

	// Statistical operations
bool ce_img_mtx_max	(client* c, packet* query, packet* resp);		// Maximum operation
bool ce_img_mtx_min	(client* c, packet* query, packet* resp);		// Minimum operation
bool ce_img_mtx_avg	(client* c, packet* query, packet* resp);		// Average filrer
bool ce_img_mtx_med	(client* c, packet* query, packet* resp);		// Median filrer
bool ce_img_mtx_gvg	(client* c, packet* query, packet* resp);		// Geometric average operation
bool ce_img_mtx_hvg	(client* c, packet* query, packet* resp);		// Harmonic average operation
bool ce_img_mtx_std	(client* c, packet* query, packet* resp);		// Standart deviation operation

	// Boolean logic operations
bool ce_img_mtx_and	(client* c, packet* query, packet* resp);		// Logical AND operation
bool ce_img_mtx_or	(client* c, packet* query, packet* resp);		// Logical OR operation
bool ce_img_mtx_xor	(client* c, packet* query, packet* resp);		// Logical XOR operation

	// Geometry distance operations
bool ce_img_mtx_eds	(client* c, packet* query, packet* resp);		// Euclidian distance operation
bool ce_img_mtx_mds	(client* c, packet* query, packet* resp);		// Manhattan distance operation

	// Diffirentiating operations
bool ce_img_mtx_grad	(client* c, packet* query, packet* resp);		// Gradient (nabla) operation
bool ce_img_mtx_div	(client* c, packet* query, packet* resp);		// Divergence operation
bool ce_img_mtx_rot	(client* c, packet* query, packet* resp);		// Rotor operation
bool ce_img_mtx_lapl	(client* c, packet* query, packet* resp);		// Laplacian operation

	// Special operations
bool ce_img_mtx_filter	(client* c, packet* query, packet* resp);		// Custom operation
bool ce_img_mtx_logic	(client* c, packet* query, packet* resp);		// Content-dependent logic

/*
 *** Whole image processing
 */

	// Information
bool ce_img_whl_stat	(client* c, packet* query, packet* resp);		// Statistical information

	// Transofrmation
bool ce_img_whl_resample	(client* c, packet* query, packet* resp);		// Resample image
bool ce_img_whl_product	(client* c, packet* query, packet* resp);		// Matrix pruduction of two images

/*
 *** Trace image operationing
 */

	// Objects separation/aggregation
bool ce_img_trc_erode	(client* c, packet* query, packet* resp);		// Erode objects
bool ce_img_trc_dilate	(client* c, packet* query, packet* resp);		// Dilate objects
bool ce_img_trc_open	(client* c, packet* query, packet* resp);		// Open objects
bool ce_img_trc_close	(client* c, packet* query, packet* resp);		// Close objects

	// Objects transformation
bool ce_img_trc_holes	(client* c, packet* query, packet* resp);		// Fill holes in objects
bool ce_img_trc_link	(client* c, packet* query, packet* resp);		// Link lines

	// Objects selection
bool ce_img_trc_scrap	(client* c, packet* query, packet* resp);		// Scrap objects
bool ce_img_trc_mark	(client* c, packet* query, packet* resp);		// Mark objects
bool ce_img_trc_mask	(client* c, packet* query, packet* resp);		// Mask objects

	// Objects identification
bool ce_img_trc_ident	(client* c, packet* query, packet* resp);		// Identificate objects
bool ce_img_trc_window	(client* c, packet* query, packet* resp);		// Identificate objects in window

/*
 *** Pixelwise image operations
 */

	// Arythmetic operations
bool ce_img_pxw_add	(client* c, packet* query, packet* resp);		// Summa operation
bool ce_img_pxw_sub	(client* c, packet* query, packet* resp);		// Substitute operation
bool ce_img_pxw_mul	(client* c, packet* query, packet* resp);		// Multiplication operation
bool ce_img_pxw_div	(client* c, packet* query, packet* resp);		// Division operation
bool ce_img_pxw_pow	(client* c, packet* query, packet* resp);		// Power operation
bool ce_img_pxw_root	(client* c, packet* query, packet* resp);		// Root operation

	// Statistical operations
bool ce_img_pxw_max	(client* c, packet* query, packet* resp);		// Maximum operation
bool ce_img_pxw_min	(client* c, packet* query, packet* resp);		// Minimum operation
bool ce_img_pxw_avg	(client* c, packet* query, packet* resp);		// Average filrer
bool ce_img_pxw_med	(client* c, packet* query, packet* resp);		// Median filrer
bool ce_img_pxw_gvg	(client* c, packet* query, packet* resp);		// Geometric average operation
bool ce_img_pxw_hvg	(client* c, packet* query, packet* resp);		// Harmonic average operation
bool ce_img_pxw_std	(client* c, packet* query, packet* resp);		// Standart deviation operation

	// Boolean logic operations
bool ce_img_pxw_and	(client* c, packet* query, packet* resp);		// Logical AND operation
bool ce_img_pxw_or	(client* c, packet* query, packet* resp);		// Logical OR operation
bool ce_img_pxw_xor	(client* c, packet* query, packet* resp);		// Logical XOR operation
bool ce_img_pxw_not	(client* c, packet* query, packet* resp);		// Logical NOT operation

	// Geometry distance operations
bool ce_img_pxw_eds	(client* c, packet* query, packet* resp);		// Euclidian distance operation
bool ce_img_pxw_mds	(client* c, packet* query, packet* resp);		// Manhattan distance operation

	// Other operations
bool ce_img_pxw_thr	(client* c, packet* query, packet* resp);		// Threshold operation

/*
 *** Single value pixelwise operations
 */

	// Arythmetic operations
bool ce_img_pxn_add	(client* c, packet* query, packet* resp);		// Summa operation
bool ce_img_pxn_subl	(client* c, packet* query, packet* resp);		// Substitute left operation
bool ce_img_pxn_mul	(client* c, packet* query, packet* resp);		// Multiplication operation
bool ce_img_pxn_divl	(client* c, packet* query, packet* resp);		// Division left operation
bool ce_img_pxn_pow	(client* c, packet* query, packet* resp);		// Power operation
bool ce_img_pxn_root	(client* c, packet* query, packet* resp);		// Root operation

	// Statistical operations
bool ce_img_pxn_max	(client* c, packet* query, packet* resp);		// Maximum operation
bool ce_img_pxn_min	(client* c, packet* query, packet* resp);		// Minimum operation
bool ce_img_pxn_avg	(client* c, packet* query, packet* resp);		// Average filrer
bool ce_img_pxn_gvg	(client* c, packet* query, packet* resp);		// Geometric average operation
bool ce_img_pxn_hvg	(client* c, packet* query, packet* resp);		// Harmonic average operation

	// Boolean logic operations
bool ce_img_pxn_and	(client* c, packet* query, packet* resp);		// Logical AND operation
bool ce_img_pxn_or	(client* c, packet* query, packet* resp);		// Logical OR operation
bool ce_img_pxn_xor	(client* c, packet* query, packet* resp);		// Logical XOR operation

	// Geometry distance operations
bool ce_img_pxn_eds	(client* c, packet* query, packet* resp);		// Euclidian distance operation
bool ce_img_pxn_mds	(client* c, packet* query, packet* resp);		// Manhattan distance operation