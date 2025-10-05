/**
 * @file    port_driver.h
 * @brief   Minimal CMSIS-style PORT driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#ifndef PORT_DRIVER_H
#define PORT_DRIVER_H

#include "S32K144.h"
#include <stdint.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------------
 * Compatibility layer: map IP_* (vendor) to CMSIS names if CMSIS not present
 * ---------------------------------------------------------------------------- */
#if defined(IP_PCC) && !defined(PCC)
  #define PCC IP_PCC
#endif

#if defined(IP_PORTA) && !defined(PORTA)
  #define PORTA IP_PORTA
#endif
#if defined(IP_PORTB) && !defined(PORTB)
  #define PORTB IP_PORTB
#endif
#if defined(IP_PORTC) && !defined(PORTC)
  #define PORTC IP_PORTC
#endif
#if defined(IP_PORTD) && !defined(PORTD)
  #define PORTD IP_PORTD
#endif
#if defined(IP_PORTE) && !defined(PORTE)
  #define PORTE IP_PORTE
#endif

/* MUX options (ALT1=GPIO) */
#define PORT_MUX_GPIO           (1U)

/* Pull configuration */
typedef struct {
    bool enablePull;   /* PE bit: true -> enable pull resistor */
    bool pullUp;       /* PS bit: true -> pull-up, false -> pull-down */
} Port_PullConfigType;

/* APIs */
void PORT_EnableClock(PORT_Type *portBase);
void PORT_PinMux(PORT_Type *portBase, uint32_t pin, uint32_t mux);
void PORT_ConfigurePull(PORT_Type *portBase, uint32_t pin, const Port_PullConfigType *cfg);

#endif /* PORT_DRIVER_H */
