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
#include "LogQueueClient.h"
#endif

#include <stdio.h>
//#include <iostream.h>

LogQueueClient::LogQueueClient()
{
  Connect();
}
 
LogQueueClient::~LogQueueClient()
{
  try
  { 
    if (mQueueHandle != INVALID_HANDLE_VALUE)
      CloseHandle(mQueueHandle);
  }
  catch(...) {}
}

void  
LogQueueClient::Put(unsigned char* message, unsigned int length)
{
  unsigned long len = 0;
  BOOL tryWrite = TRUE; // assume good connection
  if (mQueueHandle == INVALID_HANDLE_VALUE) // didn't connect previously
    if (!Connect()) // try now
      tryWrite = FALSE; // still can't

  if (tryWrite)
  {
    if (!WriteFile(mQueueHandle, 
		   message,
		   length,
		   &len,
		   NULL))                     // oops, used to, but cant now!
      if (GetLastError() == ERROR_HANDLE_EOF) // server disconnected
      {
	// clean up
	try {
	  CloseHandle(mQueueHandle);
	}
	catch(...) { }
	mQueueHandle = INVALID_HANDLE_VALUE;

	// try to connect again
	if (Connect()) 
	  WriteFile(mQueueHandle,
		    message,
		    length,
		    &len,
		    NULL);
      }
  }
}

BOOL
LogQueueClient::Connect()
{
  BOOL retVal = FALSE;
  
  mQueueHandle = CreateFile("\\\\.\\mailslot\\trap\\guiToLog",
			    GENERIC_WRITE,
			    FILE_SHARE_READ,
			    NULL,
			    OPEN_EXISTING,
			    FILE_ATTRIBUTE_NORMAL,
			    NULL);
  if (mQueueHandle != INVALID_HANDLE_VALUE)
    retVal = TRUE;
  else
  {
    int err = GetLastError();
    if (err == 99)
      return FALSE;
  }
  return retVal;
}


