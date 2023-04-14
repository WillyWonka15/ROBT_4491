//************************************************************************
//
// FILE:   testClient.c
//
// TITLE:  testing client for encoder reading
//
// gather the count of a quadrature encoder and compute position as well as speed
//
//
//************************************************************************
#include "executeCMD.h"
//
// Main
//
//global var
volatile unsigned char t0Int;
void main(void)
{

    //local variable
    volatile float32 vel = 0;
    PIXY_CCC blocks;

    ////initialize section

    //initialize interrupt module
    Interrupt_initModule();
    Interrupt_enablePIE();
    Interrupt_initVectorTable();

    //initialize device
    Device_init();

    //initialize SCI module
    usciSCIBinit(DISABLE_LOOPBACK);

    //initialize SPI module
    usciSPIinit();

    //timer0
    timer0Init();

    //timer1
    timer1Init();

    //motor driver
    motorDriverInit();

    //initialize struct PIXY_CCC
    pixyInitialize(&blocks);

    //make sure watch dog are disable
    SysCtl_disableWatchdog();

    //turn on global interrupt
    EINT;

    //pin to test interrupt frequency
    GPIO_setMasterCore(15, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_15_GPIO15);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);

    //test loop for pixycam2
    int16 err;
    unsigned char test[50];
    ePWM1dutyCtl(SPEED_0);
    ePWM2dutyCtl(SPEED_50);

    while (1)
    {

        if (t0Int & T0_INT)
        {
            //turn flag off
            t0Int &= ~T0_INT;
            //get block data
            err = pixyTargetRequest(&blocks, test);
            //if return value valid, call execute CMD
            if(err == 0)
            {
                executeCmd(&blocks);
            }
        }

    }
    return 0;
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
__interrupt void timer0ISR(void)
{
    //set flag to read data from pixy Cam
    t0Int |= T0_INT;

    //clear flag
    CLEAR_T0_INT_FLAG;

    //test
    //GpioDataRegs.GPATOGGLE.bit.GPIO15 = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

