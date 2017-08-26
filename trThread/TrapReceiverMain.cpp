#include <iostream.h>

#include "TrapReceiverService.h"

BOOL hFunction(unsigned long c);
int main(int argc, char** argv);

TrapReceiverService p;

BOOL hFunction(unsigned long c)
{
  switch(c)
  {
   case CTRL_C_EVENT:
     p.m_bIsRunning = FALSE;
  }
  return TRUE;
}

int 
main(int argc, char** argv)
{
  BOOL b;
  b = SetConsoleCtrlHandler((PHANDLER_ROUTINE)hFunction, TRUE);
  if (!b)
    cout << 
      "set console control handler failed <" << 
      GetLastError() << ">" << endl;
  try
  {
    p.m_bIsRunning = TRUE;
    p.Run();
  }
  catch(...)
  {
  }
  cout << "outta here" << endl;
  Sleep(5000);
  return 1;
}
