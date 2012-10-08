<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldx" match="/fixc:spec/fixc:field" use="@fixml"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-attr-rev.xsl */

#include "fixml-canon-attr.h"

const char *fixc_attr_fixmlify(fixc_attr_t aid)
{
	switch (aid) {
</xsl:text>

    <!-- all the attributes grouped by name -->
    <xsl:for-each select="fixc:field[count(. | key('fldx', @fixml)[1]) = 1]">
      <xsl:for-each select="key('fldx', @fixml)">
        <xsl:text>&#0009;case </xsl:text>
        <xsl:value-of select="@aid"/>
        <xsl:text>:&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:text>&#0009;&#0009;return "</xsl:text>
      <xsl:value-of select="@fixml"/>
      <xsl:text>";&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>
	case FIXC_ATTR_UNK:
	default:
		return "";
	}
}

/* fixml-attr-rev.c ends here */
</xsl:text>
  </xsl:template>

</xsl:stylesheet>