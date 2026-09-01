/*
 * command_parser.c
 *
 * Created: 19.03.2019 14:56:46
 *  Author: ssorensen
 */


#include <cstdint>
#include <cstring>
#include <Consol.h>
#include "command_parser.h"
#include "commands.h"
#include "other_pins.h"

#define COMMAND_BUFFER_SIZE 8
#define min(x, y) ((x) > (y) ? (y) : (x))
#define RO_CHANNELS 36
volatile extern bool FLAG_ANALOG_RO_DONE;
volatile extern uint16_t readout[RO_CHANNELS];
enum Analog_Readout_Mode {
    SPECTROSCOPY, READ_ACTIVE_CHANNEL, STORE_IN_BUF_AND_PRINT
};

//extern enum Analog_Readout_Mode ANALOG_RO_MODE;
static enum Analog_Readout_Mode ANALOG_RO_MODE = READ_ACTIVE_CHANNEL;

/*
 * increment current command index and decrement counter of unexecuted commands.
 */
void _command_has_been_executed();

static char cmd_buffer[COMMAND_BUFFER_SIZE * COMMAND_MAX_LENGTH];
static int circular_buffer_index = 0;
static int current_command_index = 0;
static int8_t unexecuted_commands_in_buffer = 0;

int8_t add_command_to_buffer(char *command) {
    if (get_open_slots_in_buffer()) {
        int len = (int) strlen(command) + 1;
        if (len <= COMMAND_MAX_LENGTH) {
            if (validate_command(command)) {
                strcpy((char *) &cmd_buffer[COMMAND_MAX_LENGTH * circular_buffer_index++],
                       (char *) command); //TODO: Copy from &command[1] instead of &command[0] ($ sign not needed from here on), all indices in the "interpret" and "execute" command functions must be revisited after this change has been made. Basically any time the local variable c_cmd is used.
                unexecuted_commands_in_buffer =
                        min(unexecuted_commands_in_buffer + 1, COMMAND_BUFFER_SIZE);
                if (circular_buffer_index >= COMMAND_BUFFER_SIZE) {
                    circular_buffer_index = 0;
                }
            }
            Consol::global_printf("Command received: '%s', and added to command buffer.\r\n", command);
            return 1;
        } else {
            /*
                * The received command was longer than the max allowed
                * command length.
                */
            Consol::global_printf("Command discarded: '%s', because it "
                                  "was longer than the max allowed command length: %d chars"
                                  " (the command was: %d chars long). NOTE: '$' and the "
                                  "terminating '0' also counts as part of the command.\r\n",
                                  command, COMMAND_MAX_LENGTH, len);
            return -1;
        }
    } else {
        /* The command buffer is full. The packet will not be stored. */
        Consol::global_printf("Command discarded: '%s', because the "
                              "command buffer is full\r\n", command);
        return -1;
    }

}

int8_t
get_open_slots_in_buffer(void) {
    return COMMAND_BUFFER_SIZE - unexecuted_commands_in_buffer;
}

/*
 * \brief
 * \return int8_t The number of unexecuted commands in the command buffer.
 */
int8_t get_num_unexecuted_commands() {
    return unexecuted_commands_in_buffer;
}

int8_t read_next_command(char *output_buffer) {
    strcpy((char *) output_buffer, (char *) &cmd_buffer[COMMAND_MAX_LENGTH * current_command_index]);
    return 1;
}

int8_t validate_command(char *command) {
    /*TODO:
    * Ideally, this function should look for corrupted byte
    * values and reject commands that have been corrupted during transfer.
    * I am not exactly sure how/if this could be implemented though.
    */
    return 1;
}

void
_command_has_been_executed() {
    char c_cmd[COMMAND_MAX_LENGTH];
    read_next_command(c_cmd);
    current_command_index++;
    if (current_command_index >= COMMAND_BUFFER_SIZE) {
        current_command_index = 0;
    }
    unexecuted_commands_in_buffer--;
    Consol::global_printf("Command has been executed: '%s'\r\n", c_cmd);
}

/**
 * \brief The interpretation and execution of a command shall happen in 
 *	this method. It's currently a bit more than a placeholder,
 *	but not a complete example.
 * \return int8_t
 */
