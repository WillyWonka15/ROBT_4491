//************************************************************************
//
// FILE:   pixyCam2.h
//
// TITLE:  header file to implement pixyCam2 functionality
//
//
//
//************************************************************************
#include "pixyCam2.h"
#include "usciSPI.h"
#include "driverlib.h"
#include "device.h"
//GLOBAL VARIABLE AVAILABLE ONLY IN THIS .c module
const unsigned char cccRequestPacket[REQ_PACKET_LEN] = { SYNC_16_HI, SYNC_16_LO,
PACKET_TYPE,
                                                         PAYLOAD_LEN,
                                                         MAX_SIGMAP_VALUE,
                                                         SINGLE_BLOCK_RETURN, 0,
                                                         0, 0, 0, 0, 0, 0 };
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
void pixyInitialize(PIXY_CCC *blocks)
{
    blocks->m_x = 0;
    blocks->m_y = 0;
    blocks->trackIndex = 0;
    blocks->signature = 0;
    blocks->frame = 0;
}

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
int16 pixyTargetRequest(PIXY_CCC *blocks)
{
    unsigned char resPacket[RES_PACKET_LEN];
    int16 ret;

    //test
    unsigned char *temp;

    //sending the request packet via SPI
    usciSPItxStr(cccRequestPacket, REQ_PACKET_LEN);

    //receive the packet via SPI
    usciSPIrxStr(resPacket, RES_PACKET_LEN);

    //store data into struct, make into a function
    if ((int16) resPacket[PAYLOAD_LEN_INDEX] == RES_PAYLOAD_LEN)
    {
        blocks->m_x = resPacket[M_X_INDEX_HI] << 8 | resPacket[M_X_INDEX_LO];

        blocks->m_y = resPacket[M_Y_INDEX_LO];

        blocks->trackIndex = resPacket[TRACKING_INDEX];

        blocks->signature = resPacket[SIG_INDEX];

        ret = 0;
    }
    else
    {
        ret = -1;

    }
    return ret;
}

//*************************************************************************
// Function: pixyCamGetBlocks
// - getting signature centroid base on user request of signature
// Arguments: PIXY_CCC blocks, int16 num
//
// omit to receiving 1 block at a time!!
//
// /e blocks contain all the information to distinguish the tracked block to compare, the /e num let user
// choose how many block to receive at 1 time keep it at 1 block at a time for now, what's the point of
// getting multiple block for current application? maybe it'd help since we need to
//
// return: -1 if block is not matching
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
int16 pixyCamGetBlocks(PIXY_CCC *blocks, int numBlock)
{
    //
    int16 ret;
    unsigned char signature;
    switch (blocks->signature)
    {
    case 1:
        signature = SIGNATURE_1;
        break;
    case 2:
        signature = SIGNATURE_2;
        break;
    case 3:
        signature = SIGNATURE_3;
        break;
    case 4:
        signature = SIGNATURE_4;
        break;
    case 5:
        signature = SIGNATURE_5;
        break;
    case 6:
        signature = SIGNATURE_6;
        break;
    case 7:
        signature = SIGNATURE_7;
        break;
    }

    //array contain the detected signature value
    unsigned char reqPacket[REQ_PACKET_LEN] = { SYNC_16_HI, SYNC_16_LO,
    PACKET_TYPE,
                                                PAYLOAD_LEN, signature,
                                                numBlock, 0, 0, 0, 0, 0, 0, 0 };

    //array to store respond
    unsigned char resPacket[RES_PACKET_LEN];

    //sending the request packet via SPI
    usciSPItxStr(reqPacket, REQ_PACKET_LEN);

    //receive the packet via SPI
    usciSPIrxStr(resPacket, RES_PACKET_LEN);

    //if the signature is the same and the track index match
    if ((int16) resPacket[SIG_INDEX] == blocks->signature
            && (int16) resPacket[TRACKING_INDEX] == blocks->trackIndex)
    {
        blocks->m_x = (int16) resPacket[M_X_INDEX_HI] << 8
                | resPacket[M_X_INDEX_LO];

        blocks->m_y = (int16) resPacket[M_Y_INDEX_LO];

        //blocks->trackIndex = resPacket[TRACKING_INDEX];
        blocks->frame = resPacket[FRAME_INDEX];
        //blocks->signature = resPacket[SIG_INDEX];

        ret = 0;
    }

    else
    {
        ret = -1;
    }

    return ret;
}

