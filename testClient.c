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
    //timer0Init();

    //timer1
    timer1Init();

    //motor driver
    motorDriverInit();

    //encoder data capture
    eQEP1init(DISABLE_INTERRUPT);

    //intialize struc CMD
    cmdInitialize(cmdList);

    //make sure watch dog are disable
    SysCtl_disableWatchdog();

    //turn on global interrupt
    EINT;

    //
    EALLOW;
    GPIO_disableInterrupt(GPIO_INT_XINT1);
    GPIO_disableInterrupt(GPIO_INT_XINT2);
    EDIS;


    //pin to test interrupt frequency
    GPIO_setMasterCore(15, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_15_GPIO15);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);

    //start at 0 speed
    ePWM1dutyCtl(SPEED_0);
    ePWM2dutyCtl(SPEED_0);


    //test loop for limit switch
    /*while(1)
    {
        executeCmd(cmdList, INDEX_HOME);
        for(;;);
    }*/

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


