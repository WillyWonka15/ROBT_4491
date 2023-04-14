//************************************************************************
//
// FILE:   usciSCI.c
//
// TITLE:  source file for UART interface
//
// using function from ti library for the SCI communication module
//
//
//************************************************************************
#include "usciSCI.h"
#include "driverlib.h"
#include "device.h"

unsigned char backSpaceSequence[15];
//*************************************************************************
// Function: sciInit
// - initialize the SCIB module, 8 bit per transmission, 1 stop bit and 0 parity bit
// Arguments: int16 loopBackEn
//
//  /e loopBackEn allow user to enable loop back mode
//
// return: none
// Author: Will Nguyen
// Date: Mar 23rd, 2023
// Modified: Mar 23rd, 2023
//*************************************************************************
void usciSCIBinit(int16 loopBackEn)
{
    //put device in reset
    ScibRegs.SCICTL1.bit.SWRESET = 0;
    //configure baud rate and frame characteristic
    SCI_setConfig(
            SCIB_BASE, LSPCLK, BAUD_RATE_57600,
            SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE);

    //loop back
    if(loopBackEn == ENABLE_LOOPBACK)
    {
        SCI_enableLoopback(SCIB_BASE);
    }

    //no loop back
    else
    {
        SCI_disableLoopback(SCIB_BASE);
    }


    //configure the output pin
    usciSCIBgpioInit();

    //take device out of reset
    ScibRegs.SCICTL1.bit.SWRESET = 1;
}

//*************************************************************************
// Function: usciSCIgpioInit
// - initialize the RX and TX pin for the SCIB module
//  RX at GPIO 19
//  TX at GPIO 18
//
// Arguments: none
//
//
// return: none
// Author: Will Nguyen
// Date: Mar 30th, 2023
// Modified: Mar 30th, 2023
//*************************************************************************
void usciSCIBgpioInit(void)
{
    //SCIB TX config
    GPIO_setMasterCore(SCIB_GPIO_RX, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_19_SCIRXDB);
    //set pin as i/p
    //GPIO_setDirectionMode(SCIB_GPIO_RX, GPIO_DIR_MODE_IN);
    //GPIO_setQualificationMode(SCIB_GPIO_RX, GPIO_QUAL_SYNC);

    //SCIB RX config
    GPIO_setMasterCore(SCIB_GPIO_TX, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_18_SCITXDB);
    //set pin as o/p
    //GPIO_setDirectionMode(SCIB_GPIO_TX, GPIO_DIR_MODE_OUT);
    //GPIO_setQualificationMode(SCIB_GPIO_TX, GPIO_QUAL_SYNC);
}

//*************************************************************************
// Function: usciSCItxChar
// - transmit a char to console
// Arguments: int16_t txChar
//
//
// return: none
// Author: Will Nguyen
// Date: April 2nd, 2023
// Modified: April 2nd, 2023
//*************************************************************************
void usciSCItxChar(unsigned char txChar)
{
        WAIT_FOR_SCI_TX;
        ScibRegs.SCITXBUF.bit.TXDT = txChar;
}

//*************************************************************************
// Function: usciSCItxStr
// - transmit a string onto console
// Arguments: unsigned char *txByte
//
//  /e *txByte point to the array of char to be transfer onto the console
//
// return: none
// Author: Will Nguyen
// Date: Mar 23rd, 2023
// Modified: Mar 23rd, 2023
//*************************************************************************
void usciSCItxStr(unsigned char *txByte)
{
    int16 i =0;
    while(txByte[i] != NULL)
    {
        usciSCItxChar(txByte[i]);
        i++;
    }
}

//*************************************************************************
// Function: usciSCIgets
// - get input string from user input until enterkey is hit
// Arguments: unsigned char *rxStr
//
//  /e *rxStr store the the i/p string from user
//
// return: -1 if input is longer than MAX_BUF_SIZE
// Author: Will Nguyen
// Date: April 14th, 2023
// Modified: April 14th, 2023
//*************************************************************************
unsigned char usciSCIgets(unsigned char *rxStr)
{
    //local var
    unsigned char rxBuff[MAX_BUF_SIZE];
    int16 i = 0;
    unsigned char ret;

    //ANSI escape sequence
    sprintf(backSpaceSequence, "\x1B[0J");

    //while enter key is not being hit
    while(ScibRegs.SCIRXBUF.bit.SAR != '0x0d')
    {
        //poll RX flag
        WAIT_FOR_SCI_RX;

        //receive character
        if(ScibRegs.SCIRXBUF.bit.SAR != '0x08')
        {
            rxBuff[i] = ScibRegs.SCIRXBUF.bit.SAR;
            //echo chacter back
            usciSCItxChar(ScibRegs.SCIRXBUF.bit.SAR);
            //increase pointer
            i++;
        }
        //if user enter back space
        else
        {
            i--;
            usciSCItxStr(backSpaceSequence);
        }
    }

    //check for i
    if(i >= MAX_BUF_SIZE)
    {
        ret = -1;
    }

    else
    {
        i--;
        //insert null char
        rxBuff[i] = '\0';
        sprintf(rxStr, rxBuff);
        ret = 0;
    }
    return ret;
}

//*************************************************************************
// Function: sciFlushInputBuffer
// - flush SCI input buffer
// Arguments: none
//
//
// return: none
// Author: Will Nguyen
// Date: April 14th, 2023
// Modified: April 14th, 2023
//*************************************************************************
void sciFlushInputBuffer(void)
{
   //local var
    unsigned char temp;

    //enable loop back
    usciSCIBinit(ENABLE_LOOPBACK);

    //send dummy byte to clear rxBuf
    usciSCItxChar(DUMMY_BYTE);

    //read RXBUF to clear flag
    WAIT_FOR_SCI_RX;
    temp = ScibRegs.SCIRXBUF.bit.SAR;

    //disable loopback
    usciSCIBinit(DISABLE_LOOPBACK);
}
