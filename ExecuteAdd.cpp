// ExecuteAdd.cpp : implementation file
//

#include "stdafx.h"
#include "TrapRcvr.h"
#include "resource.h"
#include "ExecuteAdd.h"

ExecuteAdd::ExecuteAdd(CWnd* pParent /*=NULL*/)
           :CDialog(ExecuteAdd::IDD, pParent)
{
  //{{AFX_DATA_INIT(ExecuteAdd)
  mArguments = _T("");
  mExecutable = _T("");
  mAddTrapData = FALSE;
  mRequireDesktop = FALSE;
  //}}AFX_DATA_INIT
}

ExecuteAdd::ExecuteAdd(CString prog,
		       CString args,
		       BOOL includeTrapData,
		       int requireDesktop)
           :CDialog(ExecuteAdd::IDD, NULL)
{
  mExecutable = prog;
  mArguments = args;
  mAddTrapData = includeTrapData;
  mRequireDesktop = requireDesktop;
}

void ExecuteAdd::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ExecuteAdd)
  DDX_Text(pDX, IDC_EDITARGUMENTS, mArguments);
  DDX_Text(pDX, IDC_EDITPROGRAM, mExecutable);
  DDX_Check(pDX, IDC_CHECKADDTRAPDATA, mAddTrapData);
  DDX_Check(pDX, IDC_CHECKREQUIREDESKTOP, mRequireDesktop);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExecuteAdd, CDialog)
  //{{AFX_MSG_MAP(ExecuteAdd)
  ON_BN_CLICKED(IDC_BUTTONBROWSE, OnButtonbrowse)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void 
ExecuteAdd::OnButtonbrowse() 
{
  char dir[256];
  GetCurrentDirectory(255, dir);

  CString browseTitle = "Program to Execute";
  CFileDialog dlg(TRUE,
		  _T("exe"),
		  "*.exe",
		  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		  _T("Exe Files|*.exe|Batch Files|*.bat|All Files|*.*||"));
  dlg.m_ofn.lpstrTitle = browseTitle;
  if (dlg.DoModal() == IDOK)
    mExecutable = dlg.GetPathName();
  UpdateData(FALSE);
  SetCurrentDirectory(dir);
}

BOOL 
ExecuteAdd::OnInitDialog() 
{
  CDialog::OnInitDialog();
	

  return TRUE; 
}


