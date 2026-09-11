<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:param name="target" select="''"/>

    <!-- Main-->
    <xsl:template match="/structure">
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

// Please note: the content of this file was generated using XSLT.

#include &lt;cstdint&gt;

#include "</xsl:text><xsl:value-of select="@keywords"/><xsl:text>.h"
#include "</xsl:text><xsl:value-of select="@name"/><xsl:text>.h"

namespace </xsl:text><xsl:value-of select="@interface"/><xsl:text>{

        static inline constexpr uint32_t key_is(uint8_t p1, uint8_t s2, uint8_t t3, uint8_t q4) {
        return (p1 &lt;&lt; 24 ) | (s2 &lt;&lt; 16 ) | (t3 &lt;&lt; 8) | q4;
        }

        static inline constexpr uint32_t default_is(uint8_t index, uint32_t default_value) {
        return default_value;
        }

        static inline constexpr uint32_t form_is(uint8_t p1, uint8_t s2, uint8_t t3, uint8_t q4) {
        return (p1 &lt;&lt; 6 ) | (s2 &lt;&lt; 4 ) | (t3 &lt;&lt; 2) | q4;
        }

        static constexpr uint32_t the_keys[] = {</xsl:text>
        <xsl:text>0,
        </xsl:text>
        <xsl:for-each select="primary">
            <xsl:sort select="@key"/>
            <xsl:call-template name="primary_rule">
                <xsl:with-param name="use_function">
                    <xsl:text>key_is</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:text>0xFFFFFFFF
};</xsl:text>

        static constexpr uint32_t the_defaults[] = {
        <xsl:text>0,
        </xsl:text>
        <xsl:for-each select="primary">
            <xsl:sort select="@key"/>
            <xsl:call-template name="primary_rule">
                <xsl:with-param name="use_function">
                    <xsl:text>default_is</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:text>0
};</xsl:text>

        static constexpr uint8_t the_forms[] = {
        <xsl:text>0,
        </xsl:text>
        <xsl:for-each select="primary">
            <xsl:sort select="@key"/>
            <xsl:call-template name="primary_rule">
                <xsl:with-param name="use_function">
                    <xsl:text>form_is</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:text>0xFF
};</xsl:text>

