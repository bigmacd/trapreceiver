//  Copyright (c) 1997 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "stdafx.h"
#include "TrEmailAdd.h"
#include "Registry.h"
#include "SmtpAuthDlg.h"

TrEmailAdd::TrEmailAdd(CWnd* pParent)
           :CDialog(TrEmailAdd::IDD, pParent),
	    mInChangeMode(FALSE)
{
  //{{AFX_DATA_INIT(TrEmailAdd)
  mFrom = _T("");
  mHost = _T("");
  mMessage = _T("");
  mPort = 25;
  mSubject = _T("");
  mTo = _T("");
  mUser = _T("");
  mJuniper = _T("");
  mAuthNeeded = FALSE;
  usessl = FALSE;
  //}}AFX_DATA_INIT
}

TrEmailAdd::TrEmailAdd(CString host,
		       CString to,
		       CString from,
		       int port,
		       CString subject,
		       CString message,
		       BOOL needAuth,
		       CString user,
		       CString juniper,
			   BOOL inUsessl)
        	 :CDialog(TrEmailAdd::IDD, NULL),
		  mInChangeMode(TRUE),
		  mHost(host),
		  mTo(to),
		  mFrom(from),
		  mPort(port),
		  mSubject(subject),
		  mMessage(message),
		  mAuthNeeded(needAuth),
		  mUser(user),
		  mJuniper(juniper),
		  usessl(inUsessl)
{
}


void TrEmailAdd::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrEmailAdd)
  DDX_Control(pDX, IDC_BUTTONSETAUTH, mAuthButton);
  DDX_Control(pDX, IDOK, mAddUpdateButton);
  DDX_Text(pDX, IDC_EDITFROM, mFrom);
  DDV_MaxChars(pDX, mFrom, 60);
  DDX_Text(pDX, IDC_EDITSMTPSERVER, mHost);
  DDV_MaxChars(pDX, mHost, 30);
  DDX_Text(pDX, IDC_EDITMESSAGE, mMessage);
  DDV_MaxChars(pDX, mMessage, 512);
  DDX_Text(pDX, IDC_EDITSMTPPORT, mPort);
  DDV_MinMaxInt(pDX, mPort, 0, 65535);
  DDX_Text(pDX, IDC_EDITSUBJECT, mSubject);
	DDV_MaxChars(pDX, mSubject, 255);
  DDX_Text(pDX, IDC_EDITTO, mTo);
  DDV_MaxChars(pDX, mTo, 60);
  DDX_Check(pDX, IDC_CHECKAUTHREQUIRED, mAuthNeeded);
  DDX_Check(pDX, IDC_CHECKSSLREQUIRED, usessl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrEmailAdd, CDialog)
  //{{AFX_MSG_MAP(TrEmailAdd)
  ON_BN_CLICKED(IDC_BUTTONSETAUTH, OnButtonSetAuth)
  ON_BN_CLICKED(IDC_CHECKAUTHREQUIRED, OnCheckAuthRequired)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL 
TrEmailAdd::OnInitDialog() 
{
  CDialog::OnInitDialog();
  if (mInChangeMode)
    mAddUpdateButton.SetWindowText("Update");
  else
    mAddUpdateButton.SetWindowText("Add");

  mAuthButton.EnableWindow(mAuthNeeded);
  return TRUE;
}

void 
TrEmailAdd::OnCancel() 
{
  CDialog::OnCancel();
}

void 
TrEmailAdd::OnOK() 
{
  UpdateData(TRUE);
  if (mHost.GetLength() == 0)
  {
    AfxMessageBox("Incorrect host specified",
		  MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  else
    if (mTo.GetLength() == 0)
    {
      AfxMessageBox("No recepient specified",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
    else
    {

      if (!mInChangeMode)
      {
	int dummy;
	try
	{
	  Registry registry("Email");
	  if (registry.find(mHost,
			    mFrom,
			    mTo,
			    mSubject, 
			    dummy))
	  {
	    AfxMessageBox("Duplicate Entry",
			  MB_OK | MB_ICONEXCLAMATION);
	    return;
	  }
	}
	catch(...)
	{

	}
      }
    }
  CDialog::OnOK();
}

void 
TrEmailAdd::OnButtonSetAuth() 
{
  int result = IDCANCEL;
  SmtpAuthDlg* dlg;

  if (mInChangeMode)
    dlg = new SmtpAuthDlg(mUser, mJuniper);
  else
    dlg = new SmtpAuthDlg;

  result = dlg->DoModal();
    
  if (result == IDOK)
  {
    mUser = dlg->User();
    mJuniper = dlg->Password();
  }
  delete dlg;
}

void 
TrEmailAdd::OnCheckAuthRequired() 
{
  UpdateData(TRUE);
  mAuthButton.EnableWindow(mAuthNeeded);
}
