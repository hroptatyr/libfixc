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
      select="translate(concat($versn, '_msg'),
              'QWERTYUIOPASDFGHJKLZXCVBNM',
              'qwertyuiopasdfghjklzxcvbnm')"/>
    <xsl:variable name="PREFX"
      select="translate($prefx,
              'qwertyuiopasdfghjklzxcvbnm',
              'QWERTYUIOPASDFGHJKLZXCVBNM')"/>

    <ec:document href="{$prefx}.h" method="text">
      <xsl:text>/* do not edit, gen'd by fixml-msg.xsl */
#if !defined INCLUDED_</xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_h_
#define INCLUDED_</xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_h_&#0010;</xsl:text>

      <xsl:text>
#undef _
#define _(x)    ((x)[0] * 256 + (x)[1])

typedef enum {
	/* first two are our own invention */
</xsl:text>

      <xsl:text>&#0009;</xsl:text>
      <xsl:call-template name="msg">
        <xsl:with-param name="prefix" select="$PREFX"/>
        <xsl:with-param name="name">
          <xsl:text>UNK</xsl:text>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:text>,&#0010;&#0009;</xsl:text>
      <xsl:call-template name="msg">
        <xsl:with-param name="prefix" select="$PREFX"/>
        <xsl:with-param name="name">
          <xsl:text>BATCH</xsl:text>
        </xsl:with-param>
        <xsl:with-param name="valnum" select="'0xffff'"/>
      </xsl:call-template>
      <xsl:text>,&#0010;</xsl:text>

      <xsl:for-each select="fixc:message">
        <xsl:text>&#0009;</xsl:text>
        <xsl:call-template name="msg">
          <xsl:with-param name="prefix" select="$PREFX"/>
          <xsl:with-param name="name" select="@name"/>
          <xsl:with-param name="valstr" select="@fix"/>
        </xsl:call-template>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>

      <xsl:text>} </xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_t;

#undef _

#endif  /* INCLUDED_</xsl:text>
      <xsl:value-of select="$prefx"/>
      <xsl:text>_h_ */&#0010;</xsl:text>
    </ec:document>
  </xsl:template>

  <xsl:template name="msg">
    <xsl:param name="prefix"/>
    <xsl:param name="name"/>
    <xsl:param name="valstr"/>
    <xsl:param name="valnum"/>

    <xsl:value-of select="$prefix"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="$name"/>
    <xsl:choose>
      <xsl:when test="$valnum">
        <xsl:text> = </xsl:text>
        <xsl:value-of select="$valnum"/>
      </xsl:when>
      <xsl:when test="$valstr">
        <xsl:text> = _("</xsl:text>
        <xsl:value-of select="$valstr"/>
        <xsl:text>")</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
