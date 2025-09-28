#include "S32K144.h"
#include <stdint.h>

/* =================== Compatibility layer =================== */
/* Map IP_* names -> standard names if needed */
#ifndef PORTA
  #ifdef IP_PORTA
    #define PORTA IP_PORTA
  #endif
#endif
#ifndef PORTB
  #ifdef IP_PORTB
    #define PORTB IP_PORTB
  #endif
#endif
#ifndef PORTC
  #ifdef IP_PORTC
    #define PORTC IP_PORTC
  #endif
#endif
#ifndef PORTD
  #ifdef IP_PORTD
    #define PORTD IP_PORTD
  #endif
#endif
#ifndef PORTE
  #ifdef IP_PORTE
    #define PORTE IP_PORTE
  #endif
#endif

#ifndef PTA
  #ifdef IP_PTA
    #define PTA IP_PTA
  #endif
#endif
#ifndef PTB
  #ifdef IP_PTB
    #define PTB IP_PTB
  #endif
#endif
#ifndef PTC
  #ifdef IP_PTC
    #define PTC IP_PTC
  #endif
#endif
#ifndef PTD
  #ifdef IP_PTD
    #define PTD IP_PTD
  #endif
#endif
#ifndef PTE
  #ifdef IP_PTE
    #define PTE IP_PTE
  #endif
#endif

#ifndef PCC
  #ifdef IP_PCC
    #define PCC IP_PCC
  #endif
#endif

/* Một số SDK đặt tên IRQ là PORTC_PORTD_IRQn */
#ifndef PORTC_IRQn
  #ifdef PORTC_PORTD_IRQn
    #define PORTC_IRQn PORTC_PORTD_IRQn
  #endif
#endif

/* __NOP fallback nếu thiếu prototype */
#ifndef __NOP
  #define __NOP() __asm volatile ("nop")
#endif
/* =========================================================== */

/*******************************************************************************
 * Definitions (pins)
 ******************************************************************************/

/* EVB S32K144 mapping */
#define LED_RED_PORT        PORTD
#define LED_RED_GPIO        PTD
#define LED_RED_PIN         0U

#define LED_GREEN_PORT      PORTD
#define LED_GREEN_GPIO      PTD
#define LED_GREEN_PIN       16U

#define BTN1_PORT           PORTC
#define BTN1_GPIO           PTC
#define BTN1_PIN            12U

#define BTN2_PORT           PORTC
#define BTN2_GPIO           PTC
#define BTN2_PIN            13U

#define BTN_ACTIVE_LOW      1U  /* nút kéo lên, nhấn = 0 */

/*******************************************************************************
 * Prototypes (minimal helpers)
 ******************************************************************************/
static void BOARD_EnablePortClock(PORT_Type *port);
static void PORT_ConfigAsGPIO(PORT_Type *port, uint32_t pin, uint8_t pullUpEnable, uint8_t irqFalling);
static void GPIO_SetDir(GPIO_Type *gpio, uint32_t pin, uint8_t output);
static void GPIO_Write(GPIO_Type *gpio, uint32_t pin, uint8_t value);
static void GPIO_Toggle(GPIO_Type *gpio, uint32_t pin);
static uint8_t GPIO_Read(GPIO_Type *gpio, uint32_t pin);
static uint8_t APP_ButtonPressed(GPIO_Type *gpio, uint32_t pin);

/*******************************************************************************
 * Variables (file-scope)
 ******************************************************************************/
