/*
 * Copyright (C) 2022 Integrated Detector Electronics AS
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

/**
 * \date   IDEAS/28.02.2022/anolsen
 * \brief
 */

#ifndef TARGET_CONFIGURATION_H
#define TARGET_CONFIGURATION_H

enum Target_terminal_address {
    GRMU_Terminal_address_BB = 26,

    NORM_Terminal_address_FM = 28,
    NORM_Terminal_address_EQM = 27,
    NORM_Terminal_address_EM = 26,
};

enum Target_oscillator_frequency {
    GRMU_Oscillator_BB = 16'000'000,

    NORM_Oscillator_FM = 16'000'000,
    NORM_Oscillator_EQM = 16'000'000,
    NORM_Oscillator_EM = 20'000'000,
};

/// @todo review the Target_queue_sizes
enum Target_queue_size
{
    The_telecommand_queue_size8     = 10 * 1'024, // 10 full messages
    The_high_priority_queue_size8   = 10 * 1'024, // 10 full messages
    The_persistent_queue_size8      = 1000 * 1'024, // 1000 full messages
    The_low_priority_queue_size8    = 100 * 1'024, // 100 full messages
};

enum Target_bit_rates {
    UART_bitrate = 115'200,
    SPI_bitrate = 4'000'000,
    I2C_bitrate = 400'000,
};

#endif //TARGET_CONFIGURATION_H
