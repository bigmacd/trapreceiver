//  Copyright (c) 1997-2001 Network Computing Technologies, Inc.
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
//

#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrMisc.h"
#include "Registry.h"

IMPLEMENT_DYNCREATE(TrMisc, CPropertyPage)

TrMisc::TrMisc() : CPropertyPage(TrMisc::IDD)
{
  //{{AFX_DATA_INIT(TrMisc)
  mLogFileSize = _T("");
  mPingTimeout = _T("");
  mPortNumber = _T("");
  mDedupNumber = _T("");
  mCheckRev = FALSE;
  mCheckDoDedup = FALSE;
  mCheckTray = FALSE;
  mCheckInforms = FALSE;
  mCheckAddVarbinds = FALSE;
  //}}AFX_DATA_INIT
}

TrMisc::~TrMisc()
{
}

void TrMisc::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrMisc)
  DDX_Control(pDX, IDC_SPINPORT, mSpinPort);
  DDX_Control(pDX, IDC_SPINDEDUP, mSpinDedup);
  DDX_Control(pDX, IDC_SPINPINGTIMEOUT, mSpinPingTimeout);
  DDX_Control(pDX, IDC_SPINLOGFILESIZE, mSpinLogFileSize);
  DDX_Text(pDX, IDC_EDITLOGFILESIZE, mLogFileSize);
  DDX_Text(pDX, IDC_EDITPINGTIMEOUT, mPingTimeout);
  DDX_Text(pDX, IDC_EDITPORTNUMBER, mPortNumber);
  DDX_Text(pDX, IDC_EDITDEDUPNUMBER, mDedupNumber);
  DDX_Check(pDX, IDC_CHECKREV, mCheckRev);
  DDX_Check(pDX, IDC_CHECKDODEDUP, mCheckDoDedup);
  DDX_Check(pDX, IDC_CHECKTRAY, mCheckTray);
  DDX_Check(pDX, IDC_CHECKINFORMS, mCheckInforms);
  DDX_Check(pDX, IDC_CHECKADDVARBINDS, mCheckAddVarbinds);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrMisc, CPropertyPage)
  //{{AFX_MSG_MAP(TrMisc)
  ON_BN_CLICKED(IDC_CHECKREV, OnCheckrev)
  ON_BN_CLICKED(IDC_CHECKDODEDUP, OnCheckDoDedup)
  ON_BN_CLICKED(IDC_CHECKTRAY, OnChecktray)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPINLOGFILESIZE, OnDeltaposSpinlogfilesize)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPINGTIMEOUT, OnDeltaposSpinpingtimeout)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPORT, OnDeltaposSpinport)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPINDEDUP, OnDeltaposSpinDedup)
  ON_BN_CLICKED(IDC_CHECKINFORMS, OnCheckInforms)
  ON_BN_CLICKED(IDC_CHECKADDVARBINDS, OnCheckAddVarbinds)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrMisc message handlers

void TrMisc::OnCheckrev() 
{
  SetModified(TRUE);
}

void TrMisc::OnCheckDoDedup() 
{
  SetModified(TRUE);
}

void TrMisc::OnChecktray() 
{
  SetModified(TRUE);
}

void 
TrMisc::OnCheckInforms() 
{
  SetModified(TRUE);
}

void 
TrMisc::OnCheckAddVarbinds() 
{
  SetModified(TRUE);
}

void 
TrMisc::OnDeltaposSpinlogfilesize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
	
	*pResult = 0;
}

void TrMisc::OnDeltaposSpinpingtimeout(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
	
	*pResult = 0;
}

void TrMisc::OnDeltaposSpinport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
  
	*pResult = 0;
}

void TrMisc::OnDeltaposSpinDedup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
  
	*pResult = 0;
}

BOOL 
TrMisc::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();

  UDACCEL udaccel[5];
  memset(udaccel, 0, 5*sizeof(UDACCEL));
  udaccel[0].nSec = 1;
  udaccel[0].nInc = 1;
  udaccel[1].nSec = 2;
  udaccel[1].nInc = 5;
  udaccel[2].nSec = 5;
  udaccel[2].nInc = 10;
  udaccel[3].nSec = 8;
  udaccel[3].nInc = 50;

  mSpinLogFileSize.SetAccel(3, (UDACCEL*)&udaccel);
  mSpinPingTimeout.SetAccel(4, (UDACCEL*)&udaccel);
  mSpinPort.SetAccel(4, (UDACCEL*)&udaccel);
  mSpinDedup.SetAccel(4, (UDACCEL*)&udaccel);

  mSpinLogFileSize.SetRange(1, 100);
  mSpinPingTimeout.SetRange(100,  32767);
  mSpinPort.SetRange(1, 32767);
  mSpinDedup.SetRange(1, 32767);

  Registry registry;
  try {
    mSpinLogFileSize.SetPos(registry.filesize());
    mLogFileSize.Format("%d", registry.filesize());

    mSpinPingTimeout.SetPos(100); //registry.pingTimeout());
    mPingTimeout.Format("%d", 100); // registry.pingTimeout());
    
    mSpinPort.SetPos(registry.port());
    mPortNumber.Format("%d", registry.port());

    int dedupint = 60;
    try {
      dedupint = registry.dedupinterval();
    }
    catch(...) {
      //      registry.dedupinterval(dedupint);
    }
    mSpinDedup.SetPos(dedupint);
    mDedupNumber.Format("%d", dedupint);
    
    mCheckRev = registry.reverseLookup();
    mCheckTray = registry.trayIcon();
    BOOL dodedup = FALSE;
    try {
      dodedup = registry.deduplicate();
    }
    catch (...) {
      registry.deduplicate(dodedup);
    }
    mCheckDoDedup = dodedup;
  }
  catch(...)
  {
    
  }
  try 
  {
    mCheckInforms = registry.informs();
  }
  catch(...)
  {
    mCheckInforms = FALSE;
    registry.informs(mCheckInforms);
  }

  try {
    mCheckAddVarbinds = registry.addVarbinds();
  }
  catch(...) {
    mCheckAddVarbinds = FALSE;
    registry.addVarbinds(mCheckAddVarbinds);
  }

  UpdateData(FALSE);
  return TRUE;
}



BOOL TrMisc::OnApply() 
{
  Registry registry;
  try {
    registry.filesize(atoi(mLogFileSize.GetBuffer(0)));
    //    registry.pingTimeout(atoi(mPingTimeout.GetBuffer(0)));
    registry.port(atoi(mPortNumber.GetBuffer(0)));
    registry.dedupinterval(atoi(mDedupNumber.GetBuffer(0)));
    
    registry.reverseLookup(mCheckRev);
    registry.deduplicate(mCheckDoDedup);
    registry.trayIcon(mCheckTray);
    registry.informs(mCheckInforms);
    registry.addVarbinds(mCheckAddVarbinds);
  }
  catch(...)
  {
    
  }

  return CPropertyPage::OnApply();
}

