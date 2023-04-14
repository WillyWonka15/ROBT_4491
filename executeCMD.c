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

volatile unsigned char test;
//*************************************************************************
// Function: executeCmd
// - move motor until the camera sit right on top of the block
//
// Arguments: PIXY_CCC *blocks
//
//
// return: none
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
void executeCmd(PIXY_CCC *blocks)
{
    //local var
    int16 err;
    int16 temp;
    int16 currSpeedX = 0;
    int16 currSpeedY = 0;
    unsigned char transmitBuffer[50];

    //set motor speed, default 50%, look into ramping speed later
    currSpeedX = ePWM1dutyCtl(SPEED_50);
    currSpeedY = ePWM2dutyCtl(SPEED_50);

    //set motor direction
    while (1)
    {
        if (test & T1_INT)
        {
            //clear flag
            test &= ~T1_INT;

            //get block
            err = pixyCamGetBlocks(blocks, 1);

            //if there is no data
            if (err == -1)
            {
                temp++; //track number of time the pixy give back bad data
                continue;
            }

            sprintf(transmitBuffer, "x: [%4d] y: [%4d] signature: [%4d]\r",
                    blocks->m_x, blocks->m_y, blocks->signature);
            usciSCItxStr(transmitBuffer);
        }

        //move until the motor reach target, testing for now, part into 2 sections to control each motor
        if (blocks->m_x < TARGET_X + 10 && blocks->m_x > TARGET_X - 10)
        {
            //stop motor
            currSpeedX = ePWM1dutyCtl(SPEED_0);
            //if y motor already stop
            if (currSpeedY == 0)
            {
                break;
            }
        }
        if (blocks->m_y < TARGET_Y + 10 && blocks->m_y > TARGET_Y - 10)
        {
            //stop motor
            currSpeedY = ePWM2dutyCtl(SPEED_0);
            //if x motor already stop
            if (currSpeedX == 0)
            {
                break;
            }
        }
    }

}

//*************************************************************************
// Function: timer1ISR
// - interrupt handler for timer 1 interrupt event
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//*************************************************************************
__interrupt void timer1ISR(void)
{
    //babyItsReady |= T1_INT;
    test |= T1_INT;

    CLEAR_T1_INT_FLAG;

}

