<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'" />
    <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

    <xsl:variable name="NameSpace" select="/error_codes/@namespace" />
    <xsl:variable name="NAMESPACE" select="translate($NameSpace, $lowercase, $uppercase)" />
    <xsl:variable name="namespace" select="translate($NameSpace, $uppercase, $lowercase)" />

    <xsl:variable name="NAME"      select="translate(/error_codes/@name, $lowercase, $uppercase)" />
    <xsl:variable name="INCLUDE_GUARD" select="concat($NAMESPACE, '_', translate($NAME, '.', '_'), '_SOURCE_H')" />


    <xsl:variable name="namespace_open">
        <xsl:if test="$NameSpace != ''">
            <xsl:text>namespace </xsl:text><xsl:value-of select="$NameSpace"/><xsl:text>
{</xsl:text>
        </xsl:if>
    </xsl:variable>

    <xsl:variable name="namespace_close">
        <xsl:if test="$NameSpace != ''">
<xsl:text>
} // </xsl:text><xsl:value-of select="$NameSpace"/>
        </xsl:if>
    </xsl:variable>

    <!--<xsl:param name="date" select="date"/>-->

    <!-- Main -->
    <xsl:template match="/error_codes">
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

#pragma once

#include &lt;backbone&gt;
#include "</xsl:text><xsl:value-of select="@name"/><xsl:text>.h"

</xsl:text>

        <xsl:value-of select="$namespace_open"/>

        <xsl:text>
    static constexpr bb::enumeration_unordered&lt;Error_code_storage, </xsl:text><xsl:value-of select="count(module)"/><xsl:text>U&gt; module_names_e{{{
        </xsl:text>
        <xsl:for-each select="module">
            <xsl:text>{Module_base_codes::</xsl:text><xsl:value-of select="@name"/><xsl:text>, "</xsl:text><xsl:value-of select="@name"/><xsl:text>"}</xsl:text>
            <xsl:if test="position() != last()">
                <xsl:text>,
        </xsl:text>
            </xsl:if>
        </xsl:for-each>
        <xsl:text>}},
        {static_cast&lt;Error_code_storage&gt;(-1), "No such module"}};


    static constexpr Error_code_storage module_from_code(Error_code_storage code) { return (code &gt;&gt; module_offset::value) &amp; module_mask::value; }
    static const char *module_name(Error_code_storage code) { return module_names_e[module_from_code(code)]; }


    static constexpr bb::enumeration_unordered&lt;Error_code_storage, </xsl:text><xsl:value-of select="count(module/error)"/><xsl:text>U&gt; error_code_names_e{{{
        </xsl:text>
        <xsl:for-each select="module">
            <xsl:variable name="module_name" select="@name"/>
            <xsl:variable name="module_cnt" select="last()"/>
            <xsl:variable name="module_index" select="position()"/>
            <xsl:for-each select="error">
                <xsl:text>{</xsl:text><xsl:value-of select="$module_name"/><xsl:text>_error_codes::</xsl:text><xsl:value-of select="@name"/>
                <xsl:text>, "</xsl:text><xsl:value-of select="@name"/><xsl:text>"}</xsl:text>
                <xsl:if test="(position() != last()) or ($module_index &lt; $module_cnt)">
                    <xsl:text>,
        </xsl:text>
                </xsl:if>
            </xsl:for-each>
        </xsl:for-each>
        <xsl:text>}},
        {static_cast&lt;Error_code_storage&gt;(-1), "No such error"}};

    static constexpr bb::enumeration_unordered&lt;Error_code_storage, </xsl:text><xsl:value-of select="count(module/error)"/><xsl:text>U&gt; error_code_descriptions_e{{{
        </xsl:text>
        <xsl:for-each select="module">
            <xsl:variable name="module_name" select="@name"/>
            <xsl:variable name="module_cnt" select="last()"/>
            <xsl:variable name="module_index" select="position()"/>
            <xsl:for-each select="error">
                <xsl:text>{</xsl:text><xsl:value-of select="$module_name"/><xsl:text>_error_codes::</xsl:text><xsl:value-of select="@name"/>
                <xsl:text>, "</xsl:text><xsl:value-of select="@brief"/><xsl:text>"}</xsl:text>
                <xsl:if test="(position() != last()) or ($module_index &lt; $module_cnt)">
                    <xsl:text>,
        </xsl:text>
                </xsl:if>
            </xsl:for-each>
        </xsl:for-each>
        <xsl:text>}},
        {static_cast&lt;Error_code_storage&gt;(-1), "No such error"}};
        </xsl:text>

        <xsl:value-of select="$namespace_close"/>

        <xsl:text>

        </xsl:text>
    </xsl:template>

</xsl:stylesheet>