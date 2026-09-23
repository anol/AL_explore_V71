<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main -->
    <xsl:template match="/command_definition">
        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>module;

export module Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_table;
import Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_lookup;
import Support.Abstract_command_table;

export namespace </xsl:text><xsl:value-of select="@interface"/><xsl:text> {

    // Adapts </xsl:text><xsl:value-of select="@interface"/><xsl:text>::get_commands() to Instruction::Abstract_command_table,
    // so callers can depend on that interface instead of on this generated module.
    class </xsl:text><xsl:value-of select="@name"/><xsl:text>_command_table : public Instruction::Abstract_command_table {
    public:
        [[nodiscard]] const Instruction::Instruction_token *get_commands() const override
        {
            return </xsl:text><xsl:value-of select="@interface"/><xsl:text>::get_commands();
        }
    };

} // </xsl:text><xsl:value-of select="@interface"/><xsl:text>
</xsl:text>
    </xsl:template>

</xsl:stylesheet>
