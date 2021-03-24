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

#ifndef __AMTQUEUECLIENT_H__
#define __AMTQUEUECLIENT_H__

#include <windows.h>
//#include <afxwin.h>

class AMTQueueClient
{

  private:

    HANDLE      mQueueHandle;

    BOOL        Connect();

  protected:

  public:

    AMTQueueClient();
    ~AMTQueueClient();

    void  Put(unsigned char* message, unsigned int length);

};
#endif
