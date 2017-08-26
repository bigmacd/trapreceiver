// ApplicationLogging.cpp : implementation file
//

#include "stdafx.h"
#include "traprcvr.h"
#include "ApplicationLogging.h"
#include "Registry.h"

ApplicationLogging::ApplicationLogging(CWnd* pParent /*=NULL*/)
	           :CDialog(ApplicationLogging::IDD, pParent)
{
  //{{AFX_DATA_INIT(ApplicationLogging)
  mSmtpLogging = FALSE;
  mActionsLogging = FALSE;
  mLoggingLogging = FALSE;
  //}}AFX_DATA_INIT
}


void 
ApplicationLogging::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ApplicationLogging)
  DDX_Check(pDX, IDC_CHECKSMTP, mSmtpLogging);
  DDX_Check(pDX, IDC_CHECKACTIONS, mActionsLogging);
  DDX_Check(pDX, IDC_CHECK3, mLoggingLogging);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ApplicationLogging, CDialog)
  //{{AFX_MSG_MAP(ApplicationLogging)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL 
ApplicationLogging::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  Registry r;
  try
  {
    mSmtpLogging = r.smtpLogging();
  }
  catch(...)
  {
    mSmtpLogging = FALSE;
    r.smtpLogging(FALSE);
  }	

  try  { mActionsLogging = r.actionLogging(); }
  catch(...)  
  {
    mActionsLogging = FALSE;
    r.actionLogging(FALSE);
  }	

  try  { mLoggingLogging = r.loggingLogging(); }
  catch(...)  
  {
    mLoggingLogging = FALSE;
    r.loggingLogging(FALSE);
  }	

  UpdateData(FALSE);
	
  return TRUE;
}

void 
ApplicationLogging::OnOK() 
{
  UpdateData(TRUE);

  Registry r;
  r.smtpLogging(mSmtpLogging);
  r.actionLogging(mActionsLogging);
  r.loggingLogging(mLoggingLogging);

  CDialog::OnOK();
}

