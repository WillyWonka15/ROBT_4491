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

#include "driverlib.h"
#include "device.h"
#include "encoderDataCapture.h"
#include "F2837xD_device.h"
#include "usciSCI.h"
#include "usciSPI.h"
#include "pixyCam2.h"
#include "Timer0.h"
#include "Timer1.h"
#include "ePWM.h"
#include "motorDriver.h"
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
void executeCmd(PIXY_CCC *blocks);


#endif /* EXECUTECMD_H_ */
