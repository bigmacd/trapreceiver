
#ifndef __TRAPFILTER_H__
#include "TrapFilter.h"
#endif

#include "Registry.h"
#include "Packet.h"
#include "TrActionAdd.h"
#include "SnmpParser.h"
#include "TrapMatcher.h"
#include "..\eventlogmessage.h"

#define GREATERTHAN 0
#define EQUALS 1
#define LESSTHAN 2
#define GTEQ 3
#define LTEQ 4
#define NOTEQUALS 5

TrapFilter::TrapFilter()
{
  mInQueue = new QueueServer("trap", "filter");
  if (mInQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
    { eventlogmessage e; e.evtlog("Could not establish communication channel for trap filter");  }

  mTrapInEvent =  CreateEvent(NULL,
			      FALSE,
			      FALSE,
			      "TrapInEvent");
}

TrapFilter::~TrapFilter()
{

}

void 
TrapFilter::main()
{

  while (State() != Thread::TERMINATED)
  {
    try
    {
      unsigned char buff[4096];
      unsigned int length = 4096;
      memset(buff, 0, 4096);

      WaitForSingleObject(mTrapInEvent, INFINITE);

      while (mInQueue->Get(buff, length))
      {      
	Registry registry("Actions");
	unsigned int count = registry.getCount();

	BOOL dropPacket = FALSE;
	Packet* p = NULL;
	for (unsigned int x = 0; x < count; x++)	
	{
	  registry.ReOpen(x);
	  int b = registry.bools();
	  if (b & 16) // discard
	  {
	    SnmpParser parser(buff+8, length-8);
	    p = parser.packet();
	    if (p && (p->Type() == V2TRAP || p->Type() == INFORMPDU || p->Type() == V1TRAP))
	    {
	      TrapMatcher matcher;
	      if (matcher.Matches(p, x))
	      {
		dropPacket = TRUE;
		break;
	      }
	      //	      delete p;
	    }
	  } // if (b & 16) // discard
	} // for (unsigned int x = 0; x < count; x++)

	if (NULL != p)
	  delete p;

	if (!dropPacket)
	  // do something with the packet 
	  mOutQueue.Put(buff, length);

      } // while (mQueue->Get(buff, length))
    } // try
    catch(...)
    {

    }
    //    ResetEvent(mTrapInEvent);
  } // while (State() != Thread::TERMINATED)
}
