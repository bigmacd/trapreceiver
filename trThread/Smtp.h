#ifndef __SMTP_H__
#define __SMTP_H__

#include "Thread.h"
#include "Packet.h"
#include "Mibdb.h"
#include "atlstr.h"

class Smtp : public Thread
{

  private:

    Packet*        mPacket;
    CString        mEmailIds;
    CString        ExpandKeywords(CString message);
    MibDb          mDb;
    char           mTimestamp[128];
    CString        doVbOidLookups(int which, BOOL doTranslation, BOOL doLogging);

  protected:



  public:

    Smtp();
    Smtp(Packet* packet, CString emailIds, char* timestamp);
    ~Smtp();

    virtual void main();

};

#endif
