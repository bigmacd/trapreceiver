//  Copyright (c) 2014 trapreceiver.com
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  trapreceiver.com
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  trapreceiver.com
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __ACTIONMGRTHREAD_H__
#include "ActionMgrThread.h"
#endif

#include "..\Registry.h"
#include "vbs.h"
#include "..\eventlogmessage.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

ActionMgrThread::ActionMgrThread(unsigned int which)
                :mInterval(1000),
		 mTimerEvent(INVALID_HANDLE_VALUE),
		 mWhich(which)
{
  mTimerEvent = CreateEvent(0, FALSE, FALSE, 0);
  if (mWhich == 0) { // service 
#ifdef _DEBUG
    cout << "starting queue server for service" << endl;
#endif
    mQueue = new QueueServer("trap", "actionMgr");
    if (mQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
      { eventlogmessage e; e.evtlog("Could not establish communication channel for trap action manager");  }
  }
  else {
    mQueue = new QueueServer("trap", "guiToAMT", TRUE);
    if (mQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
      { eventlogmessage e; e.evtlog("Could not establish communication channel for trap guiToAMT");  }
  }
  for (int x = 0; x < 1024; x++)
    mActionThreads[x] = NULL;
}

ActionMgrThread::~ActionMgrThread()
{
  try
  {CloseHandle(mTimerEvent);}
  catch(...){}
  if (mQueue != NULL)
    delete mQueue;
}

void 
ActionMgrThread::main()
{
  while (State() != Thread::TERMINATED)
  {
    WaitForSingleObject(mTimerEvent, mInterval);
    while (mQueue->MessageAvailable())
    {
      int x = 0;
      for (x = 0; x < 1024; x++)
	if (mActionThreads[x] == NULL)
	  break;

      if (x < 1024)
      {
#ifdef _DEBUG
	cout << "found an empty slot at position " << x << endl;
#endif
	unsigned char buff[4096];
	unsigned int length = 4096;
	memset(buff, 0, 4096);
	mQueue->Get(buff, length);

	SnmpParser parser(&buff[8], length-8);
	Packet* p = parser.packet();

	struct sockaddr_in realA;
	memset(&realA, 0, sizeof(sockaddr_in));
	memcpy(&(realA.sin_addr.s_addr), &buff[4], 4);
	p->RealIp(&realA);

	if (p && (p->Type() == V2TRAP || p->Type() == INFORMPDU))
	{
	  struct sockaddr_in a;
	  memset(&a, 0, sizeof(sockaddr_in));
	  memcpy(&(a.sin_addr.s_addr), &buff[4], 4);
	  V2Notification* pdu = (V2Notification*)p->pdu();
	  pdu->SenderIP(&a);
	}
	if (p != NULL)
	{
	  /////////////////////////////////////////////////////////////
	  // pass along the timestamp for emailing
	  __time32_t t = *((__time32_t*)buff);
	  struct tm* newT = NULL;
	  newT = _localtime32(&t);
	  char cTime[64];
	  memset(cTime, 0, 64);
	  strftime(cTime, 
		   64, 
		   "%X %x",
		   newT);
	  /////////////////////////////////////////////////////////////
	  ActionThread* a = new ActionThread(p, cTime, mWhich);
	  a->Start();
	  mActionThreads[x] = a;
	}
      } // if (x < 1024)
    } // while (mQueue->MessageAvailable())

    // clean up terminated threads
    for (int x = 0; x < 1024; x++)
    {
      if (mActionThreads[x] != NULL)
	if (mActionThreads[x]->State() == Thread::TERMINATED)
	{
	  delete mActionThreads[x];
	  mActionThreads[x] = NULL;
	}
    }

  } // while (State() != Thread::TERMINATED)
}
