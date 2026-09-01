

#include <stdint.h>
#include <stdbool.h>
#include <component/pio.h>
#include <component/pmc.h>
#include <sysclk.h>

#include "ioport.h"

inline static ioport_port_t arch_ioport_pin_to_port_id(ioport_pin_t pin) {
    return pin >> 5;
}

inline static Pio *arch_ioport_port_to_base(ioport_port_t port) {
    return (Pio *) ((uintptr_t) IOPORT_BASE_ADDRESS +
                    (IOPORT_PIO_OFFSET * port));
}

inline static Pio *arch_ioport_pin_to_base(ioport_pin_t pin) {
    return arch_ioport_port_to_base(arch_ioport_pin_to_port_id(pin));
}

inline static ioport_port_mask_t arch_ioport_pin_to_mask(ioport_pin_t pin) {
    return 1U << (pin & 0x1F);
}

inline static void arch_ioport_init(void) {
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_PIOB);
    sysclk_enable_peripheral_clock(ID_PIOC);
    sysclk_enable_peripheral_clock(ID_PIOD);
    sysclk_enable_peripheral_clock(ID_PIOE);
}

inline static void arch_ioport_enable_port(ioport_port_t port,
                                           ioport_port_mask_t mask) {
    arch_ioport_port_to_base(port)->PIO_PER = mask;
}

inline static void arch_ioport_disable_port(ioport_port_t port,
                                            ioport_port_mask_t mask) {
    arch_ioport_port_to_base(port)->PIO_PDR = mask;
}

inline static void arch_ioport_enable_pin(ioport_pin_t pin) {
    arch_ioport_enable_port(arch_ioport_pin_to_port_id(pin),
                            arch_ioport_pin_to_mask(pin));
}

inline static void arch_ioport_disable_pin(ioport_pin_t pin) {
    arch_ioport_disable_port(arch_ioport_pin_to_port_id(pin),
                             arch_ioport_pin_to_mask(pin));
}

inline static void arch_ioport_set_port_mode(ioport_port_t port,
                                             ioport_port_mask_t mask, ioport_mode_t mode) {
    Pio *base = arch_ioport_port_to_base(port);

    if (mode & IOPORT_MODE_PULLUP) {
        base->PIO_PUER = mask;
    } else {
        base->PIO_PUDR = mask;
    }

#if defined(IOPORT_MODE_PULLDOWN)
    if (mode & IOPORT_MODE_PULLDOWN) {
        base->PIO_PPDER = mask;
    } else {
        base->PIO_PPDDR = mask;
    }
#endif

    if (mode & IOPORT_MODE_OPEN_DRAIN) {
        base->PIO_MDER = mask;
    } else {
        base->PIO_MDDR = mask;
    }

    if (mode & (IOPORT_MODE_GLITCH_FILTER | IOPORT_MODE_DEBOUNCE)) {
        base->PIO_IFER = mask;
    } else {
        base->PIO_IFDR = mask;
    }

    if (mode & IOPORT_MODE_DEBOUNCE) {
        base->PIO_IFSCER = mask;
    } else {
        base->PIO_IFSCDR = mask;
    }

#if !defined(IOPORT_MODE_MUX_BIT1)
    if (mode & IOPORT_MODE_MUX_BIT0) {
        base->PIO_ABSR |= mask;
    } else {
        base->PIO_ABSR &= ~mask;
    }
#else
    if (mode & IOPORT_MODE_MUX_BIT0) {
        base->PIO_ABCDSR[0] |= mask;
    } else {
        base->PIO_ABCDSR[0] &= ~mask;
    }

    if (mode & IOPORT_MODE_MUX_BIT1) {
        base->PIO_ABCDSR[1] |= mask;
    } else {
        base->PIO_ABCDSR[1] &= ~mask;
    }
#endif
}

