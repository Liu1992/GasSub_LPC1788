/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH          
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File    : GUI_X_uCOS-II.C
* Purpose : Config / System dependent externals for GUI
*
* 
* Version-Date---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-II interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include "OS_CPU.H"
#include "uCOS_II.H"
#include "stdio.H"
#include "lpc177x_8x_lcd.h"
/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/
static  OS_EVENT  *DispSem;

static  int        KeyPressed;
static  char       KeyIsInited;
static  OS_EVENT  *KeySem;


/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/
int  GUI_X_GetTime (void) 
{
    return ((int)OSTimeGet());
}

void  GUI_X_Delay (int period) 
{
    OSTimeDly(period);
}

void GUI_X_ExecIdle(void) 
{
   OSTimeDly(10);
}
/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void  GUI_X_InitOS (void)
{ 

   DispSem = OSSemCreate(1);
}


void  GUI_X_Lock (void)
{ 
    unsigned  char  err;
    OSSemPend(DispSem, 0, &err);
}


void  GUI_X_Unlock (void)
{ 
    OSSemPost(DispSem);
}


unsigned int  GUI_X_GetTaskId (void) 
{ 
    return ((unsigned  int)(OSTCBCur->OSTCBPrio));
}


/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/
void GUI_X_Init (void) 
{
    KeySem = OSSemCreate(0);
}

static  void  CheckInit (void) 
{
//	if (KeyIsInited == FALSE) {
//        KeyIsInited = TRUE;
//        GUI_X_Init();
//		}
}

int  GUI_X_GetKey (void) 
{
    int r = 0;

 //   r  = KeyPressed;
 //   CheckInit();
 //   KeyPressed = 0;
    return (r);
}

int  GUI_X_WaitKey (void) 
{
    int    r = 0;
//    unsigned  char  err;


//    CheckInit();
//   if (KeyPressed == 0) {
//        OSSemPend(KeySem, 0, &err);
//    }
//    r          = KeyPressed;
//    KeyPressed = 0;
    return (r);
}

void  GUI_X_StoreKey (int k) 
{
//    KeyPressed = k;
//    OSSemPost(KeySem);
}

void GUI_X_Log(const char *s){}

void GUI_X_Warn(const char *s){}

void GUI_X_ErrorOut(const char *s){}

unsigned  char GetKey(void) {return 0xff;}


