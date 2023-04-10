//************************************************************************
//
// FILE:   encoderDataCapture.h
//
// TITLE:  source file for encoder reading
//
// using function from ti library for the eQEP module
//
//
//************************************************************************
#include "encoderDataCapture.h"
//*************************************************************************

//*************************************************************************
// Function: eQEP1init
// - initialize the eQEP1 module for measuring speed of a DC motor by Pulse Accumulation
// Arguments: int16_t enableInt
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//************************************************************************
void eQEP1init(int16_t enableInt)
{
    //disable the module for initialize
    EQEP_disableModule(EQEP1_BASE);

    //watch dog disable
    EQEP_disableWatchdog(EQEP1_BASE);

    //configure position counter to reset on a unit timeout event
    EQEP_setPositionCounterConfig(EQEP1_BASE, EQEP_POSITION_RESET_MAX_POS,
    ENCODER_MAX_COUNT_QUAD);

    //quadrature count mode
    EQEP_setDecoderConfig(EQEP1_BASE, EQEP_CONFIG_QUADRATURE);

    //zero the position upon a SW initialize
    EQEP_setInitialPosition(EQEP1_BASE, 0);
    EQEP_setSWPositionInit(EQEP1_BASE, 1);
    EQEP_setSWPositionInit(EQEP1_BASE, 0);   //clear SW init bit

    //position latch on unit time out event
    EQEP_setLatchMode(EQEP1_BASE, EQEP_LATCH_UNIT_TIME_OUT);

    //enable unit timer, sampling every 50khz (test this)
    EQEP_enableUnitTimer(EQEP1_BASE, UNIT_TIMER_PERIOD);

    //emulation stop stop immediately mode
    EQEP_setEmulationMode(EQEP1_BASE, EQEP_EMULATIONMODE_STOPIMMEDIATELY);

    //pin configuration for eQEP1
    eQEP1GPIOinit();

    //enable interrupt for a unit timeout event
    EQEP_enableInterrupt(EQEP1_BASE, EQEP_INT_UNIT_TIME_OUT | EQEP_INT_UNDERFLOW | EQEP_INT_OVERFLOW);

    //enable the module
    EQEP_enableModule(EQEP1_BASE);

    //enable interrupt
    if (enableInt == ENABLE_INTERRUPT)
    {
        //map interrupt vector to correct ISR
        eQEP1intInit();
        //enable interrupt for the eQEP1 module
        Interrupt_enable(INT_EQEP1);
    }
}

//*************************************************************************
// Function: eQEP1GPIOinit
// - initialize GPIO pin for eQEP1 module, using 3.3v signal pins
// Arguments:none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
void eQEP1GPIOinit(void)
{
    //eQEP1A config
    GPIO_setMasterCore(EQEP1_GPIO_A, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_10_EQEP1A);
    //set pin as i/p
    GPIO_setDirectionMode(EQEP1_GPIO_A, GPIO_DIR_MODE_IN);
    //synchronize to clock
    GPIO_setQualificationMode(EQEP1_GPIO_A, GPIO_QUAL_SYNC);

    //eQEP1B config
    GPIO_setMasterCore(EQEP1_GPIO_B, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_11_EQEP1B);
    //set pin as i/p
    GPIO_setDirectionMode(EQEP1_GPIO_B, GPIO_DIR_MODE_IN);
    //synchronize i/p to clock
    GPIO_setQualificationMode(EQEP1_GPIO_B, GPIO_QUAL_SYNC);
}

//*************************************************************************
// Function: encoderDataInit
// -initialize ENC_DAT struct
// Arguments: ENC_DAT *encoder
//
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//*************************************************************************
void encoderDataInit(ENC_DAT *encoder)
{
    //encoder 1
    encoder[INDEX_ENCODER_1].posCnt = 0;
    encoder[INDEX_ENCODER_1].posCurr = 0;
    encoder[INDEX_ENCODER_1].posPrev = 0;
    encoder[INDEX_ENCODER_1].dir = 0;

    //encoder 2
    encoder[INDEX_ENCODER_2].posCnt = 0;
    encoder[INDEX_ENCODER_2].posCurr = 0;
    encoder[INDEX_ENCODER_2].posPrev = 0;
    encoder[INDEX_ENCODER_2].dir = 0;
}

//*************************************************************************
// Function: eQEP1initInit
// - initialize the interrupt for eQEP1
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
void eQEP1intInit(void)
{
    //map interrupt to ISR handler
    Interrupt_register(INT_EQEP1, &eQEP1ISR);
}

//*************************************************************************
// Function: eQEP1getPos
// - this function get read acquired position from counter
// Arguments:
//
// return: the read position
// Author: Will Nguyen
// Date: Mar 19th, 2023
// Modified: Mar 19th, 2023
//*************************************************************************
uint32_t eQEP1getPos(void)
{
    volatile uint32_t pos;

    //get position
    pos = EQep1Regs.QPOSLAT;

    //return the value
    return pos;
}

//*************************************************************************
// Function: eQEP1getPos
// - this function calculating speed from accumulated position count
// Arguments:
//
// return: the measured speed
// Author: Will Nguyen
// Date: Mar 30th, 2023
// Modified: Mar 30th, 2023
//*************************************************************************
float32 eQEP1speedCalc(int32 pos)
{
    float32 vel = 0;
    vel = (pos * 2 * PI * SAMPLING_FREQ) / (ENCODER_MAX_COUNT_QUAD);

    return vel;
}



/*
// Function: eQEP1ISR
// - interrupt handler for eQEP1 module
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
__interrupt void eQEP1ISR(void)
{

    //calculating position count
    //check for overflow flag and change calculation
    encoder[INDEX_ENCODER_1].posPrev = encoder[INDEX_ENCODER_1].posCurr;
    encoder[INDEX_ENCODER_1].posCurr = (int32_t)EQep1Regs.QPOSLAT;
    encoder[INDEX_ENCODER_1].posCnt = abs(encoder[INDEX_ENCODER_1].posCurr - encoder[INDEX_ENCODER_1].posPrev);
    encoder[INDEX_ENCODER_1].dir = EQep1Regs.QEPSTS.bit.QDF;

    //if there is a unit time out interrupt
    if(EQep1Regs.QFLG.bit.UTO == 1)
    {
        babyDinnerIsReady |= DINNER;
    }

    //clear interrupt flag
    EQep1Regs.QCLR.bit.UTO = 1;
    EQep1Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}*/
