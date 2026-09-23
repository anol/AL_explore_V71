
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

namespace SpectraNode_interface{

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
        key_is(Key_CADENCE,Key_DEMO, 0, 0),
                key_is(Key_CADENCE,Key_IDLE, 0, 0),
                key_is(Key_CADENCE,Key_NOMINAL, 0, 0),
                key_is(Key_calibration,Key_adc,Key_V35, 0),
                key_is(Key_calibration,Key_adc,Key_V40, 0),
                key_is(Key_calibration,Key_BIAS, 0, 0),
                key_is(Key_calibration,Key_dac,Key_V35, 0),
                key_is(Key_calibration,Key_dac,Key_V40, 0),
                key_is(Key_calibration,Key_gain,1, 0),
                key_is(Key_calibration,Key_gain,2, 0),
                key_is(Key_calibration,Key_gain,3, 0),
                key_is(Key_calibration,Key_gain,4, 0),
                key_is(Key_calibration,Key_gain,5, 0),
                key_is(Key_calibration,Key_gain,6, 0),
                key_is(Key_calibration,Key_gain,7, 0),
                key_is(Key_calibration,Key_gain,8, 0),
                key_is(Key_calibration,Key_gain,9, 0),
                key_is(Key_calibration,Key_gain,10, 0),
                key_is(Key_calibration,Key_gain,11, 0),
                key_is(Key_calibration,Key_gain,12, 0),
                key_is(Key_calibration,Key_gain,13, 0),
                key_is(Key_calibration,Key_gain,14, 0),
                key_is(Key_calibration,Key_gain,15, 0),
                key_is(Key_calibration,Key_gain,16, 0),
                key_is(Key_calibration,Key_integration_time, 0, 0),
                key_is(Key_calibration,Key_parameter,Key_a, 0),
                key_is(Key_calibration,Key_parameter,Key_b, 0),
                key_is(Key_calibration,Key_pedestal,1, 0),
                key_is(Key_calibration,Key_pedestal,2, 0),
                key_is(Key_calibration,Key_pedestal,3, 0),
                key_is(Key_calibration,Key_pedestal,4, 0),
                key_is(Key_calibration,Key_pedestal,5, 0),
                key_is(Key_calibration,Key_pedestal,6, 0),
                key_is(Key_calibration,Key_pedestal,7, 0),
                key_is(Key_calibration,Key_pedestal,8, 0),
                key_is(Key_calibration,Key_pedestal,9, 0),
                key_is(Key_calibration,Key_pedestal,10, 0),
                key_is(Key_calibration,Key_pedestal,11, 0),
                key_is(Key_calibration,Key_pedestal,12, 0),
                key_is(Key_calibration,Key_pedestal,13, 0),
                key_is(Key_calibration,Key_pedestal,14, 0),
                key_is(Key_calibration,Key_pedestal,15, 0),
                key_is(Key_calibration,Key_pedestal,16, 0),
                key_is(Key_calibration,Key_readout_count, 0, 0),
                key_is(Key_calibration,Key_start_threshold, 0, 0),
                key_is(Key_calibration,Key_stop_count, 0, 0),
                key_is(Key_CHANNEL,Key_DEMO, 0, 0),
                key_is(Key_CHANNEL,Key_IDLE, 0, 0),
                key_is(Key_CHANNEL,Key_NOMINAL, 0, 0),
                key_is(Key_device,Key_reg_addr,0, 0),
                key_is(Key_device,Key_reg_addr,1, 0),
                key_is(Key_device,Key_reg_addr,2, 0),
                key_is(Key_device,Key_reg_addr,3, 0),
                key_is(Key_device,Key_reg_addr,4, 0),
                key_is(Key_device,Key_reg_addr,5, 0),
                key_is(Key_device,Key_reg_addr,6, 0),
                key_is(Key_device,Key_reg_addr,7, 0),
                key_is(Key_device,Key_reg_addr,8, 0),
                key_is(Key_device,Key_reg_addr,9, 0),
                key_is(Key_device,Key_reg_addr,10, 0),
                key_is(Key_device,Key_reg_addr,11, 0),
                key_is(Key_device,Key_reg_addr,12, 0),
                key_is(Key_device,Key_reg_addr,13, 0),
                key_is(Key_device,Key_reg_addr,14, 0),
                key_is(Key_device,Key_reg_addr,15, 0),
                key_is(Key_device,Key_reg_addr,16, 0),
                key_is(Key_device,Key_reg_addr,17, 0),
                key_is(Key_device,Key_reg_addr,18, 0),
                key_is(Key_device,Key_reg_addr,19, 0),
                key_is(Key_device,Key_reg_addr,20, 0),
                key_is(Key_device,Key_reg_addr,21, 0),
                key_is(Key_device,Key_reg_addr,22, 0),
                key_is(Key_device,Key_reg_addr,23, 0),
                key_is(Key_device,Key_reg_addr,24, 0),
                key_is(Key_device,Key_reg_addr,25, 0),
                key_is(Key_device,Key_reg_addr,26, 0),
                key_is(Key_device,Key_reg_addr,27, 0),
                key_is(Key_device,Key_reg_addr,28, 0),
                key_is(Key_device,Key_reg_addr,29, 0),
                key_is(Key_device,Key_serial_number, 0, 0),
                key_is(Key_format,Key_DEMO, 0, 0),
                key_is(Key_format,Key_IDLE, 0, 0),
                key_is(Key_format,Key_NOMINAL, 0, 0),
                key_is(Key_MODE,Key_active, 0, 0),
                0xFFFFFFFF
};

        static constexpr int32_t the_defaults[] = {
        0,
        2, // Key_CADENCE,Key_DEMO, 0, 0
                2, // Key_CADENCE,Key_IDLE, 0, 0
                2, // Key_CADENCE,Key_NOMINAL, 0, 0
                -35000, // Key_calibration,Key_adc,Key_V35, 0
                -40000, // Key_calibration,Key_adc,Key_V40, 0
                0, // Key_calibration,Key_BIAS, 0, 0
                -35000, // Key_calibration,Key_dac,Key_V35, 0
                -40000, // Key_calibration,Key_dac,Key_V40, 0
                1, // Key_calibration,Key_gain,1, 0
                1, // Key_calibration,Key_gain,2, 0
                1, // Key_calibration,Key_gain,3, 0
                1, // Key_calibration,Key_gain,4, 0
                1, // Key_calibration,Key_gain,5, 0
                1, // Key_calibration,Key_gain,6, 0
                1, // Key_calibration,Key_gain,7, 0
                1, // Key_calibration,Key_gain,8, 0
                1, // Key_calibration,Key_gain,9, 0
                1, // Key_calibration,Key_gain,10, 0
                1, // Key_calibration,Key_gain,11, 0
                1, // Key_calibration,Key_gain,12, 0
                1, // Key_calibration,Key_gain,13, 0
                1, // Key_calibration,Key_gain,14, 0
                1, // Key_calibration,Key_gain,15, 0
                1, // Key_calibration,Key_gain,16, 0
                100, // Key_calibration,Key_integration_time, 0, 0
                100000, // Key_calibration,Key_parameter,Key_a, 0
                10000, // Key_calibration,Key_parameter,Key_b, 0
                50, // Key_calibration,Key_pedestal,1, 0
                50, // Key_calibration,Key_pedestal,2, 0
                50, // Key_calibration,Key_pedestal,3, 0
                50, // Key_calibration,Key_pedestal,4, 0
                50, // Key_calibration,Key_pedestal,5, 0
                50, // Key_calibration,Key_pedestal,6, 0
                50, // Key_calibration,Key_pedestal,7, 0
                50, // Key_calibration,Key_pedestal,8, 0
                50, // Key_calibration,Key_pedestal,9, 0
                50, // Key_calibration,Key_pedestal,10, 0
                50, // Key_calibration,Key_pedestal,11, 0
                50, // Key_calibration,Key_pedestal,12, 0
                50, // Key_calibration,Key_pedestal,13, 0
                50, // Key_calibration,Key_pedestal,14, 0
                50, // Key_calibration,Key_pedestal,15, 0
                50, // Key_calibration,Key_pedestal,16, 0
                100000, // Key_calibration,Key_readout_count, 0, 0
                100, // Key_calibration,Key_start_threshold, 0, 0
                100, // Key_calibration,Key_stop_count, 0, 0
                18, // Key_CHANNEL,Key_DEMO, 0, 0
                18, // Key_CHANNEL,Key_IDLE, 0, 0
                18, // Key_CHANNEL,Key_NOMINAL, 0, 0
                0x0200'0323, // Key_device,Key_reg_addr,0, 0
                0x0200'0323, // Key_device,Key_reg_addr,1, 0
                0x0200'0323, // Key_device,Key_reg_addr,2, 0
                0x0200'0323, // Key_device,Key_reg_addr,3, 0
                0x0200'0323, // Key_device,Key_reg_addr,4, 0
                0x0200'0323, // Key_device,Key_reg_addr,5, 0
                0x0200'0323, // Key_device,Key_reg_addr,6, 0
                0x0200'0323, // Key_device,Key_reg_addr,7, 0
                0x0200'0323, // Key_device,Key_reg_addr,8, 0
                0x0200'0323, // Key_device,Key_reg_addr,9, 0
                0x0200'0323, // Key_device,Key_reg_addr,10, 0
                0x0200'0323, // Key_device,Key_reg_addr,11, 0
                0x0200'0323, // Key_device,Key_reg_addr,12, 0
                0x0200'0323, // Key_device,Key_reg_addr,13, 0
                0x0200'0323, // Key_device,Key_reg_addr,14, 0
                0x0200'0323, // Key_device,Key_reg_addr,15, 0
                0x0000'03C3, // Key_device,Key_reg_addr,16, 0
                0x0001'9494, // Key_device,Key_reg_addr,17, 0
                0x0000'0028, // Key_device,Key_reg_addr,18, 0
                0x0000'0009, // Key_device,Key_reg_addr,19, 0
                0, // Key_device,Key_reg_addr,20, 0
                0x0003'FFED, // Key_device,Key_reg_addr,21, 0
                0x0000'0008, // Key_device,Key_reg_addr,22, 0
                0x0003'FFFE, // Key_device,Key_reg_addr,23, 0
                0x0000'7D61, // Key_device,Key_reg_addr,24, 0
                0, // Key_device,Key_reg_addr,25, 0
                0, // Key_device,Key_reg_addr,26, 0
                0, // Key_device,Key_reg_addr,27, 0
                0, // Key_device,Key_reg_addr,28, 0
                0, // Key_device,Key_reg_addr,29, 0
                0, // Key_device,Key_serial_number, 0, 0
                Key_simple_R6, // Key_format,Key_DEMO, 0, 0
                Key_simple_R6, // Key_format,Key_IDLE, 0, 0
                Key_simple_R6, // Key_format,Key_NOMINAL, 0, 0
                Key_demo, // Key_MODE,Key_active, 0, 0
                0
};

        static constexpr uint8_t the_forms[] = {
        0,
        form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_keyword,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_array,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
                form_is(Form_keyword,Form_keyword,Form_null,Form_null),
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

const int32_t* get_table_of_defaults() { return the_defaults; }

        }