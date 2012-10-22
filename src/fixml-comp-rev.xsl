<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="cmpx" match="/fixc:spec/fixc:component" use="@fixml"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-comp-rev.xsl */

#include "fixml-canon-attr.h"
#include "fixml-comp.h"

const char *fixc_comp_fixmlify(fixc_comp_t cid)
{
	switch ((unsigned int)cid) {
</xsl:text>

    <!-- all the attributes grouped by name -->
    <xsl:for-each select="fixc:component[count(. | key('cmpx', @fixml)[1]) = 1]">
      <xsl:for-each select="key('cmpx', @fixml)">
        <xsl:text>&#0009;case </xsl:text>
        <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
        <xsl:text>_</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>:&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:text>&#0009;&#0009;return "</xsl:text>
      <xsl:value-of select="@fixml"/>
      <xsl:text>";&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>
	/* special component, doesn't exist officially */
	case FIXC_COMP_FIXML:
		return "FIXML";

	case FIXC_COMP_UNK:
	default:
		return "";
	}
}

/* fixml-comp-rev.c ends here */
</xsl:text>
  </xsl:template>

</xsl:stylesheet>