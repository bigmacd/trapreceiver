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
#include "TrMonitor.h"


TrMonitor::TrMonitor(unsigned long waitHint)
	: CDialog(TrMonitor::IDD, NULL),
	  mWaitHint(waitHint),
	  mScManager(NULL),
	  mService(NULL)
{
  //  if (mWaitHint > 100)
  //    mWaitHint = 100;
  mWaitHint = 15; // new functionality

  mScManager = OpenSCManager(NULL,
			     NULL,
			     NULL);
  if (mScManager == NULL)
    EndDialog(0);

  mService = OpenService(mScManager,
			 "TrapRcvr",
			 SERVICE_ALL_ACCESS);

  if (mService == NULL)
    EndDialog(0);
  //{{AFX_DATA_INIT(TrMonitor)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void 
TrMonitor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrMonitor)
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrMonitor, CDialog)
  //{{AFX_MSG_MAP(TrMonitor)
  ON_WM_TIMER()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void 
TrMonitor::OnTimer(UINT nIDEvent) 
{
  CDialog::OnTimer(nIDEvent);

  if (nIDEvent == TRMONITORTIMERID)
  {

    if (!QueryServiceStatus(mService,
			   &mScs))
    {
      KillTimer(TRMONITORTIMERID);
      EndDialog(0);
    }
      
    if (mScs.dwCurrentState == SERVICE_RUNNING)
    {
      KillTimer(TRMONITORTIMERID);
      EndDialog(0);
    }
    

    CWnd* cwnd = GetDlgItem(IDC_PROGRESS);
    CProgressCtrl* p = (CProgressCtrl*)cwnd;
    p->StepIt();
    mWaitHint--;
    if (!mWaitHint)
    {
    }
  }
}

BOOL 
TrMonitor::OnInitDialog() 
{
  CDialog::OnInitDialog();

  mCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);

  CWnd* cwnd = GetDlgItem(IDC_PROGRESS);
  CProgressCtrl* p = (CProgressCtrl*)cwnd;
  p->SetRange(1, (int)mWaitHint);
  p->SetPos(1);
  p->SetStep(1);
  SetTimer(TRMONITORTIMERID, 1000, NULL);
  return TRUE; 
}



BOOL TrMonitor::PreTranslateMessage(MSG* pMsg) 
{
  if (pMsg->message == WM_KEYDOWN ||
      pMsg->message == WM_SYSKEYDOWN)
    return TRUE;

  return CDialog::PreTranslateMessage(pMsg);
}





