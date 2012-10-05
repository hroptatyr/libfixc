<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ec="http://exslt.org/common"
  extension-element-prefixes="ec"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:param name="MT"/>

  <xsl:key name="cmpi" match="/fixc:spec/fixc:component" use="@name"/>

  <xsl:include href="libfixc_0_1_funs.xsl"/>

  <xsl:variable name="subc_ns"
    select="/fixc:spec/fixc:message[fixc:component]|
            /fixc:spec/fixc:component[fixc:component]"/>

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
    <xsl:apply-templates select="$subc_ns" mode="gperf"/>

    <ec:document href="{$MT}" method="text">
      <xsl:text>fixml-comp-by-ctx.c: </xsl:text>
      <xsl:for-each select="$subc_ns">
        <xsl:text> </xsl:text>
        <xsl:apply-templates select="." mode="deps"/>
        <xsl:text>.c</xsl:text>
      </xsl:for-each>
      <xsl:text>&#0010;</xsl:text>

      <xsl:text>&#0010;</xsl:text>
      <xsl:for-each select="$subc_ns">
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

    <xsl:for-each select="$subc_ns">
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
    <xsl:text>/* do not edit, gen'd by fixml-comp-by-ctx.xsl */

#include "</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>-msg.h"
#include "</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>-comp.h"
#include "fixml-comp-by-ctx.h"

#if defined __INTEL_COMPILER
# pragma warning (disable:869)
# pragma warning (disable:111)
#endif  /* __INTEL_COMPILER */
</xsl:text>

    <xsl:apply-templates select="$subc_ns" mode="include"/>

    <!-- now the switch -->
    <xsl:text>/* warn about 869 again */
#if defined __INTEL_COMPILER
# pragma warning (default:869)
# pragma warning (default:111)
#endif  /* __INTEL_COMPILER */

fixc_comp_t fixc_get_cid(fixc_ctxt_t ctx, const char *elem, size_t elen)
{
/* obtain the cid that belongs to ELEM (of size ELEN) in context CTX. */
	switch (ctx.ui16) {
</xsl:text>
    <!-- now come the cases -->
    <xsl:for-each select="$subc_ns">
      <xsl:text>&#0009;case </xsl:text>
      <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>: {
		const struct subc_</xsl:text>
      <xsl:value-of select="@name"/><xsl:text>_s *p = __ciddify_</xsl:text>
      <xsl:value-of select="@name"/><xsl:text>(elem, elen);
		return p != NULL ? p->cid : FIXC_COMP_UNK;
	}
</xsl:text>
    </xsl:for-each>
    <!-- all them contexts without a sub-component -->
    <xsl:for-each select="fixc:message[count(fixc:component) = 0]|
                          fixc:component[count(fixc:component) = 0]">
      <xsl:text>&#0009;case </xsl:text>
      <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>:&#0010;</xsl:text>
    </xsl:for-each>
    <xsl:text>&#0009;default:
		return FIXC_COMP_UNK;

	case 0:
		/* and one more special case for the root */
		if (*elem == 'F' &amp;&amp; !strcmp(elem + 1, "IXML")) {
			return FIXC_COMP_FIXML;
		}
		return FIXC_COMP_UNK;
	}
}

/* gen'd file ends here */
</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="gperf">
    <xsl:variable name="outfn">
      <xsl:value-of select="$versn"/>
      <xsl:text>_subc_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>.gperf</xsl:text>
    </xsl:variable>

    <ec:document href="./{$outfn}" method="text">
      <xsl:text>/* do not edit, gen'd by fixml-comp-by-ctx.xsl */
%{

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name comp
%define hash-function-name __cid_hash_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>
%define lookup-function-name __ciddify_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>
%null-strings
%includes

struct subc_</xsl:text><xsl:value-of select="@name"/><xsl:text>_s {
	const char *comp;
	fixc_comp_t cid;
};

%%
</xsl:text>

      <!-- loop over them fields again -->
      <xsl:for-each select="fixc:component">
        <xsl:apply-templates select="key('cmpi', @name)" mode="map"/>
      </xsl:for-each>
    </ec:document>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="include">
    <xsl:variable name="infn">
      <xsl:value-of select="$versn"/>
      <xsl:text>_subc_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>.c</xsl:text>
    </xsl:variable>

    <xsl:text>#include "</xsl:text>
    <xsl:value-of select="$infn"/>
    <xsl:text>"&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="deps">
    <xsl:value-of select="$versn"/>
    <xsl:text>_subc_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:component" mode="map">
    <xsl:if test="string-length(@fixml) &gt; 0">
      <xsl:value-of select="@fixml"/>
      <xsl:text>,(fixc_comp_t)</xsl:text>
      <xsl:value-of select="$VERSN"/>
      <xsl:text>_COMP_</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>&#0010;</xsl:text>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
