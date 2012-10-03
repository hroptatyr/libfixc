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

  <!-- this stylesheet will generate .h files -->
  <xsl:template match="fixc:spec">
    <xsl:variable name="versn" select="translate(@version, '._', '')"/>
    <xsl:variable name="prefx"
      select="translate(concat($versn, '_comp'),
              'QWERTYUIOPASDFGHJKLZXCVBNM',
              'qwertyuiopasdfghjklzxcvbnm')"/>
    <xsl:variable name="PREFX"
      select="translate($prefx,
              'qwertyuiopasdfghjklzxcvbnm',
              'QWERTYUIOPASDFGHJKLZXCVBNM')"/>

    <ec:document href="{$prefx}.h" method="text">
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
      <xsl:call-template name="comp">
        <xsl:with-param name="PREFX" select="$PREFX"/>
        <xsl:with-param name="name">
          <xsl:text>UNK</xsl:text>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:text>,&#0010;&#0009;</xsl:text>
      <xsl:call-template name="comp">
        <xsl:with-param name="PREFX" select="$PREFX"/>
        <xsl:with-param name="name">
          <xsl:text>FIXML</xsl:text>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:text>,&#0010;</xsl:text>

      <xsl:for-each select="fixc:component">
        <xsl:text>&#0009;</xsl:text>
        <xsl:call-template name="comp">
          <xsl:with-param name="PREFX" select="$PREFX"/>
          <xsl:with-param name="name" select="@name"/>
        </xsl:call-template>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>

      <xsl:text>} </xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_t;

#endif  /* INCLUDED_</xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_h_ */&#0010;</xsl:text>
    </ec:document>
  </xsl:template>

  <xsl:template name="comp">
    <xsl:param name="PREFX"/>
    <xsl:param name="name"/>

    <xsl:value-of select="$PREFX"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="$name"/>
  </xsl:template>

</xsl:stylesheet>
