#ifndef UART_CFG_H
#define UART_CFG_H

#include "Platform_Types.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_dma.h"

#define UART_SIGNALS_CNT 2

#define INTERNAL_UART0_DRIVER_INDEX (0u)
#define INTERNAL_UART1_DRIVER_INDEX (1u)

typedef struct
{
	USART_T *uart;
	uint32 clock;
	USART_Config_T mcal_config_uart;
	uint32 irq_type;
	IRQn_Type irq;
	uint8_t preemptionPriority;
	uint8_t subPriority;
} UARTSignalsCfg;

extern const UARTSignalsCfg uartSignalsCfgTable[];

#endif
