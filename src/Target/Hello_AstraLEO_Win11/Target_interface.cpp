/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 */

/**
 * \date   IDEAS/06.05.2021/aeols
 * \brief
 */

#include <cstdlib>

#include "Interface/Device_interface.h"

void Device_interface::hardware_reset() {
//    Windows_memory_manager::save_memory();
    exit(0);
}

void Device_interface::check_watchdog() {
}

void Device_interface::run_application(uint32_t) {
}

uint32_t Device_interface::get_reset_status() {
    return 0;
}

void Device_interface::assess_reset_reason(bool)
{}

bool Device_interface::is_good_reset() {
    return true;
}

const char *Device_interface::get_reset_type() {
    return "Mockup";
}

void Device_interface::clear_cache() {
}

bool Device_interface::is_working_mem(uint32_t) {
    return true;
}
