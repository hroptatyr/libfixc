<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <!-- this stylesheet will generate .h files -->
  <xsl:template match="fixc:spec">
    <xsl:variable name="prefx" select="fixc:prefix(.)"/>
    <xsl:variable name="PREFX" select="fixc:prefix(.)"/>

    <xsl:text>/* do not edit, gen'd by fixml-comp.xsl */
#if !defined INCLUDED_</xsl:text>
    <xsl:value-of select="$prefx"/>
    <xsl:text>_h_
#define INCLUDED_</xsl:text>
    <xsl:value-of select="$prefx"/>
    <xsl:text>_h_&#0010;</xsl:text>

    <xsl:text>
typedef enum {
	/* first two are our own invention */
</xsl:text>

    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_COMP_UNK,&#0010;</xsl:text>
    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_COMP_FIXML,&#0010;</xsl:text>

    <xsl:for-each select="fixc:component">
      <xsl:text>&#0009;</xsl:text>
      <xsl:apply-templates select="."/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>} </xsl:text>
    <xsl:value-of select="$prefx"/>
    <xsl:text>_t;

#endif  /* INCLUDED_</xsl:text>
    <xsl:value-of select="$prefx"/>
    <xsl:text>_h_ */&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

</xsl:stylesheet>
