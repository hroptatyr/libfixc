<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ec="http://exslt.org/common"
  xmlns:s="http://exslt.org/strings"
  extension-element-prefixes="ec s"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <xsl:param name="MT"/>

  <xsl:variable name="Po">
    <xsl:variable name="base" select="substring-before($MT, '.Po')"/>
    <xsl:variable name="pcmp" select="s:tokenize($base, '/')"/>
    <xsl:value-of select="$pcmp[last()]"/>
  </xsl:variable>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:template match="fixc:spec">
    <xsl:choose>
      <!-- dependencies, if any -->
      <xsl:when test="$MT">
        <xsl:apply-templates select="/fixc:spec" mode="deps"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="/fixc:spec" mode="beef"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="fixc:spec" mode="deps">
    <xsl:apply-templates select="fixc:message|fixc:component" mode="gperf"/>

    <ec:document href="{$MT}" method="text">
      <xsl:value-of select="$Po"/>
      <xsl:text>.c: </xsl:text>
      <xsl:for-each select="fixc:message|fixc:component">
        <xsl:text> </xsl:text>
        <xsl:apply-templates select="." mode="deps"/>
        <xsl:text>.c</xsl:text>
      </xsl:for-each>
      <xsl:text>&#0010;</xsl:text>

      <xsl:text>&#0010;</xsl:text>
      <xsl:for-each select="fixc:message|fixc:component">
        <xsl:text>&#0010;</xsl:text>
        <xsl:variable name="stem">
          <xsl:apply-templates select="." mode="deps"/>
        </xsl:variable>
        <xsl:value-of select="$stem"/>
        <xsl:text>.c: </xsl:text>
        <xsl:value-of select="$stem"/>
        <xsl:text>.gperf&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:text>&#0010;</xsl:text>
    </ec:document>

    <xsl:for-each select="fixc:message|fixc:component">
      <xsl:variable name="stem">
        <xsl:apply-templates select="." mode="deps"/>
      </xsl:variable>
      <xsl:value-of select="$stem"/>
      <xsl:text>.c&#0009;</xsl:text>
      <xsl:value-of select="$stem"/>
      <xsl:text>.gperf&#0010;</xsl:text>
    </xsl:for-each>
  </xsl:template>

  <!-- this stylesheet will generate gperf files -->
  <xsl:template match="fixc:spec" mode="beef">
    <!-- build up the main .c file that switches over the context -->
    <xsl:text>/* do not edit, gen'd by fixml-attr-by-ctx.xsl */

#include "fixml-msg-</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-comp-</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>.h"
#include "fixml-attr-by-ctx.h"

#if defined __INTEL_COMPILER
# pragma warning (disable:869)
#endif  /* __INTEL_COMPILER */
#include "fixml-canon-attr.c"
</xsl:text>

    <xsl:apply-templates select="fixc:message|fixc:component" mode="include"/>

    <!-- now the switch -->
    <xsl:text>/* warn about 869 again */
#if defined __INTEL_COMPILER
# pragma warning (default:869)
#endif  /* __INTEL_COMPILER */

fixc_attr_t fixc_get_aid(fixc_ctxt_t ctx, const char *attr, size_t alen)
{
/* obtain the aid that belongs to ATTR (of size ALEN) in context CTX. */
	/* go through implicit blocks */
	switch (ctx.ui16) {
</xsl:text>
    <!-- now come the weirdo cases -->
    <xsl:apply-templates select="fixc:message|fixc:component" mode="impblock"/>
    <xsl:text>
	default:
		break;
	}
	/* go through the real list */
	switch (ctx.ui16) {
</xsl:text>
    <!-- now come the real cases -->
    <xsl:apply-templates select="fixc:message|fixc:component" mode="case"/>
   <xsl:text>
	default: {
		const struct UNK_s *p = __aiddify_UNK(attr, alen);
		return p != NULL ? p->aid : FIXC_ATTR_UNK;
	}
	}
}

/* gen'd file ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="gperf">
    <xsl:variable name="outfn">
      <xsl:apply-templates select="." mode="deps"/>
      <xsl:text>.gperf</xsl:text>
    </xsl:variable>

    <ec:document href="./{$outfn}" method="text">
      <xsl:text>/* do not edit, gen'd by fixml-attr-by-ctx.xsl */
%{

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name attr
%define hash-function-name __aid_hash_</xsl:text>
      <xsl:apply-templates select="." mode="enum"/>
      <xsl:text>
%define lookup-function-name </xsl:text>
      <xsl:apply-templates select="." mode="hashfn"/>
      <xsl:text>
%null-strings
%includes

</xsl:text>
      <xsl:apply-templates select="." mode="struct"/><xsl:text> {
	const char *attr;
	fixc_attr_t aid;
};

%%
xmlns,FIXC_ATTR_XMLNS
v,FIXC_ATTR_V
xv,FIXC_ATTR_XV
</xsl:text>

      <!-- loop over them fields again -->
      <xsl:apply-templates select="." mode="beef"/>
    </ec:document>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="beef">
    <xsl:for-each select="fixc:field">
      <xsl:apply-templates select="key('fldi', @aid)" mode="map">
        <xsl:with-param name="cat" select="../@cat"/>
      </xsl:apply-templates>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="include">
    <xsl:text>#include "</xsl:text>
    <xsl:apply-templates select="." mode="deps"/>
    <xsl:text>.c"&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="case">
    <xsl:text>&#0009;case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/>
    <xsl:text>: {
		const </xsl:text>
    <xsl:apply-templates select="." mode="struct"/>
    <xsl:text> *p = </xsl:text>
    <xsl:apply-templates select="." mode="hashfn"/><xsl:text>(attr, alen);
		return p != NULL ? p->aid : FIXC_ATTR_UNK;
	}
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="impblock"/>

  <!-- special case for optimised implicit blocks -->
  <xsl:template
    match="fixc:component[@type='OptimisedImplicitBlockRepeating']"
    mode="case"/>

  <xsl:template
    match="fixc:component[@type='OptimisedImplicitBlockRepeating']"
    mode="impblock">
    <xsl:text>
	/* optimised implicit block */
	case </xsl:text>
    <xsl:apply-templates select="." mode="enum"/>
    <xsl:text>: {
		const </xsl:text>
    <xsl:apply-templates select="." mode="struct"/>
    <xsl:text> *p = </xsl:text>
    <xsl:apply-templates select="." mode="hashfn"/><xsl:text>(attr, alen);
		if (p != NULL) {
			return p->aid;
		}
		/* otherwise try that sub thing */
	}
</xsl:text>
    <xsl:for-each select="fixc:component">
      <xsl:apply-templates select="." mode="case"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="deps">
    <xsl:value-of select="fixc:vprefix(.)"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="enum">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="venum">
    <xsl:value-of select="fixc:ucase(fixc:vprefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="hashfn">
    <xsl:text>__aiddify_</xsl:text>
    <xsl:apply-templates select="." mode="venum"/>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="struct">
    <xsl:text>struct </xsl:text>
    <xsl:apply-templates select="." mode="venum"/>
    <xsl:text>_s</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field" mode="enum">
    <xsl:text>__ATTR_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:field" mode="map">
    <xsl:param name="cat"/>
    <xsl:variable name="aid" select="@aid"/>

    <xsl:if test="string-length(@fixml) &gt; 0">
      <xsl:for-each select=".|fixc:alias[@cat=$cat and @fixml!=../@fixml]">
        <xsl:value-of select="@fixml"/>
        <xsl:text>,(fixc_attr_t)</xsl:text>
        <xsl:value-of select="$aid"/>
        <xsl:text>&#0010;</xsl:text>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
