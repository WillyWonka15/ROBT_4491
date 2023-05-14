//************************************************************************
//
// FILE:   pixyCam2.h
//
// TITLE:  header file to implement pixyCam2 functionality
//
//
//
//************************************************************************
#ifndef PIXYCAM2_H_
#define PIXYCAM2_H_
#include "F2837xD_device.h"
#include <stdio.h>
//************************************************************************
//PRE-DEFINE CONSTANT
#define TOLERANCE 10
//************************************************************************
//PACKET INFORMATION
#define SYNC_16_HI 174
#define SYNC_16_LO 193
#define PACKET_TYPE 32
#define PAYLOAD_LEN 2
#define REQ_PACKET_LEN 13
//************************************************************************
//RETURN PACKET INFORMATION
#define MAX_SIGMAP_VALUE 255
#define MAX_BLOCK_RETURN 255
#define SINGLE_BLOCK_RETURN 1
#define RES_PACKET_LEN 20
#define RES_PAYLOAD_LEN 14
#define SIG_INDEX 6
#define M_X_INDEX_LO 8
#define M_X_INDEX_HI 9
#define M_Y_INDEX_LO 10
#define M_Y_INDEX_HI 11
#define TRACKING_INDEX 18
#define FRAME_INDEX 19
#define PAYLOAD_LEN_INDEX 3
//************************************************************************
//SIGMAP VALUE
#define SIGNATURE_1 C28X_BIT0
#define SIGNATURE_2 C28X_BIT1
#define SIGNATURE_3 C28X_BIT2
#define SIGNATURE_4 C28X_BIT3
#define SIGNATURE_5 C28X_BIT4
#define SIGNATURE_6 C28X_BIT5
#define SIGNATURE_7 C28X_BIT6
//************************************************************************
//MIDDLE POINT OF THE PIXYCAM
#define TARGET_X 165
#define TARGET_Y 198
//************************************************************************
//STRUCTURE
typedef struct PIXY_CCC
{
    volatile int16 m_x; //centroid x
    volatile int16 m_y; //centroid y
    volatile int16 trackIndex; //tracking index of signature
    volatile int16 frame;
    volatile unsigned char signature; //the signature  of the block currently detected
}PIXY_CCC;
//************************************************************************
//GLOBAL VARIABLE

//************************************************************************
//FUNCTION PROTOTYPE
//*************************************************************************
// Function: pixyInitialize
// - initialize the PIXY_CCC structure
// Arguments: PIXY_CCC *blocks
//
//
// return: none
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
void pixyInitialize(PIXY_CCC *blocks);

//*************************************************************************
// Function: pixyTargetRequest
// - collect 1 block of any signature and retrieve the data into the PIXY_CCC struct
// Arguments: PIXY_CCC *blocks
//
// /e blocks is to store the related data of current target
//
// return: -1 if no block detected and 0 if there is blocks in sight
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
int16 pixyTargetRequest(PIXY_CCC *blocks);

//*************************************************************************
// Function: pixyCamGetBlocks
// - getting signature centroid base on user request of signature
// Arguments: PIXY_CCC blocks, int16 numBlock
//
// omit to receiving 1 block at a time!!
//
// /e blocks contain all the information to distinguish the tracked block to compare, the /e numBlock let user
// choose how many block to receive at 1 time keep it at 1 block at a time for now, what's the point of
// getting multiple block for current application? maybe it'd help since we need to
//
// return: -1 if block is not matching
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
int16 pixyCamGetBlocks(PIXY_CCC *blocks, int16 numBlock);

#endif /* PIXYCAM2_H_ */
