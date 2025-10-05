/**
 * @file    main.c
 * @brief   Buttons control LEDs using CMSIS-style Port/GPIO drivers (S32K144)
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 *
 * @details
 * - BTN1 toggles RED LED
 * - BTN2 toggles GREEN LED
 * - LEDs are active-LOW on S32K144 EVB
 */

#include "S32K144.h"
#include "port_driver.h"
#include "gpio_driver.h"

/* ===== Pins (adjust if your board differs) ================================ */
/* LEDs on Port D (active-LOW) */
#define LED_RED_PIN       (0U)     /* PTD0  */
#define LED_GREEN_PIN     (15U)    /* PTD15 */

/* Buttons on Port C (active-LOW with pull-ups) */
#define BTN1_PIN          (12U)    /* PTC12 -> toggle RED   */
#define BTN2_PIN          (13U)    /* PTC13 -> toggle GREEN */

/* Masks */
#define LED_RED_MASK      (1UL << LED_RED_PIN)
#define LED_GREEN_MASK    (1UL << LED_GREEN_PIN)
#define BTN1_MASK         (1UL << BTN1_PIN)
#define BTN2_MASK         (1UL << BTN2_PIN)

/* Simple debounce delay */
static inline void DelayLoops(volatile uint32_t loops)
{
    while (loops-- > 0U) { __asm volatile("nop"); }
}

int main(void)
{
    /* Enable clocks for PORTC and PORTD */
    PORT_EnableClock(PORTC);
    PORT_EnableClock(PORTD);

    /* LEDs: MUX=GPIO, output, default OFF (active-LOW) */
    PORT_PinMux(PORTD, LED_RED_PIN,   PORT_MUX_GPIO);
    PORT_PinMux(PORTD, LED_GREEN_PIN, PORT_MUX_GPIO);
    GPIO_SetDirection(PTD, LED_RED_MASK | LED_GREEN_MASK, true);
    GPIO_SetPins(PTD, LED_RED_MASK | LED_GREEN_MASK);  /* OFF */

    /* Buttons: MUX=GPIO, input, pull-ups enabled (active-LOW) */
    const Port_PullConfigType pullUpCfg = { .enablePull = true, .pullUp = true };
    PORT_PinMux(PORTC, BTN1_PIN, PORT_MUX_GPIO);
    PORT_PinMux(PORTC, BTN2_PIN, PORT_MUX_GPIO);
    PORT_ConfigurePull(PORTC, BTN1_PIN, &pullUpCfg);
    PORT_ConfigurePull(PORTC, BTN2_PIN, &pullUpCfg);
    GPIO_SetDirection(PTC, BTN1_MASK | BTN2_MASK, false);

    /* Previous sampled level (true=HIGH, false=LOW) */
    bool prevBtn1 = true;
    bool prevBtn2 = true;

    for (;;)
    {
        /* crude debounce */
        DelayLoops(2000U);

        bool b1 = GPIO_ReadPin(PTC, BTN1_PIN); /* HIGH=not pressed, LOW=pressed */
        bool b2 = GPIO_ReadPin(PTC, BTN2_PIN
        		);

        /* BTN1 falling edge -> toggle RED */
        if ((prevBtn1 == true) && (b1 == false)) {
            GPIO_TogglePins(PTD, LED_RED_MASK);
        }
        /* BTN2 falling edge -> toggle GREEN */
        if ((prevBtn2 == true) && (b2 == false)) {
            GPIO_TogglePins(PTD, LED_GREEN_MASK);
        }

        prevBtn1 = b1;
        prevBtn2 = b2;
    }
    /* return 0; // not reached */
}
