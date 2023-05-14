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
    int16 temp = 0;
    PIXY_CCC blocks;

    //flush i/p buffer before doing any command
    sciFlushInputBuffer();

    //jogging
    unsigned char userGuide[250];

    //initialize struct PIXY_CCC
    pixyInitialize(&blocks);

    //clear screen to begin running
    ansiSeqExecute(CLEAR_SCREEN_SEQUENCE);

    ansiSeqExecute(CURSOR_HOME_SEQUENCE);

    switch (idx)
    {
    case INDEX_JOG:
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

        //
        ret = 0;

        break;

    case INDEX_BLOCK_DETECT:
        //enable timer 1 interrupt
        DINT;
        ENABLE_INTERRUPT_T1;
        EINT;

        //while enter key is not being hit
        while (ScibRegs.SCIRXBUF.bit.SAR != 0x0d)
        {
            //move to position to see block
            temp = cartesianCoordMove(USER_HOME_X, USER_HOME_Y, ERROR_05);

            if (temp == 1)
            {
                while (temp == 1)
                {
                    if (babyItsReady & T1_INT)
                    {
                        //disable timer interrupt for un-interrupted sequence
                        DINT;
                        DISABLE_INTERRUPT_T1;
                        EINT;

                        //turn flag off
                        babyItsReady &= ~T1_INT;

                        //get block data
                        err = pixyTargetRequest(&blocks);

                        //if return value valid, call auto block detection
                        if (err == 0)
                        {
                            temp = autoBlockDetection(&blocks);

                            //TCP is line up with the block center x and y
                            if (temp == 1)
                            {

                                blockSorting(&blocks);
                                temp = 0;
                            }

                            /*else if (temp == 0)
                             {
                             ret = -1;
                             break;
                             }*/
                        }
                        //
                        DINT;
                        ENABLE_INTERRUPT_T1;
                        EINT;

                    }
                }
            }
        }
        break;

    case INDEX_HOME:

        cartesianHome();

        //reset the position and certain flag after home
        babyItsReady &= ~(EQEP1_UNDERFLOW | EQEP3_UNDERFLOW);

        //zero encoder count
        DINT;
        eQEP1init(ENABLE_INTERRUPT);
        eQEP3init(ENABLE_INTERRUPT);
        EINT;

        encoderDataInit(encoderData);


        ret = 0;

        break;
    }

    //flush i/p buffer once done
    sciFlushInputBuffer();

    //turn off speed if not execute any command, extra protection
    ePWM1dutyCtl(SPEED_0);
    ePWM2dutyCtl(SPEED_0);

    //disable timer T1
    DINT;
    DISABLE_INTERRUPT_T1;
    EINT;

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
    //reset flag use in this section
    babyItsReady &= ~(T1_INT | DINNER);

    //local var
    int16 err;
    unsigned char transmitBuffer[50];
    int16 ret = 0;
    float32 aConstant = 0;
    float32 convertRatX = 0.025146;
    float32 convertRatY = 0.035274;

    //reset past speed
    encoderData[INDEX_ENCODER_X].pastSpeed = 100;
    encoderData[INDEX_ENCODER_Y].pastSpeed = 100;

    //calculate distance x and y in in related to the TCP
    encoderData[INDEX_ENCODER_X].displacement = convertRatX
            * (blocks->m_x - TARGET_X);

    encoderData[INDEX_ENCODER_Y].displacement = convertRatY
            * (blocks->m_y - TARGET_Y);

    //calculate desire TCP position related to the home postion
    encoderData[INDEX_ENCODER_X].posDesire =
            encoderData[INDEX_ENCODER_X].displacement
                    + encoderData[INDEX_ENCODER_X].posCurr;
    encoderData[INDEX_ENCODER_Y].posDesire =
            encoderData[INDEX_ENCODER_Y].displacement
                    + encoderData[INDEX_ENCODER_Y].posCurr;

    //enable T1 interrupt again to track the block
    DINT;
    ENABLE_INTERRUPT_T1;
    EINT;

    while (1)
    {
        if (babyItsReady & T1_INT)
        {
            //disable timer interrupt for un-interrupted sequence
            DINT;
            DISABLE_INTERRUPT_T1;
            EINT;

            //clear flag
            babyItsReady &= ~T1_INT;

            aConstant += 0.05;

            //get block
            err = pixyCamGetBlocks(blocks, 1);

            if (err == 0)
            {
                //calculate distance x and y in related to the TCP
                encoderData[INDEX_ENCODER_X].displacement = convertRatX
                        * (blocks->m_x - TARGET_X);

                encoderData[INDEX_ENCODER_Y].displacement = convertRatY
                        * (blocks->m_y - TARGET_Y);

                //calculate desire TCP position related to the home postion
                encoderData[INDEX_ENCODER_X].posDesire =
                        encoderData[INDEX_ENCODER_X].displacement
                                + encoderData[INDEX_ENCODER_X].posCurr;
                encoderData[INDEX_ENCODER_Y].posDesire =
                        encoderData[INDEX_ENCODER_Y].displacement
                                + encoderData[INDEX_ENCODER_Y].posCurr;

                //displaying real time position
                //sprintf(transmitBuffer, "x: [%4d] y: [%4d]\r", blocks->m_x,blocks->m_y);

                //usciSCItxStr(transmitBuffer);
            }

            //enable T1 interrupt again after done 1 sequence
            DINT;
            ENABLE_INTERRUPT_T1;
            EINT;
        }

        if (babyItsReady & DINNER)
        {
            //
            babyItsReady &= ~DINNER;

            //calculate current postion related to home every 100Hz period
            encoderData[INDEX_ENCODER_X].posCurr =
                    LEAD_SCREW
                            * encoderData[INDEX_ENCODER_X].posCnt/ ENCODER_MAX_COUNT_QUAD;

            encoderData[INDEX_ENCODER_Y].posCurr =
                    LEAD_SCREW
                            * encoderData[INDEX_ENCODER_Y].posCnt/ ENCODER_MAX_COUNT_QUAD;

            //control speed base on gathered data
            ret = controllerPID(ERROR_0125,aConstant);

            //break if on the target
            if (ret == 1)
            {
                break;
            }
        }

    }

    //
    DINT;
    DISABLE_INTERRUPT_T1;
    EINT;

    return ret;
}

