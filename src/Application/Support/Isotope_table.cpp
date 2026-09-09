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
* @file   Isotope_character.cpp
* @author AndersEmilOlsen, IDEAS
* @date   05.01.2026
* @brief  
*/


#include "Isotope_table.h"

#include <cmath>

// Define the Isotope structure.
// We still keep the name in the database (for lookup/reference),
// but later only the index is stored in the PeakResult.
typedef struct {
    char name[16];
    float energies[2]; // Up to two energies per isotope (in keV).
    int count;
} Isotope;

static Isotope isotope_db[] = {
    {"Na-22", {511.0f, 1274.5f}, 2},
    {"Co-60", {1173.2f, 1332.5f}, 2},
    {"Cs-137", {661.7f, 0.0f}, 1},
    {"Am-241", {59.5f, 0.0f}, 1},
    {"Eu-152", {344.3f, 1408.0f}, 2},
    {"Pb-210", {46.5f, 0.0f}, 1},
    {"Bi-214", {609.3f, 1120.3f}, 2},
    {"Tl-208", {2614.5f, 0.0f}, 1},
    {"K-40", {1460.8f, 0.0f}, 1},
    {"I-131", {364.5f, 0.0f}, 1},
    {"Tc-99m", {140.5f, 0.0f}, 1},
    {"Zr-95", {724.0f, 0.0f}, 1},
    {"Nb-95", {765.8f, 0.0f}, 1}
};

static int isotope_db_count = sizeof(isotope_db) / sizeof(Isotope);


// Checks the isotope database for any isotopes whose known gamma energy is within tolerance (in keV)
// of the given peak energy. Instead of copying the name, returns the index of the matching isotope.
// Returns -1 if no isotope matches.
extern "C" int identify_isotope(float peak_energy, float tolerance) {
    for (int i = 0; i < isotope_db_count; i++) {
        for (int j = 0; j < isotope_db[i].count; j++) {
            if (fabsf(peak_energy - isotope_db[i].energies[j]) <= tolerance)
                return i;
        }
    }
    return -1;
}

const char *get_isotope_name(uint8_t iso_index) {
    if (iso_index < isotope_db_count) {
        return isotope_db[iso_index].name;
    }
    return "Not Found";
}
