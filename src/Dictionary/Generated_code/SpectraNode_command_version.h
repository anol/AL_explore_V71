

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


/*
*    Please note: the content of this file was generated using XSLT.
*
*                 D O   N O T   E D I T
*/

#ifndef SpectraNode_COMMAND_VERSION_H
#define SpectraNode_COMMAND_VERSION_H

#include <cstdint>
#include <type_traits>

#define INSTRUCTION_VERSION "3"

namespace FW1038
{
    namespace version
    {
        using instruction = std::integral_constant<uint8_t, 3>;
    }
}

#endif // SpectraNode_version_h
