<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <xsl:template match="message">
    <xsl:variable name="msgtyp" select="@name"/>

    <xsl:for-each select="component">
      <xsl:value-of select="$msgtyp"/>
      <xsl:text>&#0009;</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>&#0010;</xsl:text>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
