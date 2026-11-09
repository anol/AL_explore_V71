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
#include &lt;cstdint&gt;

module Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_provider_indication;
import Domain.</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_lookup;

using namespace Instruction;
using namespace </xsl:text><xsl:value-of select="/command_definition/@interface"/><xsl:text>;
        </xsl:text>
        <xsl:call-template name="All_class_implementations"/>
    </xsl:template>

    <xsl:template name="All_class_implementations">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:call-template name="Make_lookup_table">
                    <xsl:with-param name="class_name">
                        <xsl:text>Abstract_</xsl:text>
                        <xsl:value-of select="@key"/>
                        <xsl:text>_provider</xsl:text>
                    </xsl:with-param>
                </xsl:call-template>
                <xsl:text>
namespace </xsl:text><xsl:value-of select="/command_definition/@interface"/>
                <xsl:text>{
</xsl:text>
                <xsl:call-template name="Implement_class">
                    <xsl:with-param name="class_name">
                        <xsl:text>Abstract_</xsl:text>
                        <xsl:value-of select="@key"/>
                        <xsl:text>_provider</xsl:text>
                    </xsl:with-param>
                </xsl:call-template>
                <xsl:text>
}
</xsl:text>
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="Implement_class">
        <xsl:param name="class_name"/>
        <xsl:text>
    bool </xsl:text>
        <xsl:value-of select="$class_name"/>
        <xsl:text>::on_indication(Instruction_major &amp;transaction) {
        return </xsl:text>
        <xsl:value-of select="$class_name"/>
        <xsl:text>::Lookup_table::lookup(this, transaction);
    }
</xsl:text>
        <xsl:for-each select="arg">
            <xsl:call-template name="Implement_funcs">
                <xsl:with-param name="class_name" select="$class_name"/>
                <xsl:with-param name="func_name"/>
                <xsl:with-param name="arg_count">
                    <xsl:text>0</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="arg_list"/>
                <xsl:with-param name="arg_init"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="Implement_funcs">
        <xsl:param name="class_name"/>
        <xsl:param name="func_name"/>
        <xsl:param name="arg_count"/>
        <xsl:param name="arg_list"/>
        <xsl:param name="arg_init"/>
        <xsl:variable name="my_func_name">
            <xsl:value-of select="$func_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@name| @key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:variable name="my_arg_list">
            <xsl:value-of select="$arg_list"/>
            <xsl:if test="@integer != ''">
                <xsl:text>, </xsl:text>
                <xsl:value-of select="@integer"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
            </xsl:if>
            <xsl:if test="@float != ''">
                <xsl:text>, </xsl:text>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
            </xsl:if>
        </xsl:variable>
        <xsl:variable name="my_arg_init">
            <xsl:value-of select="$arg_init"/>
            <xsl:if test="@integer != ''">
                <xsl:text>
        int </xsl:text>
                <xsl:value-of select="@integer"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>{transaction.get_token_integer(</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>)};</xsl:text>
            </xsl:if>
            <xsl:if test="@float != ''">
                <xsl:text>
        float </xsl:text>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>{transaction.get_token_float(</xsl:text>
                <xsl:value-of select="$arg_count"/>
                <xsl:text>)};</xsl:text>
            </xsl:if>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Implement_funcs">
                <xsl:with-param name="class_name" select="$class_name"/>
                <xsl:with-param name="func_name" select="$my_func_name"/>
                <xsl:with-param name="arg_count" select="1 + $arg_count"/>
                <xsl:with-param name="arg_list" select="$my_arg_list"/>
                <xsl:with-param name="arg_init" select="$my_arg_init"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="@brief != ''">
            <xsl:text>
    void </xsl:text>
            <xsl:value-of select="$class_name"/>
            <xsl:text>::p</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>(Instruction_major &amp;transaction){</xsl:text>
            <xsl:value-of select="$my_arg_init"/>
            <xsl:text>
        v</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>(transaction</xsl:text>
            <xsl:value-of select="$my_arg_list"/>
            <xsl:text>);
    }
            </xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template name="Make_lookup_table">
        <xsl:param name="class_name"/>
        <xsl:text>

template&lt;&gt; const </xsl:text>
        <xsl:value-of select="$class_name"/>
        <xsl:text>::Lookup_table::Instruction_entry
    </xsl:text>
        <xsl:value-of select="$class_name"/>
        <xsl:text>::Lookup_table::instruction_table[] = {</xsl:text>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Build_func_table">
                <xsl:with-param name="class_name" select="$class_name"/>
                <xsl:with-param name="func_name"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:text>
};
</xsl:text>
    </xsl:template>

    <xsl:template name="Build_func_table">
        <xsl:param name="class_name"/>
        <xsl:param name="func_name"/>
        <xsl:variable name="my_func_name">
            <xsl:value-of select="$func_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@name| @key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Build_func_table">
                <xsl:with-param name="class_name" select="$class_name"/>
                <xsl:with-param name="func_name" select="$my_func_name"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="@brief != ''">
            <xsl:text>
    {&amp;</xsl:text>
            <xsl:value-of select="$class_name"/>
            <xsl:text>::p</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>, Cmd</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>},</xsl:text>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
