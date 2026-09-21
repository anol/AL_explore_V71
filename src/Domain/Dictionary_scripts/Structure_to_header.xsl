<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main -->
    <xsl:template match="/structure">
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

module;
#include &lt;cstdint&gt;

export module Domain.<xsl:value-of select="@keywords"/>_structure;

export namespace <xsl:value-of select="@interface"/>
{

    enum { Form_null, Form_keyword, Form_array, Form_reserved };

    uint32_t get_version();

    void set_version(uint32_t version);

    uint32_t get_sizeof_data();

    uint32_t* get_data_buffer();

    uint32_t get_number_of_entries();

    uint32_t* get_table_of_values();

    uint32_t* get_table_of_redundant_values();

    uint32_t* get_table_of_caches();

    uint8_t* get_table_of_cache_flags();

    uint8_t* get_table_of_flags();

    const uint8_t* get_table_of_forms();

    const uint32_t* get_table_of_keys();

    const uint32_t* get_table_of_defaults();

}

    </xsl:template>

</xsl:stylesheet>
