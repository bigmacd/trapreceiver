//  Copyright (c) 2013 trapreceiver.com
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

#ifndef __TRTHREAD_H__
#include "TrThread.h"
#endif

//#include "LogFile.h"
#include "Registry.h"
#include "SnmpParser.h"

TrThread::TrThread(int port, BOOL doInforms)
         :TrapRcvrThread(port, doInforms)
{
  mTrapInEvent =  CreateEvent(NULL,
			      FALSE,
			      FALSE,
			      "TrapInEvent");
}

TrThread::~TrThread()
{

}

void
TrThread::Add(Packet* packet)
{
  // do something with the packet
  //  mQueue.Put((GenericPacket*)packet);
}

void
TrThread::Add(unsigned char* buffer,
	      unsigned int len,
	      unsigned long peerAddr)
{
  // send it off to the filter queue
  unsigned char b[4096];
  memset(b, 0, 4096);
  __time32_t t = _time32(0);
  memcpy(b, &t, 4);
  memcpy(&b[4], &peerAddr, 4);
  memcpy(&b[8], buffer, len);
  mQueue.Put(b, len+8);
  SetEvent(mTrapInEvent);
}