inline static void arch_ioport_set_pin_mode(ioport_pin_t pin,
                                            ioport_mode_t mode) {
    arch_ioport_set_port_mode(arch_ioport_pin_to_port_id(pin),
                              arch_ioport_pin_to_mask(pin), mode);
}

inline static void arch_ioport_set_port_dir(ioport_port_t port,
                                            ioport_port_mask_t mask, enum ioport_direction group_direction) {
    Pio *base = arch_ioport_port_to_base(port);

    if (group_direction == IOPORT_DIR_OUTPUT) {
        base->PIO_OER = mask;
    } else if (group_direction == IOPORT_DIR_INPUT) {
        base->PIO_ODR = mask;
    }

    base->PIO_OWER = mask;
}

inline static void arch_ioport_set_pin_dir(ioport_pin_t pin,
                                           enum ioport_direction dir) {
    Pio *base = arch_ioport_pin_to_base(pin);

    if (dir == IOPORT_DIR_OUTPUT) {
        base->PIO_OER = arch_ioport_pin_to_mask(pin);
    } else if (dir == IOPORT_DIR_INPUT) {
        base->PIO_ODR = arch_ioport_pin_to_mask(pin);
    }

    base->PIO_OWER = arch_ioport_pin_to_mask(pin);
}

inline static void arch_ioport_set_pin_level(ioport_pin_t pin,
                                             bool level) {
    Pio *base = arch_ioport_pin_to_base(pin);

    if (level) {
        base->PIO_SODR = arch_ioport_pin_to_mask(pin);
    } else {
        base->PIO_CODR = arch_ioport_pin_to_mask(pin);
    }
}

inline static void arch_ioport_set_port_level(ioport_port_t port,
                                              ioport_port_mask_t mask, enum ioport_value level) {
    Pio *base = arch_ioport_port_to_base(port);

    if (level) {
        base->PIO_SODR = mask;
    } else {
        base->PIO_CODR = mask;
    }
}

inline static bool arch_ioport_get_pin_level(ioport_pin_t pin) {
    return arch_ioport_pin_to_base(pin)->PIO_PDSR & arch_ioport_pin_to_mask(pin);
}

inline static ioport_port_mask_t arch_ioport_get_port_level(
        ioport_port_t port, ioport_port_mask_t mask) {
    return arch_ioport_port_to_base(port)->PIO_PDSR & mask;
}

inline static void arch_ioport_toggle_pin_level(ioport_pin_t pin) {
    Pio *port = arch_ioport_pin_to_base(pin);
    ioport_port_mask_t mask = arch_ioport_pin_to_mask(pin);

    if (port->PIO_PDSR & arch_ioport_pin_to_mask(pin)) {
        port->PIO_CODR = mask;
    } else {
        port->PIO_SODR = mask;
    }
}

inline static void arch_ioport_toggle_port_level(ioport_port_t port,
                                                 ioport_port_mask_t mask) {
    arch_ioport_port_to_base(port)->PIO_ODSR ^= mask;
}

inline static void arch_ioport_set_port_sense_mode(ioport_port_t port,
                                                   ioport_port_mask_t mask, enum ioport_sense pin_sense) {
    Pio *base = arch_ioport_port_to_base(port);
    /*   AIMMR    ELSR    FRLHSR
     *       0       X         X    IOPORT_SENSE_BOTHEDGES (Default)
     *       1       0         0    IOPORT_SENSE_FALLING
     *       1       0         1    IOPORT_SENSE_RISING
     *       1       1         0    IOPORT_SENSE_LEVEL_LOW
     *       1       1         1    IOPORT_SENSE_LEVEL_HIGH
     */
    switch (pin_sense) {
        case IOPORT_SENSE_LEVEL_LOW:
            base->PIO_LSR = mask;
            base->PIO_FELLSR = mask;
            break;
        case IOPORT_SENSE_LEVEL_HIGH:
            base->PIO_LSR = mask;
            base->PIO_REHLSR = mask;
            break;
        case IOPORT_SENSE_FALLING:
            base->PIO_ESR = mask;
            base->PIO_FELLSR = mask;
            break;
        case IOPORT_SENSE_RISING:
            base->PIO_ESR = mask;
            base->PIO_REHLSR = mask;
            break;
        default:
            base->PIO_AIMDR = mask;
            return;
    }
    base->PIO_AIMER = mask;
}

