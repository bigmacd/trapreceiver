//  Copyright (c) 1999-2001 Network Computing Technologies, Inc.
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
//

#ifdef _WIN32
#include "ntservice.h"
#include "NtSysUpTime.h"
#endif

class Packet;

#ifdef _WIN32
class TrapReceiverService : public CNTService
#else
class TrapReceiverService
#endif
{
  private:


  protected:


  public:
	
    TrapReceiverService();
    ~TrapReceiverService();

#ifdef _WIN32
    virtual BOOL OnInit();
    virtual BOOL OnUserControl(DWORD dwOpcode);
    void SaveStatus();

    // Control parameters
    int m_iStartParam;
    int m_iIncParam;

    // Current state
    int m_iState;
#endif
    virtual void Run();
    
};


