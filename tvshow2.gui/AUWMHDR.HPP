#ifndef AUSERMESSAGEHANDLER_HPP
#define AUSERMESSAGEHANDLER_HPP
#include <ihandler.hpp>

/******************************************************************************/
/* Class   : AUserMessageHandler                                              */
/*                                                                            */
/* Purpose : Instances of this class can be user to process user messages.    */
/*           Objects of this class process messages with the ID that the      */
/*           the object is created with. The class provides the virtual       */
/*           function userMessage which you should override to do your        */
/*           application specific processing.                                 */
/*           It is derived from IHandler                                      */
/*                                                                            */
/******************************************************************************/
class AUserMessageHandler : public IHandler
{
  public:
    AUserMessageHandler( unsigned long uwmId )
      : userMessageId( uwmId ) {;}
  Boolean
    dispatchHandlerEvent( IEvent& evt );

  protected:
    virtual Boolean
       userMessage( IEvent& evt ) { return false; }

  private:
    unsigned long   userMessageId;
};

#endif

