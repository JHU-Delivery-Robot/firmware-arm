#include "uart.h"

#include "bit_utils.h"
#include "perif/sercom.h"
#include "sam.h"
#include "sys/clock.h"

void uart_init(Sercom_t *sercom, const unsigned int baud) {
    const uint32_t pchctrl_conf = GCLK_PCHCTRL_CHEN(0x1)
                                  | GCLK_PCHCTRL_GEN_GCLK3;
    GCLK_REGS->GCLK_PCHCTRL[sercom->pch_num] = pchctrl_conf;

    sercom->mode = SERCOM_MODE_UART;
    Sercom_t foo;
    const uint32_t ctrla = SERCOM_USART_INT_CTRLA_DORD(0x0)    // MSB first
                           | SERCOM_USART_INT_CTRLA_CMODE(0x0) // Async
                           | SERCOM_USART_INT_CTRLA_FORM(0x0)  // UART no parity
                           | SERCOM_USART_INT_CTRLA_RXPO(0x3)  // RS485 pinout
                           | SERCOM_USART_INT_CTRLA_TXPO(0x3)
                           | SERCOM_USART_INT_CTRLA_SAMPR(0x0)
                           | SERCOM_USART_INT_CTRLA_RXINV(0x0) // no inversion
                           | SERCOM_USART_INT_CTRLA_TXINV(0x0)
                           | SERCOM_USART_INT_CTRLA_RUNSTDBY(0x0)
                           | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
                           | SERCOM_USART_INT_CTRLA_ENABLE(0x0);
    sercom->inst->USART_INT.SERCOM_CTRLA = ctrla;
    sercom->inst->USART_INT.SERCOM_CTRLB = 0x0; // use defaults
    while (_FLD2VAL(SERCOM_USART_INT_SYNCBUSY_CTRLB,
                    sercom->inst->USART_INT.SERCOM_SYNCBUSY));
    sercom->inst->USART_INT.SERCOM_CTRLC = 0x0; // use defaults

    const uint16_t baud_reg = 65536 * (1 - 16 * baud / SERIAL_LS_CLOCK);
    sercom->inst->USART_INT.SERCOM_BAUD = baud_reg;

    _SET_BIT(sercom->inst->USART_INT.SERCOM_CTRLA, SERCOM_USART_INT_CTRLA_ENABLE_Pos);
    while (_FLD2VAL(SERCOM_USART_INT_SYNCBUSY_ENABLE,
                    sercom->inst->USART_INT.SERCOM_SYNCBUSY));
    sercom->enabled = true;
}
