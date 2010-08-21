<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:template match="/">
		<xsl:for-each select="image">
			<table width="100%" cellspacing="0" cellpadding="0" border="0" class="storage_image">
				<colgroup>
					<col width="20px" align="center" valign="middle" />
					<col width="*" align="left" valign="middle" />
					<col width="20px" align="center" valign="middle" />
				</colgroup> 
				<tr>
					<td><img src="" alt="" lowres="" border="0" width="16px" height="16px"/></td>
					<td><xsl:value-of select="@title"/></td>
					<td><img src="" alt="" lowres="" border="0" width="16px" height="16px"/></td>
				</tr>
			</table>
		</xsl:for-each> 
	</xsl:template> 
</xsl:stylesheet>
