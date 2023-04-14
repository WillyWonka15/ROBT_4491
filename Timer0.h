//************************************************************************
//
// FILE:   Timer0.c
//
// TITLE:  source file for timer 0 to generate an interrupt of 60Hz to read
// pixy cam blocks data
//
//
//
//************************************************************************
#ifndef TIMER0_H_
#define TIMER0_H_

#include "F2837xD_device.h"
#include "driverlib.h"
#include "device.h"
//************************************************************************
//PRE-DEFINE CONSTANT
#define ENABLE_INTERRUPT 1
#define T0_INT C28X_BIT3
//************************************************************************
//TIME0 FREQUENCY
#define SYS_CLK_FREQ 100000000
#define T0_FREQ 40
//************************************************************************
//MACROS
#define CLEAR_T0_INT_FLAG CpuTimer0Regs.TCR.bit.TIF = 1
//************************************************************************
//FUNCTION PROTOTYPE
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
void timer0Init(void);

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
__interrupt void timer0ISR(void);


#endif /* TIMER0_H_ */
