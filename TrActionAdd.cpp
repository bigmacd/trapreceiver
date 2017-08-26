
#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrActionAdd.h"
#include "Registry.h"
#include "ExecuteAdd.h"
#include "EmailRecipients.h"
#include "ConfigureSound.h"
#include "ExploderDestinations.h"

#include "resource.h"

TrActionAdd::TrActionAdd(CWnd* pParent /*=NULL*/)
	: CDialog(TrActionAdd::IDD, pParent),
	  mInChangeMode(FALSE),
	  mBools(0),
	  mSoundPlayDuration(0),
	  mSoundFile("")
{
	//{{AFX_DATA_INIT(TrActionAdd)
	mEmail = FALSE;
	mExecute = FALSE;
	mExplode = FALSE;
	mPage = FALSE;
	mValue = _T("");
	mExecuteProgram = _T("");
	mExecuteArgs = _T("");
	mWatch = -1;
	mDiscard = FALSE;
	mSound = FALSE;
	mName = _T("");
	mPartOfGroup = FALSE;
	mIncludeTrapData = FALSE;
	mRequireDesktop = 0; // service
	//}}AFX_DATA_INIT

	int index = 0;
	mCheckBoxes[index++] = FALSE;
	mCheckBoxes[index++] = FALSE;
	mCheckBoxes[index++] = FALSE;
	mCheckBoxes[index++] = FALSE;
	mCheckBoxes[index++] = FALSE;

}

TrActionAdd::TrActionAdd(CString name,
			 int     watch,
			 CString value,
			 int     bools,
			 CString executable,
			 CString args,
			 BOOL    includeTrapData,
			 CString emailDest,
			 CString exploderDest,
			 int     soundPlayDuration,
			 CString soundFile,
			 BOOL    partOfGroup,
			 int      requireDesktop)
  	    :CDialog(TrActionAdd::IDD, NULL),
	     mName(name),
	     mInChangeMode(TRUE),
	     mIncludeTrapData(includeTrapData),
	     mValue(value),
	     mWatch(watch),
	     mExecuteProgram(executable),
	     mExecuteArgs(args),
	     mEmail(FALSE),
	     mExecute(FALSE),
	     mExplode(FALSE),
	     mPage(FALSE),
	     mSound(FALSE),
	     mBools(0),
	     mDiscard(FALSE),
	     mEmailDest(emailDest),
	     mExploderDest(exploderDest),
	     mSoundPlayDuration(soundPlayDuration),
	     mSoundFile(soundFile),
	     mPartOfGroup(partOfGroup),
	     mRequireDesktop(requireDesktop)
{

  if (bools & 1)
    mEmail = TRUE;
  if (bools & 2)
    mExecute = TRUE;
  if (bools & 4)
    mExplode = TRUE;
  if (bools & 8)
    mPage = TRUE;
  if (bools & 16)
    mDiscard = TRUE;
  if (bools & 32)
    mSound = TRUE;

  int index = 0;
  mCheckBoxes[index++] = mEmail;
  mCheckBoxes[index++] = mExecute;
  mCheckBoxes[index++] = mExplode;
  mCheckBoxes[index++] = mDiscard;
  mCheckBoxes[index++] = mSound;

}


void TrActionAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrActionAdd)
	DDX_Control(pDX, IDC_BUTTONEXPLODECONFIGURE, mButtonExplodeConfigure);
	DDX_Control(pDX, IDC_BUTTONSOUNDCONFIGURE, mButtonSoundConfigure);
	DDX_Control(pDX, IDC_BUTTONEMAILCONFIGURE, mButtonEmailConfigure);
	DDX_Control(pDX, IDOK, mAddUpdateButton);
	DDX_Control(pDX, IDC_BUTTONEXECUTECONFIGURE, mButtonExecute);
	DDX_Control(pDX, IDC_EDITNAME, mNameEdit);
	DDX_Check(pDX, IDC_CHECKEMAIL, mEmail);
	DDX_Check(pDX, IDC_CHECKEXECUTE, mExecute);
	DDX_Check(pDX, IDC_CHECKEXPLODE, mExplode);
	DDX_Check(pDX, IDC_CHECKPAGE, mPage);
	DDX_Text(pDX, IDC_EDITVALUE, mValue);
	DDX_CBIndex(pDX, IDC_COMBOWATCH, mWatch);
	DDX_Check(pDX, IDC_CHECKDISCARD, mDiscard);
	DDX_Check(pDX, IDC_CHECKPLAYSOUND, mSound);
	DDX_Check(pDX, IDC_CHECKPARTOFGROUP, mPartOfGroup);
	DDX_Text(pDX, IDC_EDITNAME, mName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrActionAdd, CDialog)
  //{{AFX_MSG_MAP(TrActionAdd)
  ON_BN_CLICKED(IDC_CHECKEMAIL, OnCheckemail)
  ON_BN_CLICKED(IDC_CHECKEXECUTE, OnCheckexecute)
  ON_BN_CLICKED(IDC_CHECKEXPLODE, OnCheckexplode)
  ON_BN_CLICKED(IDC_BUTTONEXECUTECONFIGURE, OnButtonexecuteconfigure)
  ON_CBN_EDITUPDATE(IDC_COMBOWATCH, OnEditupdateCombowatch)
  ON_CBN_SELCHANGE(IDC_COMBOWATCH, OnSelchangeCombowatch)
  ON_BN_CLICKED(IDC_CHECKDISCARD, OnCheckDiscard)
  ON_BN_CLICKED(IDC_BUTTONEMAILCONFIGURE, OnButtonEmailConfigure)
  ON_BN_CLICKED(IDC_BUTTONSOUNDCONFIGURE, OnButtonSoundConfigure)
  ON_BN_CLICKED(IDC_CHECKPLAYSOUND, OnCheckPlaySound)
  ON_BN_CLICKED(IDC_BUTTONEXPLODECONFIGURE, OnButtonexplodeconfigure)
  ON_BN_CLICKED(IDC_CHECKPARTOFGROUP, OnCheckPartOfGroup)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// TrActionAdd message handlers

void TrActionAdd::OnCheckPartOfGroup()
{
  UpdateData(TRUE);

  BOOL val = FALSE;
  if (!mPartOfGroup) // if box is being unchecked, restore the previous values
    val = TRUE;

  int index = 0;

  if (mPartOfGroup) {  // only update if the box is being checked.
    mCheckBoxes[index++] = mEmail;
    mCheckBoxes[index++] = mExecute;
    mCheckBoxes[index++] = mExplode;
    mCheckBoxes[index++] = mDiscard;
    mCheckBoxes[index++] = mSound;
  }

  CButton* c = (CButton*)GetDlgItem(IDC_CHECKEMAIL);
  if (c != NULL)
  {
    if (TRUE == val)
      c->SetCheck(mCheckBoxes[0]);
    else
      c->SetCheck(val);

    c->EnableWindow(val);
    if (val && mCheckBoxes[0])
      mButtonEmailConfigure.EnableWindow(val);
    if (!val)
      mButtonEmailConfigure.EnableWindow(val);
  }
  c = (CButton*)GetDlgItem(IDC_CHECKEXECUTE);
  if (c != NULL)
  {
    if (TRUE == val)
      c->SetCheck(mCheckBoxes[1]);
    else
      c->SetCheck(val);
    c->EnableWindow(val);
    if (val && mCheckBoxes[1])
      mButtonExecute.EnableWindow(val);
    if (!val)
      mButtonExecute.EnableWindow(val);
  }
  c = (CButton*)GetDlgItem(IDC_CHECKEXPLODE);
  if (c != NULL)
  {
    if (TRUE == val)
      c->SetCheck(mCheckBoxes[2]);
    else
      c->SetCheck(val);
    c->EnableWindow(val);
    if (val && mCheckBoxes[2])
      mButtonExplodeConfigure.EnableWindow(val);
    if (!val)
      mButtonExplodeConfigure.EnableWindow(val);
  }
  c = (CButton*)GetDlgItem(IDC_CHECKDISCARD);
  if (c != NULL)
  {
    if (TRUE == val)
      c->SetCheck(mCheckBoxes[3]);
    else
      c->SetCheck(val);
    c->EnableWindow(val);
  }
  c = (CButton*)GetDlgItem(IDC_CHECKPLAYSOUND);
  if (c != NULL)
  {
    if (TRUE == val)
      c->SetCheck(mCheckBoxes[4]);
    else
      c->SetCheck(val);
    c->EnableWindow(val);
    if (val && mCheckBoxes[4])
      mButtonSoundConfigure.EnableWindow(val);
    if (!val)
      mButtonSoundConfigure.EnableWindow(val);
  }
}

