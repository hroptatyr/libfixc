<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml" indent="yes"/>
  <xsl:strip-space elements="*"/>

  <xsl:param name="contents"/>
  <xsl:param name="fields"/>
  <xsl:param name="components"/>
  <xsl:param name="categories"/>
  <xsl:param name="enums"/>
  <xsl:param name="version"/>

  <xsl:key name="enums" match="Enums" use="Tag"/>

  <xsl:template match="dataroot">
    <xsl:element name="spec" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="version">
        <xsl:choose>
          <xsl:when test="@version">
            <xsl:value-of select="@version"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$version"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="extver">
        <xsl:value-of select="$version"/>
      </xsl:attribute>
      <xsl:apply-templates/>
      <xsl:apply-templates select="$components/dataroot" mode="child"/>
      <xsl:apply-templates select="$fields/dataroot" mode="child"/>
      <xsl:apply-templates select="$categories/dataroot" mode="child"/>
      <xsl:apply-templates select="$enums/dataroot" mode="egrp"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="dataroot" mode="child">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="dataroot" mode="egrp">
    <xsl:for-each select="Enums[count(. | key('enums', Tag)[1]) = 1]">
      <xsl:variable name="eid" select="Tag"/>
      <xsl:element name="enum" namespace="http://www.ga-group.nl/libfixc_0_1">
        <xsl:attribute name="eid">
          <xsl:value-of select="$eid"/>
        </xsl:attribute>
        <xsl:apply-templates select="$enums/dataroot/Enums[Tag=$eid]"/>
      </xsl:element>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="MsgType">
    <xsl:variable name="msg_id" select="MsgID/text()"/>
    <xsl:element name="message" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="fix">
        <xsl:value-of select="MsgType/text()"/>
      </xsl:attribute>
      <xsl:attribute name="fixml">
        <xsl:value-of select="AbbrName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="MessageName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="cat">
        <xsl:value-of select="Category/text()"/>
      </xsl:attribute>
      <xsl:attribute name="sec">
        <xsl:value-of select="Section/text()"/>
      </xsl:attribute>
      <xsl:attribute name="not_req_xml">
        <xsl:value-of select="NotReqXML/text()"/>
      </xsl:attribute>
      <xsl:element name="description" namespace="http://www.ga-group.nl/libfixc_0_1">
        <xsl:copy-of select="Description/text()"/>
      </xsl:element>
      <xsl:apply-templates select="$contents/dataroot/MsgContents[MsgID=$msg_id]"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="MsgContents[number(TagText/text()) &gt; 0]">
    <xsl:element name="field" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="aid">
        <xsl:value-of select="TagText/text()"/>
      </xsl:attribute>
    </xsl:element>
  </xsl:template>

  <xsl:template match="MsgContents">
    <xsl:element name="component" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="name">
        <xsl:value-of select="TagText/text()"/>
      </xsl:attribute>
    </xsl:element>
  </xsl:template>

  <xsl:template match="Components">
    <xsl:variable name="msg_id" select="MsgID/text()"/>
    <xsl:element name="component" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="fixml">
        <xsl:value-of select="AbbrName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="ComponentName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="cat">
        <xsl:value-of select="Category/text()"/>
      </xsl:attribute>
      <xsl:attribute name="type">
        <xsl:value-of select="ComponentType/text()"/>
      </xsl:attribute>
      <xsl:attribute name="not_req_xml">
        <xsl:value-of select="NotReqXML/text()"/>
      </xsl:attribute>
      <xsl:element name="description" namespace="http://www.ga-group.nl/libfixc_0_1">
        <xsl:copy-of select="Description/text()"/>
      </xsl:element>
      <xsl:apply-templates select="$contents/dataroot/MsgContents[MsgID=$msg_id]"/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="Fields">
    <xsl:element name="field" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="aid">
        <xsl:value-of select="Tag/text()"/>
      </xsl:attribute>
      <xsl:attribute name="fixml">
        <xsl:value-of select="AbbrName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="FieldName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="type">
        <xsl:value-of select="Type/text()"/>
      </xsl:attribute>
      <xsl:attribute name="not_req_xml">
        <xsl:value-of select="NotReqXML/text()"/>
      </xsl:attribute>
      <xsl:if test="UsesEnumsFromTag">
        <xsl:attribute name="eid">
          <xsl:value-of select="UsesEnumsFromTag"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:element name="description" namespace="http://www.ga-group.nl/libfixc_0_1">
        <xsl:copy-of select="Desc/text()"/>
      </xsl:element>
      <xsl:if test="BaseCatagoryXMLName">
        <xsl:element name="alias" namespace="http://www.ga-group.nl/libfixc_0_1">
          <xsl:attribute name="cat">
            <xsl:value-of select="BaseCatagory"/>
          </xsl:attribute>
          <xsl:attribute name="fixml">
            <xsl:value-of select="BaseCatagoryXMLName"/>
          </xsl:attribute>
        </xsl:element>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <xsl:template match="Categories">
    <xsl:element name="cat" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="name">
        <xsl:value-of select="Category/text()"/>
      </xsl:attribute>
      <xsl:attribute name="fixml">
        <xsl:value-of select="FIXMLFileName/text()"/>
      </xsl:attribute>
      <xsl:attribute name="not_req_xml">
        <xsl:value-of select="NotReqXML/text()"/>
      </xsl:attribute>
    </xsl:element>
  </xsl:template>

  <xsl:template match="Enums">
    <xsl:element name="item" namespace="http://www.ga-group.nl/libfixc_0_1">
      <xsl:attribute name="value">
        <xsl:value-of select="Enum/text()"/>
      </xsl:attribute>
      <xsl:copy-of select="Description/text()"/>
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
