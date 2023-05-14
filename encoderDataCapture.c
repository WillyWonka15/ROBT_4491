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
#include "driverlib.h"
#include "device.h"
//*************************************************************************
// Function: eQEP1init
// - initialize the eQEP1 module for measuring the count of an encoder per unit time
// out interval, eQEP1 is dedicate for x-axis motor
//
// Arguments: int16_t enableInt
//
//  /e ENBALE_INTERRUPT will enable unit time out, counter overflow, under flow
//  interrupt
//
//  /e DISABLE_INTERRUPT will disable interrupt functionality
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
                                  MAX_COUNT);

    //quadrature count mode
    EQEP_setDecoderConfig(EQEP1_BASE, EQEP_CONFIG_QUADRATURE);

    //zero the position upon a SW initialize
    EQEP_setInitialPosition(EQEP1_BASE, 0);
    EQEP_setSWPositionInit(EQEP1_BASE, 1);
    EQEP_setSWPositionInit(EQEP1_BASE, 0);   //clear SW init bit

    //pin configuration for eQEP1
    eQEP1GPIOinit();

    //position latch on unit time out event
    EQEP_setLatchMode(EQEP1_BASE, EQEP_LATCH_UNIT_TIME_OUT);

    //enable unit timer
    EQEP_enableUnitTimer(EQEP1_BASE, UNIT_TIMER_PERIOD);

    //enable interrupt for a unit timeout event
    EQEP_enableInterrupt(EQEP1_BASE,
                         EQEP_INT_UNIT_TIME_OUT | EQEP_INT_UNDERFLOW);

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
    else if (enableInt == DISABLE_INTERRUPT)
    {
        //disable interrupt for the eQEP1 module
        Interrupt_disable(INT_EQEP1);
    }
}

//*************************************************************************
// Function: eQEP3init
// - initialize the eQEP3 module for measuring the count of an encoder per unit time
// out interval, eQEP1 is dedicate for x-axis motor
//
// Arguments: int16_t enableInt
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: May 3rd, 2023
//************************************************************************
void eQEP3init(int16 enableInt)
{
    //disable the module for initialize
    EQEP_disableModule(EQEP3_BASE);

    //watch dog disable
    EQEP_disableWatchdog(EQEP3_BASE);

    //configure position counter to reset on a unit timeout event
    EQEP_setPositionCounterConfig(EQEP3_BASE, EQEP_POSITION_RESET_MAX_POS,
                                  MAX_COUNT);

    //quadrature count mode
    EQEP_setDecoderConfig(EQEP3_BASE, EQEP_CONFIG_QUADRATURE);

    //zero the position upon a SW initialize
    EQEP_setInitialPosition(EQEP3_BASE, 0);
    EQEP_setSWPositionInit(EQEP3_BASE, 1);
    EQEP_setSWPositionInit(EQEP3_BASE, 0);   //clear SW init bit

    //pin configuration for eQEP1
    eQEP3GPIOinit();

    //position latch on unit time out event
    EQEP_setLatchMode(EQEP3_BASE, EQEP_LATCH_UNIT_TIME_OUT);

    //enable unit timer
    EQEP_enableUnitTimer(EQEP3_BASE, UNIT_TIMER_PERIOD);

    //enable interrupt for a unit timeout event
    EQEP_enableInterrupt(EQEP3_BASE,
                         EQEP_INT_UNIT_TIME_OUT | EQEP_INT_UNDERFLOW);

    //enable the module
    EQEP_enableModule(EQEP3_BASE);

    //enable interrupt
    if (enableInt == ENABLE_INTERRUPT)
    {
        //map interrupt vector to correct ISR
        eQEP3intInit();
        //enable interrupt for the eQEP1 module
        Interrupt_enable(INT_EQEP3);
    }
    else if (enableInt == DISABLE_INTERRUPT)
    {
        //disable interrupt for the eQEP1 module
        Interrupt_disable(INT_EQEP3);
    }
}

//*************************************************************************
// Function: eQEP1GPIOinit
// - initialize GPIO pin for eQEP1 module
// GPIO 10 is dedicate for channel A
// GPIO 11 is dedicate for channel B
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
// Function: eQEP3GPIOinit
// - initialize GPIO pin for eQEP3 module
// GPIO 6 is dedicate for channel A
// GPIO 7 is dedicate for channel B
// Arguments:none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: May 3rd, 2023
//*************************************************************************
void eQEP3GPIOinit(void)
{
    //eQEP3A config
    GPIO_setMasterCore(EQEP3_GPIO_A, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_6_EQEP3A);
    //set pin as i/p
    GPIO_setDirectionMode(EQEP3_GPIO_A, GPIO_DIR_MODE_IN);
    //synchronize to clock
    GPIO_setQualificationMode(EQEP3_GPIO_A, GPIO_QUAL_SYNC);

    //eQEP3B config
    GPIO_setMasterCore(EQEP3_GPIO_B, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_7_EQEP3B);
    //set pin as i/p
    GPIO_setDirectionMode(EQEP3_GPIO_B, GPIO_DIR_MODE_IN);
    //synchronize i/p to clock
    GPIO_setQualificationMode(EQEP3_GPIO_B, GPIO_QUAL_SYNC);
}

