//
// Created by anolsen on 20.09.2019.
//

#include <samv71q21b.h>
#include "GPIO_pin.h"

#define GPIO_PIN(n) (((n)&0x1Fu) << 0u)
#define GPIO_PORT(n) ((n) >> 5u)
#define GPIO(port, pin) ((((port)&0x7u) << 5u) + ((pin)&0x1Fu))

static inline Pio *register_base(const enum gpio_port port) {
    return (Pio *) ((uint32_t) PIOA + port * 0x200);
}

GPIO_pin::GPIO_pin(const uint32_t pin_index,
                   bool output_pin,
                   gpio_function peripheral_function,
                   gpio_pull_mode pull_mode) :
        pin_mask(1U << GPIO_PIN(pin_index)),
        p_port(register_base((enum gpio_port) GPIO_PORT(pin_index))) {
    set_direction(output_pin);
    set_peripheral_function(pin_index, peripheral_function);
    set_pull_mode(pull_mode);
}

void GPIO_pin::enable() {
    p_port->PIO_PER = pin_mask;
}

void GPIO_pin::disable() {
    p_port->PIO_PDR = pin_mask;
}

void GPIO_pin::set() {
    p_port->PIO_SODR = pin_mask;
}

void GPIO_pin::clear() {
    p_port->PIO_CODR = pin_mask;
}

bool GPIO_pin::get() {
    return ((p_port->PIO_PDSR) & pin_mask) != 0;
}

void GPIO_pin::toggle() {
    if (get()) {
        clear();
    } else {
        set();
    }
}

void GPIO_pin::set_direction(bool output_pin) {
    if (output_pin) {
        p_port->PIO_OER = pin_mask;
    } else {
        p_port->PIO_ODR = pin_mask;
    }
}

void GPIO_pin::set_peripheral_function(const uint32_t pin_index, gpio_function function) {
    if (GPIO_PORTB == GPIO_PORT(pin_index)) {
        uint8_t pin = GPIO_PIN(pin_index);
        if ((pin == 4) || (pin == 5) || (pin == 6) || (pin == 7) || (pin == 12)) {
            REG_CCFG_SYSIO |= (0x1u << pin);
        }
    }
    if (function == GPIO_MODE) {
        p_port->PIO_PER = pin_mask;
    } else {
        if (function & 0x1u) {
            p_port->PIO_ABCDSR[0] |= pin_mask;
        } else {
            p_port->PIO_ABCDSR[0] &= ~pin_mask;
        }
        if (function & 0x2u) {
            p_port->PIO_ABCDSR[1] |= pin_mask;
        } else {
            p_port->PIO_ABCDSR[1] &= ~pin_mask;
        }
        p_port->PIO_PDR = pin_mask;
    }

}

void GPIO_pin::set_pull_mode(gpio_pull_mode pull_mode) {
    switch (pull_mode) {
        case GPIO_PULL_OFF:
            p_port->PIO_PUDR = pin_mask;
            p_port->PIO_PPDDR = pin_mask;
            break;
        case GPIO_PULL_UP:
            p_port->PIO_PPDDR = pin_mask;
            p_port->PIO_PUER = pin_mask;
            break;
        case GPIO_PULL_DOWN:
            p_port->PIO_PUDR = pin_mask;
            p_port->PIO_PPDER = pin_mask;
            break;
    }
}
