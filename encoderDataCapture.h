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
#ifndef ENCODERDATACAPTURE_H_
#define ENCODERDATACAPTURE_H_

#include <stdint.h>
#include "F2837xD_device.h"
#include "driverlib.h"
#include "device.h"
#include <math.h>
//************************************************************************
//SYS_CLK
#define SYS_CLK 100000000U
#define SYS_CLK_PERIOD 1/SYS_CLK
//************************************************************************
//DESIRE SAMPLING FREQUENCY
#define SAMPLING_FREQ 240U
#define SAMPLING_PERIOD 1/SAMPLING_FREQ
//************************************************************************
//TIMING SETTING FOR DATA GATHERING
#define UNIT_TIMER_PERIOD SYS_CLK/SAMPLING_FREQ
//************************************************************************
//INTERRUPT HANDLING BIT
#define DINNER C28X_BIT0
#define OVERFLOW C28X_BIT1
#define UNDERFLOW C28X_BIT2
//************************************************************************
//INTERRUPT
#define ENABLE_INTERRUPT 1
#define DISABLE_INTERRUPT 0
//************************************************************************
//ENCODER DATA
#define ENCODER_MAX_COUNT_NON_QUAD 700U
#define ENCODER_MAX_COUNT_QUAD 2800U
//************************************************************************
//DIRECTION
#define DIR_CW 1
#define DIR_CCW 0
//************************************************************************
//PRE-DEFINE CONSTANT
#define PI 3.1416
#define FREQ 120U
//************************************************************************
//eQEP pin-out
#define EQEP1_GPIO_A 10
#define EQEP1_GPIO_B 11
//************************************************************************
//STRUCTURE
typedef struct ENC_DAT
{
     volatile int32 posCurr; //store current count
     volatile int32 posPrev; //store previous count
     volatile int32 posCnt;  //store the count elapse over every Ttout
     volatile int16 dir; //store direction of flywheel
}ENC_DAT;

enum ENC_LIST_INDEX
{
    INDEX_ENCODER_1, INDEX_ENCODER_2, NUM_ENCODER
};

//GLOBAL VAR
//volatile unsigned char babyItsReady; bring this to the client you wish to use it in
//************************************************************************
//FUNCTION PROTOTYPE
//*************************************************************************
// Function: eQEP1init
// -initialize the eQEP1 module for measuring speed of a DC motor by Pulse Accumulation
// Arguments: int16_t enableInt
//
// user choice to enable velocity measurement functionality
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
void eQEP1init(int16 enableInt);

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
void encoderDataInit(ENC_DAT *encoder);

//*************************************************************************
// Function: eQEP1GPIOinit
// - initialize GPIO pin for eQEP1 module
// Arguments:none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
void eQEP1GPIOinit(void);

//*************************************************************************
// Function: eQEP1velocityMeasInit
// - initialize the eQEP1 fucntion to measure velocity
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
void eQEP1velocityMeasInit(void);

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
void eQEP1intInit(void);

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
Uint32 eQEP1getPos(void);

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
float32 eQEP1speedCalc(int32 pos);

//*************************************************************************
// Function: eQEP1ISR
// - interrupt handler for eQEP1 unit timeout event
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
__interrupt void eQEP1ISR(void);

#endif /* ENCODERDATACAPTURE_H_ */
