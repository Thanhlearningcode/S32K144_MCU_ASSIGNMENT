/**
 * @file    s32k144_exercise_2.c
 * @brief   Blink 3 LEDs: Blue -> Red -> Green (active-LOW) on S32K144 EVB
 * @version 0.1
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 *
 * @details
 * Hardware: NXP S32K144 EVB
 * Toolchain/IDE: S32 Design Studio (S32DS)
 *
 * Notes:
 * - Uses vendor IP_* register style as in the original template.
 * - LEDs on the EVB are active-LOW (write 0 to turn ON, write 1 to turn OFF).
 */

/* =========================== Includes ===================================== */
#include "S32K144.h"
#include <stdio.h>

/* =========================== Prototypes =================================== */
/**
 * @brief Busy-wait delay (simple loop).
 * @param time Loop count (implementation-dependent timing).
 */
void delay(volatile uint32_t time);

/* =========================== Definitions ================================== */
void delay(volatile uint32_t time)
{
    while (time--) { /* busy wait */ }
}

/* =========================== Application ================================== */
int main(void)
{
    /* Enable clock for PORTD */
    IP_PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;

    /* Set PTD0, PTD15, PTD16 to GPIO (MUX = ALT1) */
    IP_PORTD->PCR[0]  |= (0x1U << 8);
    IP_PORTD->PCR[15] |= (0x1U << 8);
    IP_PORTD->PCR[16] |= (0x1U << 8);

    /* Configure outputs and ensure all LEDs are OFF initially (active-LOW) */
    IP_PTD->PDDR |= (0x1U << 0) | (0x1U << 15) | (0x1U << 16);
    IP_PTD->PSOR |= (0x1U << 0);
    IP_PTD->PSOR |= (0x1U << 15);
    IP_PTD->PSOR |= (0x1U << 16);

    /* Main loop: Blue -> Red -> Green */
    while (1)
    {
        /* Blue ON, Red/Green OFF */
        IP_PTD->PCOR |= (0x1U << 0);
        IP_PTD->PSOR |= (0x1U << 15);
        IP_PTD->PSOR |= (0x1U << 16);
        delay(10000000);

        /* Red ON, Blue/Green OFF */
        IP_PTD->PCOR |= (0x1U << 15);
        IP_PTD->PSOR |= (0x1U << 0);
        IP_PTD->PSOR |= (0x1U << 16);
        delay(10000000);

        /* Green ON, Blue/Red OFF */
        IP_PTD->PCOR |= (0x1U << 16);
        IP_PTD->PSOR |= (0x1U << 0);
        IP_PTD->PSOR |= (0x1U << 15);
        delay(10000000);
    }

    /* Not reached */
    return 0;
}
