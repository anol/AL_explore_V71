<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'" />
    <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

    <xsl:variable name="NameSpace" select="/error_codes/@namespace" />
    <xsl:variable name="NAMESPACE" select="translate($NameSpace, $lowercase, $uppercase)" />
    <xsl:variable name="namespace" select="translate($NameSpace, $uppercase, $lowercase)" />

    <xsl:variable name="NAME"      select="translate(/error_codes/@name, $lowercase, $uppercase)" />
    <xsl:variable name="INCLUDE_GUARD" select="concat($NAMESPACE, '_', translate($NAME, '.', '_'), '_H')" />


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

#include &lt;cstdint&gt;
#include &lt;type_traits&gt;
#include "Dictionary/Status_code.h"</xsl:text>
        <xsl:for-each select="include">
            <xsl:text>
#include "</xsl:text><xsl:value-of select="@path"/><xsl:text>"</xsl:text>
        </xsl:for-each>
        <xsl:text>

</xsl:text>

        <xsl:value-of select="$namespace_open"/>

        <xsl:text>
    using version = std::integral_constant&lt;uint8_t, </xsl:text><xsl:value-of select="@version"/><xsl:text>&gt;;
    using module_offset = std::integral_constant&lt;uint8_t, 16U&gt;;
    using module_mask = std::integral_constant&lt;uint8_t, 0xFFU&gt;;


    enum Module_base_codes : Error_code_storage
    {
        </xsl:text>
        <xsl:for-each select="module">
            <xsl:value-of select="@name"/><xsl:text> = 0x</xsl:text><xsl:value-of select="@hex"/><xsl:text> &amp; module_mask::value</xsl:text>
            <xsl:if test="position() != last()">
                <xsl:text>,
        </xsl:text>
            </xsl:if>
        </xsl:for-each>

        <xsl:text>
    };

        </xsl:text>

        <xsl:for-each select="module">
            <xsl:text>
    enum </xsl:text><xsl:value-of select="@name"/><xsl:text>_error_codes : Error_code_storage
    {
        </xsl:text>
            <xsl:variable name="module" select="@name"/>
            <xsl:value-of select="@name"/><xsl:text>_base = Module_base_codes::</xsl:text><xsl:value-of select="@name"/><xsl:text> &lt;&lt; module_offset::value,
        </xsl:text>
            <xsl:for-each select="error">
                <xsl:value-of select="@name"/>
                <xsl:if test="@value">
                    <xsl:text> = </xsl:text>
                    <xsl:value-of select="$module"/><xsl:text>_base + </xsl:text>
                    <xsl:value-of select="@value"/>
                </xsl:if>
                <xsl:if test="position() != last()">
                    <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:text> // +</xsl:text><xsl:value-of select="position()"/><xsl:text> </xsl:text><xsl:value-of select="@brief"/>
                <xsl:if test="position() != last()">
                    <xsl:text>
        </xsl:text>
                </xsl:if>
            </xsl:for-each>
            <xsl:text>
    };
            </xsl:text>
        </xsl:for-each>

        <xsl:text>

    static constexpr int severity_cast(Error_code_storage error)
    {
        switch (error)
        {</xsl:text>
        <xsl:call-template name="list_info_cases"/>
        <xsl:text>
                return 1;
        </xsl:text>
        <xsl:call-template name="list_medium_severity_cases"/>
        <xsl:text>
                return 3;
        </xsl:text>
        <xsl:call-template name="list_high_severity_cases"/>
        <xsl:text>
                return 4;

            default:
                return 2;
        }
    }

        </xsl:text>

        <xsl:value-of select="$namespace_close"/>

        <xsl:text>

    </xsl:text>
    </xsl:template>


    <xsl:template name="list_info_cases">
        <xsl:for-each select="/error_codes/module/error">
            <xsl:if test="@severity = 1">
                <xsl:text>
            case </xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>:</xsl:text>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="list_medium_severity_cases">
        <xsl:for-each select="/error_codes/module/error">
            <xsl:if test="@severity = 3">
                <xsl:text>
            case </xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>:</xsl:text>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="list_high_severity_cases">
        <xsl:for-each select="/error_codes/module/error">
            <xsl:if test="@severity = 4">
                <xsl:text>
            case </xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>:</xsl:text>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>
