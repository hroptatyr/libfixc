<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-comp-fld.xsl */

#include "fixml-comp-fld.h"
#include "fixml-msg-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-comp-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-attr-</xsl:text><xsl:value-of select="$versn"/><xsl:text>.h"

fixc_comp_fld_t fixc_get_comp_fld(fixc_ctxt_t ctx)
{
	switch (ctx.ui16) {
</xsl:text>

    <!-- all them sub cases here -->
    <xsl:apply-templates select="fixc:message"/>
    <xsl:apply-templates select="fixc:component"/>

    <xsl:text>
	default: {
		static const struct fixc_comp_fld_s this = {0};
		return &amp;this;
	}
	}
}

/* fixml-comp-fld.c ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message">
    <xsl:text>	case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/><xsl:text>: {
		static const struct fixc_comp_fld_s this = {
			.ctx = </xsl:text>
    <xsl:apply-templates select="." mode="enum"/><xsl:text>,
			.nflds = </xsl:text>
    <xsl:value-of select="count(fixc:field)"/><xsl:text>,
			.flds = {&#0010;</xsl:text>
    <xsl:for-each select="fixc:field">
      <xsl:text>&#0009;&#0009;&#0009;&#0009;</xsl:text>
      <xsl:apply-templates select="key('fldi', @aid)" mode="enum"/>
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

  <xsl:template match="fixc:enum"/>
  <xsl:template match="fixc:field"/>

</xsl:stylesheet>
