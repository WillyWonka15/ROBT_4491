//************************************************************************
//
// FILE:   Timer1.c
//
// TITLE:  source file for timer 1 to generate a 60Hz interrupt
//
//
//
//************************************************************************
#include "Timer1.h"
#include "driverlib.h"
#include "device.h"
//************************************************************************
//FUNCTION PROTOTYPE
//*************************************************************************
// Function: timer1Init
// - initialize the timer1 to run at 60Hz
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
void timer1Init(void)
{
    int16 *temp;
    int32 period = SYS_CLK_FREQ / T1_FREQ;
    temp = &period;

    //configure period
    CpuTimer1Regs.PRD.bit.LSW = *temp;
    temp++;
    CpuTimer1Regs.PRD.bit.MSW = *temp;

    //initialize count
     CpuTimer1Regs.TIM.bit.LSW = 0;
     CpuTimer1Regs.TIM.bit.MSW = 0;

    //map interrupt handler to PIE table
    Interrupt_register(INT_TIMER1, &timer1ISR);

    //enable interrupt
    CpuTimer1Regs.TCR.bit.TIE = ENABLE_INTERRUPT;

    //enable interrupt in PIE block
    Interrupt_enable(INT_TIMER1);

    //enable CPU interrupt
    Interrupt_enableInCPU(INTERRUPT_CPU_INT13);


}
