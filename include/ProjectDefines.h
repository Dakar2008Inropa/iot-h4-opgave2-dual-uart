#ifndef PROJECTDEFINES_H_
#define PROJECTDEFINES_H_

#define F_CPU 16000000UL
#define Upper_Lower_Bit_Value 0x20

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Clangd uses stock ATmega328P headers (no USART1). XC8/-mcpu=ATmega328PB
 * already provides these — stubs are IntelliSense-only.
 */
#if !defined(UDR1)
#define UCSR1A (*(volatile uint8_t *)(0xC8))
#define UCSR1B (*(volatile uint8_t *)(0xC9))
#define UCSR1C (*(volatile uint8_t *)(0xCA))
#define UBRR1L (*(volatile uint8_t *)(0xCC))
#define UBRR1H (*(volatile uint8_t *)(0xCD))
#define UDR1   (*(volatile uint8_t *)(0xCE))
#ifndef U2X1
#define U2X1   1
#endif
#ifndef UDRE1
#define UDRE1  5
#endif
#ifndef TXEN1
#define TXEN1  3
#endif
#ifndef RXEN1
#define RXEN1  4
#endif
#ifndef UCSZ10
#define UCSZ10 1
#endif
#ifndef UCSZ11
#define UCSZ11 2
#endif
#endif

typedef void (*Function_Pointer_With_One_CharPointer_Parameter)(char *);

#if defined(_AVR_ATMEGA328PB_H_INCLUDED) || defined(_AVR_IOM2560_H_)
#define USR0_Vect_Num USART0_RX_vect
#define GreenLedBit   PORTB1
#define YellowLedBit  PORTB2
#define RedLedBit     PORTB5
#elif defined(_AVR_IOM168_H_) || defined(_AVR_IOM328P_H_)
#define USR0_Vect_Num USART_RX_vect
#endif

#endif
