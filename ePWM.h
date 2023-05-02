//************************************************************************
//
// FILE:   ePWM.h
//
// TITLE:  header file for the PWM module to generate a periodic squarewave
// for controller speed of a DC motor
//
// using function from ti library for the ePWM module
//
//
//************************************************************************
#ifndef EPWM_H_
#define EPWM_H_

#include "F2837xD_device.h"
//************************************************************************
//PRE-DEFINE CONSTANT
#define INITIALIZE_COUNT 0
//************************************************************************
//PWM FREQUENCY
#define TIMER_BASE_CLK 3125000
#define PWM_FREQ 5000
#define TIMER_PRD TIMER_BASE_CLK/PWM_FREQ
//*************************************************************************
//MOTOR DRIVER SPEED MANIPULATE
#define SPEED_0 0
#define SPEED_10 10
#define SPEED_20 20
#define SPEED_30 30
#define SPEED_40 40
#define SPEED_50 50
#define SPEED_60 60
#define SPEED_70 70
#define SPEED_80 80
#define SPEED_90 90
#define SPEED_100 100
//************************************************************************
//DUTY CYCLE
#define DUTY_0 0
#define DUTY_10 0.1
#define DUTY_20 0.2
#define DUTY_30 0.3
#define DUTY_40 0.4
#define DUTY_50 0.5
#define DUTY_60 0.6
#define DUTY_70 0.7
#define DUTY_80 0.8
#define DUTY_90 0.9
#define DUTY_100 1
//************************************************************************
//GPIO
#define EPWM1A 0
#define EPWM2A 2
//************************************************************************
//FUNCTION PROTOTYPE
//*************************************************************************
// Function: ePWM1init()
// - initialize the ePWM1 module, default at 0% DC
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 8th, 2023
// Modified: April 8th, 2023
//************************************************************************
void ePWM1init();

//*************************************************************************
// Function: ePWM2init()
// - initialize the ePWM2 module, default at 0% DC
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void ePWM2init();

//*************************************************************************
// Function: ePWM1gpioInit()
// - initialize the GPIO for ePWM1A, ePWM1A at GPIO0
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 8th, 2023
// Modified: April 8th, 2023
//************************************************************************
void ePWM1gpioInit();

//*************************************************************************
// Function: ePWM2gpioInit()
// - initialize the GPIO for ePWM2A, ePWM1A at GPIO2
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 13th, 2023
// Modified: April 13th, 2023
//************************************************************************
void ePWM2gpioInit();

//*************************************************************************
// Function: ePWM1dutyCtl
// - configure the duty cycle for ePWM1, this part drive the x axis motor
//
// Arguments: int16 duty
//
//  /e duty is a whole number represent the duty cycle user want to set the ePWM to
//
// return: the speed it was set to
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
int16 ePWM1dutyCtl(int16 duty);

//*************************************************************************
// Function: ePWM2dutyCtl
// - configure the duty cycle for ePWM2, this part drive the y axis motor
//
// Arguments: int16 duty
//
//  /e duty is a whole number represent the duty cycle user want to set the ePWM to
//
// return: the speed it was set to
// Author: Will Nguyen
// Date: April 11th, 2023
// Modified: April 11th, 2023
//************************************************************************
int16 ePWM2dutyCtl(int16 duty);


#endif /* EPWM_H_ */
