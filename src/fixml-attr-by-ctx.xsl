<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ec="http://exslt.org/common"
  extension-element-prefixes="ec"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <xsl:param name="MT"/>

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
      <xsl:text>fixml-attr-by-ctx.c: </xsl:text>
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

#include "</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>-msg.h"
#include "</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>-comp.h"
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
	switch (ctx.ui16) {
</xsl:text>
    <!-- now come the cases -->
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
      <xsl:value-of select="fixc:prefix(.)"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="@name"/>
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
      <xsl:value-of select="@name"/>
      <xsl:text>
%define lookup-function-name __aiddify_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>
%null-strings
%includes

struct </xsl:text><xsl:value-of select="@name"/><xsl:text>_s {
	const char *attr;
	fixc_attr_t aid;
};

%%
xmlns,FIXC_ATTR_XMLNS
v,FIXC_ATTR_V
</xsl:text>

      <!-- loop over them fields again -->
      <xsl:for-each select="fixc:field">
        <xsl:apply-templates select="key('fldi', @aid)" mode="map">
          <xsl:with-param name="cat" select="../@cat"/>
        </xsl:apply-templates>
      </xsl:for-each>
    </ec:document>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="include">
    <xsl:variable name="infn">
      <xsl:value-of select="fixc:prefix(.)"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>.c</xsl:text>
    </xsl:variable>

    <xsl:text>#include "</xsl:text>
    <xsl:value-of select="$infn"/>
    <xsl:text>"&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="case">
    <xsl:text>&#0009;case </xsl:text>
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>: {
		const struct </xsl:text>
    <xsl:value-of select="@name"/><xsl:text>_s *p = __aiddify_</xsl:text>
    <xsl:value-of select="@name"/><xsl:text>(attr, alen);
		return p != NULL ? p->aid : FIXC_ATTR_UNK;
	}
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="deps">
    <xsl:value-of select="fixc:prefix(.)"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
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
