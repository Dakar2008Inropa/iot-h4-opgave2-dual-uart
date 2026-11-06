#include "../include/ProjectDefines.h"
#include "../include/Uart.h"
#include "../include/timer1.h"

static volatile bool second_flag = false;
static uint32_t seconds_since_start = 0;

static void ConvertReceivedChar(char *received)
{
    if ((*received >= 'A' && *received <= 'Z') ||
        (*received >= 'a' && *received <= 'z')) {
        *received ^= Upper_Lower_Bit_Value;
    }
}

static void on_second_tick(void)
{
    PORTB ^= (1 << RedLedBit);
    second_flag = true;
}

int main(void)
{
    char received;

    DDRB |= (1 << RedLedBit);
    PORTB |= (1 << RedLedBit); /* tænd med det samme, så vi kan se LED efter flash */

    RS232Init();
    timer1_start_ms(1000, on_second_tick);
    sei();

    printf("\r\nUART0 klar (COM3). Send et bogstav.\r\n");
    fprintf(uart1_stream(), "\r\nUART1 klar (CP2102 / COM4).\r\n");

    for (;;) {
        if (second_flag) {
            second_flag = false;
            seconds_since_start++;
            fprintf(uart1_stream(),
                    "Sekunder efter opstart: %lu\r\n",
                    (unsigned long)seconds_since_start);
        }

        if (uart0_data_ready()) {
            received = (char)UDR0;
            ConvertReceivedChar(&received);

            /* Opgaven: modtag på UART0, skriv konverteret tegn på begge */
            printf("UART0: %c\r\n", received);
            fprintf(uart1_stream(), "UART1: %c\r\n", received);
        }
    }
}
