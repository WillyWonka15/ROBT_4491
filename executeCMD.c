//************************************************************************
//
// FILE:   executeCMD.h
//
// TITLE:  source file for execute command client
//
//
//
//************************************************************************
#include "executeCMD.h"
//************************************************************************
//GLOBAL VAR
volatile unsigned char babyItsReady;
ENC_DAT encoderData[NUM_ENCODER];
//*************************************************************************
// Function: executeCmd
// - move motor until the camera sit right on top of the block
//
// Arguments: PIXY_CCC *blocks, CMD *cmdList, int16 idx
//
//
// return: none
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
int16 executeCmd(CMD *cmdList, int16 idx)
{
    //
    int16 ret;

    //auto block detection
    int16 err;
    int16 temp;
    PIXY_CCC blocks;

    //jogging
    unsigned char userGuide[250];

    //testing
    volatile unsigned char test[50];

    //initialize struct PIXY_CCC
    pixyInitialize(&blocks);

    //clear screen to begin running
    ansiSeqExecute(CLEAR_SCREEN_SEQUENCE);

    ansiSeqExecute(CURSOR_HOME_SEQUENCE);

    switch (idx)
    {
    case INDEX_JOG:
        //enable the EQEP for tracking postion
        DINT;
        eQEP1init(ENABLE_INTERRUPT);
        EINT;

        //print user manual
        sprintf(userGuide,
                "Up arrow for forward y direction\r\nDown arrow for reverse y direction\r\nS key to stop moving in y direction\r\n\r\nLeft arrow for forward x direction\r\nRight arrow for reverse x direction\r\nA key to stop moving in x direction");

        usciSCItxStr(userGuide);

        while (ScibRegs.SCIRXBUF.bit.SAR != 0x0d)
        {
            //x forward
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x44)
            {
                sciFlushInputBuffer();
                FORWARD_DIR_X
                ;
                ePWM1dutyCtl(cmdList[idx].args[0]);
            }
            //x backward
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x43)
            {
                sciFlushInputBuffer();
                HOME_DIR_X
                ;
                ePWM1dutyCtl(cmdList[idx].args[0]);
            }

            //y forward
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x41)
            {
                sciFlushInputBuffer();
                FORWARD_DIR_Y
                ;
                ePWM2dutyCtl(cmdList[idx].args[0]);
            }
            //y backward
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x42)
            {
                sciFlushInputBuffer();
                HOME_DIR_Y
                ;
                ePWM2dutyCtl(cmdList[idx].args[0]);
            }

            //stopping x
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x61)
            {
                sciFlushInputBuffer();
                ePWM1dutyCtl(SPEED_0);
            }

            //stopping y
            if (ScibRegs.SCIRXBUF.bit.SAR == 0x73)
            {
                sciFlushInputBuffer();
                ePWM2dutyCtl(SPEED_0);
            }
        }

        //reset speed once done
        ePWM1dutyCtl(SPEED_0);
        ePWM2dutyCtl(SPEED_0);

        //disable the EQEP for tracking position
        DINT;
        eQEP1init(DISABLE_INTERRUPT);
        EINT;

        //
        ret = 0;

        break;

    case INDEX_BLOCK_DETECT:
        //enable the EQEP for tracking position
        DINT;
        eQEP1init(ENABLE_INTERRUPT);
        EINT;

        //while enter key is not being hit
        while (ScibRegs.SCIRXBUF.bit.SAR != 0x0d)
        {
            if (babyItsReady & T1_INT)
            {
                //disable timer interrupt for un-interrupted sequence
                DISABLE_INTERRUPT_T1;

                //turn flag off
                babyItsReady &= ~T1_INT;
                //get block data
                err = pixyTargetRequest(&blocks);
                //if return value valid, call auto block detection
                if (err == 0)
                {
                    temp = autoBlockDetection(&blocks);

                    //test loop simulate block is picked, change to another sequential sequence later
                    if (temp == 1)
                    {
                        for (;;)
                            ;
                    }

                    else if (temp == 0)
                    {
                        ret = -1;
                        break;
                    }
                }

                //
                ENABLE_INTERRUPT_T1;
            }

            //disable timer again
            DISABLE_INTERRUPT_T1;
        }

        //disable the EQEP for tracking position
        DINT;
        eQEP1init(DISABLE_INTERRUPT);
        EINT;

        break;

    case INDEX_HOME:
        //turn flag off from bouncing
        babyItsReady &= ~(X_HOME_FLAG | Y_HOME_FLAG);

        //
        EALLOW;
        GPIO_enableInterrupt(GPIO_INT_XINT1);
        GPIO_enableInterrupt(GPIO_INT_XINT2);
        EDIS;

        cartesianHome();

        //reset the position and certain flag after home
        babyItsReady &= ~UNDERFLOW;
        encoderDataInit(encoderData);
        //
        DINT;
        eQEP1init(DISABLE_INTERRUPT);
        EINT;

        //
        EALLOW;
        GPIO_disableInterrupt(GPIO_INT_XINT1);
        GPIO_disableInterrupt(GPIO_INT_XINT2);
        EDIS;

        ret = 0;

        break;
    }

    //flush i/p buffer once done
    sciFlushInputBuffer();

    //turn off speed if not execute any command, extra protection
    ePWM1dutyCtl(SPEED_0);
    ePWM2dutyCtl(SPEED_0);

    return ret;
}

