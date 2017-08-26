#ifndef __eventlogmessage_h
#include "eventlogmessage.h"
#endif

eventlogmessage::eventlogmessage() 
                :mHandle(INVALID_HANDLE_VALUE) {
  mHandle = ::RegisterEventSource(NULL, "TrapRcvr");
}

eventlogmessage::~eventlogmessage() {
  if (mHandle != INVALID_HANDLE_VALUE) {
    ::DeregisterEventSource(mHandle);
  }
}

void 
eventlogmessage::evtlog(const char* message)  {

  if (mHandle != INVALID_HANDLE_VALUE)
    ::ReportEvent(mHandle, 
		  EVENTLOG_SUCCESS, 
		  0, 
		  0, 
		  NULL, 
		  1, 
		  0, 
		  &message,
		  NULL);
}