inline static void arch_ioport_set_pin_sense_mode(ioport_pin_t pin,
                                                  enum ioport_sense pin_sense) {
    arch_ioport_set_port_sense_mode(arch_ioport_pin_to_port_id(pin),
                                    arch_ioport_pin_to_mask(pin), pin_sense);
}

void ioport_init(void) {
    arch_ioport_init();
}

void ioport_enable_pin(ioport_pin_t pin) {
    arch_ioport_enable_pin(pin);
}

void ioport_enable_port(ioport_port_t port,
                        ioport_port_mask_t mask) {
    arch_ioport_enable_port(port, mask);
}

void ioport_disable_pin(ioport_pin_t pin) {
    arch_ioport_disable_pin(pin);
}

void ioport_disable_port(ioport_port_t port,
                         ioport_port_mask_t mask) {
    arch_ioport_disable_port(port, mask);
}

void ioport_set_port_mode(ioport_port_t port,
                          ioport_port_mask_t mask, ioport_mode_t mode) {
    arch_ioport_set_port_mode(port, mask, mode);
}

void ioport_set_pin_mode(ioport_pin_t pin, ioport_mode_t mode) {
    arch_ioport_set_pin_mode(pin, mode);
}

void ioport_reset_port_mode(ioport_port_t port,
                            ioport_port_mask_t mask) {
    arch_ioport_set_port_mode(port, mask, 0);
}

void ioport_reset_pin_mode(ioport_pin_t pin) {
    arch_ioport_set_pin_mode(pin, 0);
}

void ioport_set_port_dir(ioport_port_t port,
                         ioport_port_mask_t mask, enum ioport_direction dir) {
    arch_ioport_set_port_dir(port, mask, dir);
}

void ioport_set_pin_dir(ioport_pin_t pin,
                        enum ioport_direction dir) {
    arch_ioport_set_pin_dir(pin, dir);
}

void ioport_set_pin_level(ioport_pin_t pin, bool level) {
    arch_ioport_set_pin_level(pin, level);
}

void ioport_set_port_level(ioport_port_t port,
                           ioport_port_mask_t mask, enum ioport_value level) {
    arch_ioport_set_port_level(port, mask, level);
}

bool ioport_get_pin_level(ioport_pin_t pin) {
    return arch_ioport_get_pin_level(pin);
}

ioport_port_mask_t ioport_get_port_level(ioport_pin_t port,
                                         ioport_port_mask_t mask) {
    return arch_ioport_get_port_level(port, mask);
}

void ioport_toggle_pin_level(ioport_pin_t pin) {
    arch_ioport_toggle_pin_level(pin);
}

void ioport_toggle_port_level(ioport_port_t port,
                              ioport_port_mask_t mask) {
    arch_ioport_toggle_port_level(port, mask);
}

void ioport_set_pin_sense_mode(ioport_pin_t pin,
                               enum ioport_sense pin_sense) {
    arch_ioport_set_pin_sense_mode(pin, pin_sense);
}

void ioport_set_port_sense_mode(ioport_port_t port,
                                ioport_port_mask_t mask,
                                enum ioport_sense pin_sense) {
    arch_ioport_set_port_sense_mode(port, mask, pin_sense);
}

ioport_port_t ioport_pin_to_port_id(ioport_pin_t pin) {
    return arch_ioport_pin_to_port_id(pin);
}

ioport_port_mask_t ioport_pin_to_mask(ioport_pin_t pin) {
    return arch_ioport_pin_to_mask(pin);
}

