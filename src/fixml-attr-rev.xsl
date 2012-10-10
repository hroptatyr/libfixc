<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldx" match="/fixc:spec/fixc:field" use="@fixml"/>
  <xsl:key name="catx"
    match="/fixc:spec/fixc:component|/fixc:spec/fixc:message" use="@cat"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-attr-rev.xsl */

#include "fixml-canon-attr.h"
#include "fixml-attr.h"

const char *fixc_attr_fixmlify(fixc_ctxt_t ctx, fixc_attr_t aid)
{
	switch (aid) {
</xsl:text>

    <!-- all the attributes grouped by name -->
    <xsl:for-each select="fixc:field[count(. | key('fldx', @fixml)[1]) = 1]">
      <xsl:for-each select="key('fldx', @fixml)">
        <xsl:text>&#0009;case </xsl:text>
        <xsl:apply-templates select="." mode="enum"/>
        <xsl:text>:&#0010;</xsl:text>
        <xsl:if test="fixc:alias">
          <xsl:text>&#0009;&#0009;switch (ctx.ui16) {&#0010;</xsl:text>
          <xsl:for-each select="fixc:alias">
            <xsl:for-each select="key('catx', @cat)">
              <xsl:text>&#0009;&#0009;case </xsl:text>
              <xsl:apply-templates select="." mode="enum"/>
              <xsl:text>:&#0010;</xsl:text>
            </xsl:for-each>
            <xsl:text>&#0009;&#0009;&#0009;return "</xsl:text>
            <xsl:value-of select="@fixml"/>
            <xsl:text>";&#0010;</xsl:text>
          </xsl:for-each>
          <xsl:text>
		default:
			break;
		}
</xsl:text>
        </xsl:if>
      </xsl:for-each>
      <xsl:text>&#0009;&#0009;return "</xsl:text>
      <xsl:value-of select="@fixml"/>
      <xsl:text>";&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>
	/* special attrs, not officially encountered */
	case FIXC_ATTR_XMLNS:
		return "xmlns";
	case FIXC_ATTR_V:
		return "v";

	case FIXC_ATTR_UNK:
	default:
		return "";
	}
}

/* fixml-attr-rev.c ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:message|fixc:component" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

</xsl:stylesheet>