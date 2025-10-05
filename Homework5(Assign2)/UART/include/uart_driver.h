/**
 * @file    uart_driver.h
 * @brief   Minimal CMSIS-style UART (LPUART0) driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "S32K144.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t baudrate;   /* e.g. 115200 */
    uint8_t  osr;        /* OSR = N-1 (15 => 16x) */
} Uart_ConfigType;

/* Pins: PTB0=RX, PTB1=TX (ALT2) — chỉnh nếu board khác */
void     UART0_InitPins(void);
void     UART0_Init(const Uart_ConfigType *cfg);   /* 8-N-1, enable TX/RX */
void     UART0_WriteChar(uint8_t c);
uint8_t  UART0_ReadChar(void);
void     UART0_Write(const uint8_t *data, uint32_t len);
void     UART0_WriteString(const char *s);
uint32_t UART0_ReadLine(char *buf, uint32_t maxlen); /* read until CR/LF */

#endif /* UART_DRIVER_H */
