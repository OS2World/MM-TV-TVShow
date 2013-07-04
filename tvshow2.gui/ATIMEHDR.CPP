
#ifndef _IBASE_                         //Make sure ibase.hpp is included
  #include <ibase.hpp>                  //  since that is where IC_<environ>
#endif                                  //  is defined.
#ifdef IC_PM                            //For OS/2,                           V6
  #define TIMER_ID      1              //  Set the timer ID as a constant    V6
  #define INCL_WINTIMER                 //  Include the timer routines        V6
  #include <os2.h>                      //    from the OS/2 libraries         V6
  #include <ithread.hpp>                //  Include IThread class header      V6
#endif                                                                      //V6

#include <iwindow.hpp>                  //Include IWindow class header        V6


#include "atimehdr.hpp"                 //Include ATimeHandler class header   V6
const unsigned long                     //Set the timer interval to 1000      V6
  timeInterval = 20;
  //500;//20;                  //  milliseconds (1 second)           V6

/**************************************************************************   V6
* ATimeHandler :: handleEventsFor                                         *   V6
*   Begin processing timer events for the window passed in by starting    *   V6
*     the appropriate system timer.                                       *   V6
**************************************************************************/ //V6
ATimeHandler                                                                //V6
  &ATimeHandler :: handleEventsFor( IWindow *window )                       //V6
{                                                                           //V6


/*--------------------------- Start a Timer ------------------------------|   V6
| The Presentation Manager start timer routine is called by specifying    |   V6
|   the anchor block handle for the current thread,                       |   V6
|   the handle of the window passed in,                                   |   V6
|   a unique timer ID that is below the PM TID_USER_MAX constant, and     |   V6
|   a time interval in milliseconds which is set to a constant 1000UL.    |   V6
| Since this application uses only a single timer, the timer ID is        |   V6
|   specified as a constant.                                              |   V6
| When the timer expires, PM will post a WM_TIMER event to the window     |   V6
|   specified in parameter 2 of the WinStartTimer call.  IParameter1      |   V6
|   of the timer event will contain the timer ID.                         |   V6
|------------------------------------------------------------------------*/ //V6
  timerId = TIMER_ID;                                                       //V6
  WinStartTimer( IThread::current().anchorBlock(),                          //V6
                 window->handle(), timerId, timeInterval);                  //V6

/*------------------------- Start the Handler ----------------------------|   V6
| Start the handler by explicitly calling the overridden function.        |   V6
|------------------------------------------------------------------------*/ //V6
  Inherited::handleEventsFor(window);                                       //V6
  return (*this);                                                           //V6
} /* end ATimeHandler :: handleEventsFor(...) */                            //V6


/**************************************************************************   V6
* ATimeHandler :: dispatchHandlerEvent                                    *   V6
*   Call tick when one of our timer events are dispatched.                *   V6
**************************************************************************/ //V6
IBase::Boolean                                                              //V6
  ATimeHandler :: dispatchHandlerEvent(IEvent& event)                       //V6
{                                                                           //V6
  Boolean eventProcessed(false);        //Assume event will not be proccessed V6
/*--------------------------- Test the Event -----------------------------|   V6
| This event must be a timer event and parameter 1 must contain the       |   V6
|   ID of the timer that was started by this handler.                     |   V6
|------------------------------------------------------------------------*/ //V6
  if ((event.eventId() == WM_TIMER) && (event.parameter1() == timerId))     //V6
  {                                                                         //V6
/*------------------------- Process Timer Event --------------------------|   V6
| Process the timer event by calling the virtual function tick, which     |   V6
|   should be overridden by an inheriting class.  The returned boolean    |   V6
|   value determines whether the event was actually processed by tick.    |   V6
|   The default ATimeHandler::tick function does not process the event.   |   V6
|------------------------------------------------------------------------*/ //V6
    eventProcessed = tick(event);                                           //V6
  }                                                                         //V6
  return (eventProcessed);                                                  //V6
} /* end ATimeHandler :: dispatchHandlerEvent(...) */                       //V6

/**************************************************************************   V6
* ATimeHandler :: tick                                                    *   V6
*   Default tick handler that should be overridden by inheriting classes  *   V6
**************************************************************************/ //V6
IBase::Boolean                                                              //V6
  ATimeHandler :: tick(IEvent& event)                                       //V6
{                                                                           //V6
  return (false);                       //The timer event is not processed  //V6
} /* end ATimeHandler :: tick(...) */                                       //V6

/**************************************************************************   V6
* ATimeHandler :: stopHandlingEventsFor                                   *   V6
*   Stop processing timer events for the window passed in.                *   V6
**************************************************************************/ //V6
ATimeHandler                                                                //V6
  &ATimeHandler :: stopHandlingEventsFor( IWindow *window )                 //V6
{                                                                           //V6

/*--------------------------- Stop the Timer -----------------------------|   V6
| The Presentation Manager stop timer routine is called to stop the       |   V6
|   outstanding timer which is identified by,                             |   V6
|     the anchor block handle of the current thread,                      |   V6
|     the handle of the window passed in, and                             |   V6
|     the ID stored in timerId.                                           |   V6
|------------------------------------------------------------------------*/ //V6
  WinStopTimer( IThread::current().anchorBlock(),                           //V6
                window->handle(), timerId);                                 //V6


/*-------------------------- Stop the Handler ----------------------------|   V6
| Stop the handler by explicitly calling the overridden function.  Its    |   V6
|   returned value is used to determine the success of this routine.      |   V6
|------------------------------------------------------------------------*/ //V6
  Inherited::stopHandlingEventsFor(window);                                 //V6
  return (*this);                                                           //V6
} /* end ATimeHandler :: stopHandlingEventsFor(...) */                      //V6

