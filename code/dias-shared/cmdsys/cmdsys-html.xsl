<?xml version="1.0"?> 
<xslt:stylesheet xmlns:xslt="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xslt:template match="/">
		<html><head></head><body>
		<xslt:for-each select="cmdsys">
			<xslt:if test="@system[.='DIAS']">
				<h1>DIAS-KS command system ver. <xslt:value-of select="@version"/></h1>
			</xslt:if>
			<xslt:apply-templates select="cmd | class"/>
		</xslt:for-each> 
		</body></html>
	</xslt:template>
	
	<xslt:template match="class">
		<h2>
			<xslt:value-of select="@title"/> - 
			<xslt:value-of select="@name"/>
		</h2>
		<xslt:apply-templates select="class" />
		<xslt:apply-templates select="cmd" />
	</xslt:template>
	
	<xslt:template match="cmd">
		<p>
			<xslt:value-of select="@name"/>:
			<xslt:value-of select="@title"/>
			(<xslt:for-each select="*">
				<xslt:choose>
					<xslt:when test="name()='arg'">
						<xslt:choose>
							<xslt:when test="@ref">
								<xslt:for-each select="//arg[@id=./@ref]">
									<xslt:value-of select="@title"/>
								</xslt:for-each>
							</xslt:when>
							<xslt:otherwise>
								<xslt:value-of select="@title"/>
							</xslt:otherwise> 
						</xslt:choose>
					</xslt:when>
					<xslt:when test="name()='args'">
						<xslt:choose>
							<xslt:when test="@ref">
								<xslt:for-each select="//args[@id=./@ref]">
									<xslt:value-of select="@title"/>
								</xslt:for-each>
							</xslt:when>
							<xslt:otherwise>
								<xslt:value-of select="@title"/>
							</xslt:otherwise> 
						</xslt:choose>
					</xslt:when>
					<xslt:when test="name()='info'">
					</xslt:when>
					<xslt:otherwise>!!UNKNOWN!!</xslt:otherwise>
				</xslt:choose>
				<xslt:if test="position()!=last() and name()!='info'">, </xslt:if>
			</xslt:for-each>)</p>
	</xslt:template>
</xslt:stylesheet>
