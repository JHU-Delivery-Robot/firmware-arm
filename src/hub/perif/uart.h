#ifndef UART_H_
#define UART_H_

#include "sercom.h"

/**
 * Initialize SERCOM in uart mode for communications
 * Prereqs:
 *   - UART subsystem 8Mhz clock synthesized
 *   - Clock initialization completed
 *   - DMA initialized completed?
 * Side-effects:
 *   - UART peripheral enabled and ready for use
 * @param sercom sercom object representing sercom to initialize
 */
void uart_init(Sercom_t *sercom, const unsigned int baud);

#endif /* UART_H_ */
