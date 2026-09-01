/**
 * \file
 *
 * \brief Parallel Input/Output (PIO) Controller driver for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef PIO_H_INCLUDED
#define PIO_H_INCLUDED

//#include "compiler.h"

#include <component/pio.h>
#include <stdint-gcc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Compute PIO register length */
#define PIO_DELTA   ((uint32_t) PIOB - (uint32_t) PIOA)

/* GPIO Support */
#define PIO_TYPE_Pos                  27
/* PIO Type Mask */
#define PIO_TYPE_Msk                  (0xFu << PIO_TYPE_Pos)
/* The pin is not a function pin. */
#define PIO_TYPE_NOT_A_PIN            (0x0u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral A. */
#define PIO_TYPE_PIO_PERIPH_A         (0x1u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral B. */
#define PIO_TYPE_PIO_PERIPH_B         (0x2u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral C. */
#define PIO_TYPE_PIO_PERIPH_C         (0x3u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral D. */
#define PIO_TYPE_PIO_PERIPH_D         (0x4u << PIO_TYPE_Pos)
/* The pin is an input. */
#define PIO_TYPE_PIO_INPUT            (0x5u << PIO_TYPE_Pos)
/* The pin is an output and has a default level of 0. */
#define PIO_TYPE_PIO_OUTPUT_0         (0x6u << PIO_TYPE_Pos)
/* The pin is an output and has a default level of 1. */
#define PIO_TYPE_PIO_OUTPUT_1         (0x7u << PIO_TYPE_Pos)

typedef enum _pio_type {
	PIO_NOT_A_PIN   = PIO_TYPE_NOT_A_PIN,
	PIO_PERIPH_A    = PIO_TYPE_PIO_PERIPH_A,
	PIO_PERIPH_B    = PIO_TYPE_PIO_PERIPH_B,
	PIO_PERIPH_C    = PIO_TYPE_PIO_PERIPH_C,
	PIO_PERIPH_D    = PIO_TYPE_PIO_PERIPH_D,
	PIO_INPUT       = PIO_TYPE_PIO_INPUT,
	PIO_OUTPUT_0    = PIO_TYPE_PIO_OUTPUT_0,
	PIO_OUTPUT_1    = PIO_TYPE_PIO_OUTPUT_1
} pio_type_t;

/*  Default pin configuration (no attribute). */
#define PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define PIO_DEGLITCH            (1u << 1)
/*  The pin is open-drain. */
#define PIO_OPENDRAIN           (1u << 2)

/*  The internal debouncing filter is active. */
#define PIO_DEBOUNCE            (1u << 3)

/*  Enable additional interrupt modes. */
#define PIO_IT_AIME             (1u << 4)

/*  Interrupt High Level/Rising Edge detection is active. */
#define PIO_IT_RE_OR_HL         (1u << 5)
/*  Interrupt Edge detection is active. */
#define PIO_IT_EDGE             (1u << 6)

/*  Low level interrupt is active */
#define PIO_IT_LOW_LEVEL        (0               | 0 | PIO_IT_AIME)
/*  High level interrupt is active */
#define PIO_IT_HIGH_LEVEL       (PIO_IT_RE_OR_HL | 0 | PIO_IT_AIME)
/*  Falling edge interrupt is active */
#define PIO_IT_FALL_EDGE        (0               | PIO_IT_EDGE | PIO_IT_AIME)
/*  Rising edge interrupt is active */
#define PIO_IT_RISE_EDGE        (PIO_IT_RE_OR_HL | PIO_IT_EDGE | PIO_IT_AIME)

/*
 *  The #attribute# field is a bitmask that can either be set to PIO_DEFAULT,
 *  or combine (using bitwise OR '|') any number of the following constants:
 *     - PIO_PULLUP
 *     - PIO_DEGLITCH
 *     - PIO_DEBOUNCE
 *     - PIO_OPENDRAIN
 *     - PIO_IT_LOW_LEVEL
 *     - PIO_IT_HIGH_LEVEL
 *     - PIO_IT_FALL_EDGE
 *     - PIO_IT_RISE_EDGE
 */
