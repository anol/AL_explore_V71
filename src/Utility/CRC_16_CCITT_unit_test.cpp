//
// Created by aeols on 24.09.2020.
//


#include "gtest/gtest.h"
import Utility.CRC_16_CCITT;

static void verify_CRC_octets(uint8_t *data, uint32_t length, uint8_t expected_CRC_1, uint8_t expected_CRC_2) {
    uint16_t actual_CRC = CRC_16_CCITT::crc_encode_octets(data, length - 2);
    EXPECT_EQ(expected_CRC_1, 0xFFu & (actual_CRC >> 8u));
    EXPECT_EQ(expected_CRC_2, 0xFFu & actual_CRC);
    *(data + length - 1) = 0xFFu & actual_CRC;
    *(data + length - 2) = 0xFFu & (actual_CRC >> 8u);
    EXPECT_TRUE(CRC_16_CCITT::crc_decode_octets(data, length).success());
    /*
    * Hint: You can use https://crccalc.com/ to verify as well.
    * Set "Input type" to HEX, and use "Calc CRC-16" then examine "CRC-16/CCITT-FALSE" which is NORM default.
    */
    for (int i = 0; i < length; ++i) {
        printf("%02X ", *data++);
    }
    printf("\r\n");
}

static void verify_CRC_words(uint16_t *data, uint32_t length, uint16_t expected_CRC) {
    uint16_t actual_CRC = CRC_16_CCITT::crc_encode_words(data, length - 1);
    EXPECT_EQ(expected_CRC, actual_CRC);
    *(data + length - 1) = actual_CRC;
    EXPECT_TRUE(CRC_16_CCITT::crc_decode_words(data, length).success());
    /*
    * Hint: You can use https://crccalc.com/ to verify as well
    * Set "Input type" to HEX, and use "Calc CRC-16" then examine "CRC-16/CCITT-FALSE" which is NORM default.
    */
    for (int i = 0; i < length; ++i) {
        printf("%04X ", *data++);
    }
    printf("\r\n");
}

TEST(CRC_16_CCITT_unit_test, test_octets) {
    /*
     * See ECSS-E-ST-70-41C (15 April 2016)
     * B.1 The cyclic redundancy code (CRC), B.1.5 Verification of compliance
     * Note: Two extra octets are declared for each data sequence to reserve room for the two checksum octets.
     */
    uint8_t octets_1[] = {0x00, 0x00, 0x00, 0x00};
    uint8_t octets_2[] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t octets_3[] = {0xab, 0xcd, 0xef, 0x01, 0x00, 0x00};
    uint8_t octets_4[] = {0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01, 0x00, 0x00};
    /* Initialize the look-up table */
    CRC_16_CCITT::initialize();
    /* Verify the correctness of the CRC error detection implementation */
    verify_CRC_octets(octets_1, sizeof(octets_1), 0x1Du, 0x0Fu);
    verify_CRC_octets(octets_2, sizeof(octets_2), 0xCCu, 0x9Cu);
    verify_CRC_octets(octets_3, sizeof(octets_3), 0x04u, 0xA2u);
    verify_CRC_octets(octets_4, sizeof(octets_4), 0x7Fu, 0xD5u);
}

TEST(CRC_16_CCITT_unit_test, test_wrong_crc_octets) {
    /*
     * See ECSS-E-ST-70-41C (15 April 2016)
     * B.1 The cyclic redundancy code (CRC), B.1.5 Verification of compliance
     * Note: Two extra octets are declared for each data sequence to reserve room for the two checksum octets.
     */
    uint8_t octets[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x00};
    /* Initialize the look-up table */
    CRC_16_CCITT::initialize();
    uint16_t crc = CRC_16_CCITT::crc_encode_octets(octets, 4);
    octets[4] = (crc >> 8) & 0xFF;
    octets[5] = crc & 0xFF;
    /* Verify the correctness of the CRC error detection implementation */
    EXPECT_TRUE(CRC_16_CCITT::crc_decode_octets(octets, 6).success());

    octets[1] = 0xEE;

    EXPECT_FALSE(CRC_16_CCITT::crc_decode_octets(octets, 6).success());
}

