<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:template match="/command_definition">
        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>module;
#include &lt;cstdint&gt;
#include &lt;type_traits&gt;

export module Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_version;

export namespace </xsl:text><xsl:value-of select="@product_id"/><xsl:text>
{
    namespace version
    {
        using instruction = std::integral_constant&lt;uint8_t, </xsl:text>
            <xsl:value-of select="@version"/>
            <xsl:text>&gt;;
        constexpr const char *instruction_string = "</xsl:text>
            <xsl:value-of select="@version"/>
            <xsl:text>";
    }
}
</xsl:text>
    </xsl:template>

</xsl:stylesheet>
