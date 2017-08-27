
#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrapRcvrDlg.h"
#include "TrActions.h"
#include "TrExploder.h"
#include "TrEmail.h"
#include "TrLogging.h"
#include "TrMisc.h"
#include "MibImportPropertyPage.h"
#include "ConfigureTrapData.h"
#include "Registry.h"
#include "DetailDlg.h"
#include "V2DetailDlg.h"
#include "vbs.h"
#include <fstream>
#include "MibDb.h"

#include <mmsystem.h>
#include <winsock2.h>
#include "..\eventlogmessage.h"

#include <ShlObj.h>

extern TrapRcvrApp theApp;
extern MibDb       gDb;


class CAboutDlg : public CDialog
{
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
  // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


TrapRcvrDlg::TrapRcvrDlg(CWnd* pParent /*=NULL*/)
            : CDialog(TrapRcvrDlg::IDD, pParent),
              mDoReverse(FALSE),
              mSizeInitialized(FALSE),
              m_bHidden(FALSE),
              mTray(NULL)
{
  //{{AFX_DATA_INIT(TrapRcvrDlg)
  mSource = _T("");
  mTrapCount = _T("0");
  //}}AFX_DATA_INIT
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  Registry registry;
  try { mDoReverse = registry.reverseLookup(); }
  catch(...) { registry.reverseLookup(FALSE); }

  m_pIconList[0] = m_hIcon;
  m_pIconList[1] = m_hIcon;
  m_pIconList[2] = m_hIcon;

}

void 
TrapRcvrDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrapRcvrDlg)
  DDX_Control(pDX, IDC_LIST1, mList);
  DDX_Text(pDX, IDC_STATICSOURCE, mSource);
  DDX_Text(pDX, IDC_STATICTRAPCOUNT, mTrapCount);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(TrapRcvrDlg, CDialog)
  //{{AFX_MSG_MAP(TrapRcvrDlg)
  ON_WM_PAINT()
  ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRightClickList)
  ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
  ON_WM_SYSCOMMAND()
  ON_WM_TIMER()
  ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedLinksList)
  ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  //////////////////////////////////////
  // tray message handlers
  ON_MESSAGE(WM_TRAY_NOTIFY, OnTrayNotify)
  //ON_COMMAND(ID_TRAY_RESTORE, OnTrayRestore)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrapRcvrDlg message handlers

void
TrapRcvrDlg::OnHide()
{

  // Load icon onto taskbar tray
  mTray = new CTrayNot (this,
			WM_TRAY_NOTIFY,
			NULL,
			m_pIconList);
  mTray->SetState(IDR_MAINFRAME);
  m_bHidden = TRUE;
}


void
TrapRcvrDlg::OnUnHide()
{
  //  ShowWindow(SW_RESTORE);
  //  m_bHidden = FALSE;
  /////////////////////////////////////
  // Remove icon from taskbar tray
  if (mTray)
  {
    delete mTray;
    mTray = NULL;
  }
}

LONG 
TrapRcvrDlg::OnTrayNotify (WPARAM wParam, LPARAM lParam)
{
  switch (lParam)
  {
    //  case WM_RBUTTONDOWN:
    //  {
    //    CMenu menu ;
    /////////////////////////////////////
    // Load and Verify Menu
    //    VERIFY(menu.LoadMenu(IDR_TRAY));
    //    CMenu* pPopup = menu.GetSubMenu (0) ;
    //    ASSERT(pPopup != NULL);
    
    // Get the cursor position
    //    POINT pt ;
    //    GetCursorPos (&pt) ;
    
    // Fix Microsofts' BUG!!!!
    //    SetForegroundWindow();
    
    ///////////////////////////////////
    // Display The Menu
    //    pPopup->TrackPopupMenu(TPM_LEFTALIGN |
    //			   TPM_RIGHTBUTTON,pt.x, pt.y, AfxGetMainWnd());
    //    break ;
    //  }
  case WM_LBUTTONDBLCLK:
    //////////////////////////////////
    // Unhide our Window
    if (m_bHidden)
      ShowWindow (SW_RESTORE);
    OnUnHide();
    break ;
  }
  return (0) ;
}

