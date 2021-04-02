#ifndef CLOCK_H_
#define CLOCK_H_

/**
 * Target System clock (Hz) in normal operation
 */
#define SYSTEM_TARGET_CLOCK 120000000
#define XOSC32K_FREQ 32768
#define SERIAL_HS_CLOCK 60000000
#define SERIAL_LS_CLOCK  8000000

/**
 * Prereqs:
 *   - Default system clock config
 *     - DFLL48M enabled @ 48M
 *     - DFLL40M->GCLK0->System Clocks
 *
 * Configured Clocks:
 *   - GCLK0: 120Mhz
 *   - GCLK2: 32.768khz
 *   - GCLK3: 48Mhz
 *   - GCLK4: 60Mhz
 *   - GCLK5: 16Mhz
 *
 * Tasks:
 *   1) Enable XOSC32K
 *   2) XOSC32K->FDPLL200M0->GCLK0 (sysclocks) (~120Mhz)
 *     2a) XOSC32K->FDPLL200M0
 *     2b) Enable FDPLL200M0
 *     3c) FDPLL200M0->GCLK0
 *   3) Update SystemCoreClock
 *   4) XOSC32K->GCLK2->FDFLL48M->GCLK3 (~48Mhz)
 *   5) FDFLL48M->div/6->GCLK5
 *
 *   Configure initial system clocks
 */
void clock_init(void);

#endif /* CLOCK_H_ */
