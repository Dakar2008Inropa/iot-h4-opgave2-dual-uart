#ifndef UART_H_
#define UART_H_

#include "ProjectDefines.h"
#include <stdio.h>

#define BAUD 9600UL

#define UART_NORMAL_MODE
#ifdef UART_NORMAL_MODE
#define MyUBRR (unsigned int)(F_CPU / (16UL * BAUD) - 1UL)
#else
#define MyUBRR (unsigned int)(F_CPU / (8UL * BAUD) - 1UL)
#endif

void RS232Init(void);
void Uart1Init(void);

int uart_getch(FILE *stream);
int uart_putch(char ch, FILE *stream);
int uart1_putch(char ch, FILE *stream);
FILE *uart1_stream(void);

bool uart0_data_ready(void);

void Enable_UART_Receive_Interrupt(void);
void Disable_UART_Receive_Interupt(void);
void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_CharPointer_Parameter function_to_callback);

#endif
