/*
 * commands.c separates out functions that are carried 
 * out by a command from the command parser. The functions 
 * in this file will interpret the arguments that are sent
 * with a command and execute according to it's functionality.
 *
 * Created: 15.08.2019 09:42:39
 *  Author: ssorensen
 */
#include <cstdint>
#include <Consol.h>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include "commands.h"


void Command::init() {
    Consol::global_printf("\r\nInitialise\r\n");
    hal_ide3466_init();
}

void Command::set_LG_CNFG_reg(uint8_t num, bool ch, uint32_t thresh, bool en) {

}

void Command::hal_ide3466_init() {

}

void Command::util_IDE3466_inject_charges(uint32_t pulses) {

}

void Command::util_IDE3466_set_AMUX(int preampnum) {

}

void Command::hal_ide3466_set_CALGEN_reg(uint32_t value, uint32_t mode, int i, int i1) {

}

void Command::set_HG_H_CNFG_reg(uint8_t num, bool ch, uint32_t thresh, bool en) {

}

void Command::set_HG_L_CNFG_reg(uint8_t num, bool ch, uint32_t thresh) {

}

void Command::dcal_n_pulse(char *c_cmd) {
    Consol::global_printf("\r\nDCAL N pulse - '%s'\r\n", c_cmd);
    uint32_t user_N_DCAL_pulses = 0;
    if (atoi((char *) &c_cmd[2])) {
        user_N_DCAL_pulses = (uint32_t) atoi((char *) &c_cmd[2]);
        Consol::global_printf("%u DCAL pulse(s).\n", user_N_DCAL_pulses);
    } else {
        user_N_DCAL_pulses = 1;
        Consol::global_printf("No digits detected or invalid input - default set to %u DCAL pulse(s).\n",
                              user_N_DCAL_pulses);
    }
    util_IDE3466_inject_charges(user_N_DCAL_pulses);
}

void Command::clock_amux(char *c_cmd) {
    Consol::global_printf("\r\nClock the AMUX - '%s'\r\n", c_cmd);
    uint32_t user_number_RO_CLKs;
    if (atoi((char *) &c_cmd[2])) {
        user_number_RO_CLKs = (uint32_t) atoi((char *) &c_cmd[2]);
        if (user_number_RO_CLKs > 72) {
            user_number_RO_CLKs = 72;
            Consol::global_printf("Error: Too large number entered for RO_CLK cycles set to %d (max. value)!\r\n",
                                  user_number_RO_CLKs);
        }
        Consol::global_printf("%lu RO_CLK cycles.\r\n", user_number_RO_CLKs);
    } else {
        user_number_RO_CLKs = 8;
        Consol::global_printf("No digits detected or invalid input - default set to HG-1: %lu RO_CLK cycles.\r\n",
                              user_number_RO_CLKs);
    }
    int preampnum = (int) ceil((float) (user_number_RO_CLKs / 2));
    if (user_number_RO_CLKs >= 8) {
        Consol::global_printf("Set AMUX to HG-%d.\r\n", (int) ceil((float) (user_number_RO_CLKs - 7) / 2));
    } else {
        Consol::global_printf("Set AMUX to LG-%d.\r\n", preampnum);
    }
    util_IDE3466_set_AMUX(preampnum);
    Consol::global_printf("Preamp num: %d", preampnum);
}

