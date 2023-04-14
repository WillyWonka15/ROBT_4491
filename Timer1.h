//************************************************************************
//
// FILE:   Timer0.c
//
// TITLE:  header file for timer 1 to generate a 60Hz interrupt
//
//
//
//************************************************************************
#ifndef TIMER1_H_
#define TIMER1_H_

#include "F2837xD_device.h"
#include "driverlib.h"
#include "device.h"

//************************************************************************
//PRE-DEFINE CONSTANT
#define ENABLE_INTERRUPT 1
#define T1_INT C28X_BIT4
//************************************************************************
//TIME0 FREQUENCY
#define SYS_CLK_FREQ 100000000
#define T1_FREQ 40
//************************************************************************
//MACROS
#define CLEAR_T1_INT_FLAG CpuTimer1Regs.TCR.bit.TIF = 1
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
void timer1Init(void);

//*************************************************************************
// Function: timer0ISR
// - interrupt handler for timer 1 interrupt event
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//*************************************************************************
__interrupt void timer1ISR(void);


#endif /* TIMER1_H_ */
