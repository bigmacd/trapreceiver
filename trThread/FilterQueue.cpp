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

#ifndef __FILTERQUEUE_H__
#include "FilterQueue.h"
#endif

#include <stdio.h>


//#include <iostream.h>
FilterQueue::FilterQueue()
            :mQueueHandle(INVALID_HANDLE_VALUE)
{
  Connect();
}
 
FilterQueue::~FilterQueue()
{
  try
  { 
    CloseHandle(mQueueHandle);
  }
  catch(...) {}
}

void  
FilterQueue::Put(unsigned char* message, unsigned int length)
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
FilterQueue::Connect()
{
  BOOL retVal = FALSE;
  char filePath[256];
  
  sprintf_s(filePath, "\\\\.\\mailslot\\trap\\filter", 255);

  /////////////////////////////////////////////////////////////////

  SECURITY_DESCRIPTOR sd;
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(sa);
  InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
  SetSecurityDescriptorDacl(&sd, TRUE, NULL, TRUE);
  SetSecurityDescriptorGroup(&sd, NULL, TRUE);
  SetSecurityDescriptorOwner(&sd, NULL, TRUE);
  sa.lpSecurityDescriptor = &sd;
  sa.bInheritHandle = TRUE;
  /////////////////////////////////////////////////////////////////

  //  mMailSlotHandle = CreateMailslot(msName,
  //  mQueueHandle = CreateMailslot(filePath,
  //				0,
  //				0,
  //				&sa);
  
  mQueueHandle = CreateFile(filePath,
  			    GENERIC_WRITE,
  			    FILE_SHARE_READ,
  			    &sa,
  //			    NULL,
  			    OPEN_EXISTING,
  			    FILE_ATTRIBUTE_NORMAL,
  			    NULL);
  int x;
  if (mQueueHandle != INVALID_HANDLE_VALUE)
    retVal = TRUE;
  else x = GetLastError();

  return retVal;
}


