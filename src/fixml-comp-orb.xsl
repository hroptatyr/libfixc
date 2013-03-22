<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:key name="comporb"
    match="/fixc:spec/fixc:component"
    use="@type ='OptimisedImplicitBlockRepeating'"/>

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-comp-orb.xsl */

#include "fixml-comp-</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-comp-orb.h"

fixc_comp_t fixc_get_comp_orb(fixc_ctxt_t ctx)
{
	switch (ctx.ui16) {
</xsl:text>

    <!-- all them ORBs here -->
    <xsl:apply-templates select="key('comporb', true())" mode="orb"/>

    <!-- all non-ORBs here -->
    <xsl:apply-templates select="key('comporb', false())" mode="case"/>

    <xsl:text>	default:
		return FIXC_COMP_UNK;
	}
}

/* fixml-comp-orb.c ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component" mode="orb">
    <xsl:apply-templates select="." mode="case"/>
    <xsl:text>		return (fixc_comp_t)</xsl:text>
    <xsl:apply-templates select="fixc:component[1]" mode="enum"/><xsl:text>;
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component" mode="case">
    <xsl:text>	case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/><xsl:text>:
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:enum"/>
  <xsl:template match="fixc:field"/>

</xsl:stylesheet>
