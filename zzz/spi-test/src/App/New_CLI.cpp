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

#include <climits>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cctype>
#include <cerrno>
#include "New_CLI.h"
#include "Consol.h"

enum token_type {
    KEYWORD,
    ENDTOKEN,
    DCALPULSE,
    AMUXCLOCK,
    FLAG,
    CALDAC,
    NOTOKEN,
    TIME,
    REGISTER,
    CHANNEL,
    THRESHOLD,
    PTYPE,
    SUBUNIT,
    TRIGGER,
    TRIGGER2NUM,
    GAINNUM,
    MCTNUM,
    PULSEDELAYNUM,
    PULSENOPNUM,
    SSHOLDDELAY,
    SSHOLDDEBUG,
};

struct token {
    enum token_type type;
    const char *keyword;
    int value;
    int flag;
    const struct token *next;
};

extern const struct token t_main[];
extern const struct token t_dcal[];
extern const struct token t_pulse[];
extern const struct token t_amux[];
extern const struct token t_clock[];
extern const struct token t_power[];
extern const struct token t_cal[];
extern const struct token t_dac[];
extern const struct token t_show[];
extern const struct token t_set[];
extern const struct token t_time[];
extern const struct token t_config[];
extern const struct token t_register[];
extern const struct token t_channel[];
extern const struct token t_threshold[];
extern const struct token t_pattern[];
extern const struct token t_ptype[];
extern const struct token t_subunit[];
extern const struct token t_trigger[];
extern const struct token t_counter[];
extern const struct token t_countersubunit[];
extern const struct token t_trigger2[];
extern const struct token t_trigger2set[];
extern const struct token t_gain[];
extern const struct token t_mct[];
extern const struct token t_pulse2[];
extern const struct token t_pulse2delay[];
extern const struct token t_pulse2nop[];
extern const struct token t_sshold[];
extern const struct token t_ssholddelay[];
extern const struct token t_ssholddebug[];
extern const struct token t_i2c[];

const struct token t_main[] = {
        {KEYWORD,  "help",          HELP,          0, nullptr},
        {KEYWORD,  "test",          TEST,          0, nullptr},
        {KEYWORD,  "init",          INIT,          0, nullptr},
        {KEYWORD,  "dcal",          DCAL,          0, t_dcal},
        {KEYWORD,  "amux",          AMUX,          0, t_amux},
        {KEYWORD,  "power",         POWER,         0, t_power},
        {KEYWORD,  "cal",           CAL,           0, t_cal},
        {KEYWORD,  "show",          SHOW,          0, t_show},
        {KEYWORD,  "commit",        COMMIT,        0, nullptr},
        {KEYWORD,  "set",           SET,           0, t_set},
        {KEYWORD,  "config",        CONFIG,        0, t_config},
        {KEYWORD,  "pattern",       PATTERN,       0, t_pattern},
        {KEYWORD,  "counter",       COUNTER,       0, t_counter},
        {KEYWORD,  "trigger",       TRIGGER2,      0, t_trigger2},
        {KEYWORD,  "reset",         RESET,         0, nullptr},
        {KEYWORD,  "gain",          GAIN,          0, t_gain},
        {KEYWORD,  "mct",           MCT,           0, t_mct},
        {KEYWORD,  "pulse",         NONE,          0, t_pulse2},
        {KEYWORD,  "sshold",        SSHOLD,        0, t_sshold},
        {KEYWORD,  "i2c",           I2C,           0, t_i2c},
        {KEYWORD,  "register-dump", REGISTER_DUMP, 0, nullptr},
/*
        { KEYWORD,      "special",      SPECIAL,0,      t_special},
*/
        {ENDTOKEN, "",              NONE,          0, nullptr}
};

const struct token t_dcal[] = {
        {KEYWORD,  "pulse", NONE, 0, t_pulse},
        {ENDTOKEN, "",      NONE, 0, nullptr}
};

const struct token t_pulse[] = {
        {DCALPULSE, "", DCALPULSE, 0, nullptr},
        {ENDTOKEN,  "", NONE,      0, nullptr}
};

const struct token t_amux[] = {
        {KEYWORD,  "clock", AMUX, 0, t_clock},
        {ENDTOKEN, "",      NONE, 0, nullptr}
};

const struct token t_clock[] = {
        {AMUXCLOCK, "", NONE, 0, nullptr},
        {ENDTOKEN,  "", NONE, 0, nullptr}
};

