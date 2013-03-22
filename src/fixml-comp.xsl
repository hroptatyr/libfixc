<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

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
    <xsl:text>/* do not edit, gen'd by fixml-comp.xsl */
#if !defined INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_comp_h_
#define INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_comp_h_&#0010;</xsl:text>

    <xsl:text>
#include "fixml-canon-comp.h"

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
    <xsl:value-of select="$versn"/>
    <xsl:text>_comp_t;

#endif  /* INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_comp_h_ */&#0010;</xsl:text>
  </xsl:template>

  <!-- this stylesheet will generate .h files -->
  <xsl:template match="fixc:spec" mode="deps">
      <xsl:text>/* do not edit, gen'd by fixml-comp.xsl */
%{

#include "fixml-canon-comp.h"
#include "fixml-comp-</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>.h"

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name comp
%define hash-function-name </xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>_cid_hash</xsl:text>
      <xsl:text>
%define lookup-function-name </xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>_ciddify</xsl:text>
      <xsl:text>
%null-strings
%includes

struct </xsl:text><xsl:value-of select="$versn"/><xsl:text>_comp_s {
	const char *comp;
	fixc_comp_t cid;
};

%%
FIXML,FIXC_COMP_FIXML
</xsl:text>

    <!-- loop over them components -->
    <xsl:apply-templates select="fixc:component" mode="gperf"/>
  </xsl:template>

  <xsl:template match="fixc:component">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:component" mode="gperf">
    <xsl:if test="@fixml">
      <xsl:value-of select="@fixml"/>
      <xsl:text>,(fixc_comp_t)</xsl:text>
      <xsl:apply-templates select="."/>
      <xsl:text>&#0010;</xsl:text>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
