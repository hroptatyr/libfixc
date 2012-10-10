<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldx" match="/fixc:spec/fixc:field" use="@fixml"/>

  <xsl:param name="MT"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:choose>
      <!-- dependencies, if any -->
      <xsl:when test="$MT">
        <xsl:apply-templates select="/fixc:spec" mode="deps"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="/fixc:spec" mode="hdr"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- this stylesheet will generate .h files -->
  <xsl:template match="fixc:spec" mode="hdr">
    <xsl:text>/* do not edit, gen'd by fixml-attr-rev.xsl */
#if !defined INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_attr_h_
#define INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_attr_h_&#0010;</xsl:text>

    <xsl:text>
#include "fixml-canon-attr.h"

typedef enum {
	/* first 3 are our own invention */
</xsl:text>

    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_ATTR_UNK,&#0010;</xsl:text>
    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_ATTR_XMLNS = 65535,&#0010;</xsl:text>
    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_ATTR_V = 65534,&#0010;</xsl:text>

    <!-- all the attributes grouped by name -->
    <xsl:for-each select="fixc:field">
      <xsl:text>&#0009;</xsl:text>
      <xsl:apply-templates select="." mode="enum"/>
      <xsl:text> = </xsl:text>
      <xsl:value-of select="@aid"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>} </xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_attr_t;

#endif  /* INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_attr_h_ */&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

</xsl:stylesheet>
