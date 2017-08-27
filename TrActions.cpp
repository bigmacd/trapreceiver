
#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrActions.h"
#include "TrActionAdd.h"
#include "Registry.h"

IMPLEMENT_DYNCREATE(TrActions, CPropertyPage)

TrActions::TrActions() : CPropertyPage(TrActions::IDD)
{
	//{{AFX_DATA_INIT(TrActions)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

TrActions::~TrActions()
{
}

void TrActions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrActions)
	DDX_Control(pDX, IDC_LIST, mList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrActions, CPropertyPage)
	//{{AFX_MSG_MAP(TrActions)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDELETE, OnButtondelete)
	ON_BN_CLICKED(IDC_BUTTONMODIFY, OnButtonmodify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrActions message handlers

void TrActions::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void TrActions::OnButtonadd() 
{
  TrActionAdd dlg;
  int result = dlg.DoModal();
  if (result == IDOK)
  {
    BOOL partOfGroup = dlg.mPartOfGroup;

    int col = 0;
    LV_ITEM lv;
    lv.iItem = mList.GetItemCount();

    lv.mask = LVIF_TEXT;
    lv.iSubItem = col++;
    lv.pszText = dlg.mName.GetBuffer(0);
    mList.InsertItem(&lv);

    lv.mask = LVIF_TEXT;
    lv.iSubItem = col++;
    lv.pszText = dlg.mWatchText.GetBuffer(0);
    //    mList.InsertItem(&lv);
    mList.SetItem(&lv);
    mList.SetItemData(lv.iItem, dlg.mWatch);
      
    lv.mask = LVIF_TEXT;
    lv.iSubItem = col++;
    lv.pszText = dlg.mValue.GetBuffer(0);
    mList.SetItem(&lv);	

    if (!partOfGroup) {

      // bools
      int bools = dlg.mBools;
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      CString sPort;
      sPort.Format("%d", bools); 
      lv.pszText = sPort.GetBuffer(0);
      mList.SetItem(&lv);	
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = dlg.mExecuteProgram.GetBuffer(0);
      mList.SetItem(&lv);	
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = dlg.mExecuteArgs.GetBuffer(0);
      mList.SetItem(&lv);	
      
      BOOL itd = dlg.mIncludeTrapData;
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      CString iTD;
      iTD.Format("%d", itd);
      lv.pszText = iTD.GetBuffer(0);
      mList.SetItem(&lv);	
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = dlg.mEmailDest.GetBuffer(0);
      mList.SetItem(&lv);	
      
      iTD.Format("%d", dlg.mSoundPlayDuration);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = iTD.GetBuffer(0);
      mList.SetItem(&lv); 
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = dlg.mSoundFile.GetBuffer(0);
      mList.SetItem(&lv); 
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = dlg.mExploderDest.GetBuffer(0);
      mList.SetItem(&lv);	
    }

    if (partOfGroup)
      col += 8; // makes up for all those col++ above

    BOOL pog = dlg.mPartOfGroup;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = col++;
    CString pOG;
    pOG.Format("%d", pog);
    lv.pszText = pOG.GetBuffer(0);
    mList.SetItem(&lv);	
    
    int rd = dlg.mRequireDesktop;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = col++;
    CString iTD;
    iTD.Format("%d", rd);
    lv.pszText = iTD.GetBuffer(0);
    mList.SetItem(&lv);	
      
    SetModified(TRUE);
  }
  SetFocus();		
}

void TrActions::OnButtondelete() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
  {
    mList.DeleteItem(currentIndex);

    SetModified(TRUE);
  }
  SetFocus();	
}

void TrActions::OnButtonmodify() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
    DoModify(currentIndex);
  SetFocus();	
}

