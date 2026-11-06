#include "../include/Uart.h"
#include "../include/ProjectDefines.h"

#ifndef _FDEV_SETUP_READ
#define _FDEV_SETUP_READ (F_PERM | F_NOWR)
#endif

static FILE uart_output = FDEV_SETUP_STREAM(uart_putch, NULL, _FDEV_SETUP_WRITE);
static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getch, _FDEV_SETUP_READ);
static FILE uart1_output = FDEV_SETUP_STREAM(uart1_putch, NULL, _FDEV_SETUP_WRITE);

static Function_Pointer_With_One_CharPointer_Parameter Callback_Function_Pointer = NULL;

void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_CharPointer_Parameter function_to_callback)
{
    Callback_Function_Pointer = function_to_callback;
}

void RS232Init(void)
{
    UBRR0L = (unsigned char)(MyUBRR & 0xff);
    UBRR0H = (unsigned char)(MyUBRR >> 8);
#ifdef UART_NORMAL_MODE
    UCSR0A = 0x00;
#else
    UCSR0A = (1 << U2X0);
#endif
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart_output;
    stdin = &uart_input;

    Uart1Init();
}

void Uart1Init(void)
{
    UBRR1L = (unsigned char)(MyUBRR & 0xff);
    UBRR1H = (unsigned char)(MyUBRR >> 8);
#ifdef UART_NORMAL_MODE
    UCSR1A = 0x00;
#else
    UCSR1A = (1 << U2X1);
#endif
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void Enable_UART_Receive_Interrupt(void)
{
    UCSR0B |= (1 << RXCIE0);
}

void Disable_UART_Receive_Interupt(void)
{
    UCSR0B &= (unsigned char)~(1 << RXCIE0);
}

bool uart0_data_ready(void)
{
    return (UCSR0A & (1 << RXC0)) != 0;
}

int uart_getch(FILE *stream)
{
    (void)stream;
    while (!(UCSR0A & (1 << RXC0))) {
    }
    return UDR0;
}

int uart_putch(char ch, FILE *stream)
{
    (void)stream;
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    UDR0 = ch;
    return 0;
}

int uart1_putch(char ch, FILE *stream)
{
    (void)stream;
    while (!(UCSR1A & (1 << UDRE1))) {
    }
    UDR1 = ch;
    return 0;
}

FILE *uart1_stream(void)
{
    return &uart1_output;
}

ISR(USART0_RX_vect)
{
    char received = UDR0;

    if (Callback_Function_Pointer != NULL) {
        Callback_Function_Pointer(&received);
    }

    UDR0 = received;
}