//*************************************************************************
// Function: autoBlockDetection
// - run the auto block detection functionality
//
// Arguments: PIXY_CCC *blocks
//
//
// return: 1 if the actuator is positioned on the block
// Author: Will Nguyen
// Date: April 15th, 2023
// Modified: April 15th, 2023
//************************************************************************
int16 autoBlockDetection(PIXY_CCC *blocks)
{
    //local var
    int16 err;
    int16 temp;
    static unsigned char transmitBuffer[50];
    int16 ret = 0;
    int16 currSpeedX;
    int16 currSpeedY;

    //
    ENABLE_INTERRUPT_T1;

    //
    EALLOW;
    GPIO_enableInterrupt(GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT2);
    EDIS;

    //
    while (1)
    {
        if (babyItsReady & T1_INT)
        {
            //disable timer interrupt for un-interrupted sequence
            DISABLE_INTERRUPT_T1;

            //clear flag
            babyItsReady &= ~T1_INT;

            //get block
            err = pixyCamGetBlocks(blocks, 1);

            //if there is no data
            if (err == -1)
            {
                temp++; //track number of time the pixy give back bad data
            }

            else if (err == 0)
            {
                //control speed base on gathered data
                ret = controllerPID(blocks);

                //displaying real time position
                sprintf(transmitBuffer, "x: [%4d] y: [%4d] signature: [%4d]\r",
                        blocks->m_x, blocks->m_y, blocks->signature);

                usciSCItxStr(transmitBuffer);

                //break if on the target
                if (ret == 1)
                {
                    break;
                }
            }

            //enable T1 interrupt again after done 1 sequence
            ENABLE_INTERRUPT_T1;
        }

        //if triped limit switch
        else if (babyItsReady & X_HOME_FLAG)
        {
            //turn off flag
            babyItsReady &= ~X_HOME_FLAG;

            //turn off motor
            currSpeedX = ePWM1dutyCtl(SPEED_0);
            if (currSpeedY == SPEED_0)
            {
                ret = 0;
                break;
            }

        }
        else if (babyItsReady & Y_HOME_FLAG)
        {
            //turn off flag
            babyItsReady &= ~Y_HOME_FLAG;

            //switch de-bounce

            //turn off motor
            currSpeedY = ePWM2dutyCtl(SPEED_0);
            if (currSpeedX == SPEED_0)
            {
                ret = 0;
                break;
            }
        }
    }

    //
    DISABLE_INTERRUPT_T1;

    //
    EALLOW;
    GPIO_disableInterrupt(GPIO_INT_XINT1);
    GPIO_disableInterrupt(GPIO_INT_XINT2);
    EDIS;

    return ret;
}

//*************************************************************************
// Function: controllerPID
// - this function implement PI controller
//
// Arguments: PIXY_CCC *blocks
//
//
// return: none
// Author: Will Nguyen
// Date: April 28th, 2023
// Modified: April 28th, 2023
//************************************************************************
int16 controllerPID(PIXY_CCC *blocks)
{
    static float eiX = 0;
    static float eiY = 0;
    static float P_x = 0;
    static float P_y = 0;
    static float I_x = 0;
    static float I_y = 0;
    static float TF_x = 0;
    static float TF_y = 0;

    int16 retX;
    int16 retY;
    int16 ret = 0;

    //P
    float eX = TARGET_X - blocks->m_x;
    float eY = TARGET_Y - blocks->m_y;
    P_x = abs(eX) * 100 / TARGET_X;
    P_y = abs(eY) * 100 / TARGET_Y;

    //I
    eiX += eX;
    eiY += eY;
    I_x = I_CONTROLLER_X * eiX;
    I_y = I_CONTROLLER_Y * eiY;

    //transfer function
    TF_x = P_x + I_x;
    TF_y = P_y + I_y;

    //direction
    if (eX > 0)
    {
        HOME_DIR_X
        ;
    }
    else if (eX < 0)
    {
        FORWARD_DIR_X
        ;
    }

    if (eY > 0)
    {
        HOME_DIR_Y
        ;
    }
    else if (eY < 0)
    {
        FORWARD_DIR_Y
        ;
    }

    //clamp speed
    if (TF_x > 50)
    {
        TF_x = 100;
    }
    else if (TF_x < 40 && TF_x > 5)
    {
        TF_x = 40;
    }
    else if (TF_x < 5)
    {
        TF_x = 10;
    }

    if (TF_y > 50)
    {
        TF_y = 100;
    }
    else if (TF_y < 40 && TF_y > 5)
    {
        TF_y = 50;
    }
    else if (TF_y < 5)
    {
        TF_y = 10;
    }

    //drive motor
    retX = ePWM1dutyCtl(TF_x);
    retY = ePWM2dutyCtl(TF_y);

    //stop if on the target
    if (abs(retX) < 2 && abs(retY) < 2)
    {
        ret = 1;
    }

    return ret;
}

