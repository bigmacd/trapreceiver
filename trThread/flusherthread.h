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

#ifndef __FLUSHERTHREAD_H__
#define __FLUSHERTHREAD_H__

#include "Thread.h"

#include "QueueServer.h"
#include "Mibdb.h"

#include <fstream>

class Packet;

class FlusherThread : public Thread
{


  private:

    unsigned long     mFlushInterval;
    HANDLE            mTimerEvent;

    std::ofstream     mOfile;      
    std::ofstream     mLogFile;

    QueueServer*      mGuiQueue;
    QueueServer*      mQueue;

    int               mFString[9];
    int               mOids[3];

    void              ParseFormatString();
    void              OutputTrap(Packet* p);

    MibDb             mDb;

    char              mSChar;

  protected:




  public:

    FlusherThread();
    ~FlusherThread();

    virtual void        main();

    HANDLE              TimerEvent() { return mTimerEvent; }

};

#endif