const struct token t_power[] = {
        {FLAG,     "default", NONE, F_PWR_DEFAULT, nullptr},
        {FLAG,     "low",     NONE, F_PWR_LOW,     nullptr},
        {FLAG,     "high",    NONE, F_PWR_HIGH,    nullptr},
        {ENDTOKEN, "",        NONE, 0,             nullptr}
};

const struct token t_cal[] = {
        {FLAG,     "off", NONE, F_CAL_OFF, nullptr},
        {FLAG,     "B",   NONE, F_CAL_B,   nullptr},
        {FLAG,     "C",   NONE, F_CAL_C,   nullptr},
        {FLAG,     "D",   NONE, F_CAL_D,   nullptr},
        {FLAG,     "E",   NONE, F_CAL_E,   nullptr},
        {KEYWORD,  "dac", NONE, 0,         t_dac},
        {ENDTOKEN, "",    NONE, 0,         nullptr}
};

const struct token t_dac[] = {
        {CALDAC,   "", NONE, 0, t_cal},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_show[] = {
        {KEYWORD,  "status",   SHOW_STATUS,   0, nullptr},
        {KEYWORD,  "counters", SHOW_COUNTERS, 0, nullptr},
        {KEYWORD,  "time",     SHOW_TIME,     0, nullptr},
        {KEYWORD,  "config",   SHOW_CONFIG,   0, nullptr},
        {KEYWORD,  "running",  SHOW_RUNNING,  0, nullptr},
        {KEYWORD,  "diff",     SHOW_DIFF,     0, nullptr},
        {ENDTOKEN, "",         NONE,          0, nullptr}
};

const struct token t_set[] = {
        {KEYWORD,  "time", NONE, 0, t_time},
        {ENDTOKEN, "",     NONE, 0, nullptr}
};

const struct token t_time[] = {
        {TIME,     "", NONE, 0, nullptr},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_config[] = {
        {NOTOKEN,  "",          NONE, 0,           nullptr},
        {KEYWORD,  "register",  NONE, 0,           t_register},
        {KEYWORD,  "channel",   NONE, 0,           t_channel},
        {KEYWORD,  "threshold", NONE, 0,           t_threshold},
        {FLAG,     "dbit",      NONE, F_CONF_DBIT, t_config},
        {FLAG,     "tbit",      NONE, F_CONF_TBIT, t_config},
        {ENDTOKEN, "",          NONE, 0,           nullptr}
};

const struct token t_register[] = {
        {FLAG,     "L", NONE, F_REG_L,  t_config},
        {FLAG,     "H", NONE, F_REG_HH, t_config},
        {FLAG,     "h", NONE, F_REG_hL, t_config},
        {ENDTOKEN, "",  NONE, 0,        nullptr}
};

const struct token t_channel[] = {
        {CHANNEL,  "", NONE, 0, t_config},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_threshold[] = {
        {THRESHOLD, "", NONE, 0, t_config},
        {ENDTOKEN,  "", NONE, 0, nullptr}
};

const struct token t_pattern[] = {
        {NOTOKEN,  "",        NONE, 0, nullptr},
        {KEYWORD,  "type",    NONE, 0, t_ptype},
        {KEYWORD,  "subunit", NONE, 0, t_subunit},
        {KEYWORD,  "trigger", NONE, 0, t_trigger},
        {ENDTOKEN, "",        NONE, 0, nullptr}
};

const struct token t_ptype[] = {
        {PTYPE,    "co",   NONE, F_PTYPE_CO,   t_pattern},
        {PTYPE,    "anti", NONE, F_PTYPE_ANTI, t_pattern},
        {PTYPE,    "non",  NONE, F_PTYPE_NON,  t_pattern},
        {PTYPE,    "wipe", NONE, F_PTYPE_WIPE, t_pattern},
        {ENDTOKEN, "",     NONE, 0,            nullptr}
};

const struct token t_subunit[] = {
        {SUBUNIT,  "", NONE, 0, t_pattern},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_trigger[] = {
        {TRIGGER,  "", NONE, 0, t_pattern},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_counter[] = {
        {NOTOKEN,  "",        NONE, 0,            nullptr},
        {FLAG,     "all",     NONE, F_COUNT_ALL,  t_counter},
        {FLAG,     "enable",  NONE, F_COUNT_ENAB, t_counter},
        {FLAG,     "disable", NONE, F_COUNT_DIS,  t_counter},
        {KEYWORD,  "subunit", NONE, 0,            t_countersubunit},
        {ENDTOKEN, "",        NONE, 0,            nullptr}
};

const struct token t_countersubunit[] = {
        {SUBUNIT,  "", NONE, 0, nullptr},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_trigger2[] = {
        {KEYWORD,  "disbale", NONE, F_TRIG_DISABLE, nullptr},
        {KEYWORD,  "set",     NONE, 0,              t_trigger2set},
        {ENDTOKEN, "",        NONE, 0,              nullptr}
};

const struct token t_trigger2set[] = {
        {TRIGGER2NUM, "", NONE, 0, nullptr},
        {ENDTOKEN,    "", NONE, 0, nullptr}
};

const struct token t_gain[] = {
        {GAINNUM,  "", NONE, 0, nullptr},
        {ENDTOKEN, "", NONE, 0, nullptr},
};

const struct token t_mct[] = {
        {MCTNUM,   "", NONE, 0, nullptr},
        {ENDTOKEN, "", NONE, 0, nullptr}
};

const struct token t_pulse2[] = {
        {KEYWORD,  "delay", NONE, 0, t_pulse2delay},
        {KEYWORD,  "nop",   NONE, 0, t_pulse2nop},
        {ENDTOKEN, "",      NONE, 0, nullptr}
};

const struct token t_pulse2delay[] = {
        {PULSEDELAYNUM, "", PULSEDELAY, 0, nullptr},
        {ENDTOKEN,      "", NONE,       0, nullptr}
};

const struct token t_pulse2nop[] = {
        {PULSENOPNUM, "", PULSENOP, 0, nullptr},
        {ENDTOKEN,    "", NONE,     0, nullptr}
};

const struct token t_sshold[] = {
        {FLAG,     "default", SSHOLDFLAG, F_SSH_DEF, nullptr},
        {FLAG,     "off",     SSHOLDFLAG, F_SSH_OFF, nullptr},
        {KEYWORD,  "delay",   NONE, 0,               t_ssholddelay},
        {KEYWORD,  "debug",   NONE, 0,               t_ssholddebug},
        {ENDTOKEN, "",        NONE, 0,               nullptr}
};

const struct token t_ssholddelay[] = {
        {SSHOLDDELAY, "", NONE, 0, nullptr},
        {ENDTOKEN,    "", NONE, 0, nullptr}
};

const struct token t_ssholddebug[] = {
        {SSHOLDDEBUG, "", NONE, 0, nullptr},
        {ENDTOKEN,    "", NONE, 0, nullptr}
};

const struct token t_i2c[] = {
        {KEYWORD,  "test", NONE, 0, nullptr},
        {ENDTOKEN, "",     NONE, 0, nullptr}
};

static const struct token *match_token(int *argc, char **argv[], const struct token [], New_CLI::parse_result *res);

static void show_valid_args(const struct token []);

static int parse_number(const char *, New_CLI::parse_result *, enum token_type);

static int parse_time(const char *, New_CLI::parse_result *, enum token_type);

static uint32_t my_strtonum(const char *source, int32_t min_value, int32_t max_value, const char **error_message);

static bool parse_arguments(int argc, char **argv, New_CLI::parse_result *res);

static int checkarg(char *);

static char **arg2argv(char *, int *);

static ssize_t mstrip(char *);

bool New_CLI::parse_commandline(char *line, parse_result *res) {
    bool success = false;
    int argc;
    if (mstrip(line)) {
        char **argv = arg2argv(line, &argc);
        if (argv == nullptr) {
            Consol::global_printf("error tokenizing input\r\n");
        } else {
            success = parse_arguments(argc, argv, res);
            free(argv);
        }
    }
    return success;
}

void New_CLI::print_help() {
    Consol::global_printf("\r\nNORM CLI 1.0 Commands\r\n");
    show_valid_args(t_main);
    Consol::global_printf("\r\n");
}

void New_CLI::print_result(parse_result *res) {
    Consol::global_printf("\r\n");
    Consol::global_printf("   action      = %d\r\n", res->action);
    Consol::global_printf("   amuxclock   = %d\r\n", res->amuxclock);
    Consol::global_printf("   chan        = %d\r\n", res->chan);
    Consol::global_printf("   dac         = %d\r\n", res->dac);
    Consol::global_printf("   dcalpulse   = %d\r\n", res->dcalpulse);
    Consol::global_printf("   flags       = %d\r\n", res->flags);
    Consol::global_printf("   gain        = %d\r\n", res->gain);
    Consol::global_printf("   mct         = %d\r\n", res->mct);
    Consol::global_printf("   ptype       = %d\r\n", res->ptype);
    Consol::global_printf("   pulsedelay  = %d\r\n", res->pulsedelay);
    Consol::global_printf("   pulsenop    = %d\r\n", res->pulsenop);
    Consol::global_printf("   reg         = %d\r\n", res->reg);
    Consol::global_printf("   ssholddebug = %d\r\n", res->ssholddebug);
    Consol::global_printf("   ssholddelay = %d\r\n", res->ssholddelay);
    Consol::global_printf("   subunit     = %d\r\n", res->subunit);
    Consol::global_printf("   thres       = %d\r\n", res->thres);
    Consol::global_printf("   time        = %ld\r\n", res->time);
    Consol::global_printf("   trigger     = %d\r\n", res->trigger);
    Consol::global_printf("   trigger2    = %d\r\n", res->trigger2);
    Consol::global_printf("\r\n");
}

static bool parse_arguments(int argc, char **argv, New_CLI::parse_result *res) {
    const struct token *table = t_main;
    const struct token *match;
    memset(res, 0, sizeof(*res));
    while (argc >= 0) {
        if ((match = match_token(&argc, &argv, table, res)) == nullptr) {
            Consol::global_printf("valid commands/args:\r\n");
            show_valid_args(table);
            return false;
        }
        argc--;
        argv++;
        if (match->type == NOTOKEN || match->next == nullptr)
            break;
        table = match->next;
    }
    if (argc > 0) {
        Consol::global_printf("superfluous argument: %s\r\n", argv[0]);
        return false;
    }
    return true;
}

static const struct token *
match_token(int *argc, char **argv[], const struct token table[], New_CLI::parse_result *res) {
    uint32_t i, match;
    const struct token *t = nullptr;
    const char *word = *argv[0];
    size_t wordlen = 0;
    match = 0;
    if (word != nullptr)
        wordlen = strlen(word);
    for (i = 0; table[i].type != ENDTOKEN; i++) {
        switch (table[i].type) {
            case NOTOKEN:
                if (word == nullptr || wordlen == 0) {
                    match++;
                    t = &table[i];
                }
                break;
            case KEYWORD:
                if (word != nullptr && strncmp(word, table[i].keyword, wordlen) == 0) {
                    match++;
                    t = &table[i];
                    if (t->value != NONE) {
                        res->action = static_cast<actions>(t->value);
                        Consol::global_printf("KEYWORD sets res->action = %d\r\n", res->action);
                    }
                }
                break;
            case FLAG: /* set single bits */
                if (word != nullptr && strncmp(word, table[i].keyword, wordlen) == 0) {
                    match++;
                    t = &table[i];
                    res->flags |= t->flag;
                    if (t->value != NONE) {
                        res->action = static_cast<actions>(t->value);
                        Consol::global_printf("FLAG sets res->action = %d\r\n", res->action);
                    }
                }
                break;
            case DCALPULSE:
            case AMUXCLOCK:
            case CALDAC:
            case REGISTER:
            case CHANNEL:
            case THRESHOLD:
            case SUBUNIT:
            case TRIGGER:
            case TRIGGER2NUM:
            case GAINNUM:
            case MCTNUM:
            case PULSEDELAYNUM:
            case PULSENOPNUM:
            case SSHOLDDELAY:
            case SSHOLDDEBUG:
                /* XXX add individual range checks in parse_number */
                if (word != nullptr && wordlen > 0 &&
                    parse_number(word, res, table[i].type)) {
                    match++;
                    t = &table[i];
                    if (t->value != NONE) {
                        res->action = static_cast<actions>(t->value);
                        Consol::global_printf("type(%d) set res->action = %d\r\n", table[i].type, res->action);
                    }
                }
                break;
            case TIME:
                if (word != nullptr && wordlen > 0 &&
                    parse_time(word, res, table[i].type)) {
                    match++;
                    t = &table[i];
                }
                break;
            case PTYPE:
                if (word != nullptr && strncmp(word, table[i].keyword, wordlen) == 0) {
                    match++;
                    t = &table[i];
                    res->ptype = t->value;
                }
                break;
            default:
                Consol::global_printf("%s: BAD CASE %d\r\n", __func__, table[i].type);
        }
    }
    if (match != 1) {
        if (word == nullptr)
            Consol::global_printf("missing argument:\r\n");
        else if (match > 1)
            Consol::global_printf("ambiguous argument: %s\r\n", word);
        else if (match < 1)
            Consol::global_printf("wrong argument: %s\r\n", word);
        return (nullptr);
    }
    return (t);
}

static void show_valid_args(const struct token table[]) {
    int i;
    for (i = 0; table[i].type != ENDTOKEN; i++) {
        switch (table[i].type) {
            case NOTOKEN:
                Consol::global_printf("  <cr>\r\n");
                break;
            case KEYWORD:
            case FLAG:
                Consol::global_printf("  %s\r\n", table[i].keyword);
                break;
            case DCALPULSE:
            case AMUXCLOCK:
                Consol::global_printf("  <number>\r\n");
                break;
            case CALDAC:
                Consol::global_printf("  cal dac <0..1023>\r\n");
                break;
            case TIME:
                Consol::global_printf("  <time> (describe format here)\r\n");
                break;
            case REGISTER:
                Consol::global_printf("  <L|H|h>\r\n");
                break;
            case CHANNEL:
                Consol::global_printf("  channel <1-32> or <1-4>n");
                break;
            case THRESHOLD:
                Consol::global_printf("  threshold <0-1023>\r\n");
                break;
            case PTYPE:
                Consol::global_printf("  <co|anti|non|wipe>\r\n");
                break;
            case SUBUNIT:
                Consol::global_printf("  subunit <1-36>\r\n");
                break;
            case TRIGGER:
                Consol::global_printf("  trigger <1-68>\r\n");
                break;
            case TRIGGER2NUM:
                Consol::global_printf("  trigger set <0-9>\r\n");
                break;
            case GAINNUM:
                Consol::global_printf("  gain <range-missing>\r\n"); /* XXX */
                break;
            case MCTNUM:
                Consol::global_printf("  mct <range-missing>\r\n"); /* XXX */
                break;
            case PULSEDELAYNUM:  /* XXX */
                Consol::global_printf("  pulse delay <range-missing>\r\n");
                break;
            case PULSENOPNUM:  /* XXX */
                Consol::global_printf("  pulse nop <range-missing>\r\n");
                break;
            case SSHOLDDELAY:  /* XXX */
                Consol::global_printf("  sshold delay <range-missing>\r\n");
                break;
            case SSHOLDDEBUG:  /* XXX */
                Consol::global_printf("  sshold debug <range-missing>\r\n");
                break;
            case ENDTOKEN:
                break;
        }
    }
}

static int parse_number(const char *word, struct New_CLI::parse_result *r, enum token_type type) {
/*      struct filter_set       *fs; */
    const char *errstr;
    int32_t uval;
    if (word == nullptr)
        return 0;
    uval = my_strtonum(word, 0, UINT_MAX, &errstr);
    if (errstr) {
        Consol::global_printf("%s: number is %s: %s\r\n",
                              __func__, errstr, word);
        return 0;
    }
    /* Consol::global_printf("%s: %s\r\n", __func__, word); */
    /* XXX add individual range checks */
    switch (type) {
        case DCALPULSE:
            r->dcalpulse = uval;
            Consol::global_printf("dacpulse %d\r\n", r->dcalpulse);
            break;
        case CALDAC:
            r->dac = uval;
            Consol::global_printf("dac %d\r\n", r->dac);
            break;
        case AMUXCLOCK:
            r->amuxclock = uval;
            Consol::global_printf("amuxclock %d\r\n", r->amuxclock);
            break;
        case TIME:
            r->time = uval;
            Consol::global_printf("time %ld\r\n", r->time);
            break;
        case REGISTER:
            r->reg = uval;
            Consol::global_printf("register %d\r\n", r->reg);
            break;
        case CHANNEL:
            r->chan = uval;
            Consol::global_printf("channel %d\r\n", r->chan);
            break;
        case THRESHOLD:
            r->thres = uval;
            Consol::global_printf("threshold %d\r\n", r->thres);
            break;
        case SUBUNIT:
            r->subunit = uval;
            Consol::global_printf("subunit %d\r\n", r->subunit);
            break;
        case TRIGGER:
            r->trigger = uval;
            Consol::global_printf("trigger %d\r\n", r->trigger);
            break;
        case TRIGGER2NUM:
            r->trigger2 = uval;
            Consol::global_printf("trigger set %d\r\n", r->trigger2);
            break;
        case GAINNUM:
            r->gain = uval;
            Consol::global_printf("gain %d\r\n", r->gain);
            break;
        case MCTNUM:
            r->mct = uval;
            Consol::global_printf("mct %d\r\n", r->mct);
            break;
        case PULSEDELAYNUM:
            r->pulsedelay = uval;
            Consol::global_printf("pulse delay %d\r\n", r->pulsedelay);
            break;
        case PULSENOPNUM:
            r->pulsenop = uval;
            Consol::global_printf("pulse nop %d\r\n", r->pulsenop);
            break;
        case SSHOLDDELAY:
            r->ssholddelay = uval;
            Consol::global_printf("sshold delay %d\r\n", r->ssholddelay);
            break;
        case SSHOLDDEBUG:
            r->ssholddebug = uval;
            Consol::global_printf("sshold debug %d\r\n", r->ssholddebug);
            break;
        default:
            Consol::global_printf("%s: BAD CASE", __func__);
            return 0;
    }
    return 1;
}

static int parse_time(const char *word, struct New_CLI::parse_result *r, enum token_type type) {
    const char *errstr;
    time_t uval;
    if (word == nullptr)
        return (0);
    /* C++
       time_t maxTime = std::numeric_limits<time_t>::max(); */
    uval = my_strtonum(word, 0, LONG_MAX, &errstr);
    if (errstr) {
        Consol::global_printf("number is %s: %s\r\n", errstr, word);
        return (0);
    }
    if (uval < 1) {
        Consol::global_printf("time must be > 0, parsed %ld\r\n", uval);
        return (0);
    }
    Consol::global_printf("%s: %s\r\n", __func__, word);
    /* number was parseable */
    switch (type) {
        case TIME:
            r->time = uval;
            Consol::global_printf("time %ld\r\n", r->time);
            return (1);
            break;
        default:
            Consol::global_printf("BAD CASE type %d\r\n", type);
            return (0);
    }
    return (0);
}

static uint32_t my_strtonum(const char *source, int32_t min_value, int32_t max_value, const char **error_message) {
    return strtol(source, const_cast<char **>(error_message), 10);
}

static int checkarg(char *arg) {
    size_t len;
    uint32_t i;
    if (!(len = strlen(arg)))
        return (0);
#define allowed_in_string(_x)                                           \
        ((isalnum((unsigned char)_x) || isprint((unsigned char)_x)) &&  \
        (_x != '%' && _x != '\\' && _x != ';' && _x != '&' && _x != '|'))
    for (i = 0; i < len; i++) {
        if (!allowed_in_string(arg[i])) {
            Consol::global_printf("invalid character in input\r\n");
            return (EPERM);
        }
    }
#undef allowed_in_string
    return (0);
}

static char **arg2argv(char *arg, int *argc) {
    char **argv, *ptr = arg;
    size_t len;
    uint32_t i, c = 1;
    if (checkarg(arg) != 0)
        return (nullptr);
    if (!(len = strlen(arg)))
        return (nullptr);
    /* Count elements */
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char) arg[i])) {
            /* filter out additional options */
            if (arg[i + 1] == '-') {
                Consol::global_printf("invalid input\r\n");
                return (nullptr);
            }
            arg[i] = '\0';
            c++;
        }
    }
    if (arg[0] == '\0')
        return (nullptr);
    /* Generate array */
    if ((argv = static_cast<char **>(calloc(c + 1, sizeof(char *)))) == nullptr) {
        Consol::global_printf("fatal error: %s\r\n", strerror(errno));
        return (nullptr);
    }
    argv[c] = nullptr;
    *argc = c;
    /* Fill array */
    for (i = c = 0; i < len; i++) {
        if (arg[i] == '\0' || i == 0) {
            if (i != 0)
                ptr = &arg[i + 1];
            argv[c++] = ptr;
        }
    }
    return (argv);
}

static ssize_t mstrip(char *str) {
    size_t len;
    if ((len = strlen(str)) < 1)
        return (0);
    if (isspace((unsigned char) str[len - 1])) {
        str[len - 1] = '\0';
        return (mstrip(str));
    }
    return (strlen(str));
}