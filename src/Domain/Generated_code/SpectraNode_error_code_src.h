
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

#include "SpectraNode_error_code.h"

namespace Error_handling
{
    static constexpr bb::enumeration_unordered<Error_code_storage, 3U> module_names_e{{{
        {Module_base_codes::Support_Console, "Support_Console"},
        {Module_base_codes::Support_Service, "Support_Service"},
        {Module_base_codes::Application_task, "Application_task"}}},
        {static_cast<Error_code_storage>(-1), "No such module"}};


    static constexpr Error_code_storage module_from_code(Error_code_storage code) { return (code >> module_offset::value) & module_mask::value; }
    static const char *module_name(Error_code_storage code) { return module_names_e[module_from_code(code)]; }


    static constexpr bb::enumeration_unordered<Error_code_storage, 3U> error_code_names_e{{{
        {Support_Console_error_codes::Token_limit_exceeded, "Token_limit_exceeded"},
        {Support_Service_error_codes::No_instruction_handler, "No_instruction_handler"},
        {Application_task_error_codes::Too_long_command, "Too_long_command"}}},
        {static_cast<Error_code_storage>(-1), "No such error"}};

    static constexpr bb::enumeration_unordered<Error_code_storage, 3U> error_code_descriptions_e{{{
        {Support_Console_error_codes::Token_limit_exceeded, "Too many tokens in console command"},
        {Support_Service_error_codes::No_instruction_handler, "No instruction handler was found on indication lookup"},
        {Application_task_error_codes::Too_long_command, "A command is too long (debug UART only)"}}},
        {static_cast<Error_code_storage>(-1), "No such error"}};
        
} // Error_handling