void TrapRcvrDlg::OnDestroy() 
{
  CDialog::OnDestroy();
	
  // Remove Icon from Tray
  if (mTray)
  {
    delete mTray ;
    mTray = NULL ;
  }	

}

void TrapRcvrDlg::OnTrayRestore() 
{
  // UnHide Application
  if(m_bHidden)
    ShowWindow(SW_RESTORE);
  m_bHidden = FALSE;
  //  OnTrayRestore();		
}


BOOL 
TrapRcvrDlg::OnInitDialog()
{
  char localAppData[MAX_PATH];
  SHGetFolderPath(NULL, CSIDL_APPDATA, 0, NULL, localAppData);

  // get the minimum window size
  CRect recT;
  GetClientRect(&recT);
  mMinSize = CPoint(recT.Width(), recT.Height());
  mSizeInitialized = TRUE;

  mDlgResizeHelper.Init(m_hWnd);
  mDlgResizeHelper.Fix(mList, 
		       DlgResizeHelper::kLeft, 
		       DlgResizeHelper::kNoVFix);
  mDlgResizeHelper.Fix(IDC_BUTTON1,
		       DlgResizeHelper::kWidthRight,DlgResizeHelper::kHeightBottom);
  mDlgResizeHelper.Fix(IDOK,
		       DlgResizeHelper::kWidthLeft,DlgResizeHelper::kHeightBottom);
  mDlgResizeHelper.Fix(IDC_STATICCOUNTLABEL,
		       DlgResizeHelper::kWidthRight,DlgResizeHelper::kHeightTop);
  mDlgResizeHelper.Fix(IDC_STATICTRAPCOUNT,
		       DlgResizeHelper::kWidthRight,DlgResizeHelper::kHeightTop);
  mDlgResizeHelper.Fix(IDC_STATICSOURCELABEL,
		       DlgResizeHelper::kWidthLeft,DlgResizeHelper::kHeightTop);
  mDlgResizeHelper.Fix(IDC_STATICSOURCE,
		       DlgResizeHelper::kWidthLeft,DlgResizeHelper::kHeightTop);

  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      /////////////////////////////
      // 6.40 
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_EXPORTCONFIG, "Export Config...");
      pSysMenu->AppendMenu(MF_STRING, IDM_IMPORTCONFIG, "Import Config...");
      /////////////////////////////
    }
  }

  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon
	
  RECT rect;
  mList.GetClientRect(&rect);

  int width = (rect.right/4) - 5;
  //	mList.InsertColumn(0, "Sender", LVCFMT_LEFT, width + 13);
  mList.InsertColumn(0, "Ack", LVCFMT_LEFT, 40);
  mList.InsertColumn(1, "Sender", LVCFMT_LEFT, width + 13);

  //  mList.InsertColumn(2, "Sender OID", LVCFMT_LEFT, width + 44);
  //  mList.InsertColumn(3, "Type", LVCFMT_LEFT, width - 68);

  mList.InsertColumn(2, "Message", LVCFMT_LEFT, width + 64);
  mList.InsertColumn(3, "Time", LVCFMT_CENTER, width + 11);
  mList.InsertColumn(4, "ack", LVCFMT_CENTER, 0);

  //  mList.InsertColumn(4, "Time", LVCFMT_CENTER, width + 11);
  //  mList.InsertColumn(5, "ack", LVCFMT_CENTER, 0);

  if (theApp.mServiceIsRunning)
  {
    Registry registry;
    int port = 0;
    try
    {
      port = registry.port();
      mSource.Format("%s %d", "Port ", port);
    }
    catch(...)
    {
	    
    }
  }
  else
    mSource = _T("No Source");

  UpdateData(FALSE);

  mQueue = new QueueServer("trap", "gui");
  if (mQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
    { eventlogmessage e; e.evtlog("Could not establish communication channel for trap gui");  }

  DWORD n = ListView_GetExtendedListViewStyle(mList.m_hWnd);

  ListView_SetExtendedListViewStyle 
    (mList.m_hWnd, 
     n | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

  // set the size and other constraints from last time
  Registry registry;
  try
  {
    SetWindowPos(&wndTop, 
		 registry.xPos(),
		 registry.yPos(),
		 registry.width(),
		 registry.height(),
		 SWP_SHOWWINDOW);
  }
  catch(...)
  {
    registry.width(recT.Width());
    registry.height(recT.Height());
    registry.xPos(recT.TopLeft().x);
    registry.yPos(recT.TopLeft().y);
  }

  //  gOidMap = new OidMap();
  //  gTrapMap = new TrapMap();
  //  gEnumMap = new EnumMap();

  // start the timer
  SetTimer(13244, 900, NULL);


  actionMgrThread = new ActionMgrThread(1); // flag it is from the GUI
  actionMgrThread->Start();

  BOOL tray = FALSE;
  try {
    tray = registry.trayIcon();
  }
  catch(...) {
    registry.trayIcon(tray);
  }

  return TRUE; 
}


void 
TrapRcvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
    return;
  }

  if ((nID & 0xFFF0) == SC_MAXIMIZE)
  {
    return;
  }

  if ((nID & 0xFFF0) == SC_MINIMIZE)
  {
    Registry r;
    if (r.trayIcon()) {
      OnHide();
      theApp.HideApplication();
      return;
    }
    else
      CDialog::OnSysCommand(nID, lParam);
  }
  
  if (nID == IDM_EXPORTCONFIG)
  {
    CString filename = "";

    char dir[256];
    GetCurrentDirectory(255, dir);
    
    CString browseTitle = "Export to File";
    CFileDialog dlg(TRUE,
		    _T("reg"),
		    "TrapRcvr.reg",
		    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		    _T("Registry Files (*.reg)|*.reg|All Files|*.*||"));
    dlg.m_ofn.lpstrTitle = browseTitle;
    int result;
    if ((result = dlg.DoModal()) == IDOK)
    {
      filename = dlg.GetPathName();
    }
    SetCurrentDirectory(dir);

    if (result != IDOK)
      return;
    
    TOKEN_PRIVILEGES tp;
    HANDLE hToken = NULL;
    LUID luid;
    
    if(!OpenProcessToken(GetCurrentProcess(),
			 TOKEN_ADJUST_PRIVILEGES,
			 &hToken))
    {
      AfxMessageBox("1: failed to apply privileges for export operation.");
      //	  PERR("OpenProcessToken", GetLastError() );
      return;
    }
	
    if(!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &luid))
    {
      AfxMessageBox("2: failed to apply privileges for export operation.");
      //	  PERR("LookupPrivilegeValue", GetLastError() );
      return;
    }
    
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    
    AdjustTokenPrivileges(hToken, 
			  FALSE, 
			  &tp, 
			  sizeof(TOKEN_PRIVILEGES),
			  NULL, 
			  NULL);
    
    if (GetLastError() != ERROR_SUCCESS)
    {
      AfxMessageBox("3: failed to apply privileges for export operation.");
      //	  PERR("AdjustTokenPrivileges", GetLastError() );
      return;
    }
    
    HKEY key = NULL;
    long r = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  "SOFTWARE\\Network Computing Technologies",
			  (DWORD)0, 
			  KEY_ALL_ACCESS,
			  &key);
    
    r = RegSaveKey(key,
		   filename,
		   NULL);
    if (r != ERROR_SUCCESS)
    {
      CString s;
      s.Format("%s, %d" ,"Regsavekey failed during export operation.", r);
      AfxMessageBox(s);
    }

    if (key != NULL)
      RegCloseKey(key);
    if (hToken != NULL)
      CloseHandle(hToken);
    return;
    
  } //       if (nID == IDM_EXPORTCONFIG)

  if (nID == IDM_IMPORTCONFIG)
  {
    CString filename = "";

    char dir[256];
    GetCurrentDirectory(255, dir);
    
    CString browseTitle = "Import from File";
    CFileDialog dlg(TRUE,
		    _T("reg"),
		    "",
		    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		    _T("Registry Files (*.reg)|*.reg|All Files|*.*||"));
    dlg.m_ofn.lpstrTitle = browseTitle;
    int result;
    if ((result = dlg.DoModal()) == IDOK)
      {
	filename = dlg.GetPathName();
      }
    SetCurrentDirectory(dir);

    if (result != IDOK)
      return;
    
    TOKEN_PRIVILEGES tp;
    HANDLE hToken = NULL;
    LUID luid;
    
    if(!OpenProcessToken(GetCurrentProcess(),
			 TOKEN_ADJUST_PRIVILEGES,
			 &hToken))
      {
	AfxMessageBox("1: failed to apply privileges for import operation.");
	//	  PERR("OpenProcessToken", GetLastError() );
	return;
      }
	
    if(!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &luid))
      {
	AfxMessageBox("2: failed to apply privileges for import operation.");
	//	  PERR("LookupPrivilegeValue", GetLastError() );
	return;
      }
    
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    
    AdjustTokenPrivileges(hToken, 
			  FALSE, 
			  &tp, 
			  sizeof(TOKEN_PRIVILEGES),
			  NULL, 
			  NULL);
    
    if (GetLastError() != ERROR_SUCCESS)
      {
	AfxMessageBox("3: failed to apply privileges for import operation.");
	//	  PERR("AdjustTokenPrivileges", GetLastError() );
	return;
      }
    
    HKEY key = NULL;
    long r = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  "SOFTWARE\\Network Computing Technologies",
			  (DWORD)0, 
			  KEY_ALL_ACCESS,
			  &key);
    
    r = RegRestoreKey(key,
		      filename,
		      0x00000008L);
    if (r != ERROR_SUCCESS)
      {
	CString s;
	s.Format("%s, %d" ,"RegRestoreKey failed during import operation.", r);
	AfxMessageBox(s);
      }

    if (key != NULL)
      RegCloseKey(key);
    if (hToken != NULL)
      CloseHandle(hToken);
    return;
  }

  CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void TrapRcvrDlg::OnPaint() 
{
  if (IsIconic())
    {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
    }
  else
    {
      CDialog::OnPaint();
    }
}

