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
#include "TrExploderAdd.h"
#include "Registry.h"


TrExploderAddDlg::TrExploderAddDlg(CWnd* pParent /*=NULL*/)
	         :CDialog(TrExploderAddDlg::IDD, pParent),
		  mInChangeMode(FALSE),
		  mExploderNumber(-1)
{
  //{{AFX_DATA_INIT(TrExploderAddDlg)
  mHost = _T("");
  mPort = _T("162");
  //}}AFX_DATA_INIT
}

TrExploderAddDlg::TrExploderAddDlg(CString host,
				   CString port)
	         :CDialog(TrExploderAddDlg::IDD, NULL),
		  mInChangeMode(TRUE),
		  mHost(host),
		  mPort(port)
{
  //  Registry registry("Exploder", exploderNumber);
  //  mHost = registry.exploderDestination();
  //  CString port;
  //  port.Format("%d", registry.exploderPort());
  //  mPort = port;
}

void 
TrExploderAddDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrExploderAddDlg)
  DDX_Control(pDX, IDOK, mAddUpdateButton);
  DDX_Text(pDX, IDC_EDITHOST, mHost);
  DDV_MaxChars(pDX, mHost, 35);
  DDX_Text(pDX, IDC_EDITPORT, mPort);
  DDV_MaxChars(pDX, mPort, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrExploderAddDlg, CDialog)
  //{{AFX_MSG_MAP(TrExploderAddDlg)
  ON_BN_CLICKED(IDCANCEL, OnButtonClose)
  ON_BN_CLICKED(IDOK, OnButtonAdd)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

  //int
  //TrExploderAddDlg::doModal(int nothing)
  //{
  //  mHost.Empty();
  //  mPort = _T("162");
  //
  //  return CDialog::DoModal();
  //}

BOOL 
TrExploderAddDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  if (mInChangeMode)
    mAddUpdateButton.SetWindowText("Update");
  else
    mAddUpdateButton.SetWindowText("Add");
  return TRUE;
}

void 
TrExploderAddDlg::OnButtonClose() 
{
  CDialog::OnCancel();	
}

void 
TrExploderAddDlg::OnButtonAdd() 
{
  UpdateData(TRUE);
  if (mHost.GetLength() == 0)
  {
    AfxMessageBox("Incorrect host specified",
		  MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  else
    if (mPort.GetLength() == 0)
    {
      AfxMessageBox("Incorrect port specified",
		    MB_OK | MB_ICONEXCLAMATION);
      return;
    }
    else
    {
      try
      {
	Registry registry("Exploder");
	int port = atoi(mPort.GetBuffer(0));
	
	if (!mInChangeMode)
	{
	  int dummy;
	  if (registry.find(mHost, port, dummy))
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
    }
  CDialog::OnOK();
}

