//************************************************************************
//
// FILE:   usciSCI.h
//
// TITLE:  header file for UART interface
//
// using function from ti library for the SCI communication module
//
//
//************************************************************************
#ifndef USCISCI_H_
#define USCISCI_H_
#include "F2837xD_device.h"
#include<strings.h>
#include<stdio.h>
#include<stdlib.h>
//************************************************************************
//CLOCK RATE
#define LSPCLK 25000000
//************************************************************************
//DEVICE PINOUT
#define SCIB_GPIO_RX 19
#define SCIB_GPIO_TX 18
//************************************************************************
//LOOPBACK
#define ENABLE_LOOPBACK 1
#define DISABLE_LOOPBACK 0
//************************************************************************
//BAUD RATE
#define BAUD_RATE_19200 19200
#define BAUD_RATE_57600 57600
//************************************************************************
//MACROS
#define WAIT_FOR_SCI_TX while(!ScibRegs.SCICTL2.bit.TXRDY)
#define WAIT_FOR_SCI_RX while(!ScibRegs.SCIRXST.bit.RXRDY)
//************************************************************************
//PRE-DEFINE CONSTANT
#define WORD_16 2
#define WORD_32 4
#define WORD_64 86
#define MAX_BUF_SIZE 50
#define DUMMY_BYTE 0x00
//*************************************************************************
//ANSI SEQUENCE TYPE
#define CLEAR_SCREEN_SEQUENCE 0
#define CURSOR_HOME_SEQUENCE 1
#define CURSOR_BACKSPACE_SEQUENCE 2
#define BACKSPACE_DELETE_SEQUENCE 3
//************************************************************************
//FUNCTION PROTOTYPE
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
void usciSCIBinit(int16 loopBackEn);

//*************************************************************************
// Function: usciSCIgpioInit
// - initialize the RX and TX pin for the SCIB module
// Arguments: none
//
//
// return: none
// Author: Will Nguyen
// Date: Mar 30th, 2023
// Modified: Mar 30th, 2023
//*************************************************************************
void usciSCIBgpioInit(void);

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
void usciSCItxChar(unsigned char txChar);

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
void usciSCItxStr(unsigned char *txByte);

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
unsigned char usciSCIgets(unsigned char *rxStr);

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
void sciFlushInputBuffer(void);

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
void ansiSeqExecute(int16 sequenceType);

#endif /* USCISCI_H_ */
