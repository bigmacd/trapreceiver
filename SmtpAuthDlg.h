#if !defined(AFX_SMTPAUTHDLG_H__1BFE7B14_96AD_11D5_876C_00A0247B6F8F__INCLUDED_)
#define AFX_SMTPAUTHDLG_H__1BFE7B14_96AD_11D5_876C_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmtpAuthDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SmtpAuthDlg dialog

class SmtpAuthDlg : public CDialog
{
  private:

    CString	mPassword;
    CString	mUser;

    // Construction
  public:

    SmtpAuthDlg(CWnd* pParent = NULL);   // standard constructor
    SmtpAuthDlg(CString user, CString password);

    CString User();
    CString Password();

    // Dialog Data
    //{{AFX_DATA(SmtpAuthDlg)
    enum { IDD = IDD_DIALOGSMTPAUTH };
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SmtpAuthDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:

    // Generated message map functions
    //{{AFX_MSG(SmtpAuthDlg)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
      };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPAUTHDLG_H__1BFE7B14_96AD_11D5_876C_00A0247B6F8F__INCLUDED_)
