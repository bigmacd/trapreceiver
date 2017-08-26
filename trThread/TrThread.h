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
#define __TRTHREAD_H__

#include "TrapRcvrThread.h"
#include "Packet.h"
#include "FilterQueue.h"

class TrThread : public TrapRcvrThread
{

  private:

    FilterQueue   mQueue;
    HANDLE        mTrapInEvent;

  protected:

    void  Add(Packet* packet);
    void  Add(unsigned char* buffer, 
	      unsigned int len,
	      unsigned long peerAddr);

  public:

    TrThread(int port, BOOL doInforms);
    ~TrThread();

};

#endif