HCURSOR TrapRcvrDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

void 
TrapRcvrDlg::OnTimer(UINT nIDEvent) 
{
  CDialog::OnTimer(nIDEvent);

  unsigned char buff[4096];
  unsigned int length = 4096;
  memset(buff, 0, 4096);
  while (mQueue->Get(buff, length))
    {
      SnmpParser parser(&buff[8], length-8);
      Packet* p = parser.packet();
      if (p && (p->Type() == V2TRAP || p->Type() == INFORMPDU))
	{
	  struct sockaddr_in a;
	  memset(&a, 0, sizeof(sockaddr_in));
	  memcpy(&(a.sin_addr.s_addr), &buff[4], 4);
	  V2Notification* pdu = (V2Notification*)p->pdu();
	  pdu->SenderIP(&a);
	}
      //if (p == NULL)
      //  gLogFile.message("TrDlg::OnTimer,parsed packet is null");
      if (p != NULL)
	{

	  ////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////
	  mAMTQueue.Put(buff, length); // send off to action manager version 8.0 change
	                              // now with sounds!
	  ////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////
	  
	  long oldCount = atol(mTrapCount.GetBuffer(0));
	  oldCount++;
	  mTrapCount.Format("%d", oldCount);
	  UpdateData(FALSE);

	  LV_ITEM lv;
	  lv.iItem = 0;
      
	  lv.mask = LVIF_TEXT;
	  lv.iSubItem = 0;
	  lv.pszText = "";
	  mList.InsertItem(&lv);

	  lv.mask = LVIF_TEXT;
	  //      lv.iSubItem = 0;
	  lv.iSubItem = 1;
	  CString unknown;
	  if (!mDoReverse)
	    {
	      lv.pszText = (char*)p->SenderIP();
	      //gLogFile.message("TrDlg::OnTimer,no reverse...1");
	      //gLogFile.message(p->SenderIP());
	      //gLogFile.message("TrDlg::OnTimer,1");
	    }
	  else
	    {
	      unsigned long addr = inet_addr(p->SenderIP());
	      struct hostent* hp = gethostbyaddr((const char*)&addr, 
						 sizeof(unsigned long), AF_INET);
	      if (hp == NULL)
		{
		  unknown = p->SenderIP();
		  unknown += "-u";
		  //gLogFile.message("TrDlg::OnTimer,reverse,not found...2");
		  //gLogFile.message(unknown.GetBuffer(0));
		  //gLogFile.message("TrDlg::OnTimer,2");
		  lv.pszText = unknown.GetBuffer(0);
		}
	      else
		{
		  //gLogFile.message("TrDlg::OnTimer,reverse,found...3");
		  //gLogFile.message(hp->h_name);
		  //gLogFile.message("TrDlg::OnTimer,3");
		  lv.pszText = hp->h_name;
		}
	    }

	  //      mList.InsertItem(&lv);
	  mList.SetItem(&lv);
	  mList.SetItemData(lv.iItem, (unsigned long)p);
      
	  lv.mask = LVIF_TEXT;
	  //      lv.iSubItem = 1;
	  lv.iSubItem = 2;
	  //gLogFile.message("TrDlg::OnTimer,oid,...4");
	  //gLogFile.message(p->SenderOID());
	  //gLogFile.message("TrDlg::OnTimer,4");


	  const char* msg = NULL;
	  CString defMsg;
	  string asdf;
	  Registry r;
	  try
	    {
	      if (r.translateOids())
		{
		  bool status = false;
		  if (p->Type() == V1TRAP)
		    {
		      status = gDb.FindTrap(p->SenderOID(), 			
					    p->SpecificTrapType());
		      if (true == status)
			{
			  asdf = gDb.TrapName();
			  msg = asdf.c_str();
			}
		    }
		  else
		    {
		      int specificType;
		      char* oid = new char[strlen(p->SenderOID()) + 1];
		      memset(oid, 0, strlen(p->SenderOID() + 1));
		      strcpy_s(oid, strlen(p->SenderOID())+1, p->SenderOID());

		      // manipulate the sender oid to remove the two subidentifiers
		      char* c = strrchr(oid, '.');
		      if (NULL != c)
			{
			  char* lastSub = c + 1;
			  specificType = atoi(lastSub);
			  *c = 0;
			  c = strrchr(oid, '.');
			  if (NULL != c)
			    {
			      *c = 0;
			      status = gDb.FindTrap(oid, 
						    specificType);
			      if (true == status)
				{
				  asdf = gDb.TrapName();
				  msg = asdf.c_str();
				}
			    }
			} // if (NULL != c)
		      delete [] oid;
		    }
		  //	  if (true == status)
		  //	    msg = gDb.TrapName().c_str();
		} // if (r.translateOids())
	    }
	  catch(...)
	    {

	    }

	  if (msg == NULL || strlen(msg) == 0)
	    {
	      defMsg.Format("%s  Type %d/%d", 
			    p->SenderOID(),
			    p->GenericTrapType(),
			    p->SpecificTrapType());
	      msg = defMsg.GetBuffer(0);
	    }

	  lv.pszText = (char*)msg;
	  mList.SetItem(&lv);	

	  ////////////////////////////////////////////////////////      
#if 0
	  lv.mask = LVIF_TEXT;
	  //      lv.iSubItem = 2;
	  lv.iSubItem = 3;
	  CString type;
	  if (p->Type() == V1TRAP)
	    type.Format("%d/%d", 
			p->GenericTrapType(),
			p->SpecificTrapType());
	  else
	    if (p->Type() == V2TRAP)
	      type = "v2";
	    else
	      if (p->Type() == INFORMPDU)
		type = "Inf";
	      else
		type = "n/a";
	  lv.pszText = type.GetBuffer(0);
	  mList.SetItem(&lv);	
#endif
	  ///////////////////////////////////////////////////////      
	  lv.mask = LVIF_TEXT;
	  lv.iSubItem = 3;
	  //      lv.iSubItem = 4;

	  __time32_t t = *((__time32_t*)buff);

	  struct tm* newT = NULL;
	  newT = _localtime32(&t);
	  char cTime[64];
	  memset(cTime, 0, 64);
	  strftime(cTime, 
		   64, 
		   "%X %x",
		   newT);
	  lv.pszText = cTime;
	  mList.SetItem(&lv);	

	  lv.mask = LVIF_TEXT;
	  //      lv.iSubItem = 5;
	  lv.iSubItem = 4;
	  lv.pszText = "0";
	  mList.SetItem(&lv);	

	} // if (p != NULL)
      length = 4096;
      memset(buff, 0, 4096);
    } // while (mQueue->Get(buff, length))
}