void TrActionAdd::OnCheckemail() 
{
  try
  {
    Registry registry("Email");
    if (!registry.getCount())
    {
      AfxMessageBox("There are no email destinations configured",
		    MB_OK | MB_ICONINFORMATION);
      mEmail = FALSE;
      CButton* c = (CButton*)GetDlgItem(IDC_CHECKEMAIL);
      if (c != NULL)
	c->SetCheck(0);
    }
  }
  catch(...)
  {

  }
  UpdateData(TRUE);
  mButtonEmailConfigure.EnableWindow(mEmail);
}

void TrActionAdd::OnCheckexecute() 
{
  UpdateData(TRUE);
  mButtonExecute.EnableWindow(mExecute);
}

void TrActionAdd::OnCheckexplode() 
{
  try
  {
    Registry registry("Exploder");
    if (!registry.getCount())
    {
      AfxMessageBox("There are no exploder destinations configured",
		    MB_OK | MB_ICONINFORMATION);
      mExplode = FALSE;
      CButton* c = (CButton*)GetDlgItem(IDC_CHECKEXPLODE);
      if (c != NULL)
	c->SetCheck(0);
    }
  }
  catch(...)
  {
    
  }
  UpdateData(TRUE);
  mButtonExplodeConfigure.EnableWindow(mExplode);
}

void 
TrActionAdd::OnCheckDiscard() 
{

}

