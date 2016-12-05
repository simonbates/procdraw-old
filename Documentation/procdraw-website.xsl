<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html" doctype-system="about:legacy-compat"/>

  <xsl:variable name="site-title" select="'Procdraw'"/>

  <xsl:template match="homepage|docs-index|function-spec" mode="page">
    <xsl:param name="is-homepage" select="false()"/>
    <xsl:param name="is-narrow-page" select="false()"/>
    <xsl:param name="page-title"/>
    <xsl:param name="page-section"/>
    <html>
      <head>
        <meta charset="utf-8"/>
        <meta http-equiv="x-ua-compatible" content="ie=edge"/>
        <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
        <xsl:choose>
          <xsl:when test="$is-homepage">
            <title><xsl:value-of select="$site-title"/></title>
          </xsl:when>
          <xsl:otherwise>
            <title><xsl:value-of select="$site-title"/> - <xsl:value-of select="$page-title"/></title>
          </xsl:otherwise>
        </xsl:choose>
        <link href="https://fonts.googleapis.com/css?family=Lato:400,400italic,700,300" rel="stylesheet" type="text/css"/>
        <link href="https://fonts.googleapis.com/css?family=Anonymous+Pro" rel="stylesheet" type="text/css"/>
        <link href="/css/main.css" rel="stylesheet"/>
      </head>

      <body class="procdraw-site">
        <div class="procdraw-header">
          <div class="row">
            <div class="large-6 columns">
              <a class="procdraw-sitename" href="/">Procdraw</a>
            </div>
            <div class="large-6 columns procdraw-nav">
              <xsl:call-template name="section-link">
                <xsl:with-param name="section" select="'home'"/>
                <xsl:with-param name="href" select="'/'"/>
                <xsl:with-param name="text" select="'Home'"/>
                <xsl:with-param name="page-section" select="$page-section"/>
              </xsl:call-template>
              <xsl:call-template name="section-link">
                <xsl:with-param name="section" select="'docs'"/>
                <xsl:with-param name="href" select="'/docs/'"/>
                <xsl:with-param name="text" select="'Docs'"/>
                <xsl:with-param name="page-section" select="$page-section"/>
              </xsl:call-template>
            </div>
          </div>
        </div>
        <div class="row procdraw-content">
          <div>
            <xsl:choose>
              <xsl:when test="$is-narrow-page">
                <xsl:attribute name="class">large-11 columns end</xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="class">large-12 columns</xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:if test="not($is-homepage)">
              <h1><xsl:value-of select="$page-title"/></h1>
            </xsl:if>
            <xsl:apply-templates mode="page-content" select="."/>
          </div>
        </div>
      </body>
    </html>
  </xsl:template>

  <xsl:template name="section-link">
    <xsl:param name="section"/>
    <xsl:param name="href"/>
    <xsl:param name="text"/>
    <xsl:param name="page-section"/>
    <a>
      <xsl:if test="$page-section = $section">
        <xsl:attribute name="class">procdraw-active-section</xsl:attribute>
      </xsl:if>
      <xsl:attribute name="href"><xsl:value-of select="$href"/></xsl:attribute>
      <xsl:value-of select="$text"/>
    </a>
  </xsl:template>

  <!-- Homepage -->

  <xsl:template match="homepage">
    <xsl:apply-templates mode="page" select=".">
      <xsl:with-param name="is-homepage" select="true()"/>
      <xsl:with-param name="page-section" select="'home'"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="homepage" mode="page-content">
    <div class="procdraw-home">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <!-- Docs-index -->

  <xsl:template match="docs-index">
    <xsl:apply-templates mode="page" select=".">
      <xsl:with-param name="page-title" select="'Procdraw Documentation'"/>
      <xsl:with-param name="page-section" select="'docs'"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="docs-index" mode="page-content">
    <div class="row procdraw-docs-index">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <xsl:template match="docs-index-col">
    <div class="large-4 columns">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <xsl:template match="docs-index-list">
    <h2><xsl:value-of select="@title"/></h2>
    <ul>
      <xsl:apply-templates/>
    </ul>
  </xsl:template>

  <xsl:template match="function-link">
    <a href="/docs/functions/{@name}.html"><xsl:value-of select="document(concat('docs/functions/',@name,'.xml'))/function-spec/syntax"/></a>
  </xsl:template>

  <!-- Function-spec -->

  <xsl:template match="function-spec">
    <xsl:apply-templates mode="page" select=".">
      <xsl:with-param name="is-narrow-page" select="true()"/>
      <xsl:with-param name="page-title" select="concat('Function ',@name)"/>
      <xsl:with-param name="page-section" select="'docs'"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="function-spec" mode="page-content">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="syntax">
    <h2>Syntax</h2>
    <pre><xsl:value-of select="."/></pre>
  </xsl:template>

  <xsl:template match="description">
    <h2>Description</h2>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="paramname">
    <span class="procdraw-paramname"><xsl:value-of select="."/></span>
  </xsl:template>

  <xsl:template match="parameters">
    <h2>Parameters</h2>
    <xsl:choose>
      <xsl:when test="./@none">
        <p>None.</p>
      </xsl:when>
      <xsl:otherwise>
        <table>
          <thead>
            <tr><th>Parameter</th><th>Usage</th></tr>
          </thead>
          <tbody>
            <xsl:apply-templates select="parameter"/>
          </tbody>
        </table>
      </xsl:otherwise>
    </xsl:choose>
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

  <!-- Common elements -->

  <xsl:template match="p|ul|li|strong">
    <xsl:copy>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="a">
    <xsl:copy>
      <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
