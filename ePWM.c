//************************************************************************
//
// FILE:   ePWM.h
//
// TITLE:  header file for the PWM module to generate a periodic squarewave
// for controller speed of a DC motor
//
// using function from ti library for the ePWM module
//
//
//************************************************************************
#include "ePWM.h"
#include "driverlib.h"
#include "device.h"
//*************************************************************************
// Function: ePWMinit()
// - initialize the ePWM1 module, default at 0% DC
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 8th, 2023
// Modified: April 8th, 2023
//************************************************************************
void ePWM1init()
{
    //up count mode, timer base clk running at 3125000
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_16,
                           EPWM_CLOCK_DIVIDER_1);

    //initialize count to 0
    EPWM_setTimeBaseCounter(EPWM1_BASE, INITIALIZE_COUNT);

    //Set when count up to PRD and reset at CMPA
    EPWM_setActionQualifierActionComplete(
            EPWM1_BASE, EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_HIGH_PERIOD | EPWM_AQ_OUTPUT_LOW_UP_CMPA);

    //set to running at 10kHz
    EPWM_setTimeBasePeriod(EPWM1_BASE, TIMER_PRD - 1);

    //default to 0%
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A,
    DUTY_0 * TIMER_PRD);

    //set up GPIO
    ePWM1gpioInit();
}

//*************************************************************************
// Function: ePWM2init()
// - initialize the ePWM2 module, default at 0% DC
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void ePWM2init()
{
    //up count mode, timer base clk running at 3125000
    EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setClockPrescaler(EPWM2_BASE, EPWM_CLOCK_DIVIDER_16,
                           EPWM_CLOCK_DIVIDER_1);
    //set counter to 0 upon initialize
    EPwm2Regs.TBCTR = INITIALIZE_COUNT;

    //Set when count up to PRD and reset at CMPA
    EPWM_setActionQualifierActionComplete(
            EPWM2_BASE, EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_HIGH_PERIOD | EPWM_AQ_OUTPUT_LOW_UP_CMPA);

    //set to running at 10kHz
    EPWM_setTimeBasePeriod(EPWM2_BASE, TIMER_PRD - 1);

    //default at 0% DC
    EPWM_setCounterCompareValue(EPWM2_BASE, EPWM_COUNTER_COMPARE_A,
    DUTY_0 * TIMER_PRD);

    //setting up GPIO
    ePWM2gpioInit();
}

//*************************************************************************
// Function: ePWM1gpioInit()
// - initialize the GPIO for ePWM1A, ePWM1A at GPIO0
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 8th, 2023
// Modified: April 8th, 2023
//************************************************************************
void ePWM1gpioInit()
{
    //ePWM1A
    GPIO_setMasterCore(EPWM1A, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_0_EPWM1A);
}

//*************************************************************************
// Function: ePWM2gpioInit()
// - initialize the GPIO for ePWM2A, ePWM1A at GPIO2
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void ePWM2gpioInit()
{
    //ePWM1A
    GPIO_setMasterCore(EPWM2A, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_2_EPWM2A);
}

//*************************************************************************
// Function: ePWM1dutyCtl
// - configure the duty cycle for ePWM1, this part drive the x axis motor
//
// Arguments: int16 duty
//
//  /e duty is a whole number represent the duty cycle user want to set the ePWM to
//
// return: the speed it was set to
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
int16 ePWM1dutyCtl(int16 duty)
{
    int16 ret;

    if (duty > SPEED_0)
    {
        //configure as up mode if speed is non 0
        EPwm1Regs.TBCTL.bit.CTRMODE = EPWM_COUNTER_MODE_UP;
        //turn off this functionality
        EPwm1Regs.AQCTLA.bit.ZRO = 0x00;

        EPwm1Regs.CMPA.bit.CMPA = duty * TIMER_PRD/100;
        ret = duty;
    }

    else if (duty == SPEED_0)
    {
        EPwm1Regs.CMPA.bit.CMPA = DUTY_0 * TIMER_PRD;
        //freeze any counter opperation
        EPwm1Regs.TBCTL.bit.CTRMODE = EPWM_COUNTER_MODE_STOP_FREEZE;
        //reset count to 0
        EPwm1Regs.TBCTR = INITIALIZE_COUNT;
        //force o/p to low
        EPwm1Regs.AQCTLA.bit.ZRO = EPWM_AQ_OUTPUT_LOW_ZERO;
        ret = 0;

    }

    return ret;
}

//*************************************************************************
// Function: ePWM2dutyCtl
// - configure the duty cycle for ePWM2, this part drive the y axis motor
//
// Arguments: int16 duty
//
//  /e duty is a whole number represent the duty cycle user want to set the ePWM to
//
// return: none
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
int16 ePWM2dutyCtl(int16 duty)
{
    int16 ret;

    if (duty > SPEED_0)
    {
        //configure as up mode if speed is non 0
        EPwm2Regs.TBCTL.bit.CTRMODE = EPWM_COUNTER_MODE_UP;
        //turn off this functionality
        EPwm2Regs.AQCTLA.bit.ZRO = 0;

        EPwm2Regs.CMPA.bit.CMPA = duty * TIMER_PRD/100;
        ret = duty;
    }

    else if (duty == SPEED_0)
    {
        EPwm2Regs.CMPA.bit.CMPA = DUTY_0 * TIMER_PRD;
        //freeze any counter opperation
        EPwm2Regs.TBCTL.bit.CTRMODE = EPWM_COUNTER_MODE_STOP_FREEZE;
        //reset count to 0
        EPwm2Regs.TBCTR = INITIALIZE_COUNT;
        //force o/p to low
        EPwm2Regs.AQCTLA.bit.ZRO = EPWM_AQ_OUTPUT_LOW_ZERO;
        ret = 0;
    }

    return ret;
}
