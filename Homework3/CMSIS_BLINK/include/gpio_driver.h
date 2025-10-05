/**
 * @file    gpio_driver.h
 * @brief   Minimal CMSIS-style GPIO driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "S32K144.h"
#include <stdint.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------------
 * Compatibility layer for GPIO base names.
 * Prefer CMSIS (PTA..PTE). Map vendor IP_* variants to those if needed.
 * Array form IP_GPIO[n]: A=0, B=1, C=2, D=3, E=4
 * ---------------------------------------------------------------------------- */
#if defined(IP_GPIO) && !defined(PTA)
  #define PTA (IP_GPIO[0U])
  #define PTB (IP_GPIO[1U])
  #define PTC (IP_GPIO[2U])
  #define PTD (IP_GPIO[3U])
  #define PTE (IP_GPIO[4U])
#endif

/* Per-port IP_PTx */
#if defined(IP_PTA) && !defined(PTA)
  #define PTA IP_PTA
#endif
#if defined(IP_PTB) && !defined(PTB)
  #define PTB IP_PTB
#endif
#if defined(IP_PTC) && !defined(PTC)
  #define PTC IP_PTC
#endif
#if defined(IP_PTD) && !defined(PTD)
  #define PTD IP_PTD
#endif
#if defined(IP_PTE) && !defined(PTE)
  #define PTE IP_PTE
#endif

/* Rare legacy aliases */
#if defined(GPIOD) && !defined(PTD)
  #define PTD GPIOD
#endif
#if defined(GPIOC) && !defined(PTC)
  #define PTC GPIOC
#endif

/* APIs */
void GPIO_SetDirection(GPIO_Type *gpio, uint32_t pinMask, bool output);
void GPIO_SetPins(GPIO_Type *gpio, uint32_t pinMask);     /* write 1 */
void GPIO_ClearPins(GPIO_Type *gpio, uint32_t pinMask);   /* write 0 */
void GPIO_TogglePins(GPIO_Type *gpio, uint32_t pinMask);
bool GPIO_ReadPin(GPIO_Type *gpio, uint32_t pin);         /* 0/1 level */

#endif /* GPIO_DRIVER_H */
