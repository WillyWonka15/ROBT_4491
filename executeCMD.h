//************************************************************************
//
// FILE:   executeCMD.h
//
// TITLE:  header file for execute command client
//
//
//
//************************************************************************
#ifndef EXECUTECMD_H_
#define EXECUTECMD_H_
#include "usciSCI.h"
#include "usciSPI.h"
#include "pixyCam2.h"
//#include "Timer0.h"
#include "Timer1.h"
#include "ePWM.h"
#include "motorDriver.h"
#include "cmdInterpreter.h"
#include "controllerToPlc.h"
#include "encoderDataCapture.h"
#include "driverlib.h"
#include "device.h"
//*************************************************************************
//PRE-DEFINE CONSTANT
#define P_CONTROLLER_X 1/TARGET_X
#define P_CONTROLLER_Y 1/TARGET_Y
#define I_CONTROLLER_X 0
#define I_CONTROLLER_Y 0
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
int16 executeCmd(CMD *cmdList, int16 idx);

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
int16 autoBlockDetection(PIXY_CCC *blocks);

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
int16 controllerPID(PIXY_CCC *blocks);

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
int16 cartesianHome(void);


#endif /* EXECUTECMD_H_ */
