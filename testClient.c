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
void main(void)
{

    //diable global int
    DINT;

    //local variable
    volatile float32 vel = 0;
    CMD cmdList[NUM_COMMANDS];
    unsigned char rxStr[50];
    int16 idx;
    int16 ret;

    ////initialize section
    //initialize device
    Device_init();

    //initialize interrupt module
    Interrupt_initModule();
    Interrupt_enablePIE();
    Interrupt_initVectorTable();

    //initialize SCI module
    usciSCIBinit(DISABLE_LOOPBACK);

    //initialize SPI module
    usciSPIinit();

    //timer0
    //timer0Init();

    //timer1
    timer1Init();

    //motor driver
    motorDriverInit();

    //encoder data capture
    eQEP1init(ENABLE_INTERRUPT);
    eQEP3init(ENABLE_INTERRUPT);

    //intialize struc CMD
    cmdInitialize(cmdList);

    //make sure watch dog are disable
    SysCtl_disableWatchdog();

    //
    communicationInit();


    //turn on global interrupt
    EINT;

    //pin to test interrupt frequency
    GPIO_setMasterCore(15, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_15_GPIO15);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);

    //start at 0 speed
    ePWM1dutyCtl(SPEED_0);
    ePWM2dutyCtl(SPEED_0);


    while (1)
    {
        //command selection
        cmdSelection();

        //receive user choice
        usciSCIgets(rxStr);

        //parse command
        idx = parseCmd(cmdList, rxStr);
        //if command given is bad
        if(idx == -1)
        {
            continue;
        }

        //execute command
        ret = executeCmd(cmdList, idx);
    }
}