TEST(CRC_16_CCITT_unit_test, test_words) {
    /*
     * See ECSS-E-ST-70-41C (15 April 2016)
     * B.1 The cyclic redundancy code (CRC), B.1.5 Verification of compliance
     * Note: One extra word are declared for each data sequence to reserve room for the checksum.
     */
    uint16_t words_1[] = {0x0000, 0x0000};
//    uint16_t words_2[] = {0x0000, 0x0000, 0x00};
    uint16_t words_3[] = {0xabcd, 0xef01, 0x0000};
    uint16_t words_4[] = {0x1456, 0xf89a, 0x0001, 0x0000};
    /* Initialize the look-up table */
    CRC_16_CCITT::initialize();
    /* Verify the correctness of the CRC error detection implementation */
    verify_CRC_words(words_1, sizeof(words_1) / 2, 0x1D0Fu);
//    verify_CRC_words(words_2, sizeof(words_2)/2, 0xCC9Cu);
    verify_CRC_words(words_3, sizeof(words_3) / 2, 0x04A2u);
    verify_CRC_words(words_4, sizeof(words_4) / 2, 0x7FD5u);
}

TEST(CRC_16_CCITT_unit_test, test_wrong_crc_words) {
    /*
     * See ECSS-E-ST-70-41C (15 April 2016)
     * B.1 The cyclic redundancy code (CRC), B.1.5 Verification of compliance
     * Note: Two extra octets are declared for each data sequence to reserve room for the two checksum octets.
     */
    uint16_t words[5] = {0xAABB, 0xCCDD, 0xEEFF, 0xAABB, 0x0000};
    /* Initialize the look-up table */
    CRC_16_CCITT::initialize();
    words[4] = CRC_16_CCITT::crc_encode_words(words, 4);
    /* Verify the correctness of the CRC error detection implementation */
    EXPECT_TRUE(CRC_16_CCITT::crc_decode_words(words, 5).success());

    words[1] = 0x1234;

    EXPECT_FALSE(CRC_16_CCITT::crc_decode_words(words, 5).success());
}

TEST(CRC_16_CCITT_unit_test, test_special_message) {
    uint16_t message1[] = {0x0000, 0x0000, 0x0E82, 0x0000};
    uint16_t message2[] = {0x0000, 0x0000, 0x0E82, 0x0000, 0x0000};
    uint16_t message3[] = {0x0000, 0x0000, 0x0E82, 0x0000, 0x0000, 0x0000};
    uint16_t message4[] = {0x0000, 0x0000, 0x0E82, 0x0000, 0x0000, 0x0000, 0x0000};
    CRC_16_CCITT::initialize();
    verify_CRC_words(message1, sizeof(message1) / 2, 0x9CD5);
    verify_CRC_words(message2, sizeof(message2) / 2, 0xC65E);
    verify_CRC_words(message3, sizeof(message3) / 2, 0x07C9);
    verify_CRC_words(message4, sizeof(message4) / 2, 0xD1F2);
}

TEST(CRC_16_CCITT_unit_test, test_converter) {
    char hex[] = "0E82";
    EXPECT_EQ(CRC_16_CCITT::hex_to_word(hex), 0x0E82);
}

TEST(CRC_16_CCITT_unit_test, test_encode_hex_words) {
    char message0[] = "000000000E820000";
    char message1[] = "000100000E820001";
    char message2[] = "000200000E820002";
    CRC_16_CCITT::initialize();
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words(message0, sizeof(message0)), 0xC65E);
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words(message1, sizeof(message1)), 0x6E1E);
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words(message2, sizeof(message2)), 0x86FF);
}

TEST(CRC_16_CCITT_unit_test, test_encode_hex_words_uo) {
    char message0[] = "000000000E820000";
    char message1[] = "000100000E820001";
    char message2[] = "000200000E820002";
    CRC_16_CCITT::initialize();
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words_uo(message0, sizeof(message0)), 0xC65E);
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words_uo(message1, sizeof(message1)), 0x6E1E);
    EXPECT_EQ(CRC_16_CCITT::crc_encode_hex_words_uo(message2, sizeof(message2)), 0x86FF);
}

