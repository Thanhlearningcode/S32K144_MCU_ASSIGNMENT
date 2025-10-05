/**
 * @file    uart_driver.c
 * @brief   CMSIS-only LPUART0 driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#include "uart_driver.h"

void UART0_InitPins(void)
{
    /* Enable PORTB clock */
    PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;

    /* PTB0 RX, PTB1 TX -> ALT2 (LPUART0) */
    PORTB->PCR[0] = (PORTB->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
    PORTB->PCR[1] = (PORTB->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
}

/* Giữ đơn giản: giả định clock UART ~48 MHz (FIRC). Nếu clock tree khác, chỉnh giá trị này. */
static uint32_t UART0_GetSrcClockHz(void)
{
    return 48000000UL;
}

void UART0_Init(const Uart_ConfigType *cfg)
{
    uint32_t srcHz = UART0_GetSrcClockHz();
    uint32_t osr   = (cfg && cfg->osr >= 3U && cfg->osr <= 31U) ? (uint32_t)cfg->osr : 15U;
    uint32_t baud  = (cfg && cfg->baudrate) ? cfg->baudrate : 115200UL;

    /* Enable LPUART0 clock */
    PCC->PCCn[PCC_LPUART0_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Disable TX/RX before changing BAUD */
    LPUART0->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

    /* SBR = src / (baud * (OSR+1)) */
    uint32_t sbr = srcHz / (baud * (osr + 1UL));
    if (sbr == 0U) { sbr = 1U; }

    LPUART0->BAUD = (LPUART_BAUD_OSR(osr) & LPUART_BAUD_OSR_MASK) |
                    (LPUART_BAUD_SBR(sbr) & LPUART_BAUD_SBR_MASK);

    /* 8-N-1 */
    LPUART0->CTRL &= ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_M_MASK);

    /* Enable TX/RX */
    LPUART0->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
}

void UART0_WriteChar(uint8_t c)
{
    while ((LPUART0->STAT & LPUART_STAT_TDRE_MASK) == 0U) { }
    LPUART0->DATA = (uint32_t)c;
}

uint8_t UART0_ReadChar(void)
{
    while ((LPUART0->STAT & LPUART_STAT_RDRF_MASK) == 0U) { }
    return (uint8_t)(LPUART0->DATA & 0xFFU);
}

void UART0_Write(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i) { UART0_WriteChar(data[i]); }
}

void UART0_WriteString(const char *s)
{
    while (*s) { UART0_WriteChar((uint8_t)*s++); }
}

uint32_t UART0_ReadLine(char *buf, uint32_t maxlen)
{
    uint32_t n = 0U;
    if (maxlen == 0U) { return 0U; }

    for (;;)
    {
        uint8_t ch = UART0_ReadChar();
        UART0_WriteChar(ch); /* echo */

        if (ch == '\r' || ch == '\n') { break; }
        if (n + 1U < maxlen) { buf[n++] = (char)ch; }  /* truncate if overflow */
    }
    buf[n] = '\0';
    return n;
}
