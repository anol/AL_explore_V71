//
// Created by aeols on 24.09.2020.
//

module;
#include <cstdint>

export module Utility.CRC_16_CCITT;
export import Type.Status_code;



/// Purpose: Encoding and decoding of the CCITT 16-bit CRC-function.
export class CRC_16_CCITT {
    static uint16_t lookup_table[256];

    static uint16_t Crc_opt(uint8_t D, uint16_t Chk, uint16_t *table);

    static uint16_t Crc(uint8_t Data, uint16_t Syndrome);

    static void build_lookup_table(uint16_t *table);

    static uint16_t hex_to_nibble(char sym);

public:
    /** optimized CRC functions */

    static void initialize() { build_lookup_table(lookup_table); };

    [[nodiscard]] static Status_code crc_decode_octets(const uint8_t *data, uint32_t length);

    [[nodiscard]] static Status_code crc_decode_words(const uint16_t *data, uint32_t length);

    static uint16_t crc_encode_octets(const uint8_t *data, uint32_t length);

    static uint16_t crc_encode_words(const uint16_t *data, uint32_t length);

    static uint16_t crc_encode_hex_words(const char *data, uint32_t length);

    /** unoptimized CRC functions with _uo suffix */

    [[nodiscard]] static Status_code crc_decode_octets_uo(const uint8_t *data, uint32_t length);

    [[nodiscard]] static Status_code crc_decode_words_uo(const uint16_t *data, uint32_t length);

    static uint16_t crc_encode_octets(uint16_t Syndrome, const uint8_t *data, uint32_t length);

    static uint16_t crc_encode_octets_uo(const uint8_t *data, uint32_t length);

    static uint16_t crc_encode_words_uo(const uint16_t *data, uint32_t length);

    static uint16_t crc_encode_hex_words_uo(const char *data, uint32_t length);

    static uint16_t hex_to_word(const char *sym);
};