void Command::set_cal_unit(char *c_cmd) {
    Consol::global_printf("\r\nConfigures the CALUNIT - '%s'\r\n", c_cmd);
    uint32_t user_CCAL_mode;
    uint32_t user_CALV_value;
    Consol::global_printf("CAL UNIT initialization with internal DAC (CALBYPASS = 0), internal cap (CALGEN = 0).\r\n");
    Consol::global_printf("Input: $aXm, X = {A,B,C,D,E}, m={0..1023}.\r\n"
                          "X selects CCAL mode, m selects CALV LSB value.\r\n");
    if ((isalpha(c_cmd[2]))) {
        if (c_cmd[2] == 'A') {
            user_CCAL_mode = 0x00;
            Consol::global_printf("CCAL mode %u detected: A - Calibration Unit off.\r\n", user_CCAL_mode);
        } else if (c_cmd[2] == 'B') {
            user_CCAL_mode = 0x04;
            Consol::global_printf("CCAL mode %u detected: B - internal CAP 0.1pF.\r\n", user_CCAL_mode);
        } else if (c_cmd[2] == 'C') {
            user_CCAL_mode = 0x05;
            Consol::global_printf("CCAL mode %u detected: C - internal CAP 1.0pF.\r\n", user_CCAL_mode);
        } else if (c_cmd[2] == 'D') {
            user_CCAL_mode = 0x06;
            Consol::global_printf("CCAL mode %u detected: D - internal CAP 2.6pF.\r\n", user_CCAL_mode);
        } else if (c_cmd[2] == 'E') {
            user_CCAL_mode = 0x07;
            Consol::global_printf("CCAL mode %u detected: E - internal CAP 26pF.\r\n", user_CCAL_mode);
        } else {
            user_CCAL_mode = 0x00;
            Consol::global_printf(
                    "Error: invalid input, CCAL mode NOT detected\nRevert to default CCAL mode %u: A - Calibration Unit off.\r\n",
                    user_CCAL_mode);
        }
    } else {
        user_CCAL_mode = 0x00;
        Consol::global_printf(
                "Error: invalid input, CCAL mode NOT detected\nRevert to default CCAL mode %u: A - Calibration Unit off.\r\n",
                user_CCAL_mode);
    }
    if (atoi((char *) &c_cmd[3])) {
        user_CALV_value = (uint32_t) atoi((char *) &c_cmd[3]);
        if (user_CALV_value > 1023) {
            user_CALV_value = 0;
            Consol::global_printf(
                    "Error: Too large number entered CAL UNIT CALV DAC value set to %lu (min. value)!\r\n",
                    user_CALV_value);
        }
        Consol::global_printf("CAL UNIT CALV DAC value = %lu LSB.\r\n", user_CALV_value);
    } else {
        user_CALV_value = 0;
        Consol::global_printf(
                "0 digit number or invalid input - revert to default: CAL UNIT CALV DAC value = %u LSB\r\n",
                user_CALV_value);
    }
    Consol::global_printf("-> CALGEN register\r\n");
    hal_ide3466_set_CALGEN_reg(user_CALV_value, user_CCAL_mode, 0, 0);
}

void Command::set_CNFG_reg(char *c_cmd) {
    Consol::global_printf("\r\nConfigures the CNFG_TRG - '%s'\r\n", c_cmd);
    char X = c_cmd[2];
    char CH[3];
    char NUM[5];
    char D[2];
    char T[2];
    uint8_t ch_num = 1;
    uint32_t thresh = 1023;
    bool dis_ch = false;
    bool test_en = false;
    strncpy(CH, &c_cmd[3], 2);
    strncpy(NUM, &c_cmd[5], 4);
    strncpy(D, &c_cmd[9], 1);
    strncpy(T, &c_cmd[10], 1);
    if (atoi(CH))
        ch_num = (uint8_t) atoi(CH);
    if (atoi(D))
        dis_ch = true;
    if (atoi(T))
        test_en = true;
    if (atoi(NUM)) {
        thresh = (uint32_t) atoi(NUM);
        if (thresh > 1023) {
            thresh = 1023;
            Consol::global_printf("Error: Too large number entered %cG-%u %cG%cT_thres set to %d (max. value)!\n",
                                  X == 'L' ? 'L' : 'H', ch_num, X == 'L' ? 'L' : 'H', X == 'h' ? 'L' : 'H', thresh);
        }
        Consol::global_printf("%cG-%u %cG%cT_thres = %u.\n", X == 'L' ? 'L' : 'H', ch_num, X == 'L' ? 'L' : 'H',
                              X == 'h' ? 'L' : 'H', thresh);
    } else {
        thresh = 1023;
        Consol::global_printf(
                "Error: 0 digit number or invalid threshold input - revert to default: %cG-%u %cG%cT_thres = %u.\n",
                X == 'L' ? 'L' : 'H', X == 'L' ? 'L' : 'H', ch_num, X == 'h' ? 'L' : 'H', thresh);
    }
    switch (X) {
        case 72:  /* H for High Gain high threshold */
            set_HG_H_CNFG_reg(ch_num, dis_ch, thresh, test_en);
            break;
        case 104: /* h for High Gain low threshold */
            set_HG_L_CNFG_reg(ch_num, dis_ch, thresh);
            break;
        case 76:  /* L for Low Gain high threshold */
            set_LG_CNFG_reg(ch_num, dis_ch, thresh, test_en);
            break;
    }
}

