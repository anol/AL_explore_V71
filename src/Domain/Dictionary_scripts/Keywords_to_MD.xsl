<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:param name="date" select="date"/>

    <xsl:template match="/keyword_definition">
        <xsl:text>&lt;!--

</xsl:text>
        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>
--&gt;

# </xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text> version </xsl:text>
        <xsl:value-of select="@version"/>
        <xsl:text>

Date: </xsl:text>
        <xsl:value-of select="$date"/>
        <xsl:text>

| Id | Name | Description |
|---|---|---|
</xsl:text>
        <xsl:call-template name="One_keyword">
            <xsl:with-param name="id">0</xsl:with-param>
            <xsl:with-param name="name">"No key"</xsl:with-param>
            <xsl:with-param name="brief">Special purpose</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="All_keywords"/>
        <xsl:call-template name="One_keyword">
            <xsl:with-param name="id">255</xsl:with-param>
            <xsl:with-param name="name">"Wildcard"</xsl:with-param>
            <xsl:with-param name="brief">Special purpose, used in search</xsl:with-param>
        </xsl:call-template>
    </xsl:template>

    <xsl:template name="All_keywords">
        <xsl:for-each select="descendant::key">
            <xsl:sort select="@name"/>
            <xsl:call-template name="One_keyword">
                <xsl:with-param name="id" select="position()"/>
                <xsl:with-param name="name" select="@name"/>
                <xsl:with-param name="brief" select="@brief"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="One_keyword">
        <xsl:param name="id"/>
        <xsl:param name="name"/>
        <xsl:param name="brief"/>
        <xsl:text>| </xsl:text>
        <xsl:value-of select="$id"/>
        <xsl:text> | `</xsl:text>
        <xsl:value-of select="$name"/>
        <xsl:text>` | </xsl:text>
        <xsl:value-of select="$brief"/>
        <xsl:text> |
</xsl:text>
    </xsl:template>

</xsl:stylesheet>