void TrapRcvrDlg::OnOK() 
{
  if (mQueue != NULL)
    {
      delete mQueue;
      mQueue = NULL;
    }

  // save the size and other constraints
  WINDOWPLACEMENT wp;
  BOOL b = GetWindowPlacement(&wp);
  if (b)
    {
      Registry registry;
      try
	{
	  CRect recT(&(wp.rcNormalPosition));
	  registry.width(recT.Width());
	  registry.height(recT.Height());
	  registry.xPos(recT.TopLeft().x);
	  registry.yPos(recT.TopLeft().y);    
	}
      catch(...)
	{
      
	}
    }

  if (actionMgrThread != NULL)
  {
    actionMgrThread->Exit();
    WaitForSingleObject(actionMgrThread->ThreadId(), INFINITE);
    delete actionMgrThread;
    actionMgrThread = NULL;
  }
  
  OnDestroy();

  CDialog::OnOK();
}

void TrapRcvrDlg::OnCancel() 
{
  OnOK();
}

void TrapRcvrDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  NMLISTVIEW* lv = (NMLISTVIEW*)pNMHDR;
  if (lv->iItem >= 0)
    {
      Packet* p = (Packet*)mList.GetItemData(lv->iItem);
      if (p != NULL)
	{
	  //      if (p->Type() == V1TRAP)
	  {
	    DetailDlg dlg(p, &mList, lv->iItem, p->Type());
	    dlg.DoModal();
	  }
	  //      else
	  //      if (p->Type() == V2TRAP || p->Type() == INFORMPDU)
	  //      {
	  //	V2DetailDlg dlg(p, &mList, lv->iItem);
	  //	dlg.DoModal();
	  //      }
	} 
    }
  *pResult = 0;
}

