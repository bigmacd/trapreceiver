
#include "stdafx.h"
#include "TrEmail.h"
#include "TrEmailAdd.h"
#include "Registry.h"

IMPLEMENT_DYNCREATE(TrEmail, CPropertyPage)

TrEmail::TrEmail() 
        :CPropertyPage(TrEmail::IDD)
{
  //{{AFX_DATA_INIT(TrEmail)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

TrEmail::~TrEmail()
{
}

void 
TrEmail::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrEmail)
  DDX_Control(pDX, IDC_LIST, mList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrEmail, CPropertyPage)
  //{{AFX_MSG_MAP(TrEmail)
  ON_BN_CLICKED(IDC_BUTTONADD, OnButtonAdd)
  ON_BN_CLICKED(IDC_BUTTONDELETE, OnButtonDelete)
  ON_BN_CLICKED(IDC_BUTTONMODIFY, OnButtonModify)
  ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblClkListActions)
  //}}AFX_MSG_MAP
  END_MESSAGE_MAP()


void 
TrEmail::OnButtonAdd() 
{
  TrEmailAdd dlg;
  int result = dlg.DoModal();
  if (result == IDOK)
  {
    LV_ITEM lv;
    lv.iItem = mList.GetItemCount();

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 0;
    lv.pszText = dlg.mHost.GetBuffer(0);
    mList.InsertItem(&lv);
      
    lv.mask = LVIF_TEXT;
    lv.iSubItem = 1;
    lv.pszText = dlg.mTo.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 2;
    lv.pszText = dlg.mFrom.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 3;
    CString sPort;
    sPort.Format("%d", dlg.mPort); 
    lv.pszText = sPort.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 4;
    lv.pszText = dlg.mSubject.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 5;
    lv.pszText = dlg.mMessage.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 6;
    CString c;
    c.Format("%d", dlg.mAuthNeeded);
    lv.pszText = c.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 7;
    lv.pszText = dlg.mUser.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 8;
    lv.pszText = dlg.mJuniper.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 9;
    c.Format("%d", time(0));
    lv.pszText = c.GetBuffer(0);
    mList.SetItem(&lv);	

	lv.mask = LVIF_TEXT;
	lv.iSubItem = 10;
	c.Format("%d", dlg.usessl);
	lv.pszText = c.GetBuffer(0);
	mList.SetItem(&lv);
  }
  SetFocus();	
}

void TrEmail::OnButtonDelete() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
  {
    mList.DeleteItem(currentIndex);

    SetModified(TRUE);
  }
  SetFocus();
}

void 
TrEmail::OnButtonModify() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
    DoModify(currentIndex);
  SetFocus();	
}

void 
TrEmail::OnDblClkListActions(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UINT nFlags;
  CPoint point;
  BOOL b = ::GetCursorPos( &point );
  mList.ScreenToClient( &point );
  int currentIndex = mList.HitTest(point, &nFlags);

  if (currentIndex >= 0)
    DoModify(currentIndex);
	
  *pResult = 0;
  SetFocus();
}

BOOL TrEmail::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
	
  RECT rect;
  mList.GetClientRect(&rect);
  int width = (rect.right/2);
  mList.InsertColumn(0, "SMTP Server", LVCFMT_CENTER, width);
  mList.InsertColumn(1, "Recipient", LVCFMT_CENTER, width);
  mList.InsertColumn(2, "from", LVCFMT_CENTER, 0);
  mList.InsertColumn(3, "port", LVCFMT_CENTER, 0);
  mList.InsertColumn(4, "subject", LVCFMT_CENTER, 0);
  mList.InsertColumn(5, "message", LVCFMT_CENTER, 0);
  mList.InsertColumn(6, "authneeded", LVCFMT_CENTER, 0);
  mList.InsertColumn(7, "authuser", LVCFMT_CENTER, 0);
  mList.InsertColumn(8, "authjuniper", LVCFMT_CENTER, 0);
  mList.InsertColumn(9, "id", LVCFMT_CENTER, 0);
  mList.InsertColumn(10, "usessl", LVCFMT_CENTER, 0);

  LV_ITEM lv;
  lv.mask = LVIF_TEXT;

  Registry registry("Email");
  int count = (int)registry.getCount();

  try
  {
    for (int listLength = 0; listLength < count; listLength++)
    {
      registry.ReOpen(listLength);
      lv.iItem = listLength;

      // host      
      CString data = registry.smtpServerHost();
      lv.iSubItem = 0;
      lv.pszText = data.GetBuffer(0);
      mList.InsertItem(&lv);

      // to
      data = registry.to();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      // from 
      data = registry.from();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 2;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      // port 
      data.Format("%d", registry.smtpPort());
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 3;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      // subject 
      data = registry.subject();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 4;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      // message
      data = registry.message();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 5;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);
      
      try
      {
	BOOL needAuth = registry.authNeeded();
      }
      catch(...)
      {
	//	registry.authNeeded(FALSE);
	CString c;
	c.Empty();
	//	registry.authUser(c);
	//	registry.authJuniper(c);
      }
      // auth needed
      BOOL needAuth = registry.authNeeded();
      data.Format("%d", needAuth);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 6;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);
      
      // user name for SMTP login
      data = registry.authUser();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 7;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);
      
      // user password for SMTP login
      data = registry.authJuniper();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 8;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      // email id
      int id = registry.emailId();
      data.Format("%d", id);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 9;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

	  // usessl
      try
      {
		BOOL usessl = registry.useSSL();
      }
      catch(...)
      {
	//		registry.useSSL(FALSE);				
	  }
      BOOL usessl = registry.useSSL();
      data.Format("%d", usessl);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 10;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);


    }
  }
  catch(...)
  {

  }

  return TRUE; 
}

