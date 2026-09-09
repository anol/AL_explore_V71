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
#include &lt;cstdint&gt;

#include "</xsl:text>
        <xsl:value-of select="@keywords"/>
        <xsl:text>_keyword_lookup.h"
#include "</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_lookup.h"
#include "</xsl:text>
        <xsl:value-of select="@name"/>
        <xsl:text>_command_help.h"

using namespace Instruction;

namespace </xsl:text><xsl:value-of select="@interface"/>
        <xsl:text>{</xsl:text>
        <xsl:call-template name="Build_tables"/>
        <xsl:text>

    const Instruction_token * get_commands() { return t_main; }

}
</xsl:text>
    </xsl:template>

    <!-- Build Table-->
    <xsl:template name="Build_tables">
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:variable name="provider" select="@key"/>
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="Make_table">
                        <xsl:with-param name="table_name" select="''"/>
                        <xsl:with-param name="provider" select="$provider"/>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
        <xsl:text>

    const Instruction_token t_main[] = {
        </xsl:text>
        <xsl:for-each select="commands">
            <xsl:for-each select="provider">
                <xsl:for-each select="arg">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="Make_keyword_entry">
                        <xsl:with-param name="token_text" select="@key"/>
                        <xsl:with-param name="action_id">
                            <xsl:text>Key_</xsl:text><xsl:value-of select="@key"/>
                        </xsl:with-param>
                        <xsl:with-param name="next_table">
                            <xsl:text>_</xsl:text>
                            <xsl:value-of select="@key"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:for-each>
        </xsl:for-each>
        <xsl:text>{End_token, "", No_key, (uint32_t)0, nullptr}
    };</xsl:text>
    </xsl:template>

    <!-- Make Table-->
    <xsl:template name="Make_table">
        <xsl:param name="table_name"/>
        <xsl:param name="provider"/>
        <xsl:variable name="my_table_name">
            <xsl:value-of select="$table_name"/>
            <xsl:text>_</xsl:text>
            <xsl:value-of select="@key | @integer | @float | @string | @bitmask"/>
        </xsl:variable>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <xsl:call-template name="Make_table">
                <xsl:with-param name="table_name" select="$my_table_name"/>
                <xsl:with-param name="provider" select="$provider"/>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:text>

    const Instruction_token token</xsl:text><xsl:value-of select="$my_table_name"/><xsl:text>[] = {
        </xsl:text>
        <xsl:for-each select="arg">
            <xsl:sort select="@key | @value"/>
            <!-- KEYWORD -->
            <xsl:if test="@key != ''">
                <xsl:call-template name="Make_keyword_entry">
                    <xsl:with-param name="token_text" select="@key"/>
                    <xsl:with-param name="action_id">
                        <xsl:text>Key_</xsl:text><xsl:value-of select="@key"/>
                    </xsl:with-param>
                    <xsl:with-param name="next_table">
                        <xsl:value-of select="$my_table_name"/><xsl:text>_</xsl:text><xsl:value-of select="@key"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
            <!-- INTEGER -->
            <xsl:if test="@integer != ''">
                <xsl:variable name="value_name">
                    <xsl:value-of select="@integer"/>
                </xsl:variable>
                <xsl:call-template name="Make_integer_entry">
                    <xsl:with-param name="action_id">
                        <xsl:text>Key_</xsl:text><xsl:value-of select="@integer"/>
                    </xsl:with-param>
                    <xsl:with-param name="next_table">
                        <xsl:value-of select="$my_table_name"/><xsl:text>_</xsl:text><xsl:value-of select="@integer"/>
                    </xsl:with-param>
                    <xsl:with-param name="min_value">
                        <xsl:value-of select="/command_definition/integers/integer[@key=$value_name]/@min"/>
                    </xsl:with-param>
                    <xsl:with-param name="max_value">
                        <xsl:text>static_cast&lt;int32_t&gt;(</xsl:text>
                        <xsl:value-of select="/command_definition/integers/integer[@key=$value_name]/@max"/>
                        <xsl:text>)</xsl:text>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
            <!-- FLOAT -->
            <xsl:if test="@float != ''">
                <xsl:variable name="value_name">
                    <xsl:value-of select="@float"/>
                </xsl:variable>
                <xsl:call-template name="Make_float_entry">
                    <xsl:with-param name="action_id">
                        <xsl:text>Key_</xsl:text><xsl:value-of select="@float"/>
                    </xsl:with-param>
                    <xsl:with-param name="next_table">
                        <xsl:value-of select="$my_table_name"/><xsl:text>_</xsl:text><xsl:value-of select="@float"/>
                    </xsl:with-param>
                    <xsl:with-param name="min_value">
                        <xsl:value-of select="/command_definition/floats/float[@key=$value_name]/@min"/>
                    </xsl:with-param>
                    <xsl:with-param name="max_value">
                        <xsl:value-of select="/command_definition/floats/float[@key=$value_name]/@max"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
            <!-- STRING -->
            <xsl:if test="@string != ''">
                <xsl:variable name="value_name">
                    <xsl:value-of select="@string"/>
                </xsl:variable>
                <xsl:call-template name="Make_string_entry">
                    <xsl:with-param name="action_id">
                        <xsl:text>Key_</xsl:text><xsl:value-of select="@string"/>
                    </xsl:with-param>
                    <xsl:with-param name="next_table">
                        <xsl:value-of select="$my_table_name"/><xsl:text>_</xsl:text><xsl:value-of select="@string"/>
                    </xsl:with-param>
                    <xsl:with-param name="max_length">
                        <xsl:value-of select="/command_definition/strings/str[@key=$value_name]/@max_length"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
            <!-- BITMASK -->
            <xsl:if test="@bitmask != ''">
                <xsl:variable name="value_name">
                    <xsl:value-of select="@bitmask"/>
                </xsl:variable>
                <xsl:call-template name="Make_bitmask_entry">
                    <xsl:with-param name="action_id">
                        <xsl:text>Key_</xsl:text><xsl:value-of select="@bitmask"/>
                    </xsl:with-param>
                    <xsl:with-param name="next_table">
                        <xsl:value-of select="$my_table_name"/><xsl:text>_</xsl:text><xsl:value-of select="@bitmask"/>
                    </xsl:with-param>
                    <xsl:with-param name="max_length">
                        <xsl:value-of select="/command_definition/strings/str[@key=$value_name]/@max_length"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
        </xsl:for-each>
        <!-- HELP -->
        <xsl:if test="@brief != ''">
            <xsl:call-template name="Make_help_entry">
                <xsl:with-param name="token_text">
                    <xsl:value-of select="$my_table_name"/>
                </xsl:with-param>
                <xsl:with-param name="provider" select="$provider"/>
            </xsl:call-template>
        </xsl:if>
        <xsl:text>{End_token, "", No_key, (uint32_t)0, nullptr}
    };</xsl:text>
    </xsl:template>

    <!-- Table Entry-->
    <xsl:template name="Make_keyword_entry">
        <xsl:param name="token_text"/>
        <xsl:param name="action_id"/>
        <xsl:param name="next_table"/>
        <xsl:text>{Keyword_token, "</xsl:text>
        <xsl:value-of select="$token_text"/>
        <xsl:text>", </xsl:text>
        <xsl:value-of select="$action_id"/>
        <xsl:text>, (uint32_t)0, token</xsl:text>
        <xsl:value-of select="$next_table"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

    <!-- Help Entry-->
    <xsl:template name="Make_help_entry">
        <xsl:param name="token_text"/>
        <xsl:param name="provider"/>
        <xsl:text>{help</xsl:text>
        <xsl:value-of select="$token_text"/>
        <xsl:text>, Cmd</xsl:text>
        <xsl:value-of select="$token_text"/>
        <xsl:text>, Key_</xsl:text>
        <xsl:value-of select="$provider"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

    <!-- Integer Entry-->
    <xsl:template name="Make_integer_entry">
        <xsl:param name="action_id"/>
        <xsl:param name="next_table"/>
        <xsl:param name="min_value"/>
        <xsl:param name="max_value"/>
        <xsl:text>{Integer_token, </xsl:text>
        <xsl:value-of select="$action_id"/>
        <xsl:text>, token</xsl:text>
        <xsl:value-of select="$next_table"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="$min_value"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="$max_value"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

    <!-- Float Entry-->
    <xsl:template name="Make_float_entry">
        <xsl:param name="action_id"/>
        <xsl:param name="next_table"/>
        <xsl:param name="min_value"/>
        <xsl:param name="max_value"/>
        <xsl:text>{Float_token, </xsl:text>
        <xsl:value-of select="$action_id"/>
        <xsl:text>, token</xsl:text>
        <xsl:value-of select="$next_table"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="$min_value"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="$max_value"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

    <!-- String Entry-->
    <xsl:template name="Make_string_entry">
        <xsl:param name="action_id"/>
        <xsl:param name="next_table"/>
        <xsl:param name="max_length"/>
        <xsl:text>{String_token, </xsl:text>
        <xsl:value-of select="$action_id"/>
        <xsl:text>, token</xsl:text>
        <xsl:value-of select="$next_table"/>
        <xsl:text>, (uint32_t)0, </xsl:text>
        <xsl:value-of select="$max_length"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

    <!-- Bitmask Entry-->
    <xsl:template name="Make_bitmask_entry">
        <xsl:param name="action_id"/>
        <xsl:param name="next_table"/>
        <xsl:param name="max_length"/>
        <xsl:text>{Bitmask_token, </xsl:text>
        <xsl:value-of select="$action_id"/>
        <xsl:text>, token</xsl:text>
        <xsl:value-of select="$next_table"/>
        <xsl:text>, (uint32_t)0, </xsl:text>
        <xsl:value-of select="$max_length"/>
        <xsl:text>},
        </xsl:text>
    </xsl:template>

</xsl:stylesheet>

