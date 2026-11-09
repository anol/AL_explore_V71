
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
#include <cstdint>

export module Domain.SpectraNode_structure;

export namespace SpectraNode_interface
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

    const int32_t* get_table_of_defaults();

}

    