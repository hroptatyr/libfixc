<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:key name="compf" match="fixc:component/fixc:field" use="@aid"/>
  <xsl:key name="msgf" match="fixc:message/fixc:field" use="@aid"/>

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-fld-ctx.xsl */&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>
    <xsl:text>#include "fixml-fld-ctx.h"&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>

    <xsl:text>fixc_fld_ctx_t fixc_get_fld_ctx(uint16_t fld)&#0010;</xsl:text>
    <xsl:text>{&#0010;</xsl:text>
    <xsl:text>&#0009;switch (fld) {&#0010;</xsl:text>
    <xsl:apply-templates select="fixc:field"/>
    <xsl:text>&#0009;default: {&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;static const struct fixc_fld_ctx_s this = {0};&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;return &amp;this;&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
    <xsl:text>}&#0010;</xsl:text>
  </xsl:template>

  <xsl:template name="genctx">
    <xsl:param name="node"/>
    <xsl:param name="tag"/>

    <xsl:variable name="aid" select="$node/@aid"/>

    <xsl:text>&#0009;case </xsl:text>
    <xsl:value-of select="$aid"/>
    <xsl:text>/*</xsl:text>
    <xsl:value-of select="$tag"/>
    <xsl:text>*/: {&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;static const struct fixc_fld_ctx_s this = {&#0010;</xsl:text>

    <xsl:text>&#0009;&#0009;&#0009;.fld = </xsl:text>
    <xsl:value-of select="$aid"/>
    <xsl:text>/*</xsl:text>
    <xsl:value-of select="$tag"/>
    <xsl:text>*/,&#0010;</xsl:text>

    <xsl:text>&#0009;&#0009;&#0009;.nctxs = </xsl:text>
    <xsl:for-each select="/fixc:spec">
      <xsl:value-of select="count(key('msgf', $aid)) + count(key('compf', $aid))"/>
      <xsl:text>,&#0010;</xsl:text>

      <xsl:text>&#0009;&#0009;&#0009;.ctxs = {&#0010;</xsl:text>
      <xsl:for-each select="key('msgf', $aid)">
        <xsl:text>&#0009;&#0009;&#0009;&#0009;FIXC_MSGTYP_</xsl:text>
        <xsl:value-of select="../@name"/>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:for-each select="key('compf', $aid)">
        <xsl:text>&#0009;&#0009;&#0009;&#0009;FIXC_COMP_</xsl:text>
        <xsl:value-of select="../@fixml"/>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>
    </xsl:for-each>
    <xsl:text>&#0009;&#0009;&#0009;},&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;};&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;return &amp;this;&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field">
    <xsl:call-template name="genctx">
      <xsl:with-param name="node" select="."/>
      <xsl:with-param name="tag">
        <xsl:text>FIXC_ATTR_</xsl:text>
        <xsl:value-of select="@fixml"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="fixc:message"/>
  <xsl:template match="fixc:component"/>
  <xsl:template match="fixc:enum"/>

</xsl:stylesheet>
