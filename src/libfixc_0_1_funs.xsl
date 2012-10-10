<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fixc="http://www.ga-group.nl/libfixc_0_1"
  xmlns:fn="http://exslt.org/functions"
  extension-element-prefixes="fn"
  version="1.0">

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

  <fn:function name="fixc:vprefix">
    <xsl:param name="node-set"/>
    <xsl:variable name="infix">
      <xsl:choose>
        <xsl:when test="name($node-set) = 'component'">
          <xsl:text>comp</xsl:text>
        </xsl:when>
        <xsl:when test="name($node-set) = 'message'">
          <xsl:text>msg</xsl:text>
        </xsl:when>
        <xsl:when test="name($node-set) = 'field'">
          <xsl:text>attr</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>unk</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <fn:result select="concat($versn, '_', $infix)"/>
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
        <xsl:when test="name($node-set) = 'field'">
          <xsl:text>attr</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>unk</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <fn:result select="concat('fixml_', $infix)"/>
  </fn:function>

  <fn:function name="fixc:char-at">
    <xsl:param name="str"/>
    <xsl:param name="idx"/>
    <xsl:choose>
      <xsl:when test="$idx &lt;= string-length($str)">
        <fn:result>
          <xsl:text>'</xsl:text>
          <xsl:value-of select="substring($str, $idx, 1)"/>
          <xsl:text>'</xsl:text>
        </fn:result>
      </xsl:when>
      <xsl:otherwise>
        <fn:result select="0"/>
      </xsl:otherwise>
    </xsl:choose>
  </fn:function>

</xsl:stylesheet>
