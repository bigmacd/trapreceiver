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
#define __ACTIONMGRTHREAD_H__

#include "Thread.h"
#include "ActionThread.h"
#include "QueueServer.h"


class ActionMgrThread : public Thread
{


  private:

    unsigned long     mInterval;
    HANDLE            mTimerEvent;
    unsigned int      mWhich; // 0 = service, 1 = gui
    ActionThread*     mActionThreads[1024];

    QueueServer*      mQueue;

  protected:




  public:

    ActionMgrThread(unsigned int which);
    ~ActionMgrThread();

    virtual void        main();


};

#endif
