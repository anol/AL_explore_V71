<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main-->
    <xsl:template match="/command_definition">
        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>
namespace </xsl:text><xsl:value-of select="@interface"/><xsl:text>{

    </xsl:text>
        <xsl:call-template name="Build_help_text_table"/>
        <xsl:text>
}
</xsl:text>
    </xsl:template>

    <!-- Build help text table -->
    <xsl:template name="Build_help_text_table">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="Make_help_text_entries">
                        <xsl:with-param name="table_name" select="''"/>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>

    <!-- Help Text Table Entires -->
    <xsl:template name="Make_help_text_entries">
        <xsl:param name="table_name"/>
        <xsl:variable name="my_table_name">
            <xsl:value-of select="$table_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Make_help_text_entries">
                <xsl:with-param name="table_name" select="$my_table_name"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="@brief != ''">
            <xsl:text>constexpr auto* help</xsl:text>
            <xsl:value-of select="$my_table_name"/>
            <xsl:text>{"</xsl:text>
            <xsl:value-of select="@brief"/><xsl:text>"};
    </xsl:text>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>