int 
TrEmail::GetCurrentIndex()
{
  int count = mList.GetItemCount();
  int x;
  for (x = 0; x < count; x++)
  {
    if (mList.GetItemState(x, LVIS_SELECTED) == LVIS_SELECTED)
      break;
  }
  if (x == count)
    return -1;
  else
    return x;
}

void
TrEmail::DoModify(int currentIndex)
{
  // get the item data
  CString host = mList.GetItemText(currentIndex, 0);
  CString to = mList.GetItemText(currentIndex, 1);
  CString from = mList.GetItemText(currentIndex, 2);
  CString sPort = mList.GetItemText(currentIndex, 3);
  CString subject = mList.GetItemText(currentIndex, 4);
  CString message = mList.GetItemText(currentIndex, 5);
  BOOL authNeed = atoi(mList.GetItemText(currentIndex, 6));
  CString user = mList.GetItemText(currentIndex, 7);
  CString juniper = mList.GetItemText(currentIndex, 8);
  BOOL usessl = atoi(mList.GetItemText(currentIndex, 10));
  
  // pass it to the dlg
  TrEmailAdd dlg(host, 
		 to, 
		 from, 
		 atoi(sPort.GetBuffer(0)), 
		 subject, 
		 message,
		 authNeed,
		 user,
		 juniper,
		 usessl);
  int result = dlg.DoModal();
  
  if (result == IDOK)
  {
    LV_ITEM lv;
    lv.mask = LVIF_TEXT;
    lv.iItem = currentIndex;

    lv.iSubItem = 0;
    lv.pszText = dlg.mHost.GetBuffer(0);
    mList.SetItem(&lv);
      
    lv.mask = LVIF_TEXT;
    lv.iSubItem = 1;
    lv.pszText = dlg.mTo.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 2;
    lv.pszText = dlg.mFrom.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 3;
    sPort.Format("%d", dlg.mPort); 
    lv.pszText = sPort.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 4;
    lv.pszText = dlg.mSubject.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 5;
    lv.pszText = dlg.mMessage.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 6;
    CString c;
    c.Format("%d", dlg.mAuthNeeded);
    lv.pszText = c.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 7;
    lv.pszText = dlg.mUser.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = 8;
    lv.pszText = dlg.mJuniper.GetBuffer(0);
    mList.SetItem(&lv);	

    // item 9 the id remains unchanged

	lv.mask = LVIF_TEXT;
	lv.iSubItem = 10;
	c.Format("%d", dlg.usessl);
	lv.pszText = c.GetBuffer(0);
	mList.SetItem(&lv);

    SetModified(TRUE);
  }
}

BOOL 
TrEmail::OnApply() 
{
  try 
  {
    Registry registry("Email");
    unsigned long count = registry.getCount();
    
    //  for (unsigned long x = 0; x < count; x++)
    //    registry.deleteEmail(x);
    registry.deleteEmails();
    
    int listCount = mList.GetItemCount();
    for (int regX = 0; regX < listCount; regX++)
    {
	CString host     = mList.GetItemText(regX, 0);
	CString to       = mList.GetItemText(regX, 1);
	CString from     = mList.GetItemText(regX, 2);
	CString port     = mList.GetItemText(regX, 3);
	CString subject  = mList.GetItemText(regX, 4);
	CString message  = mList.GetItemText(regX, 5);
	BOOL    needAuth = atoi(mList.GetItemText(regX, 6));
	CString user     = mList.GetItemText(regX, 7);
	CString juniper  = mList.GetItemText(regX, 8);
	int     id       = atoi(mList.GetItemText(regX, 9));
	BOOL    usessl   = atoi(mList.GetItemText(regX, 10));

	registry.insertEmail(host,
			     atoi(port.GetBuffer(0)),
			     from,
			     to,
			     subject,
			     message,
			     needAuth,
			     user,
			     juniper,
			     id,
				 usessl);
    }

  }
  catch(...)
  {

  }
  return CPropertyPage::OnApply();
}




