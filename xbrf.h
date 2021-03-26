#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/********************************************************************************
 *  RF module commands definitions.
 * 
 *  Commands are used to ask RF module for routines.
 *  They are stored as hexadecimal numbers and sent bit-by bit.
 ********************************************************************************/

/**
 *  Command sequence for RF module onboard LED initialization
 */ 
#define CMD_SEQ_LEDI 0x84

/**
 *  Command sequence for RF module onboard LED animation play
 */ 
#define CMD_SEQ_ANIM 0x284

/**
 *  Command sequence for gamepad synchronization routine start
 */ 
#define CMD_SEQ_SYNR 0x80


/********************************************************************************
 *  IO pin offsets definition.
 * 
 *  Use them to controll IO pins. Thanks to offsets equality in IO registers.
 *  This way is considered much more readable then PORTBx and DDRBx mess.
 ********************************************************************************/

/**
 *  DAT signal pin offset.
 */ 
#define SIGNAL_DAT_P_OFF 4

/**
 *  SYN signal pin offset.
 */ 
#define SIGNAL_SYN_P_OFF 1

/**
 *  CLK signal pin offset.
 */ 
#define SIGNAL_CLK_P_OFF 0

/********************************************************************************
 *  Type definitions
 ********************************************************************************/

/**
 *  Defines RF module current state
 */ 
typedef struct {

    /**
     *  Have RF module onboard LED initialization been performed?
     */ 
    uint8_t LEDI_Perfomed  : 1;

    /**
     *  Have RF module onboard LED animation play been performed?
     */ 
    uint8_t ANIM_Perfomed  : 1;
} RFM_State_Def;

/**
 *  Defines firmware current state
 */ 
typedef struct {

    /**
     *  Have gamepad synchronization routine start been asked?
     *  If true, SYN signal was recieved.
     */ 
    uint8_t     SYNR_Asked      : 1;

    /**
     *  Current command bit counter.
     *  Indicates bit of command to be sent next.
     */ 
    uint8_t     CMD_BitCounter  : 4;

    /**
     * Current command in transfer.
     */ 
    uint16_t    CMD_Sequence    : 10;
} FMW_State_Def;

/********************************************************************************
 *  Function prototypes definition
 ********************************************************************************/

/**
 *  Command sending subprogram.
 */ 
extern void CMD_Transfer(uint16_t);