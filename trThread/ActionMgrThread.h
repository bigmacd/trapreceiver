
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
