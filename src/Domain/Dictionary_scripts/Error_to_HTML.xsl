<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:param name="date" select="date"/>

    <xsl:template match="/error_codes">
        <html>
            <xsl:comment>

                <xsl:variable name="path">
                    <xsl:text>../Definition_IDEAS/Copyright.xml</xsl:text>
                </xsl:variable>
                <xsl:apply-templates select="document($path)"/>

            </xsl:comment>
            <head8>
                <style>
                    table {
                    font-family: arial, sans-serif;
                    border-collapse: collapse;
                    width: 100%;
                    }

                    td, th {
                    border: 1px solid #dddddd;
                    text-align: left;
                    padding: 8px;
                    }

                    tr:nth-child(even) {
                    background-color: #dddddd;
                    }
                </style>
                <title>
                    <xsl:value-of select="@name"/>
                </title>
            </head8>
            <body>
                <h1>
                    <xsl:value-of select="@name"/>
                    <xsl:text> version </xsl:text>
                    <xsl:value-of select="@version"/>
                </h1>
                <p>
                    Date: <xsl:value-of select="$date"/>
                </p>
                <xsl:for-each select="module">
                    <h2>
                        <xsl:value-of select="@name"/>
                        <xsl:text>: base 0x</xsl:text>
                        <xsl:value-of select="@hex"/>
                        <xsl:text>'0000</xsl:text>
                    </h2>
                    <xsl:choose>
                        <xsl:when test="count(error) = 0">
                            <p>
                                <xsl:text>No Error Code in this module.</xsl:text>
                            </p>
                        </xsl:when>
                        <xsl:otherwise>
                            <table>
                                <tr>
                                    <th>Name</th>
                                    <th>Code (decimal)</th>
                                    <th>Severity/Level</th>
                                    <th>Description</th>
                                </tr>
                                <xsl:for-each select="error">
                                    <tr>
                                        <td>
                                            <xsl:value-of select="@name"/>
                                        </td>
                                        <td>
                                            <xsl:value-of select="position()"/>
                                        </td>
                                        <td>
                                            <xsl:choose>
                                                <xsl:when test="@severity = 1">
                                                    <xsl:text>informative</xsl:text>
                                                </xsl:when>
                                                <xsl:when test="@severity = 3">
                                                    <xsl:text>medium severity</xsl:text>
                                                </xsl:when>
                                                <xsl:when test="@severity = 4">
                                                    <xsl:text>high severity</xsl:text>
                                                </xsl:when>
                                                <xsl:otherwise>
                                                    <xsl:text>low severity</xsl:text>
                                                </xsl:otherwise>
                                            </xsl:choose>
                                        </td>
                                        <td>
                                            <xsl:value-of select="@brief"/>
                                        </td>
                                    </tr>
                                </xsl:for-each>
                            </table>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:for-each>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>

