# IoT H4 — Opgave 2: Dual UART (ATmega328PB)

Bygger videre på opgave 1, så programmet bruger **begge** USART’er på ATmega328PB: UART0 via boardets USB (mEDBG) og UART1 via ekstern CP2102.

## Opgaven kort

- Modtag tegn fra PC på **UART0**, byt store/små bogstaver, og send resultatet på **både UART0 og UART1**.
- Hvert **sekund**: toggle LED, sæt et flag i timer-interrupt, og lad **main** skrive  
  `Sekunder efter opstart: N` ud på **UART1** (og nulstille flaget).

## Sådan virker vores løsning

### To serielle porte

| UART | Pins | PC-port | Rolle |
|------|------|---------|--------|
| **USART0** | PD0 (RX), PD1 (TX) | COM3 (board-USB) | Input + echo af konverteret tegn |
| **USART1** | PB3 (TX), PB4 (RX) | COM4 (CP2102) | Sekundtæller + samme konverterede tegn |

Vi initialiserer begge i `Uart.c` (`RS232Init` / `Uart1Init`) med 9600 baud, 8N1.

### Case-toggle

Bogstaver vendes med `^= 0x20` (bit 5 i ASCII). Det er samme trick som i opgave 1.

### Timer + flag (ikke blokér i interrupt)

`Timer1` kører CTC med callback hvert **1000 ms**. I interrupt gør vi kun det korte:

1. Toggle LED på **PB5**
2. Sæt `second_flag = true`

Den “tunge” del — `fprintf` af sekundteksten — kører i **main**, når flaget er set. Derefter nulstilles flaget. Så undgår vi langsom I/O inde i interrupt.

### Main-loop uden blocking read

Hvis main ventede for evigt på `uart_getch`, ville sekundtælleren stoppe mens man ikke tastede. Derfor polller vi:

- `second_flag` → skriv til UART1  
- `uart0_data_ready()` → læs UART0, konvertér, skriv til begge  

### LED

LED sidder på **PB5** (J200 pin 6), fordi PB3/PB4 er reserveret til USART1. Boardets egen LED er også PB5, så den blinker med.

### CP2102

| CP2102 | Board |
|--------|--------|
| TXD | PB4 |
| RXD | PB3 |
| GND | GND |

Skriv tegn på **COM3**. På **COM4** ser du både sekundlinjer og de konverterede tegn.

## Filer

| Fil | Indhold |
|-----|---------|
| `src/main.c` | Flag, sekundtæller, case-toggle til begge UART’er |
| `src/Uart.c` / `include/Uart.h` | USART0 + USART1 |
| `src/timer1.c` | 1 s timer-callback |
| `include/ProjectDefines.h` | `F_CPU`, LED-bit, IntelliSense-stubs for USART1 |

## Test

1. Flash via board-USB.  
2. COM4 @ 9600: `Sekunder efter opstart: 1, 2, 3…`  
3. COM3: send `d` → `UART0: D`  
4. COM4: samtidig `UART1: D`  
5. LED blinker ca. 1 Hz.
