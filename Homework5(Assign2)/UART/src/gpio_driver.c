/**
 * @file    gpio_driver.c
 * @brief   Minimal CMSIS-style GPIO helpers for S32K144 LEDs
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#include "gpio_driver.h"

void GPIO_SetOutput(GPIO_Type *gpio, uint32_t mask)
{
    gpio->PDDR |= mask;
}

void LED_On(GPIO_Type *gpio, uint32_t mask)
{
    gpio->PCOR = mask;
}

void LED_Off(GPIO_Type *gpio, uint32_t mask)
{
    gpio->PSOR = mask;
}

void LED_Toggle(GPIO_Type *gpio, uint32_t mask)
{
    gpio->PTOR = mask;
}
