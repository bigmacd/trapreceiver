#include "TrapReceiverService.h"

#ifndef _WIN32
#include <signal.h>
#include <stdio.h>
#include <fstream.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <iostream>
using namespace std;

int main(int argc, char** argv);



int 
main(int argc, char** argv)
{
#ifndef _WIN32
  try
  {

    // daemon stuff first
    int cPid;
    if ((cPid = fork()) > 0)
      exit(0);

    setpgrp();
    signal(SIGHUP, SIG_IGN);
    
    if ((cPid = fork()) > 0)
      exit(0);
    
    for (int fd = 0; fd < NOFILE; fd++)
      close(fd);
    
    chdir("/");
    
    umask(0);

    signal(SIGCLD, SIG_IGN);
    // end of daemon stuff

    // for startup and shutdown scripts
    ofstream pidFile("/var/run/procmon.pid");
    pidFile << getpid() << endl;
    pidFile.close();

    TrapReceiverService p;
    p.Run();
  }
  catch(...)
  {
  }

  return 0;
#else

  // Create the service object
  TrapReceiverService service;
  
#ifdef _DEBUG
  cout << "calling run on the service" << endl;
  try
  {
    service.m_bIsRunning = TRUE;
    service.Run();
  }
  catch(...)
  {
  }
#else
  // Parse for standard arguments (install, uninstall, version etc.)
  if (!service.ParseStandardArgs(argc, argv)) 
  {  
    // Didn't find any standard args so start the service
    // Uncomment the DebugBreak line below to enter the debugger
    // when the service is started.
    //DebugBreak();
    service.StartService();
  }
#endif
  
  // When we get here, the service has been stopped
  return service.m_Status.dwWin32ExitCode;
#endif
}

