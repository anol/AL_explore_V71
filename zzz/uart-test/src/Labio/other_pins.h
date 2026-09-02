/*
 * other_pins.h
 *
 * This file should contain all other pin assignments than those created by the ATMEL START project (those will be found in atmel_start_pins.h)
 *
 * Created: 12.03.2019 10:21:15
 *  Author: ssorensen
 */ 

#ifndef OTHER_PINS_H_INCLUDED
#define OTHER_PINS_H_INCLUDED

//Interrupt:
#define LED0 GPIO(GPIO_PORTA, 23)
#define LED1 GPIO(GPIO_PORTC, 9)

#define SW0 GPIO(GPIO_PORTA, 9) //Pull up by default
#define SW1 GPIO(GPIO_PORTB, 12) //Pull down by default

//remapping IDE3466 specific SPI pins/variables:
#define IDE3466_SPI_RESET SPI_RST_0 
#define IDE3466_SPI_CS_NUM 0

// Timer/Counter peripheral
#define SS_HOLD GPIO(GPIO_PORTE, 3)
#define THGVO_BUF GPIO(GPIO_PORTE, 4)

//From IDE3466_Driver:
#define TP1_4			GPIO(GPIO_PORTA, 0)
//#define SS_HOLD_PA02	GPIO(GPIO_PORTA, 2) //TODO: remark. This is a legacy value, right? - Simen 17.06.2019
#define RO_CLK			GPIO(GPIO_PORTA, 6)
#define DTVO_BUF		GPIO(GPIO_PORTA, 18)
#define ERR_BUF			GPIO(GPIO_PORTA, 24)
#define TLGVO_BUF		GPIO(GPIO_PORTB, 0)
//#define THGVO_BUF_PB01	GPIO(GPIO_PORTB, 1)
#define SHIFT_OUT_BUF	GPIO(GPIO_PORTC, 12)
#define GLU_BUF			GPIO(GPIO_PORTC, 14)
#define SHIFT_IN		GPIO(GPIO_PORTC, 19)
#define DCAL_PIN		GPIO(GPIO_PORTD, 19)
#define DRESET_PIN		GPIO(GPIO_PORTD, 27)
#define MSTEST_BUF		GPIO(GPIO_PORTD, 28)
#define TP1_3			GPIO(GPIO_PORTD, 30)


#endif // OTHER_PINS_H_INCLUDED
