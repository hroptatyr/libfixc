<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:key name="cmpf" match="/fixc:spec/fixc:component/fixc:field" use="@aid"/>
  <xsl:key name="msgf" match="/fixc:spec/fixc:message/fixc:field" use="@aid"/>

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-fld-ctx.xsl */

#include "fixml-fld-ctx.h"
#include "fixml-msg-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-comp-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-attr-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"

fixc_fld_ctx_t fixc_get_fld_ctx(fixc_ctxt_t ctx)
{
	switch (ctx.ui16) {
</xsl:text>

    <!-- all them sub cases here -->
    <xsl:apply-templates select="fixc:field"/>

    <xsl:text>
	default: {
		static const struct fixc_fld_ctx_s this = {0};
		return &amp;this;
	}
	}
}

/* fixml-fld-ctx.c ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field">
    <xsl:text>	case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/><xsl:text>: {
		static const struct fixc_fld_ctx_s this = {
			.fld = </xsl:text>
    <xsl:apply-templates select="." mode="enum"/><xsl:text>,
			.nmsgs = </xsl:text>
    <xsl:value-of select="count(key('msgf', @aid))"/><xsl:text>,
			.ncomps = </xsl:text>
    <xsl:value-of select="count(key('cmpf', @aid))"/><xsl:text>,
			.ctxs = {&#0010;</xsl:text>
    <xsl:for-each select="key('msgf', @aid)">
      <xsl:text>&#0009;&#0009;&#0009;&#0009;</xsl:text>
      <xsl:apply-templates select=".." mode="enum"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>
    <xsl:for-each select="key('cmpf', @aid)">
      <xsl:text>&#0009;&#0009;&#0009;&#0009;</xsl:text>
      <xsl:apply-templates select=".." mode="enum"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each><xsl:text>			},
		};
		return &amp;this;
	}
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message|fixc:field" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:message"/>
  <xsl:template match="fixc:component"/>
  <xsl:template match="fixc:enum"/>

</xsl:stylesheet>
