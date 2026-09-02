//
// Created by anolsen on 20.09.2019.
//

#include <stdint.h>
#include "IDE3466_register_bank.h"

static void on_transaction_finished_cb(void *p_user) {
    ((IDE3466_register_bank *) p_user)->on_transaction_finished();
}

static void on_transaction_failed_cb(void *p_user) {
    ((IDE3466_register_bank *) p_user)->on_transaction_failed();
}

IDE3466_register_bank::IDE3466_register_bank(SPI_proxy &proxy) :
        m_proxy(proxy), m_sequencer(state_idle) {}

void IDE3466_register_bank::initialize(bool set_write_command) {
    m_transaction.p_user = this;
    m_transaction.finish = on_transaction_finished_cb;
    m_transaction.failed = on_transaction_failed_cb;
    calibration_register.initialize(set_write_command);
    channel_low_gain.initialize(set_write_command);
    channel_high_gain.initialize(set_write_command);
    trigger_low_gain.initialize(set_write_command);
    trigger_high_gain.initialize(set_write_command);
    pattern_configuration.initialize(set_write_command);
    pattern_hit_counter.initialize(set_write_command);
    counter_enable.initialize(set_write_command);
    gain_and_timing.initialize(set_write_command);
    global_configuration.initialize(set_write_command);
    parity_registers.initialize(set_write_command);
    latch_up_detector.initialize(set_write_command);
}

void IDE3466_register_bank::for_each_rx(void *p_user, for_each_register callback) const {
    calibration_register.for_each_rx(p_user, callback);
    channel_low_gain.for_each_rx(p_user, callback);
    channel_high_gain.for_each_rx(p_user, callback);
    trigger_low_gain.for_each_rx(p_user, callback);
    trigger_high_gain.for_each_rx(p_user, callback);
    pattern_configuration.for_each_rx(p_user, callback);
    pattern_hit_counter.for_each_rx(p_user, callback);
    counter_enable.for_each_rx(p_user, callback);
    gain_and_timing.for_each_rx(p_user, callback);
    global_configuration.for_each_rx(p_user, callback);
    parity_registers.for_each_rx(p_user, callback);
    latch_up_detector.for_each_rx(p_user, callback);
}

void IDE3466_register_bank::for_each_tx(void *p_user, for_each_register callback) const {
    calibration_register.for_each_tx(p_user, callback);
    channel_low_gain.for_each_tx(p_user, callback);
    channel_high_gain.for_each_tx(p_user, callback);
    trigger_low_gain.for_each_tx(p_user, callback);
    trigger_high_gain.for_each_tx(p_user, callback);
    pattern_configuration.for_each_tx(p_user, callback);
    pattern_hit_counter.for_each_tx(p_user, callback);
    counter_enable.for_each_tx(p_user, callback);
    gain_and_timing.for_each_tx(p_user, callback);
    global_configuration.for_each_tx(p_user, callback);
    parity_registers.for_each_tx(p_user, callback);
    latch_up_detector.for_each_tx(p_user, callback);
}

void IDE3466_register_bank::read_all() {
    m_sequencer = read_all_start;
    on_transaction_finished();
}

void IDE3466_register_bank::write_all() {
    m_sequencer = write_all_start;
    on_transaction_finished();
}

void IDE3466_register_bank::on_transaction_finished() {
    if ((state_idle != m_sequencer) && (state_error != m_sequencer)) {
        m_sequencer++;
        switch (m_sequencer) {
            case read_all_calibration_register:
                calibration_register.read(m_proxy, m_transaction);
                break;
            case read_all_channel_low_gain:
                channel_low_gain.read(m_proxy, m_transaction);
                break;
            case read_all_channel_high_gain:
                channel_high_gain.read(m_proxy, m_transaction);
                break;
            case read_all_trigger_low_gain:
                trigger_low_gain.read(m_proxy, m_transaction);
                break;
            case read_all_trigger_high_gain:
                trigger_high_gain.read(m_proxy, m_transaction);
                break;
            case read_all_pattern_configuration:
                pattern_configuration.read(m_proxy, m_transaction);
                break;
            case read_all_pattern_hit_counter:
                pattern_hit_counter.read(m_proxy, m_transaction);
                break;
            case read_all_counter_enable:
                counter_enable.read(m_proxy, m_transaction);
                break;
            case read_all_gain_and_timing:
                gain_and_timing.read(m_proxy, m_transaction);
                break;
            case read_all_global_configuration:
                global_configuration.read(m_proxy, m_transaction);
                break;
            case read_all_parity_registers:
                parity_registers.read(m_proxy, m_transaction);
                break;
            case read_all_latch_up_detector:
                latch_up_detector.read(m_proxy, m_transaction);
                break;
            case write_all_calibration_register:
                calibration_register.write(m_proxy, m_transaction);
                break;
            case write_all_channel_low_gain:
                channel_low_gain.write(m_proxy, m_transaction);
                break;
            case write_all_channel_high_gain:
                channel_high_gain.write(m_proxy, m_transaction);
                break;
            case write_all_pattern_configuration:
                pattern_configuration.write(m_proxy, m_transaction);
                break;
            case write_all_counter_enable:
                counter_enable.write(m_proxy, m_transaction);
                break;
            case write_all_gain_and_timing:
                gain_and_timing.write(m_proxy, m_transaction);
                break;
            case write_all_global_configuration:
                global_configuration.write(m_proxy, m_transaction);
                break;
            case write_all_latch_up_detector:
                latch_up_detector.write(m_proxy, m_transaction);
                break;
            default:
                m_sequencer = state_idle;
                break;
        }
    }
}

void IDE3466_register_bank::on_transaction_failed() {
    m_sequencer = state_error;
}













