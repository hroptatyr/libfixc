<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>
  <xsl:key name="cmpr"
    match="/fixc:spec/fixc:component"
    use="contains(@type, 'BlockRepeating')"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-comp-rptb.xsl */

#include "fixml-attr.h"
#include "fixml-comp-rptb.h"
#include "fixml-comp.h"

#if defined __INTEL_COMPILER
# pragma warning (disable:869)
#endif  /* __INTEL_COMPILER */

fixc_attr_t
fixc_comp_rptb(fixc_ctxt_t ctx)
{
/* if CTX is a repeating block return its noX attr. */

	switch (ctx.ui16) {
</xsl:text>

    <xsl:for-each select="key('cmpr', true())">
      <xsl:apply-templates select="." mode="case"/>
      <xsl:for-each select="fixc:field">
        <xsl:apply-templates select="key('fldi', @aid)" mode="find-rpta"/>
      </xsl:for-each>
    </xsl:for-each>

    <xsl:for-each select="key('cmpr', false())">
      <xsl:apply-templates select="." mode="case"/>
      <!-- don't look for NumInGroup attrs -->
    </xsl:for-each>
    <xsl:text>
	/* all falling through to */
	default:
		return FIXC_ATTR_UNK;
	}
}

/* fixml-comp-rptb.c ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field[@type='NumInGroup']" mode="find-rpta">
    <xsl:text>&#0009;&#0009;return (fixc_attr_t)</xsl:text>
    <xsl:apply-templates select="." mode="enum"/>
    <xsl:text>;&#0010;</xsl:text>
  </xsl:template>

  <!-- catch all for non-noX fields -->
  <xsl:template match="fixc:field" mode="find-rpta"/>

  <xsl:template match="fixc:component" mode="case">
    <xsl:text>&#0009;case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/>
    <xsl:text>:&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:field" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

</xsl:stylesheet>
