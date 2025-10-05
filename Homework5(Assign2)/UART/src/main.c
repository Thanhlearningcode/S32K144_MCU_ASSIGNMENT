/**
 * @file    main.c
 * @brief   UART command control of LEDs (S32K144, CMSIS-only)
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 *
 * Commands:
 *   LED STATUS
 *   RED ON   | RED OFF
 *   GREEN ON | GREEN OFF
 *   BLUE ON  | BLUE OFF
 *   HELP
 * else -> "Command not available"
 */

#include "S32K144.h"
#include <stdint.h>
#include <stdbool.h>
#include "uart_driver.h"
#include "gpio_driver.h"

/* EVB mapping (active-LOW LEDs on Port D) */
#define LED_PORT              PORTD
#define LED_GPIO              PTD
#define LED_PORT_PCC_INDEX    PCC_PORTD_INDEX
#define LED_RED_PIN           (0U)     /* PTD0  */
#define LED_GRN_PIN           (15U)    /* PTD15 */
#define LED_BLU_PIN           (16U)    /* PTD16 */

#define LED_RED_MASK          (1UL << LED_RED_PIN)
#define LED_GRN_MASK          (1UL << LED_GRN_PIN)
#define LED_BLU_MASK          (1UL << LED_BLU_PIN)

static inline void delay_loops(volatile uint32_t n) { while (n--) { __asm volatile("nop"); } }

static void LEDs_Init(void)
{
    /* Clock for PORTD */
    PCC->PCCn[LED_PORT_PCC_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Mux ALT1 = GPIO */
    LED_PORT->PCR[LED_RED_PIN] = (LED_PORT->PCR[LED_RED_PIN] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);
    LED_PORT->PCR[LED_GRN_PIN] = (LED_PORT->PCR[LED_GRN_PIN] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);
    LED_PORT->PCR[LED_BLU_PIN] = (LED_PORT->PCR[LED_BLU_PIN] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);

    /* Outputs, default OFF */
    GPIO_SetOutput(LED_GPIO, LED_RED_MASK | LED_GRN_MASK | LED_BLU_MASK);
    LED_Off(LED_GPIO, LED_RED_MASK | LED_GRN_MASK | LED_BLU_MASK);
}

static bool is_on(GPIO_Type *gpio, uint32_t mask)
{
    /* With push-pull output and active-LOW LEDs, reading PDIR==0 suggests ON.
       (For absolute truth you could track software state.) */
    return ((gpio->PDIR & mask) == 0U);
}

static void print_help(void)
{
    UART0_WriteString("\r\nCommands:\r\n");
    UART0_WriteString("  LED STATUS\r\n");
    UART0_WriteString("  RED ON   | RED OFF\r\n");
    UART0_WriteString("  GREEN ON | GREEN OFF\r\n");
    UART0_WriteString("  BLUE ON  | BLUE OFF\r\n");
    UART0_WriteString("  HELP\r\n");
}

static void print_status(void)
{
    UART0_WriteString("\r\nLED STATUS:\r\n");
    UART0_WriteString("  RED   : ");  UART0_WriteString(is_on(LED_GPIO, LED_RED_MASK) ? "ON\r\n" : "OFF\r\n");
    UART0_WriteString("  GREEN : ");  UART0_WriteString(is_on(LED_GPIO, LED_GRN_MASK) ? "ON\r\n" : "OFF\r\n");
    UART0_WriteString("  BLUE  : ");  UART0_WriteString(is_on(LED_GPIO, LED_BLU_MASK) ? "ON\r\n" : "OFF\r\n");
}

static void str_to_upper(char *s)
{
    for (; *s; ++s) { if (*s >= 'a' && *s <= 'z') *s = (char)(*s - 'a' + 'A'); }
}

static void handle_command(char *line)
{
    str_to_upper(line);

    if      (!__builtin_strcmp(line, "HELP"))         { print_help(); }
    else if (!__builtin_strcmp(line, "LED STATUS"))   { print_status(); }
    else if (!__builtin_strcmp(line, "RED ON"))       { LED_On (LED_GPIO, LED_RED_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else if (!__builtin_strcmp(line, "RED OFF"))      { LED_Off(LED_GPIO, LED_RED_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else if (!__builtin_strcmp(line, "GREEN ON"))     { LED_On (LED_GPIO, LED_GRN_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else if (!__builtin_strcmp(line, "GREEN OFF"))    { LED_Off(LED_GPIO, LED_GRN_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else if (!__builtin_strcmp(line, "BLUE ON"))      { LED_On (LED_GPIO, LED_BLU_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else if (!__builtin_strcmp(line, "BLUE OFF"))     { LED_Off(LED_GPIO, LED_BLU_MASK);   UART0_WriteString("\r\nOK\r\n"); }
    else                                              { UART0_WriteString("\r\nCommand not available\r\n"); }
}

int main(void)
{
    LEDs_Init();

    UART0_InitPins();
    const Uart_ConfigType cfg = { .baudrate = 115200UL, .osr = 15U }; /* 16x */
    UART0_Init(&cfg);

    UART0_WriteString("\r\nS32K144 UART LED Console\r\nType HELP\r\n> ");

    c
