<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ec="http://exslt.org/common"
  extension-element-prefixes="ec"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <!-- this stylesheet will generate gperf files -->
  <xsl:template match="fixc:spec">
    <xsl:apply-templates select="fixc:message|fixc:component" mode="gperf"/>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="gperf">
    <xsl:variable name="versn" select="translate(/fixc:spec/@version, '._', '')"/>
    <xsl:variable name="infix">
      <xsl:choose>
        <xsl:when test="name(.) = 'component'">
          <xsl:text>comp</xsl:text>
        </xsl:when>
        <xsl:when test="name(.) = 'message'">
          <xsl:text>msg</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>unk</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="prefx"
      select="translate(concat($versn, '_', $infix),
              'QWERTYUIOPASDFGHJKLZXCVBNM',
              'qwertyuiopasdfghjklzxcvbnm')"/>
    <xsl:variable name="PREFX"
      select="translate($prefx,
              'qwertyuiopasdfghjklzxcvbnm',
              'QWERTYUIOPASDFGHJKLZXCVBNM')"/>

    <xsl:variable name="outfn">
      <xsl:value-of select="$prefx"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>.gperf</xsl:text>
    </xsl:variable>

    <ec:document href="{$outfn}" method="text">
      <xsl:text>/* do not edit, gen'd by fixml-attr-by-ctx.xsl */
%{

enum __attr_e {
        __ATTR_XMLNS = FIXC_ATTR_XMLNS,
        __ATTR_V = FIXC_ATTR_V,
</xsl:text>
      <!-- loop over them fields -->
      <xsl:for-each select="fixc:field">
        <xsl:text>&#0009;</xsl:text>
        <xsl:apply-templates select="key('fldi', @aid)" mode="enum"/>
        <xsl:text> = </xsl:text>
        <xsl:value-of select="@aid"/>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:text>
};

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name attr
%null-strings

struct {
	const char *attr;
	enum __attr_e aid;
};

%%
xmlns,__ATTR_XMLNS
v,__ATTR_V
</xsl:text>

      <!-- loop over them fields again -->
      <xsl:for-each select="fixc:field">
        <xsl:apply-templates select="key('fldi', @aid)" mode="map"/>
        <xsl:text>&#0010;</xsl:text>
      </xsl:for-each>
    </ec:document>
  </xsl:template>



  <xsl:template match="fixc:field" mode="enum">
    <xsl:text>__ATTR_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:field" mode="map">
    <xsl:value-of select="@fixml"/>
    <xsl:text>,</xsl:text>
    <xsl:apply-templates select="." mode="enum"/>
  </xsl:template>

</xsl:stylesheet>