void 
TrapRcvrDlg::OnButton1() 
{
  // this is really on configure.  go figure.
  CPropertySheet sheet("Configure Trap Receiver");
  unsigned long flags = sheet.m_psh.dwFlags;
  flags &= ~PSH_HASHELP;
  sheet.m_psh.dwFlags = flags;

  TrEmail emailPage;
  flags = emailPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  emailPage.m_psp.dwFlags = flags;

  TrLogging loggingPage;
  flags = loggingPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  loggingPage.m_psp.dwFlags = flags;

  TrMisc miscPage;
  flags = miscPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  miscPage.m_psp.dwFlags = flags;

  TrActions actionsPage;
  flags = actionsPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  actionsPage.m_psp.dwFlags = flags;

  TrExploder explodePage;
  flags = explodePage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  explodePage.m_psp.dwFlags = flags;

  ConfigureTrapData tdPage;
  flags = tdPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  tdPage.m_psp.dwFlags = flags;

  MibImportPropertyPage miPage;
  flags = miPage.m_psp.dwFlags;
  flags &= ~PSP_HASHELP;
  miPage.m_psp.dwFlags = flags;

  sheet.AddPage(&actionsPage);
  sheet.AddPage(&loggingPage);
  sheet.AddPage(&miscPage);
  sheet.AddPage(&emailPage);
  sheet.AddPage(&explodePage);
  sheet.AddPage(&tdPage);
  sheet.AddPage(&miPage);
  sheet.DoModal();
}

