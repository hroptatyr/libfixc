<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:fm="http://www.fixprotocol.org/FIXML-5-0-SP2/METADATA"
  xmlns="http://www.ga-group.nl/libfixc_0_1"
  version="1.0">

  <xsl:output method="xml" indent="yes"/>

  <xsl:template match="xs:schema">
    <xsl:element name="spec">
      <xsl:attribute name="version"/>
      <xsl:attribute name="extver"/>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="xs:appinfo[fm:EnumDoc]">
    <xsl:element name="enum">
      <xsl:attribute name="eid">
        <xsl:value-of select="../xs:appinfo/fm:Xref/@Tag"/>
      </xsl:attribute>
      <xsl:for-each select="fm:EnumDoc">
        <xsl:element name="item">
          <xsl:attribute name="value">
            <xsl:value-of select="@value"/>
          </xsl:attribute>
          <xsl:value-of select="normalize-space(text())"/>
        </xsl:element>
      </xsl:for-each>
    </xsl:element>
  </xsl:template>

  <xsl:template match="text()"/>

</xsl:stylesheet>
