
#pragma warning(disable: 4786)

#include "TrapReceiverService.h"
#include "Packet.h"
#include "vbs.h"
#include "UdpClient.h"
#include "Registry.h"
#include "TrThread.h"
#include "FlusherThread.h"
#include "ActionMgrThread.h"
#include "TrapFilter.h"

#include <fstream>
#include <shlobj.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "Mibdb.h"

TrapReceiverService::TrapReceiverService()
#ifdef _WIN32
                    :CNTService("TrapRcvr", 2, 6)
#endif
{
#ifdef _WIN32
  m_iStartParam = 0;
  m_iIncParam = 1;
  m_iState = m_iStartParam;
#endif

}

TrapReceiverService::~TrapReceiverService()
{
}

#ifdef _WIN32
BOOL 
TrapReceiverService::OnInit()
{
  // Read the registry parameters
  // Try opening the registry key:
  // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
  
  HKEY hkey;
  char szKey[1024];
  strcpy_s(szKey, 1024, "SYSTEM\\CurrentControlSet\\Services\\");
  strcat_s(szKey, 1024, m_szServiceName);
  strcat_s(szKey, 1024, "\\Parameters");
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		   szKey,
		   0,
		   KEY_QUERY_VALUE,
		   &hkey) == ERROR_SUCCESS) 
  {
    // Yes we are installed
    DWORD dwType = 0;
    DWORD dwSize = sizeof(m_iStartParam);
    RegQueryValueEx(hkey,
		    "Start",
		    NULL,
		    &dwType,
		    (BYTE*)&m_iStartParam,
		    &dwSize);
    dwSize = sizeof(m_iIncParam);
    RegQueryValueEx(hkey,
		    "Inc",
		    NULL,
		    &dwType,
		    (BYTE*)&m_iIncParam,
		    &dwSize);
    RegCloseKey(hkey);
  }

  // Set the initial state
  m_iState = m_iStartParam;
    
  return TRUE;
}
#endif

void 
TrapReceiverService::Run()
{
  ///////////////////////////////////////////////////////////////////////////
  //PWSTR path = NULL;
  //HRESULT hr = SHGetFolderPath(
  //				    FOLDERID_LocalAppData,
  //				    //				    FOLDERID_ProgramFiles, 
  //				    0, NULL, &path);    
  //if (SUCCEEDED(hr)) {
  //  cout << "path: " << path << endl;
  //}
  //CoTaskMemFree(path);
  //::SHGetFolderPath(NULL, csidl, NULL, 0, &path);
  ///////////////////////////////////////////////////////////////////////////


  SetDescription("TrapRcvr listens for incoming SNMP Traps and distributes them throughout the Trap Receiver application for further processing.  This is the heart of the amazing Trap Receiver from trapreceiver.com"); 
#ifdef _WIN32
  BOOL doInforms = FALSE;
  int  port = 162;
  Registry registry;
  try
  {
    port = registry.port();
    doInforms = registry.informs();
  }
  catch(...)
  {
    doInforms = FALSE;
    registry.informs(doInforms);
  }
  
  CString cd;
  CString lad;
  char as [256];
  try { cd = registry.cwd(); } 
  catch(...) { 
	GetModuleFileName(NULL, as, 255); 
	char* last = strrchr(as, '\\');
    if (last != NULL)
      *last = 0;
	cd = as;
  }
  

  char localAppData[MAX_PATH];
  SHGetFolderPath(NULL, CSIDL_APPDATA, 0, NULL, localAppData);
  lad = localAppData;
  CopyFile(cd + "\\Mibdb.db", lad + "\\Mibdb.db", TRUE);
  CopyFile(cd + "\\sendEmail.exe", lad + "\\sendEmail.exe", TRUE);
  registry.cwd(localAppData);
  
  FlusherThread*   flusherThread;
  ActionMgrThread* actionMgrThread;
  TrapFilter*      filterThread;

  // this should start first or second
  flusherThread = new FlusherThread();
  flusherThread->Start();

  // this should start first or second
  actionMgrThread = new ActionMgrThread(0);
  actionMgrThread->Start();
  
  // this should start third
  filterThread = new TrapFilter();
  filterThread->Start();

  // this should start last
  TrThread* trThread = new TrThread(port,
				    doInforms);
  trThread->Start();
  
  HANDLE h = CreateEvent(NULL, FALSE, FALSE, "trTimer");
  while (m_bIsRunning) 
  {
    unsigned long retVal = WaitForSingleObject(h, 10000);
    m_iState += m_iIncParam;
  } // while (m_bIsRunning) 
  
  if (trThread != NULL)
  {
    trThread->Exit();
    delete trThread;
    trThread = NULL;
  }

  if (flusherThread != NULL)
  {
    flusherThread->Exit();
    SetEvent(flusherThread->TimerEvent());
    WaitForSingleObject(flusherThread->ThreadId(), INFINITE);
    delete flusherThread;
    flusherThread = NULL;
  }

  if (actionMgrThread != NULL)
  {
    actionMgrThread->Exit();
    WaitForSingleObject(actionMgrThread->ThreadId(), INFINITE);
    delete actionMgrThread;
    actionMgrThread = NULL;
  }

#endif
}

#ifdef _WIN32
// Process user control requests
BOOL 
TrapReceiverService::OnUserControl(DWORD dwOpcode)
{
  switch (dwOpcode) 
  {
   case SERVICE_CONTROL_USER + 0:
       
       // Save the current status in the registry
       SaveStatus();
       return TRUE;
       
   default:
       break;
    }
  return FALSE; // say not handled
}

// Save the current status in the registry
void 
TrapReceiverService::SaveStatus()
{
  // Try opening the registry key:
  // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
  HKEY hkey = NULL;
  char szKey[1024];
  strcpy_s(szKey, 1024, "SYSTEM\\CurrentControlSet\\Services\\");
  strcat_s(szKey, 1024, m_szServiceName);
  strcat_s(szKey, 1024, "\\Status");
  DWORD dwDisp;
  DWORD dwErr;
  dwErr = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
			 szKey,
			 0,
			 "",
			 REG_OPTION_NON_VOLATILE,
			 KEY_WRITE,
			 NULL,
			 &hkey,
			 &dwDisp);
  if (dwErr != ERROR_SUCCESS) 
  {
    return;
  }	
  
  // Set the registry values
  RegSetValueEx(hkey,
		"Current",
		0,
		REG_DWORD,
		(BYTE*)&m_iState,
		sizeof(m_iState));
  
  
  // Finished with key
  RegCloseKey(hkey);
}
#endif

