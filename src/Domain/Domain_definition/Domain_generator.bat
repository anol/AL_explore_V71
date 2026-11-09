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

set SUBJECT=Domain

set TARGET_DIR=..\Generated_code
set SCRIPT_DIR=..\Domain_scripts
set XSLT=..\..\..\tool\XSLT\msxsl

:: Keywords
set KEYSRC=%SUBJECT%_keyword
set KEYTRG=%TARGET_DIR%\%KEYSRC%
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_lookup_header.xsl -o  %KEYTRG%_lookup.cppm
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_lookup_source.xsl -o  %KEYTRG%_lookup.cpp
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_MD.xsl -o    %KEYTRG%.md date="%DATE%"
%XSLT% %KEYSRC%.xml %SCRIPT_DIR%\Keywords_to_version.xsl -o %KEYTRG%_version.cppm
