<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:template match="/structure">
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
        <xsl:text>

| Identifier | Type | Default | Description |
|---|---|---|---|
</xsl:text>
        <xsl:for-each select="primary">
            <xsl:call-template name="primary_rule"/>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="primary_rule">
        <xsl:variable name="primary_name" select="@key"/>
        <xsl:for-each select="secondary">
            <xsl:call-template name="secondary_rule">
                <xsl:with-param name="primary_name" select="$primary_name"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="secondary_rule">
        <xsl:param name="primary_name"/>
        <xsl:variable name="secondary_name">
            <xsl:choose>
                <xsl:when test="(@key != '')">
                    <xsl:value-of select="$primary_name"/>.<xsl:value-of select="@key"/>
                </xsl:when>
                <xsl:when test="(@min != '') and (@max != '')">
                    <xsl:value-of select="$primary_name"/>.[<xsl:value-of select="@min"/>-<xsl:value-of select="@max"/><xsl:text>]</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="$primary_name"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:for-each select="tertiary">
            <xsl:call-template name="tertiary_rule">
                <xsl:with-param name="secondary_name" select="$secondary_name"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="tertiary_rule">
        <xsl:param name="secondary_name"/>
        <xsl:variable name="tertiary_name">
            <xsl:choose>
                <xsl:when test="(@key != '')">
                    <xsl:value-of select="$secondary_name"/>.<xsl:value-of select="@key"/>
                </xsl:when>
                <xsl:when test="(@min != '') and (@max != '')">
                    <xsl:value-of select="$secondary_name"/>.[<xsl:value-of select="@min"/>-<xsl:value-of
                        select="@max"/><xsl:text>]</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="$secondary_name"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:for-each select="quaternary">
            <xsl:call-template name="quaternary_rule">
                <xsl:with-param name="tertiary_name" select="$tertiary_name"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="quaternary_rule">
        <xsl:param name="tertiary_name"/>
        <xsl:variable name="quaternary_name">
            <xsl:choose>
                <xsl:when test="(@key != '')">
                    <xsl:value-of select="$tertiary_name"/>.<xsl:value-of select="@key"/>
                </xsl:when>
                <xsl:when test="(@min != '') and (@max != '')">
                    <xsl:value-of select="$tertiary_name"/>.[<xsl:value-of select="@min"/>-<xsl:value-of select="@max"/><xsl:text>]</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="$tertiary_name"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:for-each select="value">
            <xsl:call-template name="value_rule">
                <xsl:with-param name="quaternary_name" select="$quaternary_name"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="value_rule">
        <xsl:param name="quaternary_name"/>
        <xsl:text>| </xsl:text>
        <xsl:value-of select="$quaternary_name"/>
        <xsl:text> | </xsl:text>
        <xsl:value-of select="@type"/>
        <xsl:text> | </xsl:text>
        <xsl:choose>
            <xsl:when test="(@default != '')">
                <xsl:value-of select="@default"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:text>-</xsl:text>
            </xsl:otherwise>
        </xsl:choose>
        <xsl:text> | </xsl:text>
        <xsl:value-of select="@brief"/>
        <xsl:text> |
</xsl:text>
    </xsl:template>

</xsl:transform>
