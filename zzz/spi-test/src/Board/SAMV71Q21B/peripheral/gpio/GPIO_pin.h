//
// Created by anolsen on 20.09.2019.
//

#ifndef SPI_TEST_GPIO_PIN_H
#define SPI_TEST_GPIO_PIN_H

#include <component/pio.h>

enum gpio_function {
    GPIO_MODE = 0xffffffffu,
    PERIPHERAL_MODE_MUX_A = (0u << 0u),
    PERIPHERAL_MODE_MUX_B = (1u << 0u),
    PERIPHERAL_MODE_MUX_C = (2u << 0u),
    PERIPHERAL_MODE_MUX_D = (3u << 0u)
};

enum gpio_pull_mode {
    GPIO_PULL_OFF, GPIO_PULL_UP, GPIO_PULL_DOWN
};
enum gpio_direction {
    GPIO_DIRECTION_OFF, GPIO_DIRECTION_IN, GPIO_DIRECTION_OUT
};

enum gpio_port {
    GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, GPIO_PORTD, GPIO_PORTE
};

class GPIO_pin {
public:
    explicit GPIO_pin(uint32_t pin_index,
            bool output_pin = false,
            gpio_function peripheral_function = GPIO_MODE,
            gpio_pull_mode pull_mode= GPIO_PULL_OFF);

    void enable();

    void disable();

    void set();

    void clear();

    bool get();

    void toggle();

private:
    const uint32_t pin_mask;
    Pio *const p_port;

    void set_direction(bool output_pin);

    void set_pull_mode(gpio_pull_mode pull_mode);

    void set_peripheral_function(uint32_t pin_index, gpio_function function);
};


#endif //SPI_TEST_GPIO_PIN_H
