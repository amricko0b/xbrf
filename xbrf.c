#include "xbrf.h"

/********************************************************************************
 *  Global variables definition
 ********************************************************************************/

/**
 *  Tracks RF module current state
 */ 
static volatile RFM_State_Def  RFM_State   =   {0x0, 0x0};

/**
 *  Tracks RF module current state
 */ 
static volatile FMW_State_Def  FMW_State   =   {0x0, 0x0, 0x0};


int main() {

    /**
     *  Enable Pull-Up resistor for input pins
     */ 
    PORTB |= _BV(SIGNAL_DAT_P_OFF);
    PORTB |= _BV(SIGNAL_SYN_P_OFF);
    PORTB |= _BV(SIGNAL_CLK_P_OFF);

    /**
     *  Wait some time after power-up until RM module fully intializes
     */ 
    _delay_ms(1000);

    /**
     *  Enable interrupt by SYN signal.
     *  If button pressed, we will just remember asking for synchronization routine
     */ 
    GIMSK |= _BV(INT0);

    /**
     * Enable interrupts globally
     */
    sei();

    while (1) {

        /**
         *  If RF module onboard LED have not been enabled yeat (LEDI command was not sent) 
         */ 
        if (!RFM_State.LEDI_Perfomed) {

            /**
             *  Activate LED, remember about it and wait a for a while
             */ 
            CMD_Transfer(CMD_SEQ_LEDI);
            RFM_State.LEDI_Perfomed = 0x1;
            _delay_ms(100);
        }

        /**
         *  If RF module animation have not been played yet (ANIM command was not sent)
         */ 
        if (!RFM_State.ANIM_Perfomed) {

            /**
             *  Play animation, remember about it and wait a for a while
             */ 
            CMD_Transfer(CMD_SEQ_ANIM);
            RFM_State.LEDI_Perfomed = 0x1;
            _delay_ms(100);
        }

        /**
         *  If synchronization routine asked (SYN signal received)
         */ 
        if (FMW_State.SYNR_Asked) {

            /**
             *  Start routine and drop flag
             */ 
            CMD_Transfer(CMD_SEQ_SYNR);
            FMW_State.SYNR_Asked = 0x0;
        }
    }
}

/**
 *  External interrupt fired by LOW level of SYN signal.
 * 
 *  When the only button on RF module is pressed, SYN signal is pulled to low.
 *  That means, that gamepad synchronization routine must be provided.
 * 
 *  In order to keep handling routine short, we just rasing suitable flag here. 
 */
ISR(INT0_vect) {
    FMW_State.SYNR_Asked = 0x1;
}

/**
 *  External interrupt fired by CLK signal change.
 *  Used by firmware for bit-by-bit command transfer to RF module.
 *  
 *  Sends next bit by falling edge, increments bit counter by rising edge.
 * 
 *  RF module acts like clock source.
 */ 
ISR(PCINT0_vect) {

    /**
     *  Other bit must bit sent by falling edge of CLK.
     *  Therefore, we act only by LOW level
     */ 
    if (!(PINB & _BV(SIGNAL_CLK_P_OFF))) {

        /**
         *  Take next bit and set suitable logic level of DAT signal
         */ 
        if (FMW_State.CMD_Sequence & _BV(FMW_State.CMD_BitCounter)) {
            PORTB |= _BV(SIGNAL_DAT_P_OFF);
        } else {
            PORTB &= ~_BV(SIGNAL_DAT_P_OFF);
        }

        /**
         *  Increase bit counter in order to sent next bit on next interrupt
         */ 
        FMW_State.CMD_BitCounter++;
    }
}

/**
 *  Command sending subprogram.
 * 
 *  Prepares command for transfer and initiates transfer.
 *  Bit-by-bit sending provided via the interrupt on CLK signal.
 *  Blocks until command is totally sent - that sucks, but makes transfer reliable.
 */ 
void CMD_Transfer(uint16_t __cmd) {

    /**
     *  Disable interrupt by SYN signal in order to focus on transfer 
     */ 
    GIMSK &= ~_BV(INT0);

    /**
     *  Prepare command for transfer
     */
    FMW_State.CMD_Sequence      = __cmd;
    FMW_State.CMD_BitCounter    = 0x0;

    /**
     *  Enable interrupts by CLK signal.
     *  Do this before actual transfer start, in order not to miss CLK signal partially.
     */ 
    PCMSK |= _BV(PCINT0);
    GIMSK |= _BV(PCIE);

    /**
     *  Start transfer by pulling DAT signal to LOW
     */ 
    DDRB    |= _BV(SIGNAL_DAT_P_OFF);
    PORTB   &= ~_BV(SIGNAL_DAT_P_OFF);

    /**
     *  Wait until bit counter overflows by 10.
     *  That means - transfer is over.
     */  
    while (FMW_State.CMD_BitCounter < 10) {}

    /**
     *  Disable interrupts by CLK signal.
     */ 
    GIMSK &= ~_BV(PCIE);
    PCMSK &= ~_BV(PCINT0);

    /**
     *  Enable interrupt by SYN signal back
     */ 
    GIMSK |= _BV(INT0);
}