void Command::print_help_page() {
    Consol::global_printf("*****  HELP PAGES  *****\r\n");
    Consol::global_printf("**  Valid commands (case sensitive)  **\r\n");
    Consol::global_printf("$h -> help: show all available commands\r\n");
    Consol::global_printf("$i -> init\r\n");
    Consol::global_printf("$xNNN -> DCAL N pulse pulse\r\n");
    Consol::global_printf("$rNN -> clock AMUX to N [digits 23] clocks // TO-DO fix to total of 68 CLKs...\r\n");
    Consol::global_printf("$uN -> global - N = {power mode 0 (default),1 (low-power), 2 (high-power)}.\r\n");
    Consol::global_printf("$a[X][NNNN], configures the CALUNIT, where:\r\n");
    Consol::global_printf("  [X] ..... Can be 'A','B','C','D' or 'E'\r\n");
    Consol::global_printf("            which will set the CCAL value corresponding to:\r\n");
    Consol::global_printf("            [A - Calibration Unit off]\r\n");
    Consol::global_printf("            [B - internal CAP 0.1pF]\r\n");
    Consol::global_printf("            [C - internal CAP 1.0pF]\r\n");
    Consol::global_printf("            [D - internal CAP 2.6pF]\r\n");
    Consol::global_printf("            [E - internal CAP 26pF]\r\n");
    Consol::global_printf("     [NNNN] represents a DAC value that will be set for CALV\r\n");
    Consol::global_printf("            This number can be in range [0000-1023]\r\n\r\n");
    Consol::global_printf("** Register settings **\r\n");
    Consol::global_printf("$C[X][CH][NNNN][D][T], configures a CNFG_TRG, where:\r\n");
    Consol::global_printf("  [X] ............... can be 'L', 'H' or 'h' representing \r\n");
    Consol::global_printf(
            "                      \"[L]ow gain\"-config register, \"[H]igh gain High\"-config register and\r\n");
    Consol::global_printf("                      \"[h]igh gain Low\"-config register \r\n");
    Consol::global_printf(
            "     [CH] ........... represents a two digit channel number (01-32) for H and h, or (01-04) for L\r\n");
    Consol::global_printf("         [NNNN] ..... represents the threshold value in range (0000-1023)\r\n");
    Consol::global_printf("                      (if its a low or high threshold depends on [X])\r\n");
    Consol::global_printf("               [D]... represents the \"Channel Disable\" bitfield\r\n");
    Consol::global_printf("                      (can be 1 or 0, defaults to 0 if not provided)\r\n");
    Consol::global_printf("                  [T] represents the 'Test Enable' bitfield\r\n");
    Consol::global_printf("                      (can be 1 or 0, defaults to 0 if not provided).\r\n");
    Consol::global_printf("                      Can be ignored if [X] = 'h', because the HGLT register does\r\n");
    Consol::global_printf("                      not have this bit.\r\n");
    Consol::global_printf("$P[T][SU][NUM] Configures the pattern register for a sub unit where\r\n");
    Consol::global_printf("  [T] ........ can be 'C', 'A', 'N' or 'W' representing\r\n\r\n");
    Consol::global_printf("               \"[C]oinciding trigger\", \"[A]nti-coinciding trigger\",\r\n");
    Consol::global_printf("               \"[N]on-coinciding trigger\" (clearing previously set trigger)\r\n");
    Consol::global_printf("               and \"[W]ipe pattern\" (used to disable all triggers in reg)\r\n");
    Consol::global_printf("     [SU] .... represents a two digit sub unit number in range (01-36)\r\n");
    Consol::global_printf("         [NUM] represents the trigger number in range (01-68) that\r\n");
    Consol::global_printf("               sub_unit [SU] should coincide/anticoincide/noncoincide(\"don't care\")\r\n");
    Consol::global_printf("               with according to type [T].\r\n");
    Consol::global_printf("               This value is ignored for [T] = 'W'\r\n\r\n");
    Consol::global_printf("$E[X][SU] Enables counter logic according to:\r\n");
    Consol::global_printf("  [X] ... [X = 'A'] All counter logic \r\n");
    Consol::global_printf("          [X = 'D'] Disable all counter logic\r\n");
    Consol::global_printf("          [X = 'G'] Global enable toggle\r\n");
    Consol::global_printf("          [X = 'N'] Enable a given counter, and set CL_ENABLE_G high\r\n");
    Consol::global_printf("          [X = 'C'] Disable a given counter\r\n");
    Consol::global_printf("     [SU] Sub unit in range [01-36] that will\r\n");
    Consol::global_printf("          be enabled/disabled according to: \r\n");
    Consol::global_printf("          [X] = 'N' or [X] = 'C'\r\n\r\n");
    Consol::global_printf(
            "$T -> Trigger disable. Disables Channel and Test Enable in all CNFG registers (sets thresh to 1023)\r\n");
    Consol::global_printf("** Special case CNFG reg settings **\r\n");
    Consol::global_printf("$kNNNN -> set ch. 1 HGLT thres, enable ONLY HGLT trigger, enable TER input.\r\n");
    Consol::global_printf("$mNNNN -> set ch. 1 HGHT thres, enable ONLY HGHT trigger, enable TER input.\r\n");
    Consol::global_printf("$DNNNN -> set ch. 3 HGLT thres, enable ONLY HGLT trigger, DISABLE TER input.\r\n");
    Consol::global_printf("$JNNNN -> set ch. 3 HGHT thres, enable ONLY HGHT trigger, DISABLE TER input.\r\n");
    Consol::global_printf(
            "$FNNNN -> set ch. 3 HGHT thres, HGLT thres = 1023, enable HGLT AND HGHT trigger, DISABLE TER input.\r\n");
    Consol::global_printf("$GNNNN -> set ch. 22 HGLT thres, enable ONLY HGLT trigger, DISABLE TER input.\r\n");
    Consol::global_printf("$HNNNN -> set ch. 22 HGHT thres, enable ONLY HGHT trigger, DISABLE TER input.\r\n");
    Consol::global_printf(
            "$INNNN -> set ch. 22 HGHT thres, HGLT thres = 1023, enable HGLT AND HGHT trigger, DISABLE TER input.\r\n");
    Consol::global_printf("$d[N]  -> setup CNT-1 to trigger on \r\n");
    Consol::global_printf("  [N] = [0 = No triggers counted]\r\n");
    Consol::global_printf("        [1 = HGLT-1]\r\n");
    Consol::global_printf("        [2 = HGHT-1]\r\n");
    Consol::global_printf("        [3 = (HGLT-1, HGHT-1 coinc)]\r\n");
    Consol::global_printf("        [4 = (HGLT-1 coinc, HGHT-1 anticoinc)]\r\n");
    Consol::global_printf("        [6 = HGLT-3]\r\n");
    Consol::global_printf("        [7 = HGHT-3]\r\n");
    Consol::global_printf("        [8 = (HGLT-3,HGHT-3 coinc)]\r\n");
    Consol::global_printf("        [9=(HGLT-3 coinc, HGHT-3 anticoinc)]\r\n\r\n");
    Consol::global_printf("$q -> IDE3466 SPI RESET (SRESET)\r\n");
    Consol::global_printf("$fNNNN -> set DAC_GLOBAL_GAIN value, MCT default value = 1023 LSB.\r\n");
    Consol::global_printf("$gNNNN -> set MCT value, default DAC_GLOBAL_GAIN = 512 (0x0200) LSB value.\r\n");
    Consol::global_printf("$w -> read and dump all counter values.\r\n");
    Consol::global_printf("$eNNNN -> single pulse with delay_us(NNNN) function. N = 0 is valid input.\r\n");
    Consol::global_printf("$jNNNN -> single pulse with NNNN NOP instructions, N=0 is valid input\r\n\r\n");
    Consol::global_printf("$S[X][Y][Z] -> SS_HOLD. Changes behavior depending on the value of 'X'\r\n");
    Consol::global_printf(
            "  [XYZ] ... if [X]!= 0 and [X] != 'D' -> Initialize SS_HOLD with 'default'  delay cycles \r\n");
    Consol::global_printf(
            "                                         (or 'unspecified' amount of delay cycles if previously modified by user)\r\n");
    Consol::global_printf("  [X] ..... if [X] = 0                -> SS_HOLD goes low and deactivates\r\n");
    Consol::global_printf(
            "            if [X] in range [1-9]     -> SS_HOLD activates with up to three digits [XYZ]\r\n");
    Consol::global_printf("                                         representing the number of delay cycles \r\n");
    Consol::global_printf(
            "                                         (the digit must be higher than 15 to be valid)\r\n");
    Consol::global_printf(
            "            if [X] = 'D'(ASCII 0x44)  -> Changes behavior depending on the value of [Y]\r\n");
    Consol::global_printf("     [Y] .. Only relevant if [X] = 'D' (This mode will be removed at some point)\r\n");
    Consol::global_printf("            if [Y] = 0                -> Deactivate SS_HOLD debug mode \r\n");
    Consol::global_printf(
            "                                         (activates nominal mode with default delay cycles)\r\n");
    Consol::global_printf("            if [Y] = 2:               -> Force the SS_HOLD line to go low.\r\n");
    Consol::global_printf(
            "            else:                     -> Activate SS_HOLD debug mode (SS_HOLD signal will not go low after triggering). \r\n");
    Consol::global_printf("$pN -> I2C device test.\r\n\r\n\r\n");
    Consol::global_printf("** Special characters **\r\n");
    Consol::global_printf("//0x24 = $\r\n");
    Consol::global_printf("//0x73 = s\r\n");
    Consol::global_printf("//0x0D = CR\r\n");
    Consol::global_printf("//0x0A = LF\r\n");
    Consol::global_printf("// 0d 0a = \n\r\n");
    Consol::global_printf("*****              *****\r\n\r\n");
}

void Command::CNT_trigger(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::dcal_n_pulse_no_delay(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::enable_CL(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::i2c_test(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::pulse_width(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::pulse_width_alternate(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::set_CNFG_CL_reg(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::set_dac_global_gain(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::set_mct_value(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::set_pwr_mode(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::special_case_CNFG_settings(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::spi_test(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::ss_hold(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::test(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}

void Command::trigger_disable(char *c_cmd) {
    Consol::global_printf("<< Not implemented - '%s' >>", c_cmd);
}
