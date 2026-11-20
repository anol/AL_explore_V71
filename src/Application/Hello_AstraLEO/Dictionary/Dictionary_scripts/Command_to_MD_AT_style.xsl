<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:param name="date" select="date"/>

    <xsl:template match="/command_definition">
        <xsl:text>&lt;!--

</xsl:text>
        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>
--&gt;

# </xsl:text>
        <xsl:value-of select="@product_id"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text> command version </xsl:text>
        <xsl:value-of select="@version"/>
        <xsl:text>

Date: </xsl:text>
        <xsl:value-of select="$date"/>
        <xsl:text>
</xsl:text>
        <xsl:call-template name="All_commands"/>
    </xsl:template>

    <xsl:template name="All_commands">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:text>
## Provider: </xsl:text>
                <xsl:value-of select="@key"/>
                <xsl:text>

| Command | Description |
|---|---|
</xsl:text>
                <xsl:for-each select="arg">
                    <xsl:sort select="@key |  @integer | @float | @string"/>
                    <xsl:call-template name="argument_rule">
                        <xsl:with-param name="command"/>
                        <xsl:with-param name="argument"/>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="argument_rule">
        <xsl:param name="command"/>
        <xsl:param name="argument"/>
        <xsl:variable name="Argument_syntax">
            <xsl:if test="(@integer != '')">
                 <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <xsl:call-template name="integer_type">
                    <xsl:with-param name="type_name" select="@integer"/>
                </xsl:call-template>
            </xsl:if>
            <xsl:if test="(@float != '')">
                <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <xsl:call-template name="float_type">
                    <xsl:with-param name="type_name" select="@float"/>
                </xsl:call-template>
            </xsl:if>
            <xsl:if test="(@string != '')">
                <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <xsl:call-template name="string_type">
                    <xsl:with-param name="type_name" select="@string"/>
                </xsl:call-template>
            </xsl:if>
        </xsl:variable>
        <xsl:variable name="Command_syntax">
            <xsl:value-of select="$command"/>
            <!-- AT-style -->
            <!-- <xsl:text> </xsl:text>-->
            <!-- AT-style -->
            <xsl:if test="(@key != '')">
                <!-- AT-style -->
                <xsl:if test="($command != '')">
                    <xsl:text>_</xsl:text>
                </xsl:if>
                <xsl:if test="($command = '')">
                    <xsl:text>AT+</xsl:text>
                </xsl:if>
                <!-- AT-style -->
                <xsl:value-of select="@key"/>
            </xsl:if>
            <xsl:if test="(@integer != '')">
                <!-- AT-style -->
                <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <!-- AT-style -->
                <xsl:call-template name="integer_type">
                    <xsl:with-param name="type_name" select="@integer"/>
                </xsl:call-template>
            </xsl:if>
            <xsl:if test="(@float != '')">
                <!-- AT-style -->
                <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <!-- AT-style -->
                <xsl:call-template name="float_type">
                    <xsl:with-param name="type_name" select="@float"/>
                </xsl:call-template>
            </xsl:if>
            <xsl:if test="(@string != '')">
                <!-- AT-style -->
                <xsl:if test="($argument != '')">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:if test="($argument = '')">
                    <xsl:text>=</xsl:text>
                </xsl:if>
                <!-- AT-style -->
                <xsl:call-template name="string_type">
                    <xsl:with-param name="type_name" select="@string"/>
                </xsl:call-template>
            </xsl:if>
        </xsl:variable>
        <xsl:if test="child::*">
            <xsl:for-each select="arg">
                <xsl:sort select="@key | @integer | @float | @string"/>
                <xsl:call-template name="argument_rule">
                    <xsl:with-param name="command" select="$Command_syntax"/>
                    <xsl:with-param name="argument" select="$Argument_syntax"/>
                </xsl:call-template>
            </xsl:for-each>
        </xsl:if>
        <xsl:if test="@brief != ''">
            <xsl:text>| `</xsl:text>
            <xsl:value-of select="$Command_syntax"/>
            <xsl:text>` | </xsl:text>
            <xsl:value-of select="@brief"/>
            <xsl:text> |
</xsl:text>
        </xsl:if>
    </xsl:template>

    <!--    Integer-->
    <xsl:template name="integer_type">
        <xsl:param name="type_name"/>
        <xsl:text>&lt;</xsl:text>
        <xsl:value-of select="@integer"/>
        <xsl:text>(int </xsl:text>
        <xsl:value-of select="/command_definition/integers/integer[@key = $type_name]/@min"/>
        <xsl:text>..</xsl:text>
        <xsl:value-of select="/command_definition/integers/integer[@key = $type_name]/@max"/>
        <xsl:text>)&gt;</xsl:text>
    </xsl:template>

    <!--    Float-->
    <xsl:template name="float_type">
        <xsl:param name="type_name"/>
        <xsl:text>&lt;</xsl:text>
        <xsl:value-of select="@float"/>
        <xsl:text>(float </xsl:text>
        <xsl:value-of select="/command_definition/floats/float[@key = $type_name]/@min"/>
        <xsl:text>..</xsl:text>
        <xsl:value-of select="/command_definition/floats/float[@key = $type_name]/@max"/>
        <xsl:text>)&gt;</xsl:text>
    </xsl:template>

    <!--    String-->
    <xsl:template name="string_type">
        <xsl:param name="type_name"/>
        <xsl:text>&lt;</xsl:text>
        <xsl:value-of select="@string"/>
        <xsl:text>(string</xsl:text>
        <xsl:text>)&gt;</xsl:text>
    </xsl:template>
</xsl:stylesheet>
