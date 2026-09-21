<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:template match="/command_definition">

        <xsl:variable name="path">
            <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
        </xsl:variable>
        <xsl:apply-templates select="document($path)"/>

        <xsl:text>
#pragma once

#include "Instruction_lookup.h"
import Type.Abstract_provider;
#include "Dictionary.h"

using namespace Instruction;

namespace Transaction {
    class Transaction_major;
}

using namespace Transaction;

namespace </xsl:text><xsl:value-of select="@interface"/><xsl:text> {
    </xsl:text>
        <xsl:call-template name="All_class_definitions"/>
        <xsl:text>

}

</xsl:text>
    </xsl:template>

    <xsl:template name="All_class_definitions">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:text>
    class Abstract_</xsl:text>
                <xsl:value-of select="@key"/>
                <xsl:text>_provider : public Abstract_provider&lt;Instruction_major&gt; {
    public:
       using Lookup_table = Instruction_lookup&lt;Abstract_</xsl:text>
                <xsl:value-of select="@key"/>
                <xsl:text>_provider&gt;;
    public:
        Abstract_</xsl:text>
                <xsl:value-of select="@key"/>
                <xsl:text>_provider() : Abstract_provider(Key_</xsl:text>
                <xsl:value-of select="@key"/>
                <xsl:text>){}
        bool on_indication(Instruction_major &amp;) override;
    protected:</xsl:text>
                <xsl:for-each select="arg">
                    <xsl:call-template name="Build_virtual_funcs">
                        <xsl:with-param name="func_name"/>
                        <xsl:with-param name="arg_count">
                            <xsl:text>0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="arg_list"/>
                    </xsl:call-template>
                </xsl:for-each>
                <xsl:text>
    public:</xsl:text>
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="Build_provider_funcs">
                        <xsl:with-param name="func_name"/>
                    </xsl:call-template>
                </xsl:for-each>
                <xsl:text>
    };
            </xsl:text>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="Build_provider_funcs">
        <xsl:param name="func_name"/>
        <xsl:variable name="my_func_name">
            <xsl:value-of select="$func_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@name| @key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Build_provider_funcs">
                <xsl:with-param name="func_name" select="$my_func_name"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="@brief != ''">
            <xsl:text>
        void p</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>(Instruction_major &amp;</xsl:text>
            <xsl:text>);</xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template name="Build_virtual_funcs">
        <xsl:param name="func_name"/>
        <xsl:param name="arg_count"/>
        <xsl:param name="arg_list"/>
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
                <xsl:if test="$arg_list != ''">
                    <xsl:text>, </xsl:text>
                </xsl:if>
                <xsl:text>float </xsl:text>
                <xsl:value-of select="@float"/>
                <xsl:text>_</xsl:text>
                <xsl:value-of select="$arg_count"/>
            </xsl:if>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Build_virtual_funcs">
                <xsl:with-param name="func_name" select="$my_func_name"/>
                <xsl:with-param name="arg_count" select="1 + $arg_count"/>
                <xsl:with-param name="arg_list" select="$my_arg_list"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="@brief != ''">
            <xsl:text>
        virtual void v</xsl:text>
            <xsl:value-of select="$my_func_name"/>
            <xsl:text>(Instruction_major &amp;</xsl:text>
            <xsl:value-of select="$my_arg_list"/>
            <xsl:text>) = 0;</xsl:text>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
