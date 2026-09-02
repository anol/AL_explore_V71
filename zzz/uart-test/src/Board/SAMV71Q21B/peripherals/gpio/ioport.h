
#ifndef IOPORT_H
#define IOPORT_H

#ifdef __cplusplus
extern "C" {
#endif
#define IOPORT_CREATE_PIN(port, pin) ((IOPORT_ ## port) * 32 + (pin))
#define IOPORT_BASE_ADDRESS (uintptr_t)PIOA
#define IOPORT_PIO_OFFSET   ((uintptr_t)PIOB - (uintptr_t)PIOA)

#define IOPORT_PIOA     0
#define IOPORT_PIOB     1
#define IOPORT_PIOC     2
#define IOPORT_PIOD     3
#define IOPORT_PIOE     4
#define IOPORT_PIOF     5

enum ioport_direction {
    IOPORT_DIR_INPUT,  /*!< IOPORT input direction */
    IOPORT_DIR_OUTPUT, /*!< IOPORT output direction */
};

enum ioport_value {
    IOPORT_PIN_LEVEL_LOW,  /*!< IOPORT pin value low */
    IOPORT_PIN_LEVEL_HIGH, /*!< IOPORT pin value high */
};

enum ioport_sense {
    IOPORT_SENSE_BOTHEDGES, /*!< IOPORT sense both rising and falling edges */
    IOPORT_SENSE_FALLING,   /*!< IOPORT sense falling edges */
    IOPORT_SENSE_RISING,    /*!< IOPORT sense rising edges */
    IOPORT_SENSE_LEVEL_LOW, /*!< IOPORT sense low level  */
    IOPORT_SENSE_LEVEL_HIGH,/*!< IOPORT sense High level  */
};

#define IOPORT_MODE_MUX_MASK            (0x7 << 0) /*!< MUX bits mask */
#define IOPORT_MODE_MUX_BIT0            (  1u << 0u) /*!< MUX BIT0 mask */
#define IOPORT_MODE_MUX_BIT1            (  1u << 1u) /*!< MUX BIT1 mask */
#define IOPORT_MODE_MUX_A               (  0u << 0u) /*!< MUX function A */
#define IOPORT_MODE_MUX_B               (  1u << 0u) /*!< MUX function B */
#define IOPORT_MODE_MUX_C               (  2u << 0u) /*!< MUX function C */
#define IOPORT_MODE_MUX_D               (  3u << 0u) /*!< MUX function D */
#define IOPORT_MODE_PULLUP              (  1u << 3u) /*!< Pull-up */
#define IOPORT_MODE_PULLDOWN            (  1u << 4u) /*!< Pull-down */
#define IOPORT_MODE_OPEN_DRAIN          (  1u << 5u) /*!< Open drain */
#define IOPORT_MODE_GLITCH_FILTER       (  1u << 6u) /*!< Glitch filter */
#define IOPORT_MODE_DEBOUNCE            (  1u << 7u) /*!< Input debounce */

typedef uint32_t ioport_mode_t;
typedef uint32_t ioport_pin_t;
typedef uint32_t ioport_port_t;
typedef uint32_t ioport_port_mask_t;

bool ioport_get_pin_level(ioport_pin_t pin);

ioport_port_mask_t ioport_get_port_level(ioport_pin_t port, ioport_port_mask_t mask);

ioport_port_mask_t ioport_pin_to_mask(ioport_pin_t pin);

ioport_port_t ioport_pin_to_port_id(ioport_pin_t pin);

void ioport_disable_pin(ioport_pin_t pin);

void ioport_disable_port(ioport_port_t port, ioport_port_mask_t mask);

void ioport_enable_pin(ioport_pin_t pin);

void ioport_enable_port(ioport_port_t port, ioport_port_mask_t mask);

void ioport_init(void);

void ioport_reset_pin_mode(ioport_pin_t pin);

void ioport_reset_port_mode(ioport_port_t port, ioport_port_mask_t mask);

void ioport_set_pin_dir(ioport_pin_t pin, enum ioport_direction dir);

void ioport_set_pin_level(ioport_pin_t pin, bool level);

void ioport_set_pin_mode(ioport_pin_t pin, ioport_mode_t mode);

void ioport_set_pin_sense_mode(ioport_pin_t pin, enum ioport_sense pin_sense);

void ioport_set_port_dir(ioport_port_t port, ioport_port_mask_t mask, enum ioport_direction dir);

void ioport_set_port_level(ioport_port_t port, ioport_port_mask_t mask, enum ioport_value level);

void ioport_set_port_mode(ioport_port_t port, ioport_port_mask_t mask, ioport_mode_t mode);

void ioport_set_port_sense_mode(ioport_port_t port, ioport_port_mask_t mask, enum ioport_sense pin_sense);

void ioport_toggle_pin_level(ioport_pin_t pin);

void ioport_toggle_port_level(ioport_port_t port, ioport_port_mask_t mask);

#ifdef __cplusplus
}
#endif

#endif // IOPORT_H