//*************************************************************************
// Function: cartesianHome
// - this function home the arm and set the current x & y position to 0
//
// Arguments: PIXY_CCC *blocks
//
//
// return: 1 if the arm is successfully homed
// Author: Will Nguyen
// Date: April 25th, 2023
// Modified: April 25th, 2023
//************************************************************************
int16 cartesianHome(void)
{
    int16 currSpeedX = 0;
    int16 currSpeedY = 0;

    //set direction
    HOME_DIR_X
    ;

    HOME_DIR_Y
    ;

    //homing speed
    currSpeedX = ePWM1dutyCtl(60);
    currSpeedY = ePWM2dutyCtl(60);

    while (1)
    {
        //if reached home
        if (babyItsReady & X_HOME_FLAG)
        {
            //turn off flag
            babyItsReady &= ~X_HOME_FLAG;

            //switch de-bounce

            //turn off motor
            currSpeedX = ePWM1dutyCtl(SPEED_0);
            if (currSpeedY == SPEED_0)
            {
                break;
            }

        }
        if (babyItsReady & Y_HOME_FLAG)
        {
            //turn off flag
            babyItsReady &= ~Y_HOME_FLAG;

            //switch de-bounce

            //turn off motor
            currSpeedY = ePWM2dutyCtl(SPEED_0);
            if (currSpeedX == SPEED_0)
            {
                break;
            }
        }
    }
    return 1;
}

//*************************************************************************
// Function:xHomeISR
// - interrupt handler for x axis reach home position
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: April 24th, 2023
//************************************************************************
__interrupt void xHomeISR(void)
{
    static int count;
    babyItsReady |= X_HOME_FLAG;
    count++;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//*************************************************************************
// Function:yHomeISR
// - interrupt handler for y axis reach home position
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: April 24th, 2023
//************************************************************************
__interrupt void yHomeISR(void)
{
    babyItsReady |= Y_HOME_FLAG;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//*************************************************************************
// Function: timer1ISR
// - interrupt handler for timer 1 interrupt event every 1/40 second
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//*************************************************************************
__interrupt void timer1ISR(void)
{
    babyItsReady |= T1_INT;

    //clear flag for another interrupt request
    CLEAR_T1_INT_FLAG;
}

//*************************************************************************
// Function: eQEP1ISR
// - interrupt handler for eQEP1 module
//  this ISR set a flag whenever there is a UNIT TIME OUT EVENT, COUNTER OVERFLOW/
//  UNDERFLOW. The math for finding posCnt change base on the flag.
//
// Arguments: none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: Mar 18th, 2023
//*************************************************************************
__interrupt void eQEP1ISR(void)
{
    if (EQep1Regs.QFLG.bit.PCU == 1)
    {
        babyItsReady |= UNDERFLOW;
        //clear interrupt flag
        EQep1Regs.QCLR.bit.PCU = 1;
    }

    //if there is a unit time out interrupt
    if (EQep1Regs.QFLG.bit.UTO == 1)
    {
        //update direction every time out event, is it good enough?
        encoderData[INDEX_ENCODER_X].dir = EQep1Regs.QEPSTS.bit.QDF;

        //if pos cnt not underflowing
        encoderData[INDEX_ENCODER_X].posCnt = EQep1Regs.QPOSLAT;

        //if pos cnt underflowing
        if (babyItsReady & UNDERFLOW)
        {
            encoderData[INDEX_ENCODER_X].posCnt = MAX_COUNT - EQep1Regs.QPOSLAT;
            //babyItsReady &= ~UNDERFLOW;
        }

        //clear interrupt flag
        EQep1Regs.QCLR.bit.UTO = 1;
    }

    //calculate displacement and update position
    encoderData[INDEX_ENCODER_X].posCurr = LEAD_SCREW
            * encoderData[INDEX_ENCODER_X].posCnt / ENCODER_MAX_COUNT_QUAD;

    //clear general interrupt flag
    EQep1Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}
