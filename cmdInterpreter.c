/*************************************************************************************************
 * cmdInterpreter.h
 * - C interface file for command interpreter engine
 *
 *  Author: Will Nguyen
 *  Created on: January 19, 2023
 *  Modified: April 14th,2023
 *************************************************************************************************/
#include "cmdInterpreter.h"
#include "usciSCI.h"
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
void cmdInitialize(CMD *cmdList)
{
    //cmd name
    cmdList[INDEX_JOG].name = CMD0;
    cmdList[INDEX_BLOCK_DETECT].name = CMD1;
    cmdList[INDEX_HOME].name = CMD2;

    //nargs
    cmdList[INDEX_JOG].nArgs = CMD0_NARGS;
    cmdList[INDEX_BLOCK_DETECT].nArgs = CMD1_NARGS;
    cmdList[INDEX_HOME].nArgs = CMD2_NARGS;

    //args array initialize
    int i = 0;
    for (i = 0; i < MAX_ARGS; i++)
    {
        cmdList[INDEX_JOG].args[i] = 0;
    }
    i = 0;
    for (i = 0; i < MAX_ARGS; i++)
    {
        cmdList[INDEX_BLOCK_DETECT].args[i] = 0;
    }
    i = 0;
    for (i = 0; i < MAX_ARGS; i++)
    {
        cmdList[INDEX_HOME].args[i] = 0;
    }
}
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
int16 parseCmd(CMD *cmdList, char *cmdLine)
{
    //local variable
    char seps[] = " ";
    //unsigned char cmdName;
    char *tok = NULL;
    char *pEnd = NULL;
    int16 idx, temp;
    int16 i = 0;

    //tokenize cmd name
    tok = strtok(cmdLine, seps);

    //if not command name received
    if (tok == NULL)
    {
        //sprintf(errorStr, "enter something duh\r\n");
        return -1;
    }

    //validate cmd name
    idx = validateCmd(cmdList, tok);
    if (idx == -1)
    {
        //sprintf(errorStr, "Wrong command name, try again nerd\r\n");
        return -1;
    }

    //this loop store the remaining arguments
        do
        {
            tok = strtok(NULL, seps);
            if (tok != NULL)
            {
                temp = i;
                cmdList[idx].args[i] = strtol(tok, &pEnd, 10);
                i = temp + 1;
            }
        }
        while (tok != NULL);

    //validate command nArgs
    if (cmdList[idx].nArgs != i)
    {
        //sprintf(errorStr, "Too many arguments, try again\r\n");
        return -1;
    }
    return idx;
}


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
int16 validateCmd(CMD *cmdList, char *cmdName)
{
    int i = 0;
    int index = -1;
    int invalidCmd = 1;
    //compare command name we got with a set of preset command
    while (invalidCmd && i < NUM_COMMANDS)
    {
        invalidCmd = strcmp(cmdName, cmdList[i++].name);
    }
    if (!invalidCmd)
    {
        index = i - 1;
    }
    return index;
}


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
void cmdSelection()
{
    //local var
    unsigned char txBuf[100];

    //commands selection
    sprintf(txBuf, "Please select one of the command\r\n 1.Jogging (int16 speed)\r\n 2.Auto Block Detection\r\n 3.Home\r\n");

    //clear screen to begin running
    ansiSeqExecute(CLEAR_SCREEN_SEQUENCE);

    ansiSeqExecute(CURSOR_HOME_SEQUENCE);

    usciSCItxStr(txBuf);
}
