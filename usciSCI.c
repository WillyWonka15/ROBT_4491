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
    while(txByte[i] != '\0')
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

    //while enter key is not being hit
     while(ScibRegs.SCIRXBUF.bit.SAR != 0x0d)
    {
        //poll RX flag
        WAIT_FOR_SCI_RX;

        //
        if(i < 0 )
        {
            i = 0;
        }

        //receive character
        if(ScibRegs.SCIRXBUF.bit.SAR != 0x08)
        {
            rxBuff[i] = ScibRegs.SCIRXBUF.bit.SAR;
            //echo chacter back
            usciSCItxChar(ScibRegs.SCIRXBUF.bit.SAR);
            //increase pointer
            i++;
        }
        //if user enter back space
        else if(ScibRegs.SCIRXBUF.bit.SAR == 0x08)
        {
            i--;
            ansiSeqExecute(CURSOR_BACKSPACE_SEQUENCE);
            ansiSeqExecute(BACKSPACE_DELETE_SEQUENCE);
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
        strcpy(rxStr, rxBuff);
        ret = 0;
    }

    //flush i/p buffer
    sciFlushInputBuffer();

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

//*************************************************************************
// Function: ansiSeqExecute
// - this function execute ANSI sequence base on user type input
//
// Arguments: int16 sequenceType
//
//
// return: none
// Author: Will Nguyen
// Date: April 5th, 2023
// Modified: April 5th, 2023
//************************************************************************
void ansiSeqExecute(int16 sequenceType)
{
    volatile char ansiSequence[10];

    switch (sequenceType)
    {
    case CLEAR_SCREEN_SEQUENCE:
        sprintf(ansiSequence, "\x1B[2J");
        usciSCItxStr(ansiSequence);
        break;
    case CURSOR_HOME_SEQUENCE:
        sprintf(ansiSequence, "\x1B[H");
        usciSCItxStr(ansiSequence);
        break;
    case CURSOR_BACKSPACE_SEQUENCE:
        sprintf(ansiSequence, "\x1B[1D");
        usciSCItxStr(ansiSequence);
        break;
    case BACKSPACE_DELETE_SEQUENCE:
        sprintf(ansiSequence, "\x1B[0J");
        usciSCItxStr(ansiSequence);
        break;
    }
}