void pio_pull_up(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_pull_up_enable);
void pio_set_debounce_filter(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_cut_off);
void pio_set(Pio *p_pio, const uint32_t ul_mask);
void pio_clear(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get(Pio *p_pio, const pio_type_t ul_type,
		const uint32_t ul_mask);
void pio_set_peripheral(Pio *p_pio, const pio_type_t ul_type,
		const uint32_t ul_mask);
void pio_set_input(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_attribute);
void pio_set_output(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_default_level,
		const uint32_t ul_multidrive_enable,
		const uint32_t ul_pull_up_enable);
uint32_t pio_configure(Pio *p_pio, const pio_type_t ul_type,
		const uint32_t ul_mask, const uint32_t ul_attribute);
uint32_t pio_get_output_data_status(const Pio *p_pio,
		const uint32_t ul_mask);
void pio_set_multi_driver(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_multi_driver_enable);
uint32_t pio_get_multi_driver_status(const Pio *p_pio);
void pio_pull_down(Pio *p_pio, const uint32_t ul_mask,
                   const uint32_t ul_pull_down_enable);
void pio_enable_output_write(Pio *p_pio, const uint32_t ul_mask);
void pio_disable_output_write(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get_output_write_status(const Pio *p_pio);
void pio_sync_output_write(Pio *p_pio, const uint32_t ul_mask);
void pio_set_schmitt_trigger(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get_schmitt_trigger(const Pio *p_pio);
void pio_configure_interrupt(Pio *p_pio, const uint32_t ul_mask,
		const uint32_t ul_attr);
void pio_enable_interrupt(Pio *p_pio, const uint32_t ul_mask);
void pio_disable_interrupt(Pio *p_pio, const uint32_t ul_mask);
uint32_t pio_get_interrupt_status(const Pio *p_pio);
uint32_t pio_get_interrupt_mask(const Pio *p_pio);
void pio_set_additional_interrupt_mode(Pio *p_pio,
		const uint32_t ul_mask, const uint32_t ul_attribute);
void pio_set_writeprotect(Pio *p_pio, const uint32_t ul_enable);
uint32_t pio_get_writeprotect_status(const Pio *p_pio);
void pio_capture_set_mode(Pio *p_pio, uint32_t ul_mode);
void pio_capture_enable(Pio *p_pio);
void pio_capture_disable(Pio *p_pio);
uint32_t pio_capture_read(const Pio *p_pio, uint32_t * pul_data);
void pio_capture_enable_interrupt(Pio *p_pio, const uint32_t ul_mask);
void pio_capture_disable_interrupt(Pio * p_pio, const uint32_t ul_mask);
uint32_t pio_capture_get_interrupt_status(const Pio *p_pio);
uint32_t pio_capture_get_interrupt_mask(const Pio *p_pio);
/* GPIO Support */
uint32_t pio_get_pin_value(uint32_t pin);
void pio_set_pin_high(uint32_t pin);
void pio_set_pin_low(uint32_t pin);
void pio_toggle_pin(uint32_t pin);
void pio_enable_pin_interrupt(uint32_t pin);
void pio_disable_pin_interrupt(uint32_t pin);
Pio *pio_get_pin_group(uint32_t pin);
uint32_t pio_get_pin_group_id(uint32_t pin);
uint32_t pio_get_pin_group_mask(uint32_t pin);
uint32_t pio_configure_pin(uint32_t ul_pin, const uint32_t ul_flags);
void pio_set_pin_group_high(Pio *p_pio, uint32_t ul_mask);
void pio_set_pin_group_low(Pio *p_pio, uint32_t ul_mask);
void pio_toggle_pin_group(Pio *p_pio, uint32_t ul_mask);
uint32_t pio_configure_pin_group(Pio *p_pio, uint32_t ul_mask,
		const uint32_t ul_flags);
enum pio_io_drive_mode {
    PIO_IO_DRIVE_LOW = 0,
    PIO_IO_DRIVE_HIGH,
};
void pio_set_io_drive(Pio *p_pio, uint32_t ul_line,
                      enum pio_io_drive_mode mode);
void pio_keypad_enable(Pio *p_pio);
void pio_keypad_disable(Pio *p_pio);
void pio_keypad_set_row_num(Pio *p_pio, uint8_t num);
uint8_t pio_keypad_get_row_num(const Pio *p_pio);
void pio_keypad_set_column_num(Pio *p_pio, uint8_t num);
uint8_t pio_keypad_get_column_num(const Pio *p_pio);
void pio_keypad_set_debouncing_value(Pio *p_pio, uint16_t value);
uint16_t pio_keypad_get_debouncing_value(const Pio *p_pio);
void pio_keypad_enable_interrupt(Pio *p_pio, uint32_t ul_mask);
void pio_keypad_disable_interrupt(Pio *p_pio, uint32_t ul_mask);
uint32_t pio_keypad_get_interrupt_mask(const Pio *p_pio);
uint32_t pio_keypad_get_press_status(const Pio *p_pio);
uint32_t pio_keypad_get_release_status(const Pio *p_pio);
uint8_t pio_keypad_get_simult_press_num(const Pio *p_pio);
uint8_t pio_keypad_get_simult_release_num(const Pio *p_pio);
uint8_t pio_keypad_get_press_row_index(const Pio *p_pio, uint8_t queue);
uint8_t pio_keypad_get_press_column_index(const Pio *p_pio, uint8_t queue);
uint8_t pio_keypad_get_release_row_index(const Pio *p_pio, uint8_t queue);
uint8_t pio_keypad_get_release_column_index(const Pio *p_pio, uint8_t queue);


#ifdef __cplusplus
}
#endif

#endif /* PIO_H_INCLUDED */
