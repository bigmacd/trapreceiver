//  Copyright (c) 2001 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __TRAPFILTER_H__
#define __TRAPFILTER_H__

  //#include "compat.h"
#include "Thread.h"

#ifndef __TRQUEUECLIENT_H__
#include "TrQueueClient.h"
#endif

#include "QueueServer.h"

class Packet;

class TrapFilter : public Thread
{

  private:

    QueueServer*  mInQueue;
    TrQueueClient mOutQueue;    

    HANDLE        mTrapInEvent;

    void Forwared();


  protected:


  public:

    TrapFilter();
    ~TrapFilter();

    virtual void main(void);
};
#endif


