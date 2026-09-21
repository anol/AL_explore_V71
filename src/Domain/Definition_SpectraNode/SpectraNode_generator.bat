::
:: Copyright (C) 2026 Integrated Detector Electronics AS
:: All Rights Reserved.
::
:: NOTICE: All information contained herein is, and remains
:: the property of Integrated Detector Electronics AS and its suppliers,
:: if any. The intellectual and technical concepts contained
:: herein are proprietary to Integrated Detector Electronics AS
:: and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
:: patents in process, and are protected by trade secret or copyright law.
:: Dissemination of this information or reproduction of this material
:: is strictly forbidden unless prior written permission is obtained
:: from Integrated Detector Electronics AS.
::

set SUBJECT=SpectraNode

set TARGET_DIR=..\Generated_code
set SCRIPT_DIR=..\Dictionary_scripts
set XSLT=..\..\..\tool\XSLT\msxsl

:: Keywords
set KEYSRC=%SUBJECT%_keyword
set KEYTRG=%TARGET_DIR%\%KEYSRC%
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_lookup_header.xsl -o  %KEYTRG%_lookup.cppm
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_lookup_source.xsl -o  %KEYTRG%_lookup.cpp
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_HTML.xsl -o    %KEYTRG%.html date="%DATE%"
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_version.xsl -o %KEYTRG%_version.cppm

:: Commands
set CMDSRC=%SUBJECT%_command
set CMDTRG=%TARGET_DIR%\%CMDSRC%
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_version.xsl -o %CMDTRG%_version.cppm
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_lookup_header.xsl -o %CMDTRG%_lookup.cppm
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_lookup_source.xsl -o %CMDTRG%_lookup.cpp
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_HTML_AT_style.xsl -o %CMDTRG%.html  date="%DATE%"
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_help.xsl -o %CMDTRG%_help.cppm
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_empty_help.xsl -o %CMDTRG%_empty_help.cppm

:: Reports
set RPTSRC=%SUBJECT%_report
set RPTTRG=%TARGET_DIR%\%RPTSRC%

:: Structure
set DEFSRC=%SUBJECT%_structure
set DEFTRG=%TARGET_DIR%\%DEFSRC%
%XSLT% %DEFSRC%.xml %SCRIPT_DIR%\Structure_to_header.xsl -o %DEFTRG%.cppm
%XSLT% %DEFSRC%.xml %SCRIPT_DIR%\Structure_to_source.xsl -o %DEFTRG%.cpp
%XSLT% %DEFSRC%.xml %SCRIPT_DIR%\Structure_to_HTML.xsl -o   %DEFTRG%.html date="%DATE%"
%XSLT% %DEFSRC%.xml %SCRIPT_DIR%\Structure_to_version.xsl -o %DEFTRG%_version.cppm

:: Providers
set PRVTRG=%TARGET_DIR%\%SUBJECT%_provider
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_indication_header.xsl -o %PRVTRG%_indication.cppm
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_indication_source.xsl -o %PRVTRG%_indication.cpp
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_skeleton_header.xsl -o %PRVTRG%_skeleton.cppm
%XSLT% %CMDSRC%.xml %SCRIPT_DIR%\Command_to_skeleton_source.xsl -o %PRVTRG%_skeleton.cpp
