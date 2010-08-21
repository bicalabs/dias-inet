<?xml version="1.0"?>
<xslt:stylesheet xmlns:xslt="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xslt:output method="text" indent="yes"/>
<xslt:preserve-space elements="cmdsys subsys class div cmd enum entry"/>
<!--
/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel service-specific publuc functions code definitions                   *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITE development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 28.07.2004 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*                                                                              *
*******************************************************************************/
-->

	<xslt:template match="/">
		<xslt:for-each select="cmdsys">
			<xslt:if test="@title[.='DIAS']">
/*******************************************************************************
*  This is a part of <xslt:value-of select="@title"/> ver<xslt:value-of select="concat(@ver,'.',@subver)"/>, aIm SUITE.Internet                          *
*                                                                              *
*  <xslt:value-of select="@title"/> kernel service-specific publuc functions code definitions              *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  <xslt:value-of select="@title"/> development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  File is automatically generated from dias_cmdsys.xml                        *
*                                                                              *
*******************************************************************************/

#pragma once

#include "..\shared-ks\aim.h"


/*******************************************************************************
  <xslt:value-of select="@name"/>
*******************************************************************************/

#define	DCMD<xslt:value-of select="concat('	',@codebase)"/>

				<xslt:for-each select="subsys[attribute::title='img']">


#define DCMD_OBJINFO		DCMD + 0x10000
#define	DCMD_OBJINFO_STORAGE	DCMD_OBJINFO + DOBJT_STORAGE
#define	DCMD_OBJINFO_IMAGE	DCMD_OBJINFO + DOBJT_IMAGE
#define DCMD_OBJGET		DCMD + 0x20000
#define	DCMD_OBJGET_STORAGE	DCMD_OBJGET + DOBJT_STORAGE
#define	DCMD_OBJGET_IMAGE	DCMD_OBJGET + DOBJT_IMAGE

#define	DCMD_MEASURE		DCMD + 0x0000
					
//
// <xslt:value-of select="@name"/>
//

#define	DCMD_<xslt:value-of select="@title"/><xslt:choose>
						<xslt:when test="@codebase"><xslt:value-of select="concat('	',@codebase)"/></xslt:when>
						<xslt:when test="@codeoff"><xslt:value-of select="concat('	DCMD + ',@codeoff)"/></xslt:when></xslt:choose>

					<xslt:apply-templates select="class"/>
					
					<xslt:apply-templates select=".//enum"/>
								   
enum ce_sub_modes { ce_subm_zero = 0, ce_subm_absolute = 1, ce_subm_module = 2 };

				</xslt:for-each>
			</xslt:if>
		</xslt:for-each> 
	</xslt:template>

	<xslt:template match="class">

// ** <xslt:value-of select="@name"/>
#define DCMD_<xslt:value-of select="parent::*/attribute::title"/>_<xslt:value-of select="@title"/><xslt:choose>
			<xslt:when test="@codebase"><xslt:value-of select="concat('	',@codebase)"/></xslt:when>
			<xslt:when test="@codeoff"><xslt:value-of select="concat('	DCMD_',parent::*/attribute::title,' + ',@codeoff)"/>
			</xslt:when>
		</xslt:choose>

		<xslt:apply-templates select="div" />
	</xslt:template>

	<xslt:template match="div">
		
//  * <xslt:value-of select="@name"/>
		<xslt:apply-templates select="cmd" />
	</xslt:template>

	<xslt:template match="cmd">
#define	DCMD_<xslt:value-of select="concat(parent::*/parent::*/parent::*/attribute::title,'_',parent::*/parent::*/attribute::title,'_',@title)"/><xslt:choose>
			<xslt:when test="@codebase"><xslt:value-of select="concat('	',@codebase)"/></xslt:when>
			<xslt:when test="@codeoff"><xslt:value-of select="concat('	DCMD_',parent::*/parent::*/parent::*/attribute::title,'_',parent::*/parent::*/attribute::title,' + ',@codeoff)"/>
			</xslt:when>
		</xslt:choose>	// <xslt:value-of select="@name"/> 
	</xslt:template>

	<xslt:template match="enum">

// *** <xslt:value-of select="@name"/>
enum ce_<xslt:value-of select="concat(parent::*/parent::*/attribute::title,'_',@title)"/> { 
		<xslt:for-each select="entry">
	ce_<xslt:value-of select="concat(@title,' = ',@val)"/>, // (<xslt:value-of select="@symbol"/>) <xslt:value-of select="@name"/></xslt:for-each>
};
	</xslt:template>

</xslt:stylesheet>
