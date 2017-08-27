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


