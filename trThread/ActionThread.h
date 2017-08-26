//  Copyright (c) 1999 Network Computing Technologies, Inc.
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

#ifndef __ACTIONTHREAD_H__
#define __ACTIONTHREAD_H__

#include "Thread.h"
#include "Packet.h"
#include "Smtp.h"

class MibDb;

class ActionThread : public Thread
{

  private:

  //  MibDb mDb;
    Packet*     mPacket;
    Smtp*       mSmtp;
    char        mTimestamp[128];
    unsigned int mWhich;

  protected:



  public:

    ActionThread(Packet* p, char* timestamp, unsigned int which);
    ~ActionThread();


    void main(void);


};

#endif
