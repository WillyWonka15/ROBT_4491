/*************************************************************************************************
 * cmdInterpreter.h
 * - C interface file for command interpreter engine
 *
 *  Author: Will Nguyen
 *  Created on: January 19, 2023
 *  Modified: April 14th,2023
 *************************************************************************************************/
#ifndef CMDINTERPRETER_H_
#define CMDINTERPRETER_H_
/**************************************************************************************************/
#include "F2837xD_device.h"
/**************************************************************************************************/
#define     MAX_ARGS        20
#define     CMD0            "1"
#define     CMD1            "2"
#define     CMD2            "3"
#define     CMD0_NARGS      1
#define     CMD1_NARGS      0
#define     CMD2_NARGS      0
#define     BYTE_SZ         256
#define     CONT            0
#define     DISP            1
/**************************************************************************************************/
//Structure
typedef struct CMD{
    const char *name;
    int16 nArgs;
    int16 args[MAX_ARGS];
}CMD;
//Enum of command index
enum COMMAND_LIST_INDEX
{
   INDEX_JOG, INDEX_BLOCK_DETECT,INDEX_HOME, NUM_COMMANDS
};
//Global Variable
//extern char errorStr[50];
/**************************************************************************************************/
//Function Prototype
/************************************************************************************
 * Function: cmdInitialize
 * - receive an array of type CMD to initialize
 * argument:
 * Arguments: CMD *cmdList point to the array of type CMD from main
 *
 * return: NONE
 * Author: Will Nguyen
 * Date: Jan 12th,2023
 * Modified: April 14th,2023
 ************************************************************************************/
void cmdInitialize (CMD *cmdList);

/************************************************************************************
 * Function: parseCmd
 * - parse command names and nargs from user entered command
 * argument:
 * Arguments: CMD *cmdList point contain all command names and nargs, char *cmdLine enter by user
 *
 * return: -1 if command not valid
 * Author: Will Nguyen
 * Date: Jan 12th, 2023
 * Modified: April 14th, 2023
 ************************************************************************************/
int16 parseCmd (CMD *cmdList, char * cmdLine);

/************************************************************************************
 * Function: validateCmd
 * - validate command names
 * argument:
 * Arguments: CMD *cmdList point contain all command names and nargs, char *cmdName enter by user
 *
 * return: index of command base on give command name, -1 if command is invalid
 * Author: Will Nguyen
 * Date: Jan 12th, 2023
 * Modified: April 14th,2023
 ************************************************************************************/
int16 validateCmd (CMD *cmdList, char *cmdName);

/************************************************************************************
 * Function: cmdSelection
 * - a printout description of each command
 * argument: None
 *
 * Arguments: None
 *
 * return: none
 * Author: Will Nguyen
 * Date: April 14th, 2023
 * Modified: April 14th,2023
 ************************************************************************************/
void cmdSelection();

#endif /* CMDINTERPRETER_H_ */