//*************************************************************************
// Function: cartesianCoordMove
// - this function move the arm to a set position
//
// Arguments: float32 xCoord, float32 yCoord, float32 error
//
// \e xCoord and yCoord is the real X and Y coordinate in inches user want TCP to be at
// \e error is the amount of accpetable error in the move
//
// return: 1 if at the postion
// Author: Will Nguyen
// Date: April 25th, 2023
// Modified: May 4th, 2023
//************************************************************************
int16 cartesianCoordMove(float32 xCoord, float32 yCoord, float32 error)
{
    //reset flag use in this section
    babyItsReady &= ~(T1_INT | DINNER);

    //local var
    int16 ret = 0;

    //
    float32 aConstant = 0;

    //reset past speed
    encoderData[INDEX_ENCODER_X].pastSpeed = 100;
    encoderData[INDEX_ENCODER_Y].pastSpeed = 100;

    //calculate desire TCP position related to the home postion
    encoderData[INDEX_ENCODER_X].posDesire = xCoord;

    encoderData[INDEX_ENCODER_Y].posDesire = yCoord;

    //calculate distance x and y in in related to the TCP
    encoderData[INDEX_ENCODER_X].displacement =
            encoderData[INDEX_ENCODER_X].posDesire
                    - encoderData[INDEX_ENCODER_X].posCurr;

    encoderData[INDEX_ENCODER_Y].displacement =
            encoderData[INDEX_ENCODER_Y].posDesire
                    - encoderData[INDEX_ENCODER_Y].posCurr;

    while (1)
    {
        if (babyItsReady & T1_INT)
        {
            //turn flag off
            babyItsReady &= ~T1_INT;

            aConstant += 0.05;

            encoderData[INDEX_ENCODER_X].displacement =
                    encoderData[INDEX_ENCODER_X].posDesire
                            - encoderData[INDEX_ENCODER_X].posCurr;

            encoderData[INDEX_ENCODER_Y].displacement =
                    encoderData[INDEX_ENCODER_Y].posDesire
                            - encoderData[INDEX_ENCODER_Y].posCurr;
        }

        if (babyItsReady & DINNER)
        {
            //
            babyItsReady &= ~DINNER;

            //calculate current postion related to home every 100Hz period
            encoderData[INDEX_ENCODER_X].posCurr =
                    LEAD_SCREW
                            * encoderData[INDEX_ENCODER_X].posCnt/ ENCODER_MAX_COUNT_QUAD;

            encoderData[INDEX_ENCODER_Y].posCurr =
                    LEAD_SCREW
                            * encoderData[INDEX_ENCODER_Y].posCnt/ ENCODER_MAX_COUNT_QUAD;

            //control speed base on gathered data
            ret = controllerPID(error, aConstant);

            //break if on the target
            if (ret == 1)
            {
                break;
            }
        }
    }
    return ret;
}

