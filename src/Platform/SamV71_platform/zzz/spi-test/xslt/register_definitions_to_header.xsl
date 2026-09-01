<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>
    <xsl:template match="/register_definitions">
        <xsl:text>
        </xsl:text>
        <xsl:for-each select="block">
            <xsl:text>IDE3466_register&lt;</xsl:text>
            <xsl:value-of select="@base"/>
            <xsl:text>,</xsl:text>
            <xsl:choose>
                <xsl:when test="@access = 'read-only'">
                    <xsl:text>true,</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>false,</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
            <xsl:value-of select="@size"/>
            <xsl:text>&gt; </xsl:text>
            <xsl:value-of select="@tag"/>
            <xsl:text>{"</xsl:text>
            <xsl:value-of select="@tag"/>
            <xsl:text>","</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>","</xsl:text>
            <xsl:value-of select="@brief"/>
            <xsl:text>"};
            </xsl:text>
        </xsl:for-each>
        <xsl:text>
        </xsl:text>
    </xsl:template>
</xsl:stylesheet>