BOOL
TrActions::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  RECT rect;
  mList.GetClientRect(&rect);
  //  int width = (rect.right / 2);
  int width = (rect.right / 3);
  int col = 0;
  mList.InsertColumn(col++, "Name", LVCFMT_CENTER, width);
  mList.InsertColumn(col++, "Watch", LVCFMT_CENTER, width);
  mList.InsertColumn(col++, "Value", LVCFMT_CENTER, width);
  mList.InsertColumn(col++, "Bools", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "Execute", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "Args", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "td", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "emaildests", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "soundDuration", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "soundFile", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "exploderdests", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "partofgroup", LVCFMT_CENTER, 0);
  mList.InsertColumn(col++, "requireDesktop", LVCFMT_CENTER, 0);

  Registry registry("Actions");
  int count = (int)registry.getCount();

  LV_ITEM lv;
  CString data;

  try
  {
    for (int listLength = 0; listLength < count; listLength++)
    {
      registry.ReOpen(listLength);
      lv.iItem = listLength;

      data = registry.name();
      col = 0;
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.InsertItem(&lv);

      int w = registry.watch();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;

      switch (w)
      {
         case COMMUNITY:
	   data = "Community";
	   break;
         case GENERIC:
	   data = "Generic Type";
	   break;
         case SENDERIP:
	   data = "Sender IP";
	   break;
         case SENDEROID:
	   data = "Sender OID";
	   break;
         case SPECIFIC:
	   data = "Specific Type";
	   break;
         case VARBINDOID:
	   data = "Varbind OID";
	   break;
         case VARBINDVALUE:
	   data = "Varbind Value";
	   break;
      }
      lv.pszText = data.GetBuffer(0);
      //mList.InsertItem(&lv);
      mList.SetItem(&lv); 
      mList.SetItemData(listLength, w);

      data = registry.value();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data.Format("%d", registry.bools());
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data = registry.executable();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data = registry.arguments();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data.Format("%d", registry.includeTrapData());
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data = registry.emailRecipients();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 
      
      data.Format("%d", registry.soundPlayDuration());
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data = registry.soundFile();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data = registry.exploderDestinations();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      data.Format("%d", registry.partOfGroup());
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv); 

      int runfrom = 1; // default all to gui
      try { runfrom = registry.runFrom(); } catch (...) { registry.runFrom(runfrom); } 
      data.Format("%d", runfrom);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = col++;
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
TrActions::GetCurrentIndex()
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
TrActions::DoModify(int currentIndex)
{
  // get the item data
  int     index = 0; // !! must be zero when adding groups
  CString name = mList.GetItemText(currentIndex, index++);
  int     watch = mList.GetItemData(currentIndex); index++;
  CString value = mList.GetItemText(currentIndex, index++);
  CString bools = mList.GetItemText(currentIndex, index++);
  CString execute = mList.GetItemText(currentIndex, index++);
  CString args = mList.GetItemText(currentIndex, index++);
  CString iTD = mList.GetItemText(currentIndex, index++);
  CString emailDests = mList.GetItemText(currentIndex, index++);
  int     soundPlayDuration = atoi(mList.GetItemText(currentIndex, index++));
  CString soundFile = mList.GetItemText(currentIndex, index++);
  CString exploderDests = mList.GetItemText(currentIndex, index++);
  BOOL    partOfGroup = (BOOL)atoi(mList.GetItemText(currentIndex, index++));
  int     requireDesktop = atoi(mList.GetItemText(currentIndex, index++));

  // pass it to the dlg
  TrActionAdd dlg(name, 
		  watch, 
		  value,
		  atoi(bools.GetBuffer(0)),
		  execute, 
		  args,
		  (BOOL)atoi(iTD.GetBuffer(0)),
		  emailDests,
		  exploderDests,
		  soundPlayDuration,
		  soundFile,
		  partOfGroup,
		  requireDesktop);
  int result = dlg.DoModal();
  
  if (result == IDOK)
  {
    index = 0;
    LV_ITEM lv;
    lv.iItem = currentIndex;

    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mName.GetBuffer(0);
    mList.SetItem(&lv);

    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mWatchText.GetBuffer(0);
    mList.SetItem(&lv);

    mList.SetItemData(currentIndex, dlg.mWatch);
      
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mValue.GetBuffer(0);
    mList.SetItem(&lv);	

    // bools
    int bools = dlg.mBools;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    CString sPort;
    sPort.Format("%d", bools); 
    lv.pszText = sPort.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mExecuteProgram.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mExecuteArgs.GetBuffer(0);
    mList.SetItem(&lv);	

    BOOL itd = dlg.mIncludeTrapData;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    CString iTD;
    iTD.Format("%d", itd);
    lv.pszText = iTD.GetBuffer(0);
    mList.SetItem(&lv);	

    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mEmailDest.GetBuffer(0);
    mList.SetItem(&lv);	

    iTD.Format("%d", dlg.mSoundPlayDuration);
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = iTD.GetBuffer(0);
    mList.SetItem(&lv); 
    
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mSoundFile.GetBuffer(0);
    mList.SetItem(&lv); 
    
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    lv.pszText = dlg.mExploderDest.GetBuffer(0);
    mList.SetItem(&lv);	

    BOOL pog = dlg.mPartOfGroup;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    CString pOG;
    pOG.Format("%d", pog);
    lv.pszText = pOG.GetBuffer(0);
    mList.SetItem(&lv);	

    int rd = dlg.mRequireDesktop;
    lv.mask = LVIF_TEXT;
    lv.iSubItem = index++;
    iTD.Format("%d", rd);
    lv.pszText = iTD.GetBuffer(0);
    mList.SetItem(&lv);	

    SetModified(TRUE);
  }

}

BOOL TrActions::OnApply() 
{
  try
  {
    Registry registry("Actions");
    unsigned long count = registry.getCount();

    //  for (unsigned long x = 0; x < count; x++)
    //    registry.deleteAction(x);
    registry.deleteActions();

   
    int listCount = mList.GetItemCount();
    for (int regX = 0; regX < listCount; regX++)
    {
      //int index = 1;
      int     index = 0;  // !! needs to be 0 for groups
      CString name = mList.GetItemText(regX, index++);
      int     watch = mList.GetItemData(regX); index++;
      CString value = mList.GetItemText(regX, index++);
      CString bools = mList.GetItemText(regX, index++);
      CString exe = mList.GetItemText(regX, index++);
      CString args = mList.GetItemText(regX, index++);
      CString iTD = mList.GetItemText(regX, index++);
      CString emailDests = mList.GetItemText(regX, index++);
      int     soundDuration = atoi(mList.GetItemText(regX, index++));
      CString soundFile = mList.GetItemText(regX, index++);
      CString exploderDests = mList.GetItemText(regX, index++);
      BOOL    partOfGroup = (BOOL)atoi(mList.GetItemText(regX, index++));
      int     requireDesktop = atoi(mList.GetItemText(regX, index++));

// change for groups in 7.47
//      registry.insertAction(watch,

      registry.insertAction(name, 
			    watch,
			    value,
			    atoi(bools.GetBuffer(0)),
			    exe,
			    args,
			    atoi(iTD.GetBuffer(0)),
			    emailDests,
			    exploderDests,
			    soundDuration,
			    soundFile,
			    partOfGroup,
			    requireDesktop);
    }
  }
  catch(...)
  {

  }
  return CPropertyPage::OnApply();
}

