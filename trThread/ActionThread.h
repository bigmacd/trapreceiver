
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
