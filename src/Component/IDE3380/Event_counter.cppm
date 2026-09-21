/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   Event_counter.h
* @author AndersEmilOlsen, IDEAS
* @date   20.04.2026
* @brief  
*/

module;
#include <cstdint>

export module Component.Event_counter;


export namespace Application {
    class Event_counter {
        // STM32U575RG::STM32U5_timer the_counter{STM32U575RG::STM32U5_timer::Event_counter};

    public:
        void initialize() { /*the_counter.initialize();*/ }
        void print_diag() const { /*the_counter.print_diag();*/ }
        uint32_t reset_event_count() { return /*the_counter.reset_count();*/ false; }
    };
} // Application