int8_t execute_next_command() {
    char c_cmd[COMMAND_MAX_LENGTH];
    uint32_t CNT_val = 0;

    if (unexecuted_commands_in_buffer == 0)
        return 0;

    read_next_command(c_cmd);

    switch (c_cmd[1]) {
        case 's'://$s -> for spi
            Command::spi_test(c_cmd);
            break;
        case 'S'://$S -> for SS_HOLD
            Command::ss_hold(c_cmd);
            break;
            /* I2C dev test */
        case 'p': // $pN -> I2C device test.
            Command::i2c_test(c_cmd);
            break;
        case 'h':
            /* Help page $h -> help: show all available commands */
            Command::print_help_page();
            break;
        case 'e':
            /*
             * Timing functions
             * $eNNNN -> single pulse with delay_us(NNNN) function. N = 0 is
             * valid input.
             * TP1_4, PA00 digital I/O test pin. (TP1_4_PA00)
             */
            Command::pulse_width(c_cmd);
            break;
        case 'j':
            /*
             * $jNNNN -> single pulse with NNNN NOP instructions
             * N=0 is valid input.
             * TP1_4, PA00 digital I/O test pin. (TP1_4_PA00)
             */
            Command::pulse_width_alternate(c_cmd);
            break;
        case 'l':
//            temporary_mock::util_IDE3466_set_all_CNTs_to_zero();
            break;


            /* MCT and DAC_GLOBAL_GAIN g h*/
        case 'g': // $gNNNN -> set MCT value, default DAC_GLOBAL_GAIN = 0x0200 LSB value.
            Command::set_mct_value(c_cmd);
            break;
        case 'f': // $fNNNN -> set DAC_GLOBAL_GAIN value, MCT default value = 1023 LSB.
            Command::set_dac_global_gain(c_cmd);
            break;

            /* ANALOGUE PART TESTS */
        case 'x': // $xNNN -> DCAL N pulse pulse
            Command::dcal_n_pulse(c_cmd);
            break;
        case 'y':
            Command::dcal_n_pulse_no_delay(c_cmd);
            break;
        case 'r': // $r23 -> clock AMUX to N [digits 23] clocks
            Command::clock_amux(c_cmd);
            break;
        case 't': // $t[x] -> test number x. Used for debugging hri functions
            Command::test(c_cmd);
            break;
        case 'c': // $c toggle "store to buffer and print"
            if (ANALOG_RO_MODE == STORE_IN_BUF_AND_PRINT)
//                temporary_mock::util_NORM_dump_readout_buffers();
            ANALOG_RO_MODE = (ANALOG_RO_MODE == STORE_IN_BUF_AND_PRINT) ? READ_ACTIVE_CHANNEL : STORE_IN_BUF_AND_PRINT;
            Consol::global_printf("Toggle STORE IN BUF AND PRINT mode: %s\r\n",
                                  ANALOG_RO_MODE == STORE_IN_BUF_AND_PRINT ? "ON" : "OFF");
            break;

            /* GENERAL SETTINGS */
        case 'u': // $uN -> global - N = {power mode 0 (default),1 (low-power), 2 (high-power)}
            Command::set_pwr_mode(c_cmd);
            break;

            /* CAL UNIT setting for HG input*/
        case 'a': // $a[X][NNNN] -> setup CALUNIT
            Command::set_cal_unit(c_cmd);
            break;

            /*CNFG register settings */
        case 'C': // $C[X][CH][NNNN][D][T] where
            /* $C[X][CH][NNNN][D][T] where
            *    [X] ................ can be 'L', 'H' or 'h' representing
            *                           "[L]ow gain"-config register, "[H]igh gain High"-config register and
            *                           "[h]igh gain Low"-config register
            *       [CH] ............ represents a two digit channel number (01-32) for H and h, or (01-04) for L
            *           [NNNN] ...... represents the threshold value in range (0000-1023)
            *                           (if its a low or high threshold depends on [X])
            *                 [D].... represents the 'Channel Disable" bitfield
            *                           (can be 1 or 0, defaults to 0 if not provided)
            *                    [T] represents the 'Test Enable' bitfield
            *                           (can be 1 or 0, defaults to 0 if not provided).
            *                           Can be ignored if [X] = 'h', because the HGLT register does
            *                           not have this bit.
            */
            Command::set_CNFG_reg(c_cmd);
            break;
        case 'P':
            /* $P[T][SU][NUM] Configures the pattern register for a sub unit where
            *    [T] ................ can be 'C', 'A', 'N' or 'W' representing
            *                         "[C]oinciding trigger", "[A]nti-coinciding trigger",
            *                         "[N]on-coinciding trigger" (clearing previously set trigger)
            *                         and "[W]ipe pattern" (used to disable all triggers in reg)
            *       [SU] ............ represents a two digit sub unit number in range (01-36)
            *           [NUM] ....... represents the trigger number in range (01-68) that
            *                         sub_unit [SU] should coincide/anticoincide/noncoincide("don't care")
            *                         with according to [T].
            *						  This value is ignored for [T] = 'W'
            */
            Command::set_CNFG_CL_reg(c_cmd);
            break;
        case 'E':
            /* $E[X][SU] Enables counter logic according to:
            *    [X] ... [X = 'A'] All counter logic
            *            [X = 'D'] Disable all counter logic
            *            [X = 'G'] Global enable toggle
            *            [X = 'N'] Enable a given counter
            *       [SU] Sub unit in range [01-36] that will
            *            be enabled if X = 'N'. This also sets
            *            CL_ENABLE_G high.
            */
            Command::enable_CL(c_cmd);
            break;
        case 'T': // $T -> Trigger disable. Disables Channel and Test Enable in all CNFG registers (sets thresh to 1023)
            Command::trigger_disable(c_cmd);
            break;

            /** SPECIAL CASE CNFG REGISTER SETTINGS **/
            /*HG-1 channel settings */
        case 'k': // $kNNNN -> set ch. HG-1 HGLT thres, enable ONLY HGLT trigger, enable TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;
        case 'm': // $mNNNN -> set ch. HG-1 HGHT thres, enable ONLY HGHT trigger, enable TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;

            /*HG-3 channel settings */
        case 'D': // $DNNNN -> set ch. HG-3 HGLT thres, enable ONLY HGLT trigger, DISABLE TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;
        case 'J': // $ENNNN -> set ch. HG-3 HGHT thres, enable ONLY HGHT trigger, DISABLED TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;
        case 'F': // $FNNNN -> set ch. 3 HGHT thres, HGLT thres = 1023, enable HGLT AND HGHT trigger, DISABLE TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;

            /*HG-22 channel settings */
        case 'G': // $GNNNN -> set ch. HG-22 HGLT thres, enable ONLY HGLT trigger, DISABLE TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;
        case 'H': // $HNNNN -> set ch. HG-22 HGHT thres, enable ONLY HGHT trigger, DISABLED TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;
        case 'I': // $INNNN -> set ch. 22 HGHT thres, HGLT thres = 1023, enable HGLT AND HGHT trigger, DISABLE TER input
            Command::special_case_CNFG_settings(c_cmd);
            break;

            /* COUNTER CNT1 */
        case 'd': // $dN -> setup counter 1 to trigger on N = {0=No triggers counted, 1=HGLT-1, 2=HGHT-1, 3=(HGLT-1,HGHT-1 coinc), 4-(HGLT-1 coinc, HGHT-1 anticoinc), 6=HGLT-3, 7=HGHT-3, 8=(HGLT-3,HGHT-3 coinc), 9=(HGLT-3 coinc, HGHT-3 anticoinc)}.
            Command::CNT_trigger(c_cmd);
            break;

        case 'w': // $w -> read counter CNT1 value
            Consol::global_printf("Counter-register-dump:\r\n");
            for (int su = 1; su <= 36; su++) {
//                temporary_mock::hri_ide3466_read_CNT_reg(&CNT_val, su);
                Consol::global_printf("CNT%d: %lu\r\n", su, CNT_val);
            }
            break;

            /* SPI PART TESTS */
        case 'q': // $q -> IDE3466 SPI RESET (SRESET)
            Consol::global_printf("IDE3466 SPI reset (SRESET).\r\n");
//            temporary_mock::IDE3466_spi_init(IDE3466_SPI_CS_NUM);
            break;

        case 'i': // $i -> init
            Command::init();
            break;

        default:
            Consol::global_printf("Error! Unknown command letter received...\r\nTry help pages. Enter $h.\r\n");
            //do nothing
            break;
    }
    //*/


    _command_has_been_executed();
    return 1;
}