void 
TrActionAdd::OnOK() 
{
  UpdateData(TRUE);
  if (mEmail)
    mBools = 1;
  if (mExecute)
    mBools |= 2;
  if (mExplode)
    mBools |= 4;
  if (mPage)
    mBools |= 8;
  if (mDiscard)
    mBools |= 16;
  if (mSound)
    mBools |= 32;
  
  if (mName.GetLength() == 0)
    {
      AfxMessageBox("You must specify a name for this watch.",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  
  if (mWatch == CB_ERR)
  {
    AfxMessageBox("You must specify something to watch.",
 		  MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  
  if (mValue.GetLength() == 0)
  {
    AfxMessageBox("You must specify a value for which to watch.",
		  MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  BOOL someActionSelected = FALSE;
  if (mExplode)
    someActionSelected = TRUE;

  if (mExecute)
  {
    if (mExecuteProgram.GetLength() == 0)
    {
      AfxMessageBox("You must specify a program to execute.",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
    else
      someActionSelected = TRUE;
  }
  
  if (mEmail)
    someActionSelected = TRUE;    
  
  if (mPage)
    someActionSelected = TRUE;    

  if (mDiscard)
    someActionSelected = TRUE;    

  if (mSound)
  {
    if (mSoundFile.GetLength() == 0)
    {
      AfxMessageBox("You must specify a sound file to play",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
    else
      someActionSelected = TRUE;    
  }

  if (someActionSelected == FALSE && !mPartOfGroup)
  {
      AfxMessageBox("You must specify at least one action",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
  }

  // make sure the name exists
  Registry r;
  BOOL found = FALSE;
  r.ReOpen("Actions");
  int cnt = r.getCount();
  for (int x = 0; x < cnt; x++) {
    r.ReOpen(x);
    if (r.name() == mName) {
      found = TRUE;
      break;
    }
  }

  if (!mInChangeMode) {
    if (!mPartOfGroup && found) {
      AfxMessageBox("A watch with a matching Name was found.  Master watches (the ones that contain the action) must have a unique name.",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }    
  
    if (mPartOfGroup && !found) {
      AfxMessageBox("Did not find a matching Name.  When specifying Part of AND Group, configure the watch and action first.",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  }


  CWnd* cwnd = GetDlgItem(IDC_COMBOWATCH);
  if (cwnd != NULL)
    ((CComboBox*)cwnd)->GetLBText(mWatch, mWatchText);

  // going to allow this now to enable AND and OR of a watch
#if 0
  try
  {
    if (!mInChangeMode)
    {
      int dummy;
      Registry registry("Actions");
      if (registry.find(mWatch,
			mValue,
			dummy))
      {
	AfxMessageBox("Duplicate Entry",
		      MB_OK | MB_ICONEXCLAMATION);
	return;
      }
    }
  }
  catch(...)
  {

  }
#endif
  
  CDialog::OnOK();
}

BOOL TrActionAdd::OnInitDialog() 
{
  CDialog::OnInitDialog();
  if (mInChangeMode)
  {
    mAddUpdateButton.SetWindowText("Update");
    mButtonExplodeConfigure.EnableWindow(mExplode);
    mButtonExecute.EnableWindow(mExecute);
    mButtonEmailConfigure.EnableWindow(mEmail);
    mButtonSoundConfigure.EnableWindow(mSound);
    //    mNameEdit.EnableWindow(FALSE);

    // if this action is the master record (not flagged as part of group)
    // disable the part of group checkbox.  
    CButton* c = (CButton*)GetDlgItem(IDC_CHECKPARTOFGROUP);
    if (c != NULL)
      c->EnableWindow(~mPartOfGroup);

  }
  else
    mAddUpdateButton.SetWindowText("Add");
  
  
  if (mPartOfGroup) {

    CButton* c = (CButton*)GetDlgItem(IDC_CHECKEMAIL);
    if (c != NULL)  {
      c->EnableWindow(false);
      mButtonEmailConfigure.EnableWindow(false);
    }
    c = (CButton*)GetDlgItem(IDC_CHECKEXECUTE);
    if (c != NULL)  {
      c->EnableWindow(false);
      mButtonExecute.EnableWindow(false);
    }
    c = (CButton*)GetDlgItem(IDC_CHECKEXPLODE);
    if (c != NULL)  {
      c->EnableWindow(false);
      mButtonExplodeConfigure.EnableWindow(false);
    }
    c = (CButton*)GetDlgItem(IDC_CHECKDISCARD);
    if (c != NULL)  {
      c->SetCheck(false);
      c->EnableWindow(false);
    }
    c = (CButton*)GetDlgItem(IDC_CHECKPLAYSOUND);
    if (c != NULL)  {
      c->EnableWindow(false);
      mButtonSoundConfigure.EnableWindow(false);
    }
  } // if (partOfGroup)

  if (mWatch != CB_ERR)
  {
    CWnd* cwnd = GetDlgItem(IDC_COMBOWATCH);
    if (cwnd != NULL)
      ((CComboBox*)cwnd)->SetCurSel(mWatch);
  }

  return TRUE;
  
}

void TrActionAdd::OnButtonexecuteconfigure() 
{
  ExecuteAdd dlg(mExecuteProgram, mExecuteArgs, mIncludeTrapData, mRequireDesktop);
  if (dlg.DoModal() == IDOK)
  {
    mExecuteProgram = dlg.mExecutable;
    mExecuteArgs = dlg.mArguments;
    mIncludeTrapData = dlg.mAddTrapData;
    mRequireDesktop = dlg.mRequireDesktop;
  }

#if 0
  CString browseTitle = "Program to Execute";
  CFileDialog dlg(TRUE,
		  _T("exe"),
		  "*.exe",
		  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		  _T("Exe Files|*.exe|Batch Files|*.bat|All Files|*.*||"));
  dlg.m_ofn.lpstrTitle = browseTitle;
  if (dlg.DoModal() == IDOK)
    mExecuteProgram = dlg.GetPathName();

  UpdateData(FALSE);	
#endif
}


void TrActionAdd::OnEditupdateCombowatch() 
{
}


void 
TrActionAdd::OnSelchangeCombowatch() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_COMBOWATCH);
  if (cwnd != NULL)
    ((CComboBox*)cwnd)->GetLBText(mWatch, mWatchText);
}


void TrActionAdd::OnButtonEmailConfigure() 
{
  EmailRecipients dlg(mEmailDest);
  //  dlg.EmailList(mEmailDest);

  int result = dlg.DoModal();
  if (result == IDOK)
  {
    mEmailDest = dlg.EmailList();
  }
}

void 
TrActionAdd::OnButtonSoundConfigure() 
{
  ConfigureSound dlg(mSoundPlayDuration, mSoundFile);

  int result = dlg.DoModal();
  if (result == IDOK)
  {
    mSoundPlayDuration = dlg.mDuration;
    mSoundFile = dlg.mFilename;
  }
}

void 
TrActionAdd::OnCheckPlaySound() 
{
  UpdateData(TRUE);
  mButtonSoundConfigure.EnableWindow(mSound);
}

void TrActionAdd::OnButtonexplodeconfigure() 
{
  ExploderDestinations dlg(mExploderDest);

  int result = dlg.DoModal();
  if (result == IDOK)
  {
    mExploderDest = dlg.ExploderList();
  }
}
