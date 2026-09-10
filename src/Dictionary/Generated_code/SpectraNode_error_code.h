
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

#include <cstdint>
#include <type_traits>
#include "Status_code.h"

namespace Dictionary
{
    using version = std::integral_constant<uint8_t, 1>;
    using module_offset = std::integral_constant<uint8_t, 16U>;
    using module_mask = std::integral_constant<uint8_t, 0xFFU>;


    enum Module_base_codes : Error_code_storage
    {
        Support_Console = 0x24 & module_mask::value,
        Support_Service = 0x2A & module_mask::value,
        Application_task = 0xA4 & module_mask::value
    };

        
    enum Support_Console_error_codes : Error_code_storage
    {
        Support_Console_base = Module_base_codes::Support_Console << module_offset::value,
        Token_limit_exceeded // +1 Too many tokens in console command
    };
            
    enum Support_Service_error_codes : Error_code_storage
    {
        Support_Service_base = Module_base_codes::Support_Service << module_offset::value,
        No_instruction_handler // +1 No instruction handler was found on indication lookup
    };
            
    enum Application_task_error_codes : Error_code_storage
    {
        Application_task_base = Module_base_codes::Application_task << module_offset::value,
        Too_long_command // +1 A command is too long (debug UART only)
    };
            

    static constexpr int severity_cast(Error_code_storage error)
    {
        switch (error)
        {
                return 1;
        
                return 3;
        
                return 4;

            default:
                return 2;
        }
    }

        
} // Error_handling

