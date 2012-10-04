<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ec="http://exslt.org/common"
  xmlns:fn="http://exslt.org/functions"
  extension-element-prefixes="ec fn"
  version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:key name="fldi" match="/fixc:spec/fixc:field" use="@aid"/>

  <xsl:param name="MT"/>

  <xsl:variable name="_versn" select="translate(/fixc:spec/@version, '._', '')"/>
  <xsl:variable name="versn" select="fixc:lcase($_versn)"/>
  <xsl:variable name="VERSN" select="fixc:ucase($_versn)"/>

  <fn:function name="fixc:ucase">
    <xsl:param name="str"/>
    <fn:result
      select="translate($str,
              'qwertyuiopasdfghjklzxcvbnm',
              'QWERTYUIOPASDFGHJKLZXCVBNM')"/>
  </fn:function>

  <fn:function name="fixc:lcase">
    <xsl:param name="str"/>
    <fn:result
      select="translate($str,
              'QWERTYUIOPASDFGHJKLZXCVBNM',
              'qwertyuiopasdfghjklzxcvbnm')"/>
  </fn:function>

  <fn:function name="fixc:prefix">
    <xsl:param name="node-set"/>
    <xsl:variable name="infix">
      <xsl:choose>
        <xsl:when test="name($node-set) = 'component'">
          <xsl:text>comp</xsl:text>
        </xsl:when>
        <xsl:when test="name($node-set) = 'message'">
          <xsl:text>msg</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>unk</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <fn:result select="concat($versn, '_', $infix)"/>
  </fn:function>

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
      <xsl:text>fixml-attr-by-ctx.c:</xsl:text>
      <xsl:apply-templates select="fixc:message|fixc:component" mode="deps"/>
      <xsl:text>&#0010;</xsl:text>
    </ec:document>
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
    <xsl:text>-comp.h"&#0010;&#0010;</xsl:text>

    <xsl:apply-templates select="fixc:message|fixc:component" mode="include"/>

    <!-- now the switch -->
    <xsl:text>
fixc_attr_t fixc_get_aid(
	union __attribute__((transparent_union)) {
		uint16_t ctx;
		fixc_comp_t comp;
		fixc_msg_type_t msg;
	}, const char *attr, size_t alen)
{
/* obtain the aid that belongs to ATTR (of size ALEN) in context CTX. */
	switch (ctx) {
</xsl:text>
    <!-- now come the cases -->
    <xsl:apply-templates select="fixc:message|fixc:component" mode="case"/>
   <xsl:text>
	default:
		return FIXC_ATTR_UNK;
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

enum __attr_e {
        __ATTR_XMLNS = FIXC_ATTR_XMLNS,
        __ATTR_V = FIXC_ATTR_V,
</xsl:text>
      <!-- loop over them fields -->
      <xsl:for-each select="fixc:field">
        <xsl:text>&#0009;</xsl:text>
        <xsl:apply-templates select="key('fldi', @aid)" mode="enum"/>
        <xsl:text> = </xsl:text>
        <xsl:value-of select="@aid"/>
        <xsl:text>,&#0010;</xsl:text>
      </xsl:for-each>
      <xsl:text>
};

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name attr
%null-strings

struct {
	const char *attr;
	enum __attr_e aid;
};

%%
xmlns,__ATTR_XMLNS
v,__ATTR_V
</xsl:text>

      <!-- loop over them fields again -->
      <xsl:for-each select="fixc:field">
        <xsl:apply-templates select="key('fldi', @aid)" mode="map"/>
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
    <xsl:text>:&#0010;</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:component|fixc:message" mode="deps">
    <xsl:text> </xsl:text>
    <xsl:value-of select="fixc:prefix(.)"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>.c</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:field" mode="enum">
    <xsl:text>__ATTR_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:field" mode="map">
    <xsl:if test="string-length(@fixml) &gt; 0">
      <xsl:value-of select="@fixml"/>
      <xsl:text>,</xsl:text>
      <xsl:apply-templates select="." mode="enum"/>
      <xsl:text>&#0010;</xsl:text>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
