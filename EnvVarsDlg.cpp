//  Copyright (c) 2000 Network Computing Technologies, Inc.
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
#include "traprcvr.h"
#include "EnvVarsDlg.h"
#include "Registry.h"

EnvVarsDlg::EnvVarsDlg(CWnd* pParent /*=NULL*/)
	   :CDialog(EnvVarsDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(EnvVarsDlg)
  mCommunity = FALSE;
  mGenericType = FALSE;
  mIpAddress = FALSE;
  mRealIpAddress = FALSE;
  mSenderOID = FALSE;
  mSpecificType = FALSE;
  mVbData = FALSE;
  mVbOID = FALSE;
  mCommunityString = _T("");
  mGenericTypeString = _T("");
  mIpAddressString = _T("");
  mRealIpAddressString = _T("");
  mSenderOIDString = _T("");
  mSpecificTypeString = _T("");
  mVbDataString = _T("");
  mVbOIDString = _T("");
  //}}AFX_DATA_INIT
}


void EnvVarsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(EnvVarsDlg)
  DDX_Check(pDX, IDC_CHECKCOMMUNITY, mCommunity);
  DDX_Check(pDX, IDC_CHECKGENERICTYPE, mGenericType);
  DDX_Check(pDX, IDC_CHECKIPADDRESS, mIpAddress);
  DDX_Check(pDX, IDC_CHECKREALIP1, mRealIpAddress);
  DDX_Check(pDX, IDC_CHECKSENDEROID, mSenderOID);
  DDX_Check(pDX, IDC_CHECKSPECIFICTYPE, mSpecificType);
  DDX_Check(pDX, IDC_CHECKVARBINDDATA, mVbData);
  DDX_Check(pDX, IDC_CHECKVARBINDOID, mVbOID);
  DDX_Text(pDX, IDC_EDITCOMMUNITY, mCommunityString);
  DDX_Text(pDX, IDC_EDITGENERICTYPE, mGenericTypeString);
  DDX_Text(pDX, IDC_EDITIPADDRESS, mIpAddressString);
  DDX_Text(pDX, IDC_EDITREALIP1, mRealIpAddressString);
  DDX_Text(pDX, IDC_EDITSENDEROID, mSenderOIDString);
  DDX_Text(pDX, IDC_EDITSPECIFICTYPE, mSpecificTypeString);
  DDX_Text(pDX, IDC_EDITVBDATA, mVbDataString);
  DDX_Text(pDX, IDC_EDITVBOID, mVbOIDString);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EnvVarsDlg, CDialog)
  //{{AFX_MSG_MAP(EnvVarsDlg)
  ON_BN_CLICKED(IDC_CHECKCOMMUNITY, OnCheckcommunity)
  ON_BN_CLICKED(IDC_CHECKGENERICTYPE, OnCheckgenerictype)
  ON_BN_CLICKED(IDC_CHECKIPADDRESS, OnCheckipaddress)
  ON_BN_CLICKED(IDC_CHECKREALIP1, OnCheckrealipaddress)
  ON_BN_CLICKED(IDC_CHECKSENDEROID, OnChecksenderoid)
  ON_BN_CLICKED(IDC_CHECKSPECIFICTYPE, OnCheckspecifictype)
  ON_BN_CLICKED(IDC_CHECKVARBINDDATA, OnCheckvarbinddata)
  ON_BN_CLICKED(IDC_CHECKVARBINDOID, OnCheckvarbindoid)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void 
EnvVarsDlg::OnCheckcommunity() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITCOMMUNITY);
  if (cwnd != NULL)
    cwnd->EnableWindow(mCommunity);
}

void 
EnvVarsDlg::OnCheckgenerictype() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITGENERICTYPE);
  if (cwnd != NULL)
    cwnd->EnableWindow(mGenericType);	
}

void 
EnvVarsDlg::OnCheckipaddress() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITIPADDRESS);
  if (cwnd != NULL)
    cwnd->EnableWindow(mIpAddress);	
}

void 
EnvVarsDlg::OnCheckrealipaddress() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITREALIP1);
  if (cwnd != NULL)
    cwnd->EnableWindow(mRealIpAddress);	
}

