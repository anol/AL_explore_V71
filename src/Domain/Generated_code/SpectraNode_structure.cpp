
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

module Domain.SpectraNode_structure;
import Domain.SpectraNode_keyword_lookup;

namespace SpectraNode{

        static inline constexpr uint32_t key_is(uint8_t p1, uint8_t s2, uint8_t t3, uint8_t q4) {
        return (p1 << 24 ) | (s2 << 16 ) | (t3 << 8) | q4;
        }

        static inline constexpr uint32_t default_is(uint8_t index, uint32_t default_value) {
        return default_value;
        }

        static inline constexpr uint32_t form_is(uint8_t p1, uint8_t s2, uint8_t t3, uint8_t q4) {
        return (p1 << 6 ) | (s2 << 4 ) | (t3 << 2) | q4;
        }

        static constexpr uint32_t the_keys[] = {0,
        0xFFFFFFFF
};

        static constexpr uint32_t the_defaults[] = {
        0,
        0
};

        static constexpr uint8_t the_forms[] = {
        0,
        0xFF
};
constexpr uint32_t The_number_of_entries = sizeof(the_keys) / sizeof(uint32_t);


/*
 * @note: The cache flags are stored within the structure buffer in order to be persistent.
 *        They must be kept on reset in order to restore cached values.
 *        The other flags are not persistent so that they are re-initialized on reset
 */
struct Structure_buffer{
    uint32_t the_sentinel_1;
    uint32_t the_version;
    uint32_t the_sentinel_2;
    uint32_t the_values[The_number_of_entries];
    uint32_t the_redundant_values[The_number_of_entries];
    uint32_t the_caches[The_number_of_entries];
    uint8_t the_cache_flags[The_number_of_entries];
    uint32_t the_sentinel_3;
};

extern "C" {
    Structure_buffer the_structure_definition __attribute__((section (".management_repos")));
}

uint8_t the_flags[The_number_of_entries]{};

uint32_t get_version() { return the_structure_definition.the_version; }

void set_version(uint32_t version) {
    the_structure_definition.the_sentinel_1 = 0xABBA'1111;
    the_structure_definition.the_version = version;
    the_structure_definition.the_sentinel_2 = 0xBABE'2222;
    the_structure_definition.the_sentinel_3 = 0xCAFE'3333;
}

uint32_t get_sizeof_data() { return sizeof(Structure_buffer); }

uint32_t* get_data_buffer() { return reinterpret_cast<uint32_t*>(&the_structure_definition); }

uint32_t get_number_of_entries() { return The_number_of_entries; }

uint32_t* get_table_of_values() { return the_structure_definition.the_values; }

uint32_t* get_table_of_redundant_values() { return the_structure_definition.the_redundant_values; }

uint32_t* get_table_of_caches() { return the_structure_definition.the_caches; }

uint8_t* get_table_of_cache_flags() { return the_structure_definition.the_cache_flags; }

uint8_t* get_table_of_flags() { return the_flags; }

const uint8_t* get_table_of_forms() { return the_forms; }

const uint32_t* get_table_of_keys() { return the_keys; }

const uint32_t* get_table_of_defaults() { return the_defaults; }

        }