//*************************************************************************
// Function: controllerPID
// - this function implement PI controller
//
// Arguments: float32 error
// \e error is the square of acceptable error in inches
//
//
// return: 1 if at the desire location
// Author: Will Nguyen
// Date: April 28th, 2023
// Modified: April 28th, 2023
//************************************************************************
int16 controllerPID(float32 error, float32 a)
{
    static float32 eiX = 0;
    static float32 eiY = 0;
    static float32 lasteX = 0;
    static float32 lasteY = 0;
    float32 I_x = 0;
    float32 I_y = 0;
    static float32 eX = 0;
    static float32 eY = 0;
    float32 TF_x = 0;
    float32 TF_y = 0;

    int16 ret = 0;

    if(a > 1)
    {
        a = 1;
    }

    //P
    lasteX = eX;
    lasteY = eY;

    eX = encoderData[INDEX_ENCODER_X].posDesire
            - encoderData[INDEX_ENCODER_X].posCurr;

    eY = encoderData[INDEX_ENCODER_Y].posDesire
            - encoderData[INDEX_ENCODER_Y].posCurr;

    float32 P_x = P_CONTROLLER_X * eX * a;
    float32 P_y = P_CONTROLLER_Y * eY * a;

    //I
    eiX += eX;
    eiY += eY;

    //reset from wind up
    if(eX * lasteX < 0)
    {
        eiX = 0;
    }
    if(eY * lasteY < 0)
    {
        eiY = 0;
    }

    I_x = I_CONTROLLER_X * eiX;
    I_y = I_CONTROLLER_Y * eiY;

    //transfer function
    TF_x = 100*(P_x + I_x);
    TF_y = 100*(P_y + I_y);
    //stop if on the target, maybe using displacement is too slow
    if ((eX * eX < error) && (eY * eY < error))
    {
        ePWM1dutyCtl(SPEED_0);
        ePWM2dutyCtl(SPEED_0);
        ret = 1;
        return ret;
    }

    else
    {
        //direction control
        if (eX > 0)
        {
            FORWARD_DIR_X
            ;
        }
        else if (eX < 0)
        {
            HOME_DIR_X
            ;
        }

        if (eY > 0)
        {
            FORWARD_DIR_Y
            ;
        }
        else if (eY < 0)
        {
            HOME_DIR_Y
            ;
        }

        if(TF_x * TF_x > 10000)
        {
            TF_x = 100;
        }
        if(TF_y * TF_y > 10000)
        {
            TF_y = 100;
        }

        //drive motor
        encoderData[INDEX_ENCODER_X].pastSpeed = ePWM1dutyCtl((int16) TF_x);
        encoderData[INDEX_ENCODER_Y].pastSpeed = ePWM2dutyCtl((int16) TF_y);
    }

    return ret;
}

//*************************************************************************
// Function: blockSorting
// - this function move TCP to the correct sylos to drop the block
//
// Arguments: PIXY_CCC *blocks
// \e blocks store the signature information to decide where to go
//
//
// return: 1 if block sucessfully drop
// Author: Will Nguyen
// Date: April 28th, 2023
// Modified: May 5th, 2023
//************************************************************************
int16 blockSorting(PIXY_CCC *blocks)
{
    //reset flag use in this section
    babyItsReady &= ~PLC_DONE;
    //local var
    int16 ret = 0;

    //suck block
    GET_BLOCK;

    //wait for PLC to secure the block
    WAIT_FOR_PLC_PICK_UP;

    //enable T1 interrupt
    /*DINT;
    ENABLE_INTERRUPT_T1;
    EINT;*/

    switch (blocks->signature)
    {
    case 1:
        ret = cartesianCoordMove(DROP_SIG1_X, DROP_SIG1_Y, ERROR_0125);
        break;
    case 2:
        ret = cartesianCoordMove(DROP_SIG2_X, DROP_SIG2_Y, ERROR_0125);
        break;
    case 3:
        ret = cartesianCoordMove(DROP_SIG3_X, DROP_SIG3_Y, ERROR_0125);
        break;
    }

    //disable T1 interrupt
    /*DINT;
    DISABLE_INTERRUPT_T1;
    EINT;*/

    //release block
    RELEASE_BLOCK;

    //wait for PLC to release
    WAIT_FOR_PLC_DROP;

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
    int16 count = 0;

    //set direction
    HOME_DIR_X
    ;

    HOME_DIR_Y
    ;

    //homing speed
    ePWM1dutyCtl(60);
    ePWM2dutyCtl(60);

    while (1)
    {
        //if reached home
        if (babyItsReady & DINNER)
        {
            //turn off flag
            babyItsReady &= ~DINNER;

            //if poscnt of both encoder stop increasing for 3 sec, arm is homed
            if (encoderData[INDEX_ENCODER_X].posCnt
                    == encoderData[INDEX_ENCODER_X].prePosCnt
                    && encoderData[INDEX_ENCODER_Y].posCnt
                            == encoderData[INDEX_ENCODER_Y].prePosCnt)
            {
                count++;
                //if 3 second went by with out moving
                if (count == TIME_OUT_3_SEC)
                {
                    ePWM1dutyCtl(SPEED_0);
                    ePWM2dutyCtl(SPEED_0);

                    break;
                }
            }
        }
    }
    return 1;
}

