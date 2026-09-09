<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:variable name="new_line">
        <xsl:text>
</xsl:text>
    </xsl:variable>

    <xsl:template match="/command_definition">
        <xsl:text>
/*
* Copyright (C) 2026 Integrated Detector Electronics AS
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of Integrated Detector Electronics AS and its suppliers,
* if any. The intellectual and technical concepts contained
* herein are proprietary to Integrated Detector Electronics AS
* and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Integrated Detector Electronics AS.
*
*/

//                          W A R N I N G
//
// The content of this file was generated using XSLT. Please, do not touch!
//
//                          W A R N I N G
        </xsl:text>
        <xsl:value-of select="$new_line"/>

        <xsl:text>#include &lt;</xsl:text>
        <xsl:text>cstdint</xsl:text>
        <xsl:text>&gt;</xsl:text>
        <xsl:value-of select="$new_line"/>

        <xsl:text>#include "</xsl:text>
        <xsl:value-of select="@keywords"/>
        <xsl:text>.h"</xsl:text>
        <xsl:value-of select="$new_line"/>

        <xsl:text>#include "</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_test.h"</xsl:text>
        <xsl:value-of select="$new_line"/>

        <xsl:text>using namespace </xsl:text>
        <xsl:value-of select="@interface"/>
        <xsl:text>;</xsl:text>
        <xsl:value-of select="$new_line"/>

        <xsl:text>
constexpr uint8_t the_test_instructions[][Max_number_of_tokens] = {
   {No_key},</xsl:text>
        <xsl:for-each select="commands">
            <xsl:call-template name="Build_commands"/>
        </xsl:for-each>
        <xsl:text>   {No_key}
};

uint32_t </xsl:text>
        <xsl:value-of select="@interface"/>
        <xsl:text>::get_test_instruction_count() {
    return (sizeof(the_test_instructions) / Max_number_of_tokens) -1;
}

const uint8_t (&amp; </xsl:text>
        <xsl:value-of select="@interface"/>
        <xsl:text>::get_test_instruction(uint32_t index))[Max_number_of_tokens]{
    return (index &lt; get_test_instruction_count()) ? the_test_instructions[index] : the_test_instructions[0];
}
        </xsl:text>
    </xsl:template>

    <xsl:template name="Build_commands">
        <xsl:value-of select="$new_line"/>
        <xsl:for-each select="arg">
            <xsl:sort select="@key"/>
            <xsl:if test="not(@special='no_test')">
                <xsl:call-template name="Build_subcommands">
                    <xsl:with-param name="id">
                        <xsl:text>   {Cmd_</xsl:text>
                        <xsl:value-of select="@name"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="Build_subcommands">
        <xsl:param name="id"/>
        <xsl:for-each select="arg">
            <xsl:if test="not(@special='no_test')">
                <xsl:call-template name="Build_arguments">
                    <xsl:with-param name="id" select="$id"/>
                </xsl:call-template>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="Build_arguments">
        <xsl:param name="id"/>
        <xsl:choose>
            <xsl:when test="@key='_default_'">
                <xsl:value-of select="$id"/>
                <xsl:text>, No_key}, // </xsl:text>
                <xsl:value-of select="@brief"/>
                <xsl:value-of select="$new_line"/>
            </xsl:when>
            <xsl:when test="@key!=''">
                <xsl:variable name="arg">
                    <xsl:value-of select="$id"/>
                    <xsl:text>, Key_</xsl:text>
                    <xsl:value-of select="@key"/>
                </xsl:variable>
                <xsl:if test="@brief!=''">
                    <xsl:value-of select="$arg"/>
                    <xsl:text>}, // </xsl:text>
                    <xsl:value-of select="@brief"/>
                    <xsl:value-of select="$new_line"/>
                </xsl:if>
                <xsl:call-template name="Build_subcommands">
                    <xsl:with-param name="id" select="$arg"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@integer!=''">
                <xsl:variable name="arg">
                    <xsl:value-of select="$id"/>
                    <xsl:text>, Int_</xsl:text>
                    <xsl:value-of select="@number"/>
                </xsl:variable>
                <xsl:if test="@brief!=''">
                    <xsl:value-of select="$arg"/>
                    <xsl:text>}, // </xsl:text>
                    <xsl:value-of select="@brief"/>
                    <xsl:value-of select="$new_line"/>
                </xsl:if>
                <xsl:call-template name="Build_subcommands">
                    <xsl:with-param name="id" select="$arg"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@string!=''">
                <xsl:variable name="arg">
                    <xsl:value-of select="$id"/>
                    <xsl:text>, Str_</xsl:text>
                    <xsl:value-of select="@string"/>
                </xsl:variable>
                <xsl:if test="@brief!=''">
                    <xsl:value-of select="$arg"/>
                    <xsl:text>}, // </xsl:text>
                    <xsl:value-of select="@brief"/>
                    <xsl:value-of select="$new_line"/>
                </xsl:if>
                <xsl:call-template name="Build_subcommands">
                    <xsl:with-param name="id" select="$arg"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@bitmask!=''">
                <xsl:variable name="arg">
                    <xsl:value-of select="$id"/>
                    <xsl:text>, Str_</xsl:text>
                    <xsl:value-of select="@bitmask"/>
                </xsl:variable>
                <xsl:if test="@brief!=''">
                    <xsl:value-of select="$arg"/>
                    <xsl:text>}, // </xsl:text>
                    <xsl:value-of select="@brief"/>
                    <xsl:value-of select="$new_line"/>
                </xsl:if>
                <xsl:call-template name="Build_subcommands">
                    <xsl:with-param name="id" select="$arg"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$id"/>
                <xsl:text>, No_key}, // </xsl:text>
                <xsl:value-of select="@brief"/>
                <xsl:value-of select="$new_line"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>