void 
EnvVarsDlg::OnChecksenderoid() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITSENDEROID);
  if (cwnd != NULL)
    cwnd->EnableWindow(mSenderOID);		
}

void 
EnvVarsDlg::OnCheckspecifictype() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITSPECIFICTYPE);
  if (cwnd != NULL)
    cwnd->EnableWindow(mSpecificType);		
}

void 
EnvVarsDlg::OnCheckvarbinddata() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITVBDATA);
  if (cwnd != NULL)
    cwnd->EnableWindow(mVbData);	
}

void 
EnvVarsDlg::OnCheckvarbindoid() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITVBOID);
  if (cwnd != NULL)
    cwnd->EnableWindow(mVbOID);		
}

BOOL 
EnvVarsDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  Registry r;
  try
  {
    mCommunity = r.includeEnvCommunity();
    CWnd* cwnd = GetDlgItem(IDC_EDITCOMMUNITY);
    if (cwnd != NULL)
      cwnd->EnableWindow(mCommunity);		
    mIpAddress = r.includeEnvIpAddress();
    cwnd = GetDlgItem(IDC_EDITIPADDRESS);
    if (cwnd != NULL)
      cwnd->EnableWindow(mIpAddress);		
    mGenericType = r.includeEnvGenericType();
    cwnd = GetDlgItem(IDC_EDITGENERICTYPE);
    if (cwnd != NULL)
      cwnd->EnableWindow(mGenericType);		
    mSpecificType = r.includeEnvSpecificType();
    cwnd = GetDlgItem(IDC_EDITSPECIFICTYPE);
    if (cwnd != NULL)
      cwnd->EnableWindow(mSpecificType);		
    mSenderOID = r.includeEnvSenderOID();
    cwnd = GetDlgItem(IDC_EDITSENDEROID);
    if (cwnd != NULL)
      cwnd->EnableWindow(mSenderOID);		
    mVbData = r.includeEnvVbData();
    cwnd = GetDlgItem(IDC_EDITVBDATA);
    if (cwnd != NULL)
      cwnd->EnableWindow(mVbData);		
    mVbOID = r.includeEnvVbOID();
    cwnd = GetDlgItem(IDC_EDITVBOID);
    if (cwnd != NULL)
      cwnd->EnableWindow(mVbOID);		

    mCommunityString = r.envCommunity();
    mIpAddressString = r.envIpAddress();
    mGenericTypeString = r.envGenericType();
    mSpecificTypeString = r.envSpecificType();
    mSenderOIDString = r.envSenderOID();
    mVbDataString = r.envVbData();
    mVbOIDString = r.envVbOID();
  }
  catch(int x)
  {
    if (x == 0)
    {
      r.includeEnvCommunity(FALSE);
      r.includeEnvIpAddress(FALSE);
      r.includeEnvGenericType(FALSE);
      r.includeEnvSpecificType(FALSE);
      r.includeEnvSenderOID(FALSE);
      r.includeEnvVbData(FALSE);
      r.includeEnvVbOID(FALSE);

      CString empty;
      empty.Empty();
      r.envCommunity(empty);
      r.envIpAddress(empty);
      r.envGenericType(empty);
      r.envSpecificType(empty);
      r.envSenderOID(empty);
      r.envVbData(empty);
      r.envVbOID(empty);
    }
  }


  try {
    mRealIpAddress = r.includeEnvRealIpAddress();
  }
  catch(...) {
      r.includeEnvRealIpAddress(FALSE);
      mRealIpAddress = FALSE;
  }
  CWnd* cwnd = GetDlgItem(IDC_EDITREALIP1);
  if (cwnd != NULL)
    cwnd->EnableWindow(mRealIpAddress);		
  
  try {
    mRealIpAddressString = r.envRealIpAddress();
  }
  catch(...) {
    CString empty;
    empty.Empty();
    r.envRealIpAddress(empty);
    mRealIpAddressString = empty;
  }

  UpdateData(FALSE);
  return TRUE;
}

void 
EnvVarsDlg::OnOK() 
{
	

  CDialog::OnOK();
}
