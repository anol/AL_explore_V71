<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main -->
    <xsl:template match="/keyword_definition">

        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>

        <xsl:text>export module Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_keyword_lookup;

export namespace </xsl:text>
        <xsl:value-of select="@interface"/>
        <xsl:text> {

    constexpr int get_keyword_version() { return </xsl:text>
        <xsl:value-of select="@version"/>
        <xsl:text>; }

    const char *get_keyword(unsigned char key);

    enum Keys : unsigned char {
        No_key,

        </xsl:text>
        <xsl:call-template name="enumeration"/>
        <xsl:text>
        number_of_keys,
        Literal_value,
        Wildcard = 0xFF
    };

}
</xsl:text>
    </xsl:template>

    <xsl:template name="enumeration">
        <xsl:for-each select="descendant::key">
            <xsl:sort select="@name"/>
            <xsl:text>Key_</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>,
        </xsl:text>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>
