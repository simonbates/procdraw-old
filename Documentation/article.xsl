<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html"/>

  <xsl:template match="article">
    <xsl:text disable-output-escaping="yes">&lt;!DOCTYPE html&gt;
</xsl:text>
    <html>
      <head>
        <title><xsl:value-of select="@title"/></title>
      </head>
      <body>
        <h1><xsl:value-of select="@title"/></h1>
        <xsl:apply-templates/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="section">
    <h2><xsl:value-of select="@title"/></h2>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="ul | li | code">
    <xsl:copy>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="functionref">
    <a>
      <xsl:attribute name="href">
        <xsl:choose>
          <xsl:when test="@name = '+'">
            <xsl:text>sum</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@name"/>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:text>.html</xsl:text>
      </xsl:attribute>
      <xsl:value-of select="@name"/>
    </a>
  </xsl:template>

</xsl:stylesheet>
