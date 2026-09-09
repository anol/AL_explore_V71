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
* @file   Bias_DAC.cpp
* @author AndersEmilOlsen, IDEAS
* @date   19.03.2026
* @brief  
*/


#include "Bias_DAC.h"
#include "dac.h"
#include "Print_support.h"

namespace Calibration {
    uint32_t cnt_out_of_range{};
    uint32_t cnt_busy_in_cal{};
    uint8_t in_calibration = 0;

    volatile uint32_t Bias_DAC::the_DAC_setting{};

    void Bias_DAC::initialize() {
        enum { Initial_DAC_setting = 0, };
        HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Initial_DAC_setting);
        is_initialized_flag = true;
    }

    void Bias_DAC::print_diag() {
        PRINTF("DAC: out-of-range=%d, busy=%d\r\n", cnt_out_of_range, cnt_busy_in_cal);
    }

    int32_t Bias_DAC::calibrate(int32_t raw, int32_t cal35, int32_t cal45) {
        double m = ((double) (-40700.0 + 35000.0)) / ((double) (cal45 - cal35)); // slope: 10000/9970 ≈ 1.00301
        return (int32_t) (-40700.0 + m * (raw - cal45));
    }

    void Bias_DAC::set_dac_test(int32_t value) {
        //-30000 = 0V and -50000 = 3.3V
        //convert -30000 to 0 and -50000 to 4095
        //convert value to dac value
        if (value > -35000 || value < -45000) {
            in_calibration = 0;
            return;
        }
        in_calibration = 1;
        int temp = value * -1;
        temp -= 30000;
        temp = (temp * 4095) / 20000;
        if (temp > 4095) {
            temp = 4095;
        }
        if (temp < 0) {
            temp = 0;
        }

        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);
    }

    void Bias_DAC::set_dac_cal_test(int32_t value) {
        //-30000 = 0V and -50000 = 3.3V
        //convert -30000 to 0 and -50000 to 4095
        //convert value to dac value
        if (value > -35000 || value < -45000) {
            in_calibration = 0;
            return;
        }
        in_calibration = 1;
        int temp = calibrate(value, the_cal_35V, the_cal_45V) * -1;
        temp -= 30000;
        temp = (temp * 4095) / 20000;
        if (temp > 4095) {
            temp = 4095;
        }
        if (temp < 0) {
            temp = 0;
        }
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);
    }


    bool Bias_DAC::set_dac_value_with_calibration(int32_t value) {
        the_DAC_pre_cal = value;
        //-30000 = 0V and -50000 = 3.3V
        //convert -30000 to 0 and -50000 to 4095
        //convert value to dac value
        if (value > -30000 || value < -48000) {
            cnt_out_of_range++;
            return false;
        }
        if (in_calibration) {
            cnt_busy_in_cal++;
            return false;
        }
        float temp = calibrate(value, the_cal_35V, the_cal_45V);
        temp *= -1.0f;
        temp -= 30000.0f;
        temp = (temp * 4095.0f) / 20000.0f;
        if (temp > 4095.0f) {
            temp = 4095.0f;
        }
        if (temp < 0.0f) {
            temp = 0.0f;
        }
        the_DAC_setting = static_cast<uint32_t>(temp);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, the_DAC_setting);
        return true;
    }
} // Calibration
