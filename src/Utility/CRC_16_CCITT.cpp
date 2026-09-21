//
// Created by aeols on 24.09.2020.
//

module;
#include <cstdint>

module Utility.CRC_16_CCITT;
import Type.Status_code;

/*
 * The following source code is based on the software implementation described in
 * the telemetry and telecommand packet utilization standard (ECSS-E-ST-70-41C 15 April 2016).
 *
 * IDEAS/2020-09-24/Aeo
 */

/* Look-up table, only required for optimized CRC version */
__attribute__((section (".application_data")))
uint16_t CRC_16_CCITT::lookup_table[256];

/* Unoptimized CRC version */
/* One step unoptimized CRC */
uint16_t CRC_16_CCITT::Crc(uint8_t Data, uint16_t Syndrome) {
    for (uint8_t icrc = 0; icrc < 8; icrc++) {
        if ((Data & 0x80u) ^ ((Syndrome & 0x8000u) >> 8u)) {
            Syndrome = ((Syndrome << 1u) ^ 0x1021u) & 0xFFFFu;
        } else {
            Syndrome = (Syndrome << 1u) & 0xFFFFu;
        }
        Data = Data << 1u;
    }
    return (Syndrome);
}

/* Optimized CRC version */
/* Look-up table initialization */
void CRC_16_CCITT::build_lookup_table(uint16_t table[]) {
    uint16_t itable; /* Loop index */
    uint16_t tmp; /* Temporary value */
    for (itable = 0; itable < 256; itable++) {
        tmp = 0;
        if ((itable & 1u) != 0) tmp = tmp ^ 0x1021u;
        if ((itable & 2u) != 0) tmp = tmp ^ 0x2042u;
        if ((itable & 4u) != 0) tmp = tmp ^ 0x4084u;
        if ((itable & 8u) != 0) tmp = tmp ^ 0x8108u;
        if ((itable & 16u) != 0) tmp = tmp ^ 0x1231u;
        if ((itable & 32u) != 0) tmp = tmp ^ 0x2462u;
        if ((itable & 64u) != 0) tmp = tmp ^ 0x48C4u;
        if ((itable & 128u) != 0) tmp = tmp ^ 0x9188u;
        table[itable] = tmp;
    }
}

uint16_t CRC_16_CCITT::Crc_opt(uint8_t D, uint16_t Chk, uint16_t table[]) {
    return (((Chk << 8u) & 0xFF00u) ^ table[(((Chk >> 8u) ^ D) & 0x00FFu)]);
}

uint16_t CRC_16_CCITT::crc_encode_words(const uint16_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc_opt(0xFFu & (*data >> 8u), Chk, lookup_table);
        Chk = Crc_opt(0xFFu & (*data), Chk, lookup_table);
    }
    return Chk;
}

uint16_t CRC_16_CCITT::crc_encode_hex_words(const char *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    auto word_count = length / 4;
    while (word_count--) {
        uint16_t word = hex_to_word(data);
        data += 4;
        Chk = Crc_opt(0xFFu & (word >> 8u), Chk, lookup_table);
        Chk = Crc_opt(0xFFu & (word), Chk, lookup_table);
    }
    return Chk;
}

uint16_t CRC_16_CCITT::crc_encode_octets(const uint8_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc_opt(*data, Chk, lookup_table);
    }
    return Chk;
}

Status_code CRC_16_CCITT::crc_decode_words(const uint16_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc_opt(0xFFu & (*data >> 8u), Chk, lookup_table);
        Chk = Crc_opt(0xFFu & (*data), Chk, lookup_table);
    }
    return Status_code(Chk == 0);
}

Status_code CRC_16_CCITT::crc_decode_octets(const uint8_t *data, uint32_t length) {
/* Decoding procedure */
/* The error detection syndrome, S(x) is given by: */
/* S(x)=(x^16 * C¤(x) + x^n * L(x)) modulo G(x) */
/* If S(x) = 0 then no error is detected. */
    uint16_t Chk = 0xFFFF; /* Reset syndrome to all ones */
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc_opt(*data, Chk, lookup_table);
    }
    return Status_code(Chk == 0);
}

Status_code CRC_16_CCITT::crc_decode_octets_uo(const uint8_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF; /* Reset syndrome to all ones */
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc(*data, Chk);
    }
    return Status_code(Chk == 0);
}

Status_code CRC_16_CCITT::crc_decode_words_uo(const uint16_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF; /* Reset syndrome to all ones */
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc(0xFFu & (*data >> 8u), Chk);
        Chk = Crc(0xFFu & (*data), Chk);
    }
    return Status_code(Chk == 0);
}

uint16_t CRC_16_CCITT::crc_encode_octets(uint16_t Syndrome, const uint8_t *data, uint32_t length) {
    for (uint32_t index = 0; index < length; index++, data++) {
        Syndrome = Crc(*data, Syndrome);
    }
    return Syndrome;
}

uint16_t CRC_16_CCITT::crc_encode_octets_uo(const uint8_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc(*data, Chk);
    }
    return Chk;
}

uint16_t CRC_16_CCITT::crc_encode_words_uo(const uint16_t *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    for (uint32_t index = 0; index < length; index++, data++) {
        Chk = Crc(0xFFu & (*data >> 8u), Chk);
        Chk = Crc(0xFFu & (*data), Chk);
    }
    return Chk;
}

uint16_t CRC_16_CCITT::crc_encode_hex_words_uo(const char *data, uint32_t length) {
    uint16_t Chk = 0xFFFF;
    auto word_count = length / 4;
    while (word_count--) {
        uint16_t word = hex_to_word(data);
        data += 4;
        Chk = Crc(0xFFu & (word >> 8u), Chk);
        Chk = Crc(0xFFu & (word), Chk);
    }
    return Chk;
}

uint16_t CRC_16_CCITT::hex_to_nibble(char sym) {
    return (sym > '9') ? (9 + (sym & 0x0F)) : (sym & 0x0F);
}

uint16_t CRC_16_CCITT::hex_to_word(const char *sym) {
    uint16_t result = hex_to_nibble(*sym++);
    result <<= 4;
    result |= hex_to_nibble(*sym++);
    result <<= 4;
    result |= hex_to_nibble(*sym++);
    result <<= 4;
    result |= hex_to_nibble(*sym);
    return result;
}