static volatile uint8_t s_btn1Edge = 0U;
static volatile uint8_t s_btn2Edge = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_EnablePortClock(PORT_Type *port)
{
    if (port == PORTA)
    {
        PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else if (port == PORTB)
    {
        PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else if (port == PORTC)
    {
        PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else if (port == PORTD)
    {
        PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else if (port == PORTE)
    {
        PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else
    {
        /* no-op */
    }
    return;
}

/* PORT: cấu hình chân làm GPIO, tuỳ chọn pull-up và ngắt cạnh xuống */
static void PORT_ConfigAsGPIO(PORT_Type *port, uint32_t pin, uint8_t pullUpEnable, uint8_t irqFalling)
{
    uint32_t pcr;
    pcr = 0U;

    if (pullUpEnable != 0U)
    {
        pcr |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* pull-up */
    }

    pcr |= PORT_PCR_MUX(1U); /* ALT1 = GPIO */

    if (irqFalling != 0U)
    {
        pcr |= PORT_PCR_IRQC(10U); /* IRQC=10: falling edge */
    }

    port->PCR[pin] = pcr;
    return;
}

static void GPIO_SetDir(GPIO_Type *gpio, uint32_t pin, uint8_t output)
{
    if (output != 0U)
    {
        gpio->PDDR |= (1UL << pin);
    }
    else
    {
        gpio->PDDR &= ~(1UL << pin);
    }
    return;
}

static void GPIO_Write(GPIO_Type *gpio, uint32_t pin, uint8_t value)
{
    if (value != 0U
    		) {
        gpio->PSOR = (1UL << pin);
    }
    else
    {
        gpio->PCOR = (1UL << pin);
    }
    return;
}

static void GPIO_Toggle(GPIO_Type *gpio, uint32_t pin)
{
    gpio->PTOR = (1UL << pin);
    return;
}

static uint8_t GPIO_Read(GPIO_Type *gpio, uint32_t pin)
{
    uint8_t level;
    level = (uint8_t)((gpio->PDIR >> pin) & 0x1UL);
    return level;
}

/* Kiểm tra đang nhấn (active-low) */
static uint8_t APP_ButtonPressed(GPIO_Type *gpio, uint32_t pin)
{
    uint8_t level;
    uint8_t pressed;

    level = GPIO_Read(gpio, pin);

#if BTN_ACTIVE_LOW
    pressed = (level == 0U) ? 1U : 0U;
#else
    pressed = (level != 0U) ? 1U : 0U;
#endif

    return pressed;
}

/* ISR: PORTC – đọc ISF, xóa cờ (W1C), đặt cờ event */
void PORTC_IRQHandler(void)
{
    uint32_t isf12;
    uint32_t isf13;

    isf12 = (BTN1_PORT->PCR[BTN1_PIN] & PORT_PCR_ISF_MASK);
    isf13 = (BTN2_PORT->PCR[BTN2_PIN] & PORT_PCR_ISF_MASK);

    if (isf12 != 0U)
    {
        BTN1_PORT->PCR[BTN1_PIN] |= PORT_PCR_ISF_MASK;
        s_btn1Edge = 1U;
    }

    if (isf13 != 0U
    		) {
        BTN2_PORT->PCR[BTN2_PIN] |= PORT_PCR_ISF_MASK;
        s_btn2Edge = 1U;
    }

    return;
}

int main(void)
{
    /* Bật clock cho PORTC (buttons) và PORTD (LEDs) */
    BOARD_EnablePortClock(BTN1_PORT);
    BOARD_EnablePortClock(LED_RED_PORT);

    /* BTN: GPIO + pull-up + ngắt cạnh xuống */
    PORT_ConfigAsGPIO(BTN1_PORT, BTN1_PIN, 1U, 1U);
    PORT_ConfigAsGPIO(BTN2_PORT, BTN2_PIN, 1U, 1U);

    /* LED: GPIO output, không ngắt */
    PORT_ConfigAsGPIO(LED_RED_PORT,   LED_RED_PIN,   0U, 0U);
    PORT_ConfigAsGPIO(LED_GREEN_PORT, LED_GREEN_PIN, 0U, 0U);

    /* Hướng chân */
    GPIO_SetDir(BTN1_GPIO, BTN1_PIN, 0U);
    GPIO_SetDir(BTN2_GPIO, BTN2_PIN, 0U);
    GPIO_SetDir(LED_RED_GPIO,   LED_RED_PIN,   1U);
    GPIO_SetDir(LED_GREEN_GPIO, LED_GREEN_PIN, 1U);

    /* LED off ban đầu */
    GPIO_Write(LED_RED_GPIO,   LED_RED_PIN,   0U);
    GPIO_Write(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);

    /* Enable NVIC cho PORTC (dùng CMSIS API, không cần symbol NVIC) */
#ifdef PORTC_IRQn
    NVIC_ClearPendingIRQ(PORTC_IRQn);
    NVIC_SetPriority(PORTC_IRQn, 0xA0U >> 4); /* CMSIS priority scale */
    NVIC_EnableIRQ(PORTC_IRQn);
#else
    /* Nếu cả PORTC_IRQn lẫn PORTC_PORTD_IRQn đều không có, cần kiểm tra lại tên IRQ trong device header */
#endif

    /* Xóa cờ treo nếu có */
    BTN1_PORT->PCR[BTN1_PIN] |= PORT_PCR_ISF_MASK;
    BTN2_PORT->PCR[BTN2_PIN] |= PORT_PCR_ISF_MASK;

    for (;;)
    {
        if (s_btn1Edge != 0U)
        {
            s_btn1Edge = 0U;
            if (APP_ButtonPressed(BTN1_GPIO, BTN1_PIN) != 0U)
            {
                GPIO_Toggle(LED_RED_GPIO, LED_RED_PIN);
            }
        }

        if (s_btn2Edge != 0U)
        {
            s_btn2Edge = 0U;
            if (APP_ButtonPressed(BTN2_GPIO, BTN2_PIN) != 0U)
            {
                GPIO_Toggle(LED_GREEN_GPIO, LED_GREEN_PIN);
            }
        }

        __NOP();
    }

    /* không tới đây */
    return 0;
}
