<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
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
        <xsl:apply-templates select="/fixc:spec" mode="hdr"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- this stylesheet will generate .h files -->
  <xsl:template match="fixc:spec" mode="hdr">
    <xsl:text>/* do not edit, gen'd by fixml-msg.xsl */
#if !defined INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_msg_h_
#define INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_msg_h_&#0010;</xsl:text>

    <xsl:text>
#include "fixml-canon-msgt.h"

#undef _
#define _(x, y)    ((x) * 256U + (y) * 1U)

typedef enum {
	/* first 3 are our own invention */
</xsl:text>

    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_MSGT_UNK,&#0010;</xsl:text>
    <xsl:text>&#0009;</xsl:text>
    <xsl:value-of select="$VERSN"/>
    <xsl:text>_MSGT_BATCH = 0xffff,&#0010;</xsl:text>

    <xsl:for-each select="fixc:message">
      <xsl:text>&#0009;</xsl:text>
      <xsl:apply-templates select="." mode="enum"/>
      <xsl:text>,&#0010;</xsl:text>
    </xsl:for-each>

    <xsl:text>} </xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_msgt_t;

#undef _

#endif  /* INCLUDED_</xsl:text>
    <xsl:value-of select="$versn"/>
    <xsl:text>_msg_h_ */&#0010;</xsl:text>
  </xsl:template>

  <!-- this stylesheet will generate .gperf files -->
  <xsl:template match="fixc:spec" mode="deps">
      <xsl:text>/* do not edit, gen'd by fixml-msg.xsl */
%{

#include "fixml-canon-msgt.h"
#include "fixml-msg.h"
#include "fixml-msg-</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>.h"

#define __fixml_mtypify	</xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>_mtypify

%}
%7bit
%readonly-tables
%enum
%switch=1
%struct-type
%define slot-name msgt
%define hash-function-name </xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>_mty_hash</xsl:text>
      <xsl:text>
%define lookup-function-name </xsl:text>
      <xsl:value-of select="$versn"/><xsl:text>_mtypify</xsl:text>
      <xsl:text>
%null-strings
%includes

struct fixml_msgt_s {
	const char *msgt;
	fixc_msgt_t mty;
};

/* forward */
inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__((__gnu_inline__))
#endif
const struct fixml_msgt_s*
__fixml_mtypify(register const char *str, register unsigned int len);

fixc_msgt_t
fixc_get_mty(const char *elem, size_t elen)
{
	const struct fixml_msgt_s *p = __fixml_mtypify(elem, elen);
	return p != NULL ? (fixc_msgt_t)p->mty : FIXC_MSGT_UNK;
}

%%
Batch,(fixc_msgt_t)FIXC_MSGT_BATCH
</xsl:text>

    <!-- loop over them messages -->
    <xsl:apply-templates select="fixc:message" mode="gperf"/>
  </xsl:template>

  <xsl:template match="fixc:message">
    <xsl:value-of select="fixc:ucase(fixc:prefix(.))"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="@name"/>
  </xsl:template>

  <xsl:template match="fixc:message" mode="enum">
    <xsl:apply-templates select="."/>
    <xsl:text> = _(</xsl:text>
    <xsl:value-of select="fixc:char-at(@fix, 1)"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="fixc:char-at(@fix, 2)"/>
    <xsl:text>)/* "</xsl:text>
    <xsl:value-of select="@fix"/>
    <xsl:text>" */</xsl:text>
  </xsl:template>

  <xsl:template match="fixc:message" mode="gperf">
    <xsl:if test="@fixml">
      <xsl:value-of select="@fixml"/>
      <xsl:text>,(fixc_msgt_t)</xsl:text>
      <xsl:apply-templates select="."/>
      <xsl:text>&#0010;</xsl:text>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
