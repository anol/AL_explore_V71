<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text" omit-xml-declaration="yes" encoding="US-ASCII"/>

    <!-- Main -->
    <xsl:template match="/command_definition">
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

#ifndef <xsl:value-of select="@name"/>_test_h
#define <xsl:value-of select="@name"/>_test_h

#include &lt;Instruction/CLI_token.h&gt;

namespace <xsl:value-of select="@interface"/>
{
    enum { Max_number_of_tokens = 16 };

    uint32_t get_test_instruction_count();

    const uint8_t (&amp;get_test_instruction(uint32_t index))[Max_number_of_tokens];

}

#endif // <xsl:value-of select="@name"/>_test_h
    </xsl:template>

</xsl:stylesheet>
