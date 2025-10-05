/**
 * @file    gpio_driver.h
 * @brief   Minimal CMSIS-style GPIO helpers for S32K144 LEDs
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "S32K144.h"
#include <stdint.h>
#include <stdbool.h>

/* Active-LOW LED helpers */
void GPIO_SetOutput(GPIO_Type *gpio, uint32_t mask);
void LED_On (GPIO_Type *gpio, uint32_t mask);   /* drive 0 -> ON  */
void LED_Off(GPIO_Type *gpio, uint32_t mask);   /* drive 1 -> OFF */
void LED_Toggle(GPIO_Type *gpio, uint32_t mask);

#endif /* GPIO_DRIVER_H */
