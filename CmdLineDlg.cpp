#include "stdafx.h"
#include "traprcvr.h"
#include "CmdLineDlg.h"
#include "Registry.h"

CmdLineDlg::CmdLineDlg(CWnd* pParent /*=NULL*/)
	   :CDialog(CmdLineDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CmdLineDlg)
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


void CmdLineDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CmdLineDlg)
  DDX_Check(pDX, IDC_CHECKCOMMUNITY, mCommunity);
  DDX_Check(pDX, IDC_CHECKGENERICTYPE, mGenericType);
  DDX_Check(pDX, IDC_CHECKIPADDRESS, mIpAddress);
  DDX_Check(pDX, IDC_CHECKREALIP, mRealIpAddress);
  DDX_Check(pDX, IDC_CHECKSENDEROID, mSenderOID);
  DDX_Check(pDX, IDC_CHECKSPECIFICTYPE, mSpecificType);
  DDX_Check(pDX, IDC_CHECKVBDATA, mVbData);
  DDX_Check(pDX, IDC_CHECKVBOID, mVbOID);
  DDX_Text(pDX, IDC_EDITCOMMUNITY, mCommunityString);
  DDX_Text(pDX, IDC_EDITGENERICTYPE, mGenericTypeString);
  DDX_Text(pDX, IDC_EDITIPADDRESS, mIpAddressString);
  DDX_Text(pDX, IDC_EDITREALIP, mRealIpAddressString);
  DDX_Text(pDX, IDC_EDITSENDEROID, mSenderOIDString);
  DDX_Text(pDX, IDC_EDITSPECIFICTYPE, mSpecificTypeString);
  DDX_Text(pDX, IDC_EDITVBDATA, mVbDataString);
  DDX_Text(pDX, IDC_EDITVBOID, mVbOIDString);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CmdLineDlg, CDialog)
  //{{AFX_MSG_MAP(CmdLineDlg)
  ON_BN_CLICKED(IDC_CHECKCOMMUNITY, OnCheckcommunity)
  ON_BN_CLICKED(IDC_CHECKGENERICTYPE, OnCheckgenerictype)
  ON_BN_CLICKED(IDC_CHECKIPADDRESS, OnCheckipaddress)
  ON_BN_CLICKED(IDC_CHECKREALIP, OnCheckrealipaddress)
  ON_BN_CLICKED(IDC_CHECKSENDEROID, OnChecksenderoid)
  ON_BN_CLICKED(IDC_CHECKSPECIFICTYPE, OnCheckspecifictype)
  ON_BN_CLICKED(IDC_CHECKVBDATA, OnCheckvbdata)
  ON_BN_CLICKED(IDC_CHECKVBOID, OnCheckvboid)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
CmdLineDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  Registry r;
  try
  {
    mCommunity = r.includeCmdCommunity();
    CWnd* cwnd = GetDlgItem(IDC_EDITCOMMUNITY);
    if (cwnd != NULL)
      cwnd->EnableWindow(mCommunity);		
    mIpAddress = r.includeCmdIpAddress();
    cwnd = GetDlgItem(IDC_EDITIPADDRESS);
    if (cwnd != NULL)
      cwnd->EnableWindow(mIpAddress);		
    mGenericType = r.includeCmdGenericType();
    cwnd = GetDlgItem(IDC_EDITGENERICTYPE);
    if (cwnd != NULL)
      cwnd->EnableWindow(mGenericType);		
    mSpecificType = r.includeCmdSpecificType();
    cwnd = GetDlgItem(IDC_EDITSPECIFICTYPE);
    if (cwnd != NULL)
      cwnd->EnableWindow(mSpecificType);		
    mSenderOID = r.includeCmdSenderOID();
    cwnd = GetDlgItem(IDC_EDITSENDEROID);
    if (cwnd != NULL)
      cwnd->EnableWindow(mSenderOID);		
    mVbData = r.includeCmdVbData();
    cwnd = GetDlgItem(IDC_EDITVBDATA);
    if (cwnd != NULL)
      cwnd->EnableWindow(mVbData);		
    mVbOID = r.includeCmdVbOID();
    cwnd = GetDlgItem(IDC_EDITVBOID);
    if (cwnd != NULL)
      cwnd->EnableWindow(mVbOID);		

    mCommunityString = r.cmdCommunity();
    mIpAddressString = r.cmdIpAddress();
    mGenericTypeString = r.cmdGenericType();
    mSpecificTypeString = r.cmdSpecificType();
    mSenderOIDString = r.cmdSenderOID();
    mVbDataString = r.cmdVbData();
    mVbOIDString = r.cmdVbOID();
  }
  catch(int x)
  {
    if (x == 0)
    {
      r.includeCmdCommunity(FALSE);
      r.includeCmdIpAddress(FALSE);
      r.includeCmdGenericType(FALSE);
      r.includeCmdSpecificType(FALSE);
      r.includeCmdSenderOID(FALSE);
      r.includeCmdVbData(FALSE);
      r.includeCmdVbOID(FALSE);

      CString empty;
      empty.Empty();
      r.cmdCommunity(empty);
      r.cmdIpAddress(empty);
      r.cmdGenericType(empty);
      r.cmdSpecificType(empty);
      r.cmdSenderOID(empty);
      r.cmdVbData(empty);
      r.cmdVbOID(empty);
    }
  }	

  try {
    mRealIpAddress = r.includeCmdRealIpAddress();
  }
  catch(...) {
    r.includeCmdRealIpAddress(FALSE);
    mRealIpAddress = FALSE;
  }
  CWnd* cwnd = GetDlgItem(IDC_EDITREALIP);
  if (cwnd != NULL)
    cwnd->EnableWindow(mRealIpAddress);

  try {
    mRealIpAddressString = r.cmdRealIpAddress();
  }
  catch(...) {
    CString empty;
    empty.Empty();
    r.cmdRealIpAddress(empty);
    mRealIpAddressString = empty;
  }

  UpdateData(FALSE);
  return TRUE;
}

void 
CmdLineDlg::OnCheckcommunity() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITCOMMUNITY);
  if (cwnd != NULL)
    cwnd->EnableWindow(mCommunity);
}

void 
CmdLineDlg::OnCheckgenerictype() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITGENERICTYPE);
  if (cwnd != NULL)
    cwnd->EnableWindow(mGenericType);		
}

void 
CmdLineDlg::OnCheckipaddress() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITIPADDRESS);
  if (cwnd != NULL)
    cwnd->EnableWindow(mIpAddress);		
}

void 
CmdLineDlg::OnCheckrealipaddress() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITREALIP);
  if (cwnd != NULL)
    cwnd->EnableWindow(mRealIpAddress);		
}

void 
CmdLineDlg::OnChecksenderoid() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITSENDEROID);
  if (cwnd != NULL)
    cwnd->EnableWindow(mSenderOID);			
}

void 
CmdLineDlg::OnCheckspecifictype() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITSPECIFICTYPE);
  if (cwnd != NULL)
    cwnd->EnableWindow(mSpecificType);			
}

void 
CmdLineDlg::OnCheckvbdata() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITVBDATA);
  if (cwnd != NULL)
    cwnd->EnableWindow(mVbData);		
}

void 
CmdLineDlg::OnCheckvboid() 
{
  UpdateData(TRUE);
  CWnd* cwnd = GetDlgItem(IDC_EDITVBOID);
  if (cwnd != NULL)
    cwnd->EnableWindow(mVbOID);			
}

void CmdLineDlg::OnOK() 
{
	
  CDialog::OnOK();
}
