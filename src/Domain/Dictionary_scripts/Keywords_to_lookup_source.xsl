<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main-->
    <xsl:template match="/keyword_definition">

        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>

        <xsl:text>module Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_keyword_lookup;

namespace </xsl:text>
        <xsl:value-of select="@interface"/>
        <xsl:text> {

    constexpr char const *the_keywords[number_of_keys] = {
        "!", // No_key
    </xsl:text>
        <xsl:call-template name="Keywords"/>
        <xsl:text>};

    const char *get_keyword(unsigned char key) {
        if (key &lt; number_of_keys) return the_keywords[key];
        else return "?";
    }

}
</xsl:text>
    </xsl:template>

    <xsl:template name="Keywords">
        <xsl:for-each select="descendant::key">
            <xsl:sort select="@name"/>
            <xsl:text>    "</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>", // Key_</xsl:text>
            <xsl:value-of select="@name"/>
            <xsl:text>
    </xsl:text>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>

