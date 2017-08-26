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

#ifndef __QUEUECLIENT_H__
#include "TrQueueClient.h"
#endif

#include <stdio.h>

extern const int NUM_QUEUES;
const char*  mSourceNames[] = 
{
    "logger",
    "gui",
    "actionMgr"
};


//#include <iostream.h>
TrQueueClient::TrQueueClient()
{
  for (int i = 0; i < NUM_QUEUES; i++)
  {
    mQueueHandles[i] = INVALID_HANDLE_VALUE;
    Connect(i);
  }
}
 
TrQueueClient::~TrQueueClient()
{
  try
  { 
    for (int i = 0; i < NUM_QUEUES; i++)
      if (mQueueHandles[i] != INVALID_HANDLE_VALUE)
	CloseHandle(mQueueHandles[i]);
  }
  catch(...) {}
}

void  
TrQueueClient::Put(unsigned char* message, unsigned int length)
{
  for (int i = 0; i < NUM_QUEUES; i++)
  {
    unsigned long len = 0;
    BOOL tryWrite = TRUE; // assume good connection
    if (mQueueHandles[i] == INVALID_HANDLE_VALUE) // didn't connect previously
      if (!Connect(i)) // try now
	tryWrite = FALSE; // still can't

    if (tryWrite)
    {
      if (!WriteFile(mQueueHandles[i], 
		     message,
		     length,
		     &len,
		     NULL))                     // oops, used to, but cant now!
	if (GetLastError() == ERROR_HANDLE_EOF) // server disconnected
	{
	  // clean up
	  try {
	    CloseHandle(mQueueHandles[i]);
	  }
	  catch(...) { }
	  mQueueHandles[i] = INVALID_HANDLE_VALUE;

	  // try to connect again
	  if (Connect(i)) 
	    WriteFile(mQueueHandles[i],
		      message,
		      length,
		      &len,
		      NULL);
	}
    }
  }
}

BOOL
TrQueueClient::Connect(int index)
{
  BOOL retVal = FALSE;
  char filePath[256];
  
  if (index >=0 && index < NUM_QUEUES)
  {
    sprintf_s(filePath, 255, "\\\\.\\mailslot\\trap\\%s", mSourceNames[index]);
    mQueueHandles[index] = CreateFile(filePath,
				      GENERIC_WRITE,
				      FILE_SHARE_READ,
				      NULL,
				      OPEN_EXISTING,
				      FILE_ATTRIBUTE_NORMAL,
				      NULL);
    if (mQueueHandles[index] != INVALID_HANDLE_VALUE)
      retVal = TRUE;
  }
  return retVal;
}


