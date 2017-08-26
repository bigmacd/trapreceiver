#ifndef __eventlogmessage_h
#define __eventlogmessage_h

#include <afxwin.h>

class eventlogmessage {
private:
  HANDLE mHandle;
public:
  eventlogmessage();
  ~eventlogmessage();
  void evtlog(const char* message);
};

#endif
