<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html"/>

  <xsl:template match="function-spec">
    <xsl:text>---
title: "Function </xsl:text><xsl:value-of select="@name"/><xsl:text>"
name: "</xsl:text><xsl:value-of select="@name"/><xsl:text>"
syntax: "</xsl:text><xsl:value-of select="syntax"/><xsl:text>"
---
</xsl:text>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="syntax">
    <h2>Syntax</h2>
    <pre><xsl:value-of select="."/></pre>
  </xsl:template>

  <xsl:template match="description">
    <h2>Description</h2>
    <p><xsl:apply-templates/></p>
  </xsl:template>

  <xsl:template match="paramname">
    <span class="procdraw-paramname"><xsl:value-of select="."/></span>
  </xsl:template>

  <xsl:template match="parameters">
    <h2>Parameters</h2>
    <table>
      <thead>
        <tr><th>Parameter</th><th>Usage</th></tr>
      </thead>
      <tbody>
        <xsl:apply-templates select="parameter"/>
      </tbody>
    </table>
  </xsl:template>

  <xsl:template match="parameter">
    <tr>
      <td><xsl:value-of select="@name"/></td>
      <td><xsl:value-of select="."/></td>
    </tr>
  </xsl:template>

  <xsl:template match="examples">
    <h2>Examples</h2>
    <pre>
      <xsl:apply-templates select="example"/>
    </pre>
  </xsl:template>

  <xsl:template match="example">
    <xsl:value-of select="@expr"/> => <xsl:value-of select="@value"/><xsl:text>
</xsl:text>
  </xsl:template>

</xsl:stylesheet>
