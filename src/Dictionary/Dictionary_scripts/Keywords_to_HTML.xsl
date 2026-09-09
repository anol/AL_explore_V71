<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:param name="date" select="date"/>

    <xsl:template match="/keyword_definition">
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
                <table>
                    <tr>
                        <th>Id</th>
                        <th>Name</th>
                        <th>Description</th>
                    </tr>
                    <xsl:call-template name="One_keyword">
                        <xsl:with-param name="id">0</xsl:with-param>
                        <xsl:with-param name="name">"No key"</xsl:with-param>
                        <xsl:with-param name="brief">Special purpose</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="All_keywords"/>
                    <xsl:call-template name="One_keyword">
                        <xsl:with-param name="id">255</xsl:with-param>
                        <xsl:with-param name="name">"Wildcard"</xsl:with-param>
                        <xsl:with-param name="brief">Special purpose, used in search</xsl:with-param>
                    </xsl:call-template>
                </table>
            </body>
        </html>
    </xsl:template>

    <xsl:template name="All_keywords">
        <xsl:for-each select="descendant::key">
            <xsl:sort select="@name"/>
            <xsl:call-template name="One_keyword">
                <xsl:with-param name="id" select="position()"/>
                <xsl:with-param name="name" select="@name"/>
                <xsl:with-param name="brief" select="@brief"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="One_keyword">
        <xsl:param name="id"/>
        <xsl:param name="name"/>
        <xsl:param name="brief"/>
        <tr>
            <td>
                <xsl:value-of select="$id"/>
            </td>
            <td>
                <xsl:value-of select="$name"/>
            </td>
            <td>
                <xsl:value-of select="$brief"/>
            </td>
        </tr>
    </xsl:template>

</xsl:stylesheet>