void 
TrapRcvrDlg::OnRightClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
  POINT point;
  GetCursorPos(&point);
  
  CMenu cRightPopMenu;
  cRightPopMenu.LoadMenu(IDR_MENU1);

  if (mList.GetItemCount() <= 0)
    {
      // disable the  option
      cRightPopMenu.EnableMenuItem(ID_CLEARTHISTRAP,
				   MF_DISABLED | MF_GRAYED);
      cRightPopMenu.EnableMenuItem(ID_CLEARGUI,
				   MF_DISABLED | MF_GRAYED);
    }
  else
    {
      int count = mList.GetItemCount();
      int x = 0;
      for (x = 0; x < count; x++)
	{
	  unsigned int state = mList.GetItemState(x, 
						  LVIS_SELECTED|LVIS_FOCUSED);
	  if (state & (LVIS_SELECTED | LVIS_FOCUSED))
	    {
	      cRightPopMenu.EnableMenuItem(ID_CLEARTHISTRAP,
					   MF_ENABLED);
	      mList.EnsureVisible(x, FALSE);
	      break;
	    }
	}    
      if (x == count)
	cRightPopMenu.EnableMenuItem(ID_CLEARTHISTRAP,
				     MF_DISABLED | MF_GRAYED);
    }
  CMenu* pPopup = cRightPopMenu.GetSubMenu(0);
  int x = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				 point.x,
				 point.y,
				 this);
	
  *pResult = 0;
}