//*************************************************************************
// Function: encoderDataInit
// -initialize ENC_DAT and COORD structure
// Arguments: ENC_DAT *encoder COORD *coord
//
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//*************************************************************************
void encoderDataInit(ENC_DAT *encoder)
{
    //encoder x
    encoder[INDEX_ENCODER_X].posCnt = 0;
    encoder[INDEX_ENCODER_X].prePosCnt = 0;
    encoder[INDEX_ENCODER_X].posCurr = 0;
    encoder[INDEX_ENCODER_X].dir = 0;
    encoder[INDEX_ENCODER_X].posDesire = 0;
    encoder[INDEX_ENCODER_X].displacement = 0;
    encoder[INDEX_ENCODER_X].pastSpeed = 0;

    //encoder y
    encoder[INDEX_ENCODER_Y].posCnt = 0;
    encoder[INDEX_ENCODER_Y].prePosCnt = 0;
    encoder[INDEX_ENCODER_Y].posCurr = 0;
    encoder[INDEX_ENCODER_Y].dir = 0;
    encoder[INDEX_ENCODER_Y].posDesire = 0;
    encoder[INDEX_ENCODER_Y].displacement = 0;
    encoder[INDEX_ENCODER_Y].pastSpeed = 0;
}

//*************************************************************************
// Function: eQEP1initInit
// - initialize the interrupt for eQEP1
//  unit time out interrupt
//  position counter overflow interrupt
//  position counter underflow interrupt
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
// Function: eQEP3initInit
// - initialize the interrupt for eQEP3
//  unit time out interrupt
//  position counter overflow interrupt
//  position counter underflow interrupt
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: May 3rd, 2023
//*************************************************************************
void eQEP3intInit(void)
{
    //map interrupt to ISR handler
    Interrupt_register(INT_EQEP3, &eQEP3ISR);
}

//*************************************************************************
// Function: eQEP1ISR
// - interrupt handler for eQEP1 module
//  this ISR set a flag whenever there is a UNIT TIME OUT EVENT, COUNTER OVERFLOW/
//  UNDERFLOW. The math for finding posCnt change base on the flag.
//
// Arguments: none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
/*__interrupt void eQEP1ISR(void)
 {
if (EQep1Regs.QFLG.bit.PCU == 1)
    {
        babyItsReady |= UNDERFLOW;
        //clear interrupt flag
        EQep1Regs.QCLR.bit.PCU = 1;
    }

    //if there is a unit time out interrupt
    if (EQep1Regs.QFLG.bit.UTO == 1)
    {
        //update direction every time out event, is it good enough?
        encoderData[INDEX_ENCODER_X].dir = EQep1Regs.QEPSTS.bit.QDF;

        //if pos cnt not underflowing
        encoderData[INDEX_ENCODER_X].posCnt = EQep1Regs.QPOSLAT;

        //if pos cnt underflowing
        if (babyItsReady & UNDERFLOW)
        {
            encoderData[INDEX_ENCODER_X].posCnt = MAX_COUNT - EQep1Regs.QPOSLAT;
            //babyItsReady &= ~UNDERFLOW;
        }

        //clear interrupt flag
        EQep1Regs.QCLR.bit.UTO = 1;
    }

    //calculate displacement and update position
    encoderData[INDEX_ENCODER_X].posCurr = LEAD_SCREW
            * encoderData[INDEX_ENCODER_X].posCnt / ENCODER_MAX_COUNT_QUAD;

    //clear general interrupt flag
    EQep1Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
 }*/

//*************************************************************************
// Function: eQEP3ISR
// - interrupt handler for eQEP3 module
//  this ISR set a flag whenever there is a UNIT TIME OUT EVENT, COUNTER OVERFLOW/
//  UNDERFLOW. The math for finding posCnt change base on the flag.
//
// Arguments: none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: May 3rd, 2023
//*************************************************************************
/*__interrupt void eQEP3ISR(void)
 {
if (EQep1Regs.QFLG.bit.PCU == 1)
    {
        babyItsReady |= EQEP3_UNDERFLOW;
        //clear interrupt flag
        EQep3Regs.QCLR.bit.PCU = 1;
    }

    //if there is a unit time out interrupt
    if (EQep3Regs.QFLG.bit.UTO == 1)
    {
        //update direction every time out event, is it good enough?
        encoderData[INDEX_ENCODER_Y].dir = EQep3Regs.QEPSTS.bit.QDF;

        //if pos cnt not underflowing
        encoderData[INDEX_ENCODER_3].posCnt = EQep3Regs.QPOSLAT;

        //if pos cnt underflowing
        if (babyItsReady & UNDERFLOW)
        {
            encoderData[INDEX_ENCODER_Y].posCnt = MAX_COUNT - EQep3Regs.QPOSLAT;
            //babyItsReady &= ~UNDERFLOW;
        }

        //clear interrupt flag
        EQep3Regs.QCLR.bit.UTO = 1;
    }

    //calculate displacement and update position
    encoderData[INDEX_ENCODER_Y].posCurr = LEAD_SCREW
            * encoderData[INDEX_ENCODER_Y].posCnt / ENCODER_MAX_COUNT_QUAD;

    //clear general interrupt flag
    EQep3Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
 }*/
