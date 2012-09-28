<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:key name="compn" match="fixc:component" use="@name"/>
  <xsl:key name="compf" match="fixc:component" use="@fixml"/>

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-comp-sub.xsl */&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>
    <xsl:text>#include "fixml-comp-sub.h"&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>
    <xsl:text>&#0010;</xsl:text>

    <xsl:text>fixc_comp_sub_t fixc_get_comp_sub(uint16_t ctx)&#0010;</xsl:text>
    <xsl:text>{&#0010;</xsl:text>
    <xsl:text>&#0009;switch (ctx) {&#0010;</xsl:text>
    <xsl:apply-templates select="fixc:message"/>
    <xsl:apply-templates select="fixc:component[
                                 count(. | key('compf', @fixml)[1]) = 1 and
                                 @name != 'StandardHeader' and
                                 @name != 'StandardTrailer' and
                                 @name != 'HopGrp']"/>
    <xsl:text>&#0009;default: {&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;static const struct fixc_comp_sub_s this = {0};&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;return &amp;this;&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
    <xsl:text>}&#0010;</xsl:text>
  </xsl:template>

  <xsl:template name="gensub">
    <xsl:param name="node"/>
    <xsl:param name="tag"/>

    <xsl:text>&#0009;case </xsl:text>
    <xsl:value-of select="$tag"/>
    <xsl:text>: {&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;static const struct fixc_comp_sub_s this = {&#0010;</xsl:text>

    <xsl:text>&#0009;&#0009;&#0009;.ctx = </xsl:text>
    <xsl:value-of select="$tag"/>
    <xsl:text>,&#0010;</xsl:text>

    <xsl:text>&#0009;&#0009;&#0009;.nsubs = </xsl:text>
    <xsl:value-of select="count($node/fixc:component[
                          @name != 'StandardHeader' and
                          @name != 'StandardTrailer'])"/>
    <xsl:text>,&#0010;</xsl:text>

    <xsl:text>&#0009;&#0009;&#0009;.subs = {&#0010;</xsl:text>
    <xsl:for-each select="$node/fixc:component[
                          @name != 'StandardHeader' and
                          @name != 'StandardTrailer']">
      <xsl:variable name="name" select="@name"/>
      <xsl:text>&#0009;&#0009;&#0009;&#0009;FIXC_COMP_</xsl:text>
      <xsl:value-of select="/fixc:spec/fixc:component[@name=$name]/@fixml"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>
    <xsl:text>&#0009;&#0009;&#0009;},&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;};&#0010;</xsl:text>
    <xsl:text>&#0009;&#0009;return &amp;this;&#0010;</xsl:text>
    <xsl:text>&#0009;}&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:message">
    <xsl:call-template name="gensub">
      <xsl:with-param name="node" select="."/>
      <xsl:with-param name="tag">
        <xsl:text>FIXC_MSGTYP_</xsl:text>
        <xsl:choose>
          <xsl:when test="@name = 'XMLnonFIX'">
            <!-- cheat the uncameller -->
            <xsl:text>XmlNonFix</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@name"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="fixc:component">
    <xsl:call-template name="gensub">
      <xsl:with-param name="node" select="."/>
      <xsl:with-param name="tag">
        <xsl:text>FIXC_COMP_</xsl:text>
        <xsl:value-of select="@fixml"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="fixc:enum"/>
  <xsl:template match="fixc:field"/>

</xsl:stylesheet>
