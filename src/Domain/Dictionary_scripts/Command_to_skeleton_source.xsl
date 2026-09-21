<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:template match="/command_definition">

        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>
        <xsl:text>
module;
#include &lt;iostream&gt;

module Domain.</xsl:text><xsl:value-of select="@name"/><xsl:text>_provider_skeleton;

using namespace </xsl:text><xsl:value-of select="/command_definition/@interface"/><xsl:text>;
        </xsl:text>
        <xsl:call-template name="All_class_implementations"/>
        <xsl:text>
</xsl:text>
    </xsl:template>

    <!--/////////////////////////////////////////////////////////////////////////-->

    <xsl:template name="All_class_implementations">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:variable name="class_name">
                    <xsl:text>Implement_</xsl:text>
                    <xsl:value-of select="@key"/>
                    <xsl:text>_provider</xsl:text>
                </xsl:variable>
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="Implement_class">
                        <xsl:with-param name="class_name" select="$class_name"/>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>

    <!--/////////////////////////////////////////////////////////////////////////-->

    <xsl:template name="Implement_class">
        <xsl:param name="class_name"/>
        <xsl:call-template name="Traverse_functions">
            <xsl:with-param name="class_name" select="$class_name"/>
            <xsl:with-param name="func_name"/>
            <xsl:with-param name="arg_count">
                <xsl:text>0</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="arg_list"/>
            <xsl:with-param name="arg_print"/>
        </xsl:call-template>
    </xsl:template>

    <!--/////////////////////////////////////////////////////////////////////////-->

    <xsl:template name="Traverse_functions">
        <xsl:param name="class_name"/>
        <xsl:param name="func_name"/>
        <xsl:param name="arg_count"/>
        <xsl:param name="arg_list"/>
        <xsl:param name="arg_print"/>
        <xsl:variable name="my_func_name">
            <xsl:value-of select="$func_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@name| @key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:variable name="my_arg_list">
            <xsl:value-of select="$arg_list"/>
            <xsl:if test="@integer != ''">
                <xsl:text>, </xsl:text>
                <xsl:text>int </xsl:text>
                <xsl:value-of select="@integer"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
            </xsl:if>
            <xsl:if test="@float != ''">
                <xsl:text>, </xsl:text>
                <xsl:text>float </xsl:text>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
            </xsl:if>
        </xsl:variable>
        <xsl:variable name="my_arg_print">
            <xsl:value-of select="$arg_print"/>
            <xsl:if test="@integer != ''">
                <xsl:text>"</xsl:text>
                <xsl:if test="$arg_print != ''">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:value-of select="@integer"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>=" &lt;&lt; </xsl:text>
                <xsl:value-of select="@integer"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text> &lt;&lt; </xsl:text>
            </xsl:if>
            <xsl:if test="@float != ''">
                <xsl:text>"</xsl:text>
                <xsl:if test="$arg_print != ''">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>=" &lt;&lt; </xsl:text>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text> &lt;&lt; </xsl:text>
            </xsl:if>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Traverse_functions">
                <xsl:with-param name="class_name" select="$class_name"/>
                <xsl:with-param name="func_name" select="$my_func_name"/>
                <xsl:with-param name="arg_count" select="1 + $arg_count"/>
                <xsl:with-param name="arg_list" select="$my_arg_list"/>
                <xsl:with-param name="arg_print" select="$my_arg_print"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:call-template name="Implement_function_body">
            <xsl:with-param name="class_name" select="$class_name"/>
            <xsl:with-param name="func_name" select="$my_func_name"/>
            <xsl:with-param name="arg_list" select="$my_arg_list"/>
            <xsl:with-param name="arg_print" select="$my_arg_print"/>
        </xsl:call-template>
    </xsl:template>

    <!--/////////////////////////////////////////////////////////////////////////-->

    <xsl:template name="Implement_function_body">
        <xsl:param name="class_name"/>
        <xsl:param name="func_name"/>
        <xsl:param name="arg_list"/>
        <xsl:param name="arg_print"/>
        <xsl:variable name="name">
            <xsl:value-of select="$class_name"/>
            <xsl:text>::v</xsl:text>
            <xsl:value-of select="$func_name"/>
        </xsl:variable>
        <xsl:if test="@brief != ''">
            <xsl:text>
void </xsl:text>
            <xsl:value-of select="$name"/>
            <xsl:text>(Transaction_major &amp;</xsl:text>
            <xsl:value-of select="$arg_list"/>
            <xsl:text>){
    std::cout &lt;&lt; "</xsl:text>
            <xsl:value-of select="$name"/>
            <xsl:text>:" &lt;&lt; </xsl:text>
            <xsl:value-of select="$arg_print"/>
            <xsl:text>std::endl;
}
            </xsl:text>
        </xsl:if>
    </xsl:template>

    <!--/////////////////////////////////////////////////////////////////////////-->

</xsl:stylesheet>
