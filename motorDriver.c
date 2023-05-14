//************************************************************************
//
// FILE:   motorDriver.h
//
// TITLE:  source file for implementing motor driver VNH7070AS
//
//
//
//************************************************************************
#include"motorDriver.h"
#include "ePWM.h"
#include "driverlib.h"
#include "device.h"
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
void motorDriverInit()
{
    //ePWM1 for x direction
    ePWM1init();

    //ePWM2 for y direction
    ePWM2init();

    //GPIO for direction of x
    GPIO_setMasterCore(GPIO_X_DIR_CCW, GPIO_CORE_CPU1);
    GPIO_setMasterCore(GPIO_X_DIR_CW, GPIO_CORE_CPU1);

    GPIO_setPinConfig(GPIO_66_GPIO66);
    GPIO_setPinConfig(GPIO_131_GPIO131);

    GPIO_setDirectionMode(GPIO_X_DIR_CCW, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(GPIO_X_DIR_CW, GPIO_DIR_MODE_OUT);
    //default CCW
    CCW_DIR_X;

    //GPIO for direction of y
    GPIO_setMasterCore(GPIO_Y_DIR_CCW, GPIO_CORE_CPU1);
    GPIO_setMasterCore(GPIO_Y_DIR_CW, GPIO_CORE_CPU1);

    GPIO_setPinConfig(GPIO_63_GPIO63);
    GPIO_setPinConfig(GPIO_64_GPIO64);

    GPIO_setDirectionMode(GPIO_Y_DIR_CCW, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(GPIO_Y_DIR_CW, GPIO_DIR_MODE_OUT);

    //GPIO for limit switch to home x
    /*GPIO_setMasterCore(GPIO_X_HOME, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_130_GPIO130);
    GPIO_setDirectionMode(GPIO_X_HOME, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(GPIO_X_HOME, GPIO_PIN_TYPE_PULLUP);*/

    //set up interrupt on the falling edge
    /*Interrupt_register(INT_XINT1, &xHomeISR);
    GPIO_setInterruptPin(GPIO_X_HOME, GPIO_INT_XINT1);
    GPIO_setInterruptType(GPIO_X_HOME, GPIO_INT_TYPE_RISING_EDGE);
    GPIO_enableInterrupt(GPIO_INT_XINT1);
    Interrupt_enable(INT_XINT1);*/

    //GPIO for limit switch to home y
    /*GPIO_setMasterCore(GPIO_Y_HOME, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_26_GPIO26);
    GPIO_setDirectionMode(GPIO_Y_HOME, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(GPIO_Y_HOME, GPIO_PIN_TYPE_PULLUP);

    //set up output pin to the limit switch
    GPIO_setMasterCore(125, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_125_GPIO125);
    GPIO_setDirectionMode(125, GPIO_DIR_MODE_OUT);

    GPIO_setMasterCore(29, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_29_GPIO29);
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_OUT);

    //set both to low
    GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;
    GpioDataRegs.GPDCLEAR.bit.GPIO125 = 1;

    //set up interrupt on the falling edge
    Interrupt_register(INT_XINT2, &yHomeISR);
    GPIO_setInterruptPin(GPIO_Y_HOME, GPIO_INT_XINT2);
    GPIO_setInterruptType(GPIO_Y_HOME,GPIO_INT_TYPE_RISING_EDGE);
    GPIO_enableInterrupt(GPIO_INT_XINT2);
    Interrupt_enable(INT_XINT2);*/

    //default CCW
    CCW_DIR_Y;
}

