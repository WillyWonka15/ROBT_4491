//************************************************************************
//
// FILE:   Timer0.c
//
// TITLE:  source file for timer 0 to generate an interrupt of 60Hz to read
// pixy cam data
//
//
//
//************************************************************************
#include "Timer0.h"

//*************************************************************************
// Function: timer0Init
// - initialize the timer0 to run at 60Hz
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 6th, 2023
// Modified: April 6th, 2023
//************************************************************************
void timer0Init(void)
{
    int16 *temp;
    int32 period = SYS_CLK_FREQ/T0_FREQ;
    temp = &period;

    //configure period
    CpuTimer0Regs.PRD.bit.LSW = *temp;
    temp++;
    CpuTimer0Regs.PRD.bit.MSW = *temp;

    //enable interrupt
    CpuTimer0Regs.TCR.bit.TIE = ENABLE_INTERRUPT;

    //map interrupt handler to PIE table
    Interrupt_register(INT_TIMER0, &timer0ISR);

    //enable interrupt in PIE block
    Interrupt_enable(INT_TIMER0);
}

//*************************************************************************
// Function: timer0ISR
// - interrupt handler for timer 0 interrupt event
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 6th, 2023
// Modified: April 6th, 2023
//*************************************************************************
/*__interrupt void timer0ISR(void)
{
     //set flag to read data from pixy Cam
    babyItsReady |= T0_INT;

    //clear flag
    CLEAR_INT_FLAG;

    //test
    //GpioDataRegs.GPATOGGLE.bit.GPIO15 = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

}*/
