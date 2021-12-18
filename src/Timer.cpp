
#include "Timer.h"  

// 

// ===== Timer list =====
// ISR/IRQ	TC	Channel	Due pins
// TC0	TC0	0	2, 13
// TC1	TC0	1	60, 61
// TC2	TC0	2	58
// TC3	TC1	0	none
// TC4	TC1	1	none
// TC5	TC1	2	none
// TC6	TC2	0	4, 5
// TC7	TC2	1	3, 10
// TC8	TC2	2	11, 12

// TC3_HANDLER() located in pressure sensor class



/**
 * @brief Timer interrupt
 * 
 * @param tc the desired timer counter instance
 * @param _channel The channel of tc
 * @param _irq corresponding NVIC (nested vector interrupt controller)
 * @param _frequency frequency. 4 = 1 second.
 */
void timerStart(struct Timer pTimer) {
    // Tc *tc, uint32_t _channel, IRQn_Type _irq, uint32_t _frequency
    Tc *tc = pTimer.timer_clock;
    uint32_t _channel = pTimer.channel;
    IRQn_Type _irq = pTimer.irq;
    uint32_t _frequency = pTimer.frequency;

    pmc_set_writeprotect(false);
    pmc_enable_periph_clk((uint32_t)_irq);
    TC_Configure(tc, _channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    uint32_t rc = VARIANT_MCK/32/_frequency; //32 to have a resolution a frequ min of 4 sec
    TC_SetRA(tc, _channel, rc/2); //50% high, 50% low
    TC_SetRC(tc, _channel, rc);
    TC_Start(tc, _channel);
    tc->TC_CHANNEL[_channel].TC_IER=TC_IER_CPCS;
    tc->TC_CHANNEL[_channel].TC_IDR=~TC_IER_CPCS;
    NVIC_EnableIRQ(_irq);
}

void timerStop(struct Timer pTimer){
    Tc *tc = pTimer.timer_clock;
    uint32_t _channel = pTimer.channel;
    IRQn_Type _irq = pTimer.irq;

    pmc_disable_periph_clk((uint32_t)_irq);
    NVIC_DisableIRQ(_irq);
}
