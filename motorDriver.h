//************************************************************************
//
// FILE:   motorDriver.h
//
// TITLE:  header file for implementing motor driver VNH7070AS
//
//
//
//************************************************************************
#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_
#include "ePWM.h"
//*************************************************************************
//DIRECTION PIN
#define GPIO_X_DIR_CCW 66
#define GPIO_X_DIR_CW 131
#define GPIO_Y_DIR_CCW 63
#define GPIO_Y_DIR_CW 64
//*************************************************************************
//MACRO
#define CCW_DIR_X GpioDataRegs.GPCSET.bit.GPIO66 = 1; GpioDataRegs.GPECLEAR.bit.GPIO131 = 1;
#define CW_DIR_X GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1; GpioDataRegs.GPESET.bit.GPIO131 = 1;
#define CCW_DIR_Y GpioDataRegs.GPBSET.bit.GPIO63 = 1; GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;
#define CW_DIR_Y GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1; GpioDataRegs.GPCSET.bit.GPIO64 = 1;
//*************************************************************************
// Function: motorDriverInit
// - initialize pwm module and pins needed for both motor driver, x and y direction
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void motorDriverInit();

#endif /* MOTORDRIVER_H_ */
