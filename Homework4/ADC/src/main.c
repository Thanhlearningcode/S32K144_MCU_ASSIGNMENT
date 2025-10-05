/**
 * @file    main.c
 * @brief   ADC0 reads potentiometer, drive LEDs by ranges (IP_* headers)
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyen Van Thanh
 *
 * Ranges (mV) -> LED:
 *  - 3750..5000 -> Red
 *  - 2500..3750 -> Green
 *  - 1250..2500 -> Blue
 *  -    0..1250 -> None
 */

#include "device_registers.h"
#include <stdint.h>

/* ===== Board mapping (EVB phổ biến) =======================================
 * POT on PTB0 -> ADC0_SE8  (nếu board bạn khác, đổi phần dưới)
 */
#define POT_PORT              IP_PORTB
#define POT_PIN               (0U)
#define POT_PORT_PCC_INDEX    PCC_PORTB_INDEX
#define ADC0_CHANNEL          (8U)

/* LEDs on Port D (active-LOW) */
#define LED_PORT              IP_PORTD
#define LED_GPIO              IP_PTD
#define LED_PORT_PCC_INDEX    PCC_PORTD_INDEX
#define LED_RED_PIN           (0U)    /* PTD0  */
#define LED_GREEN_PIN         (15U)   /* PTD15 */
#define LED_BLUE_PIN          (16U)   /* PTD16 */

static inline void delay_loops(volatile uint32_t n)
{
    while (n--) { __asm volatile("nop"); }
}

/* ================================ ADC0 ==================================== */
static void ADC0_Init(void)
{
    /* Clocks: ADC0 + involved PORTs */
    IP_PCC->PCCn[PCC_ADC0_INDEX]     |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[POT_PORT_PCC_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[LED_PORT_PCC_INDEX] |= PCC_PCCn_CGC_MASK;

    /* POT pin to analog: MUX=0, pull disabled */
    POT_PORT->PCR[POT_PIN] =
        (POT_PORT->PCR[POT_PIN] & ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK))
        | PORT_PCR_MUX(0U);

    /* Basic ADC setup, single-ended, SW trigger */
    IP_ADC0->SC1[0] = ADC_SC1_ADCH(31U);  /* disable channel initially */
    /* Giữ CFG/SC2/SC3 mặc định cho đơn giản bài tập */
}

/* One-shot conversion; return 12-bit raw */
static uint16_t ADC0_ReadRaw(uint8_t ch)
{
    IP_ADC0->SC1[0] = ADC_SC1_ADCH(ch);          /* start conversion */
    while ((IP_ADC0->SC1[0] & ADC_SC1_COCO_MASK) == 0U) { /* wait */ }
    return (uint16_t)(IP_ADC0->R[0] & 0xFFFFU);
}

/* Convert raw to mV (giả sử Vref = 5000 mV) */
static uint16_t adc_raw_to_mV(uint16_t raw)
{
    return (uint16_t)((((uint32_t)raw) * 5000UL) / 4095UL);
}

/* ================================ LEDs ==================================== */
static void LEDs_Init(void)
{
    /* MUX ALT1 = GPIO */
    LED_PORT->PCR[LED_RED_PIN]   = (LED_PORT->PCR[LED_RED_PIN]   & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);
    LED_PORT->PCR[LED_GREEN_PIN] = (LED_PORT->PCR[LED_GREEN_PIN] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);
    LED_PORT->PCR[LED_BLUE_PIN]  = (LED_PORT->PCR[LED_BLUE_PIN]  & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1U);

    /* Outputs */
    LED_GPIO->PDDR |= (1UL << LED_RED_PIN) | (1UL << LED_GREEN_PIN) | (1UL << LED_BLUE_PIN);

    /* All OFF (active-LOW -> write 1) */
    LED_GPIO->PSOR = (1UL << LED_RED_PIN) | (1UL << LED_GREEN_PIN) | (1UL << LED_BLUE_PIN);
}

static inline void led_on(uint32_t pin)  { LED_GPIO->PCOR = (1UL << pin); } /* 0 -> ON  */
static inline void led_off(uint32_t pin) { LED_GPIO->PSOR = (1UL << pin); } /* 1 -> OFF */
static inline void leds_all_off(void)
{
    LED_GPIO->PSOR = (1UL << LED_RED_PIN) | (1UL << LED_GREEN_PIN) | (1UL << LED_BLUE_PIN);
}

/* ================================== App =================================== */
int main(void)
{
    ADC0_Init();
    LEDs_Init();

    for (;;)
    {
        uint16_t raw = ADC0_ReadRaw((uint8_t)ADC0_CHANNEL);
        uint16_t mv  = adc_raw_to_mV(raw);

        leds_all_off();

        if (mv >= 3750U)            { led_on(LED_RED_PIN);   }  /* 3750..5000 */
        else if (mv >= 2500U)       { led_on(LED_GREEN_PIN); }  /* 2500..3749 */
        else if (mv >= 1250U)       { led_on(LED_BLUE_PIN);  }  /* 1250..2499 */
        else                        { /* none */ }

        delay_loops(5000U); /* pace + chống rung gần ngưỡng */
    }
    /* return 0; */
}