BOOL
TrapRcvrDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
  if (lParam == 0)
    {
      unsigned int menuId = (wParam & 0x0000ffff);
      switch (menuId)
	{
	case ID_CLEARGUI:
	  {
	    int count = mList.GetItemCount();
	    for (int x = 0; x < count; x++)
	      {
		Packet* p = (Packet*)mList.GetItemData(x);

		////////////////////////////////////////////////////////////
		if (p != NULL)
		  {
		    unsigned int len = p->TotalLength();
		    unsigned char* t = new unsigned char[len];
		    p->Build(t);
	   
		    unsigned char b[4096];
		    memset(b, 0, 4096);
		    __time32_t tm = _time32(0);
		    memcpy(b, &tm, 4);
		    b[4] = 'D';
		    if (t != NULL)
		      {
			memcpy(&b[5], t, len);
			delete [] t;
		      }
		    mLogQueue.Put(b, len+5);
		    ///////////////////////////////////////////////////////////
	   
		    delete p;
		  }
	      }
	    mList.DeleteAllItems();
	    mTrapCount = _T("0");
	    UpdateData(FALSE);
	    break;
	  }

	case ID_CLEARTHISTRAP:
	  {
	    int n = mList.GetSelectedCount();
	    if (!n) break;

	    int* ar = new int[n];
	    int tmp = n;

	    POSITION pos = mList.GetFirstSelectedItemPosition();
	    int x = mList.GetNextSelectedItem(pos);

	    // save off the list of selected items
	    while (x != -1)
	      {
		tmp = tmp - 1;
		ar[tmp] = x;
		x = mList.GetNextSelectedItem(pos);
	      }

	    // use the list to delete stuff
	    for (int j = 0; j < n; j++)
	      {
		int xx = ar[j];
		Packet* p = (Packet*)mList.GetItemData(xx);
		if (p != NULL)
		  {
		    unsigned int len = p->TotalLength();
		    unsigned char* t = new unsigned char[len];
		    p->Build(t);
	   
		    unsigned char b[4096];
		    memset(b, 0, 4096);
		    __time32_t tm = _time32(0);
		    memcpy(b, &tm, 4);
		    b[4] = 'D';
		    if (t != NULL)
		      {
			memcpy(&b[5], t, len);
			delete [] t;
		      }
		    mLogQueue.Put(b, len+5);
	   
		    delete p;
		  }
		mList.DeleteItem(xx);
	      }
	    delete ar;

	    mTrapCount.Format("%d", mList.GetItemCount());
	    UpdateData(FALSE);	   
	  }
     
	default:       
	  break;
	} // switch (menuID)
    } // if (lParam == 0)
  return CDialog::OnCommand(wParam, lParam);
}

