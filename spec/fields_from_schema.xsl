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

  <xsl:template match="fm:Xref[@ComponentType='Field']">
    <xsl:element name="field">
      <xsl:attribute name="aid">
        <xsl:value-of select="@Tag"/>
      </xsl:attribute>
      <xsl:attribute name="fixml">
        <xsl:value-of select="@AbbrName"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:attribute name="type">
        <xsl:value-of select="@Type"/>
      </xsl:attribute>
      <xsl:attribute name="not_req_xml">
        <xsl:text>0</xsl:text>
      </xsl:attribute>
      <xsl:if test="@UsesEnumsFromTag">
        <xsl:attribute name="eid">
          <xsl:value-of select="@UsesEnumsFromTag"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:apply-templates select="../../xs:documentation" mode="field"/>
      <xsl:if test="@CategoryAbbrName">
        <xsl:element name="alias">
          <xsl:attribute name="cat">
            <xsl:value-of select="@Category"/>
          </xsl:attribute>
          <xsl:attribute name="fixml">
            <xsl:value-of select="@CategoryAbbrName"/>
          </xsl:attribute>
        </xsl:element>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <xsl:template match="xs:documentation" mode="field">
    <xsl:element name="description">
      <xsl:value-of select="normalize-space(text())"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="text()"/>

</xsl:stylesheet>
