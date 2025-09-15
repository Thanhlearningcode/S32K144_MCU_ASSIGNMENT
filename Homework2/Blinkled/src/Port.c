#include "Port.h"

Port_status configure_PortPin(PORT_Type *port, uint8_t pin, GPIO_Mode mode)
{
    if (pin >= 32)
    {
        return PORT_ERROR_INVALID_PORT;
    }

    switch (mode)
    {
        case GPIO_OUTPUT:
            port->PCR[pin] = (port->PCR[pin] & ~0x3) | 0x01;  // Mode Output
            break;
        case GPIO_INPUT:
            port->PCR[pin] = (port->PCR[pin] & ~0x3) | 0x00;  // Mode Input
            break;
        case GPIO_INPUT_PULLUP:
            port->PCR[pin] = (port->PCR[pin] & ~0x3) | 0x02;  // Mode input Pull-Up
            break;
        case GPIO_INPUT_PULLDOWN:
            port->PCR[pin] = (port->PCR[pin] & ~0x3) | 0x03;  // Mode Input Pull-Down
            break;
        default:
            return PORT_ERROR_UNKNOWN;
    }
    return PORT_ERROR_NONE;
}

void GPIO_SetPin(PORT_Type *port, uint8_t  pin, uint8_t state)
{
	if (pin<32)
	{
		if (state == 1)
		{
			port->GPCLR |= (1U << pin);
		}
		else
		{
			port->GPCHR |= (1U<<pin);
		}
	}
}
