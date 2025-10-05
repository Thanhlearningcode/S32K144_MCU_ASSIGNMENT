/**
 * @file    gpio_driver.c
 * @brief   Minimal CMSIS-style GPIO driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#include "gpio_driver.h"

void GPIO_SetDirection(GPIO_Type *gpio, uint32_t pinMask, bool output)
{
    if (output) { gpio->PDDR |= pinMask; }
    else        { gpio->PDDR &= ~pinMask; }
}

void GPIO_SetPins(GPIO_Type *gpio, uint32_t pinMask)
{
    gpio->PSOR = pinMask;
}

void GPIO_ClearPins(GPIO_Type *gpio, uint32_t pinMask)
{
    gpio->PCOR = pinMask;
}

void GPIO_TogglePins(GPIO_Type *gpio, uint32_t pinMask)
{
    gpio->PTOR = pinMask;
}

bool GPIO_ReadPin(GPIO_Type *gpio, uint32_t pin)
{
    return ((gpio->PDIR >> pin) & 1U) ? true : false;
}