        <xsl:text>
constexpr uint32_t The_number_of_entries = sizeof(the_keys) / sizeof(uint32_t);


/*
 * @note: The cache flags are stored within the structure buffer in order to be persistent.
 *        They must be kept on reset in order to restore cached values.
 *        The other flags are not persistent so that they are re-initialized on reset
 */
struct Structure_buffer{
    uint32_t the_sentinel_1;
    uint32_t the_version;
    uint32_t the_sentinel_2;
    uint32_t the_values[The_number_of_entries];
    uint32_t the_redundant_values[The_number_of_entries];
    uint32_t the_caches[The_number_of_entries];
    uint8_t the_cache_flags[The_number_of_entries];
    uint32_t the_sentinel_3;
};

extern "C" Structure_buffer the_structure_definition __attribute__((section (".management_repos")));
Structure_buffer the_structure_definition;

uint8_t the_flags[The_number_of_entries]{};

uint32_t get_version() { return the_structure_definition.the_version; }

void set_version(uint32_t version) {
    the_structure_definition.the_sentinel_1 = 0xABBA'1111;
    the_structure_definition.the_version = version;
    the_structure_definition.the_sentinel_2 = 0xBABE'2222;
    the_structure_definition.the_sentinel_3 = 0xCAFE'3333;
}

uint32_t get_sizeof_data() { return sizeof(Structure_buffer); }

uint32_t* get_data_buffer() { return reinterpret_cast&lt;uint32_t*&gt;(&amp;the_structure_definition); }

uint32_t get_number_of_entries() { return The_number_of_entries; }

uint32_t* get_table_of_values() { return the_structure_definition.the_values; }

uint32_t* get_table_of_redundant_values() { return the_structure_definition.the_redundant_values; }

uint32_t* get_table_of_caches() { return the_structure_definition.the_caches; }

uint8_t* get_table_of_cache_flags() { return the_structure_definition.the_cache_flags; }

uint8_t* get_table_of_flags() { return the_flags; }

const uint8_t* get_table_of_forms() { return the_forms; }

const uint32_t* get_table_of_keys() { return the_keys; }

const uint32_t* get_table_of_defaults() { return the_defaults; }

        }</xsl:text>

    </xsl:template>

    <!-- primary_rule-->
    <xsl:template name="primary_rule">
        <xsl:param name="use_function"/>
        <xsl:variable name="primary_name" select="@key"/>
        <xsl:for-each select="secondary">
            <xsl:sort select="@key"/>
            <xsl:call-template name="secondary_rule">
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="primary_name">
<!--                    <xsl:text>Cmd_</xsl:text><xsl:value-of select="$primary_name"/>-->
                    <xsl:text>Key_</xsl:text><xsl:value-of select="$primary_name"/>
                </xsl:with-param>
                <xsl:with-param name="format">
                    <xsl:text>Form_keyword</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="secondary_rule">
        <xsl:param name="use_function"/>
        <xsl:param name="primary_name"/>
        <xsl:param name="format"/>
        <xsl:variable name="key" select="@key"/>
        <xsl:variable name="min" select="@min"/>
        <xsl:variable name="max" select="@max"/>
        <xsl:choose>
            <xsl:when test="(@key != '')">
                <xsl:for-each select="tertiary">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="tertiary_rule">
                        <xsl:with-param name="index">
                            <xsl:text>0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="secondary_name">
                            <xsl:value-of select="$primary_name"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Key_</xsl:text><xsl:value-of select="$key"/>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_keyword</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:when>
            <xsl:when test="(@min != '') and (@max != '')">
                <xsl:call-template name="secondary_recursive">
                    <xsl:with-param name="use_function" select="$use_function"/>
                    <xsl:with-param name="primary_name" select="$primary_name"/>
                    <xsl:with-param name="format" select="$format"/>
                    <xsl:with-param name="min" select="$min"/>
                    <xsl:with-param name="max" select="$max"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:for-each select="tertiary">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="tertiary_rule">
                        <xsl:with-param name="index">
                            <xsl:text>0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="secondary_name">
                            <xsl:value-of select="$primary_name"/>
                            <xsl:text>, 0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_null</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="secondary_recursive">
        <xsl:param name="use_function"/>
        <xsl:param name="primary_name"/>
        <xsl:param name="format"/>
        <xsl:param name="min"/>
        <xsl:param name="max"/>
        <xsl:if test="($min &lt;= $max)">
            <xsl:for-each select="tertiary">
                <xsl:sort select="@key"/>
                <xsl:call-template name="tertiary_rule">
                    <xsl:with-param name="index" select="$min"/>
                    <xsl:with-param name="use_function" select="$use_function"/>
                    <xsl:with-param name="secondary_name">
                        <xsl:value-of select="$primary_name"/>
                        <xsl:text>,</xsl:text>
                        <xsl:value-of select="$min"/>
                    </xsl:with-param>
                    <xsl:with-param name="format">
                        <xsl:value-of select="$format"/>
                        <xsl:text>,</xsl:text>
                        <xsl:text>Form_array</xsl:text>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:for-each>
            <xsl:call-template name="secondary_recursive">
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="primary_name" select="$primary_name"/>
                <xsl:with-param name="format" select="$format"/>
                <xsl:with-param name="min" select="$min + 1"/>
                <xsl:with-param name="max" select="$max"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <xsl:template name="tertiary_rule">
        <xsl:param name="index"/>
        <xsl:param name="use_function"/>
        <xsl:param name="secondary_name"/>
        <xsl:param name="format"/>
        <xsl:variable name="key" select="@key"/>
        <xsl:variable name="min" select="@min"/>
        <xsl:variable name="max" select="@max"/>
        <xsl:choose>
            <xsl:when test="(@key != '')">
                <xsl:for-each select="quaternary">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="quaternary_rule">
                        <xsl:with-param name="index" select="$index"/>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="tertiary_name">
                            <xsl:value-of select="$secondary_name"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Key_</xsl:text><xsl:value-of select="$key"/>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_keyword</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:when>
            <xsl:when test="(@min != '') and (@max != '')">
                <xsl:call-template name="tertiary_recursive">
                    <xsl:with-param name="use_function" select="$use_function"/>
                    <xsl:with-param name="secondary_name" select="$secondary_name"/>
                    <xsl:with-param name="format" select="$format"/>
                    <xsl:with-param name="min" select="$min"/>
                    <xsl:with-param name="max" select="$max"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:for-each select="quaternary">
                    <xsl:sort select="@key"/>
                    <xsl:call-template name="quaternary_rule">
                        <xsl:with-param name="index" select="$index"/>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="tertiary_name">
                            <xsl:value-of select="$secondary_name"/>
                            <xsl:text>, 0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_null</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="tertiary_recursive">
        <xsl:param name="use_function"/>
        <xsl:param name="secondary_name"/>
        <xsl:param name="format"/>
        <xsl:param name="min"/>
        <xsl:param name="max"/>
        <xsl:for-each select="quaternary">
            <xsl:sort select="@key"/>
            <xsl:call-template name="quaternary_rule">
                <xsl:with-param name="index" select="$min"/>
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="tertiary_name">
                    <xsl:value-of select="$secondary_name"/>
                    <xsl:text>,</xsl:text>
                    <xsl:value-of select="$min"/>
                </xsl:with-param>
                <xsl:with-param name="format">
                    <xsl:value-of select="$format"/>
                    <xsl:text>,</xsl:text>
                    <xsl:text>Form_array</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="($min &lt; $max)">
            <xsl:call-template name="tertiary_recursive">
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="secondary_name" select="$secondary_name"/>
                <xsl:with-param name="format" select="$format"/>
                <xsl:with-param name="min" select="$min + 1"/>
                <xsl:with-param name="max" select="$max"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <xsl:template name="quaternary_rule">
        <xsl:param name="index"/>
        <xsl:param name="use_function"/>
        <xsl:param name="tertiary_name"/>
        <xsl:param name="format"/>
        <xsl:variable name="key" select="@key"/>
        <xsl:variable name="min" select="@min"/>
        <xsl:variable name="max" select="@max"/>
        <xsl:choose>
            <xsl:when test="(@key != '')">
                <xsl:for-each select="value">
                    <xsl:call-template name="value_rule">
                        <xsl:with-param name="index" select="$index"/>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="quaternary_name">
                            <xsl:value-of select="$tertiary_name"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Key_</xsl:text><xsl:value-of select="$key"/>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_keyword</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:when>
            <xsl:when test="(@min != '') and (@max != '')">
                <xsl:call-template name="quaternary_recursive">
                    <xsl:with-param name="use_function" select="$use_function"/>
                    <xsl:with-param name="tertiary_name" select="$tertiary_name"/>
                    <xsl:with-param name="format" select="$format"/>
                    <xsl:with-param name="min" select="$min"/>
                    <xsl:with-param name="max" select="$max"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:for-each select="value">
                    <xsl:call-template name="value_rule">
                        <xsl:with-param name="index" select="$index"/>
                        <xsl:with-param name="use_function" select="$use_function"/>
                        <xsl:with-param name="quaternary_name">
                            <xsl:value-of select="$tertiary_name"/>
                            <xsl:text>, 0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="format">
                            <xsl:value-of select="$format"/>
                            <xsl:text>,</xsl:text>
                            <xsl:text>Form_null</xsl:text>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:for-each>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="quaternary_recursive">
        <xsl:param name="use_function"/>
        <xsl:param name="tertiary_name"/>
        <xsl:param name="format"/>
        <xsl:param name="min"/>
        <xsl:param name="max"/>
        <xsl:for-each select="value">
            <xsl:call-template name="value_rule">
                <xsl:with-param name="index" select="$min"/>
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="quaternary_name">
                    <xsl:value-of select="$tertiary_name"/>
                    <xsl:text>,</xsl:text>
                    <xsl:value-of select="$min"/>
                </xsl:with-param>
                <xsl:with-param name="format">
                    <xsl:value-of select="$format"/>
                    <xsl:text>,</xsl:text>
                    <xsl:text>Form_array</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:for-each>
        <xsl:if test="($min &lt; $max)">
            <xsl:call-template name="quaternary_recursive">
                <xsl:with-param name="use_function" select="$use_function"/>
                <xsl:with-param name="tertiary_name" select="$tertiary_name"/>
                <xsl:with-param name="format" select="$format"/>
                <xsl:with-param name="min" select="$min + 1"/>
                <xsl:with-param name="max" select="$max"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <xsl:template name="value_rule">
        <xsl:param name="index"/>
        <xsl:param name="use_function"/>
        <xsl:param name="quaternary_name"/>
        <xsl:param name="format"/>
        <xsl:choose>
            <xsl:when test="$use_function = 'form_is'">
                <xsl:value-of select="$use_function"/>
                <xsl:text>(</xsl:text>
                <xsl:value-of select="$format"/>
                <xsl:text>),
                </xsl:text>
            </xsl:when>
            <xsl:when test="$use_function = 'key_is'">
                <xsl:value-of select="$use_function"/>
                <xsl:text>(</xsl:text>
                <xsl:value-of select="$quaternary_name"/>
                <xsl:text>),
                </xsl:text>
            </xsl:when>
            <xsl:otherwise>
                <xsl:variable name="default" select="item[@index=$index]/@default"/>
                <xsl:choose>
                    <xsl:when test="($default != '')">
                        <xsl:value-of select="$default"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:choose>
                            <xsl:when test="$target = ''">
                                <xsl:value-of select="@default"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:choose>
                                    <xsl:when test="count(default) != 0">
                                        <xsl:for-each select="default">
                                            <xsl:if test="@target = $target">
                                                <xsl:value-of select="@value"/>
                                            </xsl:if>
                                        </xsl:for-each>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:value-of select="@default"/>
                                    </xsl:otherwise>
                                </xsl:choose>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:otherwise>
                </xsl:choose>
                <xsl:text>, // </xsl:text>
                <xsl:value-of select="$quaternary_name"/>
                <xsl:text>
                </xsl:text>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>

