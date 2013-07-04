
#ifndef _IBASE_                         //Make sure ibase.hpp is included
  #include <ibase.hpp>                  //  since that is where IC_<environ>
#endif                                  //  is defined.
#include "auwmhdr.hpp"

/******************************************************************************/
/* AUserMessageHandler::dispatchHandlerEvent - catch user events              */
/******************************************************************************/
IBase::Boolean AUserMessageHandler::dispatchHandlerEvent( IEvent& evt )
{
  if ( evt.eventId() == userMessageId )
    return userMessage( evt );

  return false;
}  /* end  AUserMessageHandler::dispatchHandlerEvent(...)  */

