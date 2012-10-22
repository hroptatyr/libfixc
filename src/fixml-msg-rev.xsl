<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="msgx" match="/fixc:spec/fixc:message" use="@fixml"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:text>/* do not edit, gen'd by fixml-msg-rev.xsl */

#include "fixml-canon-msgt.h"
#include "fixml-msg.h"

const char *fixc_msgt_fixmlify(fixc_msgt_t mty)
{
	switch ((unsigned int)mty) {
</xsl:text>

    <!-- all the attributes grouped by name -->
    <xsl:for-each select="fixc:message[count(. | key('msgx', @fixml)[1]) = 1]">
      <xsl:for-each select="key('msgx', @fixml)">
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
	case FIXC_MSGT_BATCH:
		return "Batch";

	case FIXC_MSGT_UNK:
	default:
		return "";
	}
}

/* fixml-msg-rev.c ends here */
</xsl:text>
  </xsl:template>

</xsl:stylesheet>