//
// Created by anolsen on 20.09.2019.
//

#ifndef SPI_TEST_IDE3466_REGISTER_BANK_H
#define SPI_TEST_IDE3466_REGISTER_BANK_H

#include "IDE3466_register.h"

class IDE3466_register_bank {
    enum Transaction_state {
        state_idle,
        state_error,
        read_all_start,
        read_all_calibration_register,
        read_all_channel_low_gain,
        read_all_channel_high_gain,
        read_all_trigger_low_gain,
        read_all_trigger_high_gain,
        read_all_pattern_configuration,
        read_all_pattern_hit_counter,
        read_all_counter_enable,
        read_all_gain_and_timing,
        read_all_global_configuration,
        read_all_parity_registers,
        read_all_latch_up_detector,
        write_all_start,
        write_all_calibration_register,
        write_all_channel_low_gain,
        write_all_channel_high_gain,
        write_all_pattern_configuration,
        write_all_counter_enable,
        write_all_gain_and_timing,
        write_all_global_configuration,
        write_all_latch_up_detector
    };

public:
    explicit IDE3466_register_bank(SPI_proxy &proxy);

    void initialize(bool set_write_command);

    void for_each_tx(void *p_user, for_each_register callback) const;

    void for_each_rx(void *p_user, for_each_register callback) const;

    void read_all();

    void write_all();

    void on_transaction_finished();

    void on_transaction_failed();

    bool is_idle() const { return state_idle == m_sequencer; }

    IDE3466_register<0x189u, false, 1> calibration_register{"calibration_register", "Calibration (CALGEN)",
                                                            "Configuration of the calibration charge generation"};
    IDE3466_register<0x130u, false, 4> channel_low_gain{"channel_low_gain", "Low-gain channels (CNFG_LG)",
                                                        "Low-gain channel config and trigger threshold"};
    IDE3466_register<0x134u, false, 64> channel_high_gain{"channel_high_gain", "High-gain channels (CNFG_HG)",
                                                          "High-gain channel config and trigger threshold"};
    IDE3466_register<0x174u, true, 1> trigger_low_gain{"trigger_low_gain", "Trigger low gain (TFF-LG)", ""};
    IDE3466_register<0x175u, true, 3> trigger_high_gain{"trigger_high_gain", "Trigger high gain (TFF-HG)", ""};
    IDE3466_register<0x000u, false, 252> pattern_configuration{"pattern_configuration",
                                                               "Pattern configuration (CNFG-CL)", ""};
    IDE3466_register<0x100u, true, 36> pattern_hit_counter{"pattern_hit_counter", "Pattern hit counter (CNT)", ""};
    IDE3466_register<0x182u, false, 3> counter_enable{"counter_enable", "Counter enable (CL-ENABLE)", ""};
    IDE3466_register<0x186u, false, 1> gain_and_timing{"gain_and_timing", "Gain and timing (GAIN_AND_MCT)", ""};
    IDE3466_register<0x185u, false, 1> global_configuration{"global_configuration",
                                                            "Global configuration (GLOBAL-CONF)",
                                                            "Misc. global settings"};
    IDE3466_register<0x187u, true, 15> parity_registers{"parity_registers", "Parity registers (PARITY_REG)", ""};
    IDE3466_register<0x181u, false, 1> latch_up_detector{"latch_up_detector", "Latch up detector (SELTHR)", ""};

private:
    SPI_proxy &m_proxy;
    volatile int m_sequencer;
    SPI_proxy::SPI_transaction m_transaction{};

};

#endif //SPI_TEST_IDE3466_REGISTER_BANK_H
