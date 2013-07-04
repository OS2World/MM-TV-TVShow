#ifndef _ATIMEHDR_
#define _ATIMEHDR_
/*****************************************************************************
* TIME HANDLER SAMPLE PROGRAM - Version 6: Class Header (atimehdr.hpp)       *
*                                                                            *
* COPYRIGHT: Copyright (C) International Business Machines Corp., 1992,1995. *
*                                                                            *
* DISCLAIMER OF WARRANTIES:                                                  *
*   The following [enclosed] code is sample code created by IBM              *
*   Corporation.  This sample code is not part of any standard IBM product   *
*   and is provided to you solely for the purpose of assisting you in the    *
*   development of your applications.  The code is provided "AS IS",         *
*   without warranty of any kind.  IBM shall not be liable for any damages   *
*   arising out of your use of the sample code, even if they have been       *
*   advised of the possibility of such damages.                              *
*****************************************************************************/
//NOTE: WE RECOMMEND USING A FIXED-SPACE FONT TO LOOK AT THE SOURCE.

#ifndef _IHANDLER_                                                          //V6
  #include <ihandler.hpp>               //Include IHandler class header       V6
#endif                                                                      //V6

/**************************************************************************   V6
* Class:   ATimeHandler                                                   *   V6
*                                                                         *   V6
* Purpose: New Handler class that processes timer events.                 *   V6
*                                                                         *   V6
**************************************************************************/ //V6
class ATimeHandler : public IHandler                                        //V6
{                                                                           //V6
typedef IHandler
  Inherited;
public:                                                                     //V6
  ATimeHandler() : timerId(0) { }       //Initialize timerId data member      V6
virtual ~ATimeHandler() { }                                                 //V6
/*----------------- Override Public IHandler Functions -------------------|   V6
| These public IHandler functions are overridden to add start and stop    |   V6
|   timer calls, respectively.                                            |   V6
|------------------------------------------------------------------------*/ //V6
virtual ATimeHandler                                                        //V6
  &handleEventsFor(IWindow *window),                                        //V6
  &stopHandlingEventsFor(IWindow *window );                                 //V6

protected:                                                                  //V6
/*------------------- Override Protected IHandler Function ---------------|   V6
| The dispatchHandlerEvent function is overridden to call the tick        |   V6
|   function for processing timer events.                                 |   V6
| The tick function is added in ATimeHandler and should be overridden     |   V6
|   by classes that inherit from ATimeHandler.                            |   V6
|------------------------------------------------------------------------*/ //V6
Boolean                                                                     //V6
  dispatchHandlerEvent(IEvent& event);                                      //V6
virtual Boolean                                                             //V6
  tick(IEvent& event);                                                      //V6

private:                                                                    //V6
/*--------------------------- Private Data Member ------------------------|   V6
| The timerId private data member is used by the ATimeHandler functions   |   V6
|   to save the ID of the outstanding timer call.                         |   V6
|------------------------------------------------------------------------*/ //V6
  unsigned long timerId;                                                    //V6
};                                                                          //V6
#endif                                                                      //V6
