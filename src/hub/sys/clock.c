#include "clock.h"

#include <stdint.h>
#include "sam.h"
#include "system_samd51.h"

#include "bit_utils.h"
#include "globals.h"

void clock_init() {
    // 1. Configure and enable XOSC32K
    const uint32_t xosc32_ctla = OSC32KCTRL_XOSC32K_CGM_XT                // standard mode
                                 | OSC32KCTRL_XOSC32K_STARTUP_CYCLE16384  // 2^12 cycle startup
                                 | OSC32KCTRL_XOSC32K_ONDEMAND(0x0)       // always run
                                 | OSC32KCTRL_XOSC32K_RUNSTDBY(0x1)       // always run in stndby
                                 | OSC32KCTRL_XOSC32K_EN32K(0x1)          // enable 32kHz output
                                 | OSC32KCTRL_XOSC32K_ENABLE(0x1);        // enable peripheral
    OSC32KCTRL_REGS->OSC32KCTRL_XOSC32K = xosc32_ctla;
    while (!_FLD2VAL(OSC32KCTRL_STATUS_XOSC32KRDY, OSC32KCTRL_REGS->OSC32KCTRL_STATUS));

    // 2. Configure FDPLL200M_0
    // ratio is multiplication factor needed to generate target clock from base
    // clock
    const uint32_t pll0_ratio = OSCCTRL_DPLLRATIO_LDR(SYSTEM_TARGET_CLOCK/XOSC32K_FREQ)
                                | OSCCTRL_DPLLRATIO_LDRFRAC(0); // don't use fractional part
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLRATIO = pll0_ratio;
    while (_FLD2VAL(OSCCTRL_DPLLSYNCBUSY_DPLLRATIO, OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS));

    const uint32_t pll0_ctrlb_val = OSCCTRL_DPLLCTRLB_DIV(0)          // Don't divide down clock
                                    | OSCCTRL_DPLLCTRLB_LTIME_DEFAULT // Don't timeout lock
                                    | OSCCTRL_DPLLCTRLB_REFCLK_XOSC32 // use 32k xtal osc as base
                                    | OSCCTRL_DPLLCTRLB_WUF(0x1);     // Start outputting clock before lock
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLB = pll0_ctrlb_val;

    const uint8_t pll0_ctrla_val = OSCCTRL_DPLLCTRLA_ONDEMAND(0)     // Always run
                                   | OSCCTRL_DPLLCTRLA_RUNSTDBY(0x1) // run in standby
                                   | OSCCTRL_DPLLCTRLA_ENABLE(0x1);  // Enable pll
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLA = pll0_ctrla_val;
    while (_FLD2VAL(OSCCTRL_DPLLSYNCBUSY_ENABLE, OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS));
    while (!_FLD2VAL(OSCCTRL_DPLLSTATUS_LOCK, OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS));

    // 2a.  FDPLL200M_0->GCLK0 (switch over system clock to high speed clock
    // Default value for GCLK0 is 0x106 at reset, which means that the source is
    // DFLL48M and the generator is enabled
    const uint32_t gclk0 = GCLK_GENCTRL_DIV(0x1)     // Don't divide
                           | GCLK_GENCTRL_DIVSEL(0)  // Standard divider mode
                           | GCLK_GENCTRL_OE(0x0)    // Don't output to pin
                           | GCLK_GENCTRL_GENEN(0x1) // enable clock generator
                           | GCLK_GENCTRL_SRC_DPLL0; // Source clock from DPLL200M_0
    GCLK_REGS->GCLK_GENCTRL[0] = gclk0;
    while (GCLK_SYNCBUSY_GENCTRL_GCLK0 & GCLK_REGS->GCLK_SYNCBUSY);

    // 2b  Update SystemCoreClock
    SystemCoreClock = SYSTEM_TARGET_CLOCK / XOSC32K_FREQ;

    // 3. XOSC32K->GCLK2
    const uint32_t gclk2 = GCLK_GENCTRL_DIV(0x1)       // Don't divide
                           | GCLK_GENCTRL_DIVSEL(0)    // Standard divider mode
                           | GCLK_GENCTRL_OE(0x0)      // Don't output to pin
                           | GCLK_GENCTRL_GENEN(0x1)   // enable clock generator
                           | GCLK_GENCTRL_SRC_XOSC32K; // Source clock from DPLL200M_0
    GCLK_REGS->GCLK_GENCTRL[2] = gclk2;
    while (GCLK_SYNCBUSY_GENCTRL_GCLK2 & GCLK_REGS->GCLK_SYNCBUSY);

    // Use DFLL48M to generate stable USB Clock from XOSC32k
    // note that this will create the frequency 48.005Mhz, error +0.01% which is
    // within USB FS spec of +/-0.25%
    _CLR_BIT(OSCCTRL_REGS->OSCCTRL_DFLLCTRLA, OSCCTRL_DFLLCTRLA_ENABLE_Pos);
    while (_FLD2VAL(OSCCTRL_DPLLSYNCBUSY_ENABLE,OSCCTRL_REGS->OSCCTRL_DFLLSYNC));
    // configure DFLL for locked mode
    const uint32_t dfll_mul = OSCCTRL_DFLLMUL_CSTEP(31)      //0.5 full scale val
                              | OSCCTRL_DFLLMUL_FSTEP(32767) // 0.5 full scale val
                              | OSCCTRL_DFLLMUL_MUL(1465);   // 48e6 / XOSC32K_FREQ
    OSCCTRL_REGS->OSCCTRL_DFLLMUL = dfll_mul;
    while (_FLD2VAL(OSCCTRL_DFLLSYNC_DFLLMUL, OSCCTRL_REGS->OSCCTRL_DFLLSYNC));
    const uint8_t dfll_ctrlb = OSCCTRL_DFLLCTRLB_WAITLOCK(0x1)
                               | OSCCTRL_DFLLCTRLB_MODE(0x1);
    // Enable lock and wait for lock
    OSCCTRL_REGS->OSCCTRL_DFLLCTRLB |= dfll_ctrlb;
    while (_FLD2VAL(OSCCTRL_DFLLSYNC_DFLLCTRLB, OSCCTRL_REGS->OSCCTRL_DFLLCTRLB));
    while (!_FLD2VAL(OSCCTRL_STATUS_DFLLRDY, OSCCTRL_REGS->OSCCTRL_STATUS));

    // DFLL48M->GCLK3
    const uint32_t gclk3 = GCLK_GENCTRL_DIV(0x1)     // Don't divide
                           | GCLK_GENCTRL_DIVSEL(0)  // Standard divider mode
                           | GCLK_GENCTRL_OE(0x0)    // Don't output to pin
                           | GCLK_GENCTRL_GENEN(0x1) // enable clock generator
                           | GCLK_GENCTRL_SRC_DFLL;  // Source clock from DFLL48M
    GCLK_REGS->GCLK_GENCTRL[3] = gclk3;
    while (GCLK_SYNCBUSY_GENCTRL_GCLK3 & GCLK_REGS->GCLK_SYNCBUSY);


    // DFLL48M->div6->GCLK5
    const uint32_t gclk5 = GCLK_GENCTRL_DIV(0x6)     // scale to 8Mhz
                           | GCLK_GENCTRL_DIVSEL(0)  // Standard divider mode
                           | GCLK_GENCTRL_OE(0x0)    // Don't output to pin
                           | GCLK_GENCTRL_GENEN(0x1) // enable clock generator
                           | GCLK_GENCTRL_SRC_DFLL;  // Source clock from DFLL48M
    GCLK_REGS->GCLK_GENCTRL[5] = gclk5;
    while (GCLK_SYNCBUSY_GENCTRL_GCLK5 & GCLK_REGS->GCLK_SYNCBUSY);
}
