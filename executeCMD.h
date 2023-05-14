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
#include "Timer0.h"
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
//PI CONTROLLER
#define P_CONTROLLER_X 25
#define P_CONTROLLER_Y 25
#define I_CONTROLLER_X 0.4
#define I_CONTROLLER_Y 0.4
#define D_CONTROLLER_X 0.3
#define D_CONTROLLER_Y 0.3
//USER HOME
#define USER_HOME_X 2.5
#define USER_HOME_Y 6.5
//DROP SIGNATURE 1
#define DROP_SIG1_X 7.4
#define DROP_SIG1_Y 2
//DROP SIGNATURE 2
#define DROP_SIG2_X 7.4
#define DROP_SIG2_Y 4.35
//DROP SIGNATURE 3
#define DROP_SIG3_X 7.4
#define DROP_SIG3_Y 7.05
//TIME OUT FOR HOME
#define TIME_OUT_3_SEC 300
//ACCEPTABLE ERROR
#define ERROR_0125 0.0156
#define ERROR_05 0.25
//MACROS
#define WAIT_FOR_PLC_PICK_UP while(GpioDataRegs.GPDDAT.bit.GPIO123 == 1);
#define WAIT_FOR_PLC_DROP while(GpioDataRegs.GPDDAT.bit.GPIO123 == 0);
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
// Arguments: float32 error
// \e error is the square of acceptable error in inches
//
//
// return: 1 if at the desire location
// Author: Will Nguyen
// Date: April 28th, 2023
// Modified: April 28th, 2023
//************************************************************************
int16 controllerPID(float32 error, float32 a);

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
int16 blockSorting(PIXY_CCC *blocks);

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
int16 cartesianCoordMove(float32 xCoord, float32 yCoord, float32 error);


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