//*************************************************************************
// Function:plcDoneISR
// - interrupt handler for plc communicate to microcontroller
//
// Arguments:
//
//
// return: none
// Author: Will Nguyen
// Date: April 24th, 2023
// Modified: may 7th, 2023
//************************************************************************
/*__interrupt void plcDoneISR(void)
{
    babyItsReady |= PLC_DONE;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}*/


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
__interrupt
void timer1ISR(void)
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
__interrupt
void eQEP1ISR(void)
{
    if (EQep1Regs.QFLG.bit.PCU == 1)
    {
        babyItsReady |= EQEP1_UNDERFLOW;
        //clear interrupt flag
        EQep1Regs.QCLR.bit.PCU = 1;
    }

    //if there is a unit time out interrupt
    if (EQep1Regs.QFLG.bit.UTO == 1)
    {
        babyItsReady |= DINNER;
        //update direction every time out event, is it good enough?
        encoderData[INDEX_ENCODER_X].dir = EQep1Regs.QEPSTS.bit.QDF;

        //update previous count
        encoderData[INDEX_ENCODER_X].prePosCnt =
                encoderData[INDEX_ENCODER_X].posCnt;

        //if pos cnt not underflowing
        encoderData[INDEX_ENCODER_X].posCnt = EQep1Regs.QPOSLAT;

        //if pos cnt underflowing
        if (babyItsReady & EQEP1_UNDERFLOW)
        {
            encoderData[INDEX_ENCODER_X].posCnt = MAX_COUNT - EQep1Regs.QPOSLAT;
            //babyItsReady &= ~UNDERFLOW;
        }

        //clear interrupt flag
        EQep1Regs.QCLR.bit.UTO = 1;
    }
    //clear general interrupt flag
    EQep1Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

//*************************************************************************
// Function: eQEP3ISR
// - interrupt handler for eQEP3 module
//  this ISR set a flag whenever there is a UNIT TIME OUT EVENT, COUNTER OVERFLOW/
//  UNDERFLOW. The math for finding posCnt change base on the flag.
//
// Arguments: none
//
// return: none
// Author: Will Nguyen
// Date: Mar 18th, 2023
// Modified: May 3rd, 2023
//*************************************************************************
__interrupt
void eQEP3ISR(void)
{
    if (EQep3Regs.QFLG.bit.PCU == 1)
    {
        babyItsReady |= EQEP3_UNDERFLOW;
        //clear interrupt flag
        EQep3Regs.QCLR.bit.PCU = 1;
    }

    //if there is a unit time out interrupt
    if (EQep3Regs.QFLG.bit.UTO == 1)
    {
        babyItsReady |= DINNER;
        //update direction every time out event, is it good enough?
        encoderData[INDEX_ENCODER_Y].dir = EQep3Regs.QEPSTS.bit.QDF;

        //update previous count
        encoderData[INDEX_ENCODER_Y].prePosCnt =
                encoderData[INDEX_ENCODER_Y].posCnt;

        //if pos cnt not underflowing
        encoderData[INDEX_ENCODER_Y].posCnt = EQep3Regs.QPOSLAT;

        //if pos cnt underflowing
        if (babyItsReady & EQEP3_UNDERFLOW)
        {
            encoderData[INDEX_ENCODER_Y].posCnt = MAX_COUNT - EQep3Regs.QPOSLAT;
            //babyItsReady &= ~UNDERFLOW;
        }

        //clear interrupt flag
        EQep3Regs.QCLR.bit.UTO = 1;
    }

    //clear general interrupt flag
    EQep3Regs.QCLR.bit.INT = 1;

    //issue PIE ack
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}
