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
* @file   Isotope_character.h
* @author AndersEmilOlsen, IDEAS
* @date   05.01.2026
* @brief  
*/


#ifndef IDEAS_PCB8063_FIRMWARE_ISOTOPE_CHARACTER_H
#define IDEAS_PCB8063_FIRMWARE_ISOTOPE_CHARACTER_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int identify_isotope(float peak_energy, float tolerance);

const char *get_isotope_name(uint8_t iso_index);

#ifdef __cplusplus
}
#endif

#endif //IDEAS_PCB8063_FIRMWARE_ISOTOPE_CHARACTER_H
