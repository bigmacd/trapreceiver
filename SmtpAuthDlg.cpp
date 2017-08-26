// SmtpAuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "traprcvr.h"
#include "SmtpAuthDlg.h"
#include "Base64.h"


SmtpAuthDlg::SmtpAuthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SmtpAuthDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(SmtpAuthDlg)
  mPassword = _T("");
  mUser = _T("");
  //}}AFX_DATA_INIT
}
  
SmtpAuthDlg::SmtpAuthDlg(CString user, CString password)
            :CDialog(SmtpAuthDlg::IDD, NULL)
{
  CBase64 cb6;
  cb6.Decode(user.GetBuffer(0));
  const char* decodeMsg = cb6.DecodedMessage();
  mUser = decodeMsg;

  CBase64 cb62;
  cb62.Decode(password.GetBuffer(0));
  decodeMsg = cb62.DecodedMessage();
  mPassword = decodeMsg;
}
  
void 
SmtpAuthDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(SmtpAuthDlg)
  DDX_Text(pDX, IDC_EDITPASSWORD, mPassword);
  DDX_Text(pDX, IDC_EDITUSER, mUser);
  //}}AFX_DATA_MAP
}
  
  
BEGIN_MESSAGE_MAP(SmtpAuthDlg, CDialog)
    //{{AFX_MSG_MAP(SmtpAuthDlg)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString 
SmtpAuthDlg::User()
{
  CBase64 cb62;
  cb62.Encode(mUser.GetBuffer(0));
  CString retVal;
  retVal = cb62.EncodedMessage();
  return retVal;
}

CString 
SmtpAuthDlg::Password()
{
  CBase64 cb62;
  cb62.Encode(mPassword.GetBuffer(0));
  CString retVal;
  retVal = cb62.EncodedMessage();
  return retVal;
}

/////////////////////////////////////////////////////////////////////////////
// SmtpAuthDlg message handlers
