<?xml version="1.0"?>
<xslt:stylesheet xmlns:xslt="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xslt:output method="text" indent="yes"/>
<xslt:preserve-space elements="cmdsys subsys class div cmd enum entry"/>
<!--
/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel service-specific publuc functions lookup tables module               *
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
*  <xslt:value-of select="@title"/> kernel service-specific publuc functions lookup tables module          *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  <xslt:value-of select="@title"/> development team                                                       *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  File is automatically generated from dias_cmdsys.xml                        *
*                                                                              *
*******************************************************************************/

				<xslt:for-each select="subsys[attribute::title='img']">
//***
// <xslt:value-of select="@name"/>
//***

					<xslt:apply-templates select="class"/>
	
//***
// GLOBAL LOOKUP-TABLE
//***

diascm_lookup _cm_lookup_table[] = {<xslt:for-each select="class">
	{<xslt:choose>
								<xslt:when test="@codebase"><xslt:value-of select="concat(' ',@codebase)"/></xslt:when>
								<xslt:when test="@codeoff">
									<xslt:choose>
										<xslt:when test="parent::*/attribute[name()='codebase']">
											<xslt:value-of select="concat(' ',parent::*/attribute::codebase,' + ',@codeoff)"/>
										</xslt:when>
										<xslt:otherwise>
											<xslt:value-of select="concat(' ',parent::*/parent::*/attribute::codebase,' + ',parent::*/attribute::codeoff,' + ',@codeoff)"/>
										</xslt:otherwise>
									</xslt:choose>
								</xslt:when>
						</xslt:choose>, <xslt:choose>
								<xslt:when test="@codebase"><xslt:value-of select="concat(' ',@codebase)"/></xslt:when>
								<xslt:when test="@codeoff">
									<xslt:choose>
										<xslt:when test="parent::*/attribute[name()='codebase']">
											<xslt:value-of select="concat(' ',parent::*/attribute::codebase,' + ',@codeoff)"/>
										</xslt:when>
										<xslt:otherwise>
											<xslt:value-of select="concat(' ',parent::*/parent::*/attribute::codebase,' + ',parent::*/attribute::codeoff,' + ',@codeoff)"/>
										</xslt:otherwise>
									</xslt:choose>
								</xslt:when>
						</xslt:choose> + <xslt:value-of select="count(child::*/child::cmd)"/>, _cm_proc_<xslt:value-of select="parent::*/attribute::title"/>_<xslt:value-of select="@title"/>_table},</xslt:for-each>
	{ 0, 0, NULL }
};

				</xslt:for-each>
			</xslt:if>
		</xslt:for-each> 
	</xslt:template>

	<xslt:template match="class">

// *** <xslt:value-of select="@name"/>
diascm_proc _cm_proc_<xslt:value-of select="parent::*/attribute::title"/>_<xslt:value-of select="@title"/>_table[] = {<xslt:apply-templates select="div" />
	NULL
};
	</xslt:template>

	<xslt:template match="div">

	// <xslt:value-of select="@name"/>
		<xslt:apply-templates select="cmd" />
	</xslt:template>

	<xslt:template match="cmd">
	ce_<xslt:value-of select="concat(parent::*/parent::*/parent::*/attribute::title,'_',parent::*/parent::*/attribute::title,'_',@title)"/>,		// <xslt:value-of select="@name"/>
	</xslt:template>

</xslt:stylesheet>
