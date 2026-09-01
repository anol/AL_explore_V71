/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2019 Sebastian Benoit, IDEAS <sebastian.benoit@ideas.no>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef SPI_TEST_NEW_CLI_H
#define SPI_TEST_NEW_CLI_H

#include <sys/types.h>

enum actions {
    NONE,
    HELP,
    TEST,
    INIT,
    DCAL,
    AMUX,
    POWER,
    CAL,
    SHOW,
    SHOW_STATUS,
    SHOW_COUNTERS,
    SHOW_TIME,
    SHOW_CONFIG,
    SHOW_RUNNING,
    SHOW_DIFF,
    COMMIT,
    SET,
    CONFIG,
    PATTERN,
    COUNTER,
    TRIGGER2,
    RESET,
    GAIN,
    MCT,
    PULSEDELAY,
    PULSENOP,
    SSHOLD,
    SSHOLDFLAG,
    I2C,
    REGISTER_DUMP,
};
/* power (default|low|high) */
#define F_PWR_NONE      0x00
#define F_PWR_DEFAULT   0x1
#define F_PWR_LOW       0x2
#define F_PWR_HIGH      0x3
/* cal (off|B|C|D|E) */
#define F_CAL_NONE      0x00
#define F_CAL_OFF       0x01
#define F_CAL_A         0x02
#define F_CAL_B         0x04
#define F_CAL_C         0x08
#define F_CAL_D         0x10
#define F_CAL_E         0x20
/* config .. tbit dbit */
#define F_CONF_DBIT     0x1
#define F_CONF_TBIT     0x2
/* config register L|H|h */
#define F_REG_L         0x1
#define F_REG_HH        0x2
#define F_REG_hL        0x4
/* config pattern co|anti|non|wipe */
#define F_PTYPE_CO      0x1
#define F_PTYPE_ANTI    0x2
#define F_PTYPE_NON     0x4
#define F_PTYPE_WIPE    0x4
/* counter ... */
#define F_COUNT_ALL     0x1
#define F_COUNT_ENAB    0x2
#define F_COUNT_DIS     0x4
/* trigger disable */
#define F_TRIG_DISABLE  0x1
/* sshold */
#define F_SSH_DEF       0x1
#define F_SSH_OFF       0x2

class New_CLI {
public:
    struct parse_result {
        int flags;
        enum actions action;
        int dac;           /* cal ... dac number */
        time_t time;
        int reg;           /* config ... */
        int chan;
        int thres;
        int dcalpulse;     /* dcal pulse */
        int amuxclock;     /* amux clock */
        int ptype;         /* pattern ... */
        int subunit;
        int trigger;
        int trigger2;      /* trigger set no, 1-9 */
        int gain;          /* gain ... */
        int mct;           /* mct ... */
        int pulsedelay;    /* pulse delay ... */
        int pulsenop;      /* pulse nop ... */
        int ssholddelay;   /* sshold ... */
        int ssholddebug;
    };

    static bool parse_commandline(char *commandline, parse_result *res);

    static void print_result(parse_result *res);

    static void print_help();
};

#endif //SPI_TEST_NEW_CLI_H