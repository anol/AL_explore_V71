/* commands.h
 *
 * Created: 15.08.2019 09:44:51
 *  Author: ssorensen
 */

/* commands.c separates out the functions that are carried
 * out by a command from the command parser. The functions
 * in this file will interpret the arguments that are sent
 * with a command and execute according to it's functionality.
 *
 * Created: 15.08.2019 09:42:39
 *  Author: ssorensen
 */

class Command {

public:
    static void clock_amux(char *c_cmd);

    static void CNT_trigger(char *c_cmd);

    static void dcal_n_pulse(char *c_cmd);

    static void dcal_n_pulse_no_delay(char *c_cmd);

    static void enable_CL(char *c_cmd);

    static void i2c_test(char *c_cmd);

    static void init();

    static void pulse_width(char *c_cmd);

    static void pulse_width_alternate(char *c_cmd);

    static void set_cal_unit(char *c_cmd);

    static void set_CNFG_CL_reg(char *c_cmd);

    static void set_CNFG_reg(char *c_cmd);

    static void set_dac_global_gain(char *c_cmd);

    static void set_mct_value(char *c_cmd);

    static void set_pwr_mode(char *c_cmd);

    static void special_case_CNFG_settings(char *c_cmd);

    static void spi_test(char *c_cmd);

    static void ss_hold(char *c_cmd);

    static void test(char *c_cmd);

    static void trigger_disable(char *c_cmd);

    static void print_help_page();

    static void set_LG_CNFG_reg(uint8_t num, bool ch, uint32_t thresh, bool en);

    static void hal_ide3466_init();

    static void util_IDE3466_inject_charges(uint32_t pulses);

    static void util_IDE3466_set_AMUX(int preampnum);

    static void hal_ide3466_set_CALGEN_reg(uint32_t value, uint32_t mode, int i, int i1);

    static void set_HG_H_CNFG_reg(uint8_t num, bool ch, uint32_t thresh, bool en);

    static void set_HG_L_CNFG_reg(uint8_t num, bool ch, uint32_t thresh);
};