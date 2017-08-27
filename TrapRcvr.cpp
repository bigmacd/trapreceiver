
#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrapRcvrDlg.h"
#include "TrAlert.h"
#include "TrMonitor.h"
#include "Registry.h"
#include "Base64.h"
#include "TrAttention.h"

#include "MibDb.h"

#include <winsvc.h>


BEGIN_MESSAGE_MAP(TrapRcvrApp, CWinApp)
	//{{AFX_MSG_MAP(TrapRcvrApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrapRcvrApp construction

TrapRcvrApp::TrapRcvrApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only TrapRcvrApp object

TrapRcvrApp theApp;
MibDb       gDb;

/////////////////////////////////////////////////////////////////////////////
// TrapRcvrApp initialization

BOOL 
TrapRcvrApp::InitInstance()
{
  Elevate();
  CreateMutex(NULL,
	      TRUE,
	      _T("TR SI")); 
  if (GetLastError() == ERROR_ALREADY_EXISTS)
  {
    CWnd *pWndPrev;
    CWnd *pWndChild;
    
    // Determine if another window with our class name/Window title exists
    // The title "Instance " is set up latter, in the InitDialog function.
    if (pWndPrev = CWnd::FindWindow(_T("#32770"),"Trap Receiver"))
    {
      // if so, does it have any popups?
      pWndChild = pWndPrev->GetLastActivePopup(); 
      
      if (pWndPrev->IsIconic()) 
	// If iconic, restore the main window
	pWndPrev->ShowWindow(SW_RESTORE);      
      
      // Bring the main window or it's popup to the foreground
      pWndChild->SetForegroundWindow();         
      
      // and we are done activating the previous one.
      return FALSE;                             
    }
  }
//  if (!AfxSocketInit())
//  {
//    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//    return FALSE;
//  }

  // Standard initialization

#ifdef _AFXDLL
  Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//  Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  mServiceIsRunning = TRUE;
  if (!ScmForTrapsIsRunning())
  {
	//////////////////////////////////
    // If we are on Windows 7 or 2008, don't start a TrAlert Dialog
	// because the UAC will prevent us from starting it.
    // Just tell the user to start it.
    OSVERSIONINFO osVer;
	memset(&osVer, 0, sizeof(OSVERSIONINFO));
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osVer);
	// if (osVer.dwMajorVersion >= 6)
	if (1)
	{
	   TrAttention dlg;
	   dlg.DoModal();
	}
	else 
	{
	  TrAlert dlg;
      int result = dlg.DoModal();
      if (result == IDOK)  // user said go ahead and start it
      {
        if (!StartupScmForTraps())
        {
         AfxMessageBox("Failed to Start Up TrapRcvr service.\nCheck the Event Log");
         mServiceIsRunning = FALSE;
        }
      }
      else
        mServiceIsRunning = FALSE;
    }
  }

  // adjust the registry for version 5.5
  try
  {
    Registry r;
    CString version = r.versionString();
    if (version == "5.50")
    {
      try 
      {
	if (r.synchronized()) 
	{
	  // nothing to do for this version
	} 
      }
      catch(...)
      { // here if version 5.50 (first time for this functionality)
	// add ids to all email
	CString emailIds;
	emailIds.Empty();
	BOOL inList = FALSE;

	r.ReOpen("Email");
	int count = r.getCount();
	for (int x = 0; x < count; x++)
	{
	  if (inList)
	    emailIds += ',';

	  r.ReOpen(x);
	  __time32_t t = _time32(0);
	  r.emailId(t);
	  Sleep(1500);
	  
	  CString c;
	  c.Format("%d", t);
	  emailIds += c.Right(6);

	  inList = TRUE;

	  // modify all email auth to encoded
	  // since we have the key opened
	  c = r.authUser(); // user
	  CBase64 cb6;
	  cb6.Encode(c.GetBuffer(0));
	  c = cb6.EncodedMessage();
	  r.authUser(c);

	  c = r.authJuniper(); // password
	  CBase64 cb62;
	  cb62.Encode(c.GetBuffer(0));
	  c = cb62.EncodedMessage();
	  r.authJuniper(c);
	  // cool, done with email auth too
	}

	// add all emails to actions
	r.ReOpen("Actions");
	count = r.getCount();
	for (int y = 0; y < count; y++)
	{
	  r.ReOpen(y);
	  r.emailRecipients(emailIds);
	  r.soundPlayDuration(0);
	  r.soundFile("");
	}	

	// sync up the registry with our version
	r.ReOpen("");
	r.synchronized("5.50");
	r.version(r.versionString());

      }
    }
  }
  catch(...)
  {

  }

  UpdateForExploderDestinations();
  // change for groups in 7.47
  UpdateForActionLogic();

  Registry r;
  try {
    r.cwd();
  }
  catch(...) {
    char as [256]; 
    GetModuleFileName(NULL, as, 255); 
    char* last = strrchr(as, '\\');
    if (last != NULL)
      *last = 0;
    r.cwd(as);
  }  

  CString d = r.cwd();
  d += "\\Mibdb.db";
  gDb.Connect(d.GetBuffer(0));

  try
  {
    TrapRcvrDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
  }
  catch(...)
  {

  }

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}

BOOL
TrapRcvrApp::ScmForTrapsIsRunning()
{
  BOOL   retVal    = FALSE;
  SC_HANDLE scManager = NULL;

  scManager = OpenSCManager(NULL,
			    NULL,
			    //SC_MANAGER_ENUMERATE_SERVICE); 
				NULL);
  SC_HANDLE service = NULL;
  if (scManager != NULL)
    service = OpenService(scManager,
			  "TrapRcvr",
			  //SERVICE_ALL_ACCESS);
			  SERVICE_QUERY_STATUS);

  if (service != NULL)
  {
    SERVICE_STATUS scs;
    if (QueryServiceStatus(service,
			   &scs))
      if (scs.dwCurrentState == SERVICE_RUNNING)
	retVal = TRUE;
  }

  try
  {
    if (scManager != NULL)
      CloseServiceHandle(scManager);
    if (service != NULL)
      CloseServiceHandle(service);
  }
  catch (...)
  {}

  return retVal;
}

BOOL
TrapRcvrApp::StartupScmForTraps()
{
  BOOL retVal = FALSE;
  SC_HANDLE scManager = NULL;
  
  scManager = OpenSCManager(NULL,
			    NULL,
			    NULL);
				
  SC_HANDLE service  = NULL;
  if (scManager != NULL)
    service = OpenService(scManager,
			  "TrapRcvr",
			  SERVICE_ALL_ACCESS);

   if (service != NULL)
    if (StartService(service,
		     0,
		     NULL))
    {
      TrMonitor dlg(0);
      dlg.DoModal();
      SERVICE_STATUS scs;
      if (QueryServiceStatus(service,
			     &scs))
	if (scs.dwCurrentState == SERVICE_RUNNING)
	  retVal = TRUE;
    }

  try
  {
    if (scManager != NULL)
      CloseServiceHandle(scManager);
    if (service != NULL)
      CloseServiceHandle(service);
  }
  catch(...)
  {}

  return retVal;
}

void
TrapRcvrApp::UpdateForExploderDestinations()
{
  // adjust the registry for version 5.5
  try
  {
    Registry r;
    CString version = r.versionString();
    if (version == "5.50")
    {
      // here if version 6.11 (first time for this functionality)
      // add ids to all exploders
      CString exploderIds;
      exploderIds.Empty();
      BOOL inList = FALSE;
      
      r.ReOpen("Exploder");
      int count = r.getCount();
      for (int x = 0; x < count; x++)
      {
	if (inList)
	  exploderIds += ',';
	
	r.ReOpen(x);
	int t = time(0);
	r.exploderId(t);
	Sleep(1500);
	
	CString c;
	c.Format("%d", t);
	exploderIds += c.Right(6);
	inList = TRUE;
      }
      
      // add all exploders to actions
      r.ReOpen("Actions");
      count = r.getCount();
      for (int y = 0; y < count; y++)
      {
	r.ReOpen(y);
	r.exploderDestinations(exploderIds);
      }	
      
      // sync up the registry with our version
      r.ReOpen("");
      r.synchronized("6.11");
      r.version(r.versionString());

    } // if (version == "5.50")
  }
  catch(...)
  {

  }

}

void
TrapRcvrApp::UpdateForActionLogic()
{
  // adjust the registry for version 7.47
  try
  {
    Registry r;
    CString dbVersion = r.version();
    if (dbVersion != "7.47")
    {

      // here if version 7.47 (first time for this functionality)
      r.ReOpen("Actions");
      int count = r.getCount();
      CString oGroup;
      for (int x = 0; x < count; x++)
      {
	r.ReOpen(x);
	// add a name to each watch
	CString inName;
	inName.Format("%s%d", "watch", x);
	r.name(inName);
	r.partOfGroup(FALSE);
      }
      
      // sync up the registry with our version
      r.ReOpen("");
      r.synchronized("7.47");
      r.version("7.47");
    } // if (dbVersion != "7.47")
  }
  catch(...)
  {

  }
}

BOOL 
TrapRcvrApp::IsRunAsAdmin()
{
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    // Allocate and initialize a SID of the administrators group.
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(
        &NtAuthority, 
        2, 
        SECURITY_BUILTIN_DOMAIN_RID, 
        DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0, 
        &pAdministratorsGroup))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    // Determine whether the SID of administrators group is enabled in 
    // the primary access token of the process.
    if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:
    // Centralized cleanup for all allocated resources.
    if (pAdministratorsGroup)
    {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    // Throw the error if something failed in the function.
    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return fIsRunAsAdmin;
}

void 
TrapRcvrApp::Elevate()
{
  // Check the current process's "run as administrator" status.
  BOOL fIsRunAsAdmin;
  try
  {
    fIsRunAsAdmin = IsRunAsAdmin();
  }
  catch (DWORD dwError)
  {
  }

  // Elevate the process if it is not run as administrator.
  if (!fIsRunAsAdmin)
  {
    char szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
      // Launch itself as administrator.
      SHELLEXECUTEINFO sei = { sizeof(sei) };
      sei.lpVerb = "runas";
      sei.lpFile = szPath;
      sei.hwnd = NULL; //hWnd;
      sei.nShow = SW_NORMAL;
      
      if (!ShellExecuteEx(&sei))
      {
	DWORD dwError = GetLastError();
	if (dwError == ERROR_CANCELLED)
	{
	  // The user refused the elevation.
	  // Do nothing ...
	}
      }
    } // if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
  } // if (!fIsRunAsAdmin)
}