void 
TrapRcvrDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  //  CListCtrl::OnLButtonDown(nFlags, point);
  NMITEMACTIVATE* nma = (NMITEMACTIVATE*)pNMHDR;
  if (nma != NULL)
    {
      CPoint point;
      point.x = 15;//nma->ptAction.x;
      point.y = nma->ptAction.y;
      int index = -1;
      if ((index = mList.HitTest(point, NULL)) != -1)
	mList.SetItemState(index,
			   LVIS_SELECTED | LVIS_FOCUSED, 
			   LVIS_SELECTED | LVIS_FOCUSED);
    }
  *pResult = 0;
}


void 
TrapRcvrDlg::OnSize(UINT nType, int cx, int cy) 
{
  CDialog::OnSize(nType, cx, cy);
  
  mDlgResizeHelper.OnSize();
}

void 
TrapRcvrDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI ) 
{
  if (mSizeInitialized)
    lpMMI->ptMinTrackSize = mMinSize;
}

void 
TrapRcvrDlg::OnItemchangedLinksList(NMHDR* pNMHDR, LRESULT* pResult) 
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  *pResult = 0;

  if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
    return;	// No change

  // Old check box state
  BOOL prevState = (BOOL)(((pNMListView->uOldState & 
			    LVIS_STATEIMAGEMASK)>>12)-1);   

  if (prevState < 0)	// On startup there's no previous state 
    prevState = 0; // so assign as false (unchecked)

  // New check box state
  BOOL currState =
    (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
  if (currState < 0) // On non-checkbox notifications assume false
    currState = 0; 

  if (prevState == currState) // No change in check box
    return;
	
  if (!prevState && currState) // is wasn't checked now it is
    {
      CString state = mList.GetItemText(pNMListView->iItem, 5);
      if (state == "0")
	{
	  LV_ITEM lv;
	  lv.iItem = pNMListView->iItem;
	  lv.mask = LVIF_TEXT;
	  lv.iSubItem = 5;
	  lv.pszText = "1";
	  mList.SetItem(&lv);
	  Packet* p = (Packet*)mList.GetItemData(pNMListView->iItem);
	  if (p != NULL)
	    {
	      unsigned int len = p->TotalLength();
	      unsigned char* t = new unsigned char[len];
	      p->Build(t);
	     
	      unsigned char b[4096];
	      memset(b, 0, 4096);
	      __time32_t tm = _time32(0);
	      memcpy(b, &tm, 4);
	      b[4] = 'A';
	      if (t != NULL)
		{
		  memcpy(&b[5], t, len);
		  delete [] t;
		}
	      mLogQueue.Put(b, len+5);
	    }
	}
    }
  else
    if (!currState && prevState) // is was checked now it isn't
      mList.SetCheck(pNMListView->iItem);
}

