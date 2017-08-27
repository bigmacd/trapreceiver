
#if !defined(AFX_TREMAILADD_H__2EC3BB85_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_)
#define AFX_TREMAILADD_H__2EC3BB85_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class TrEmailAdd : public CDialog
{

  private:

    BOOL    mInChangeMode;

  public:

    TrEmailAdd(CWnd* pParent = NULL);
    TrEmailAdd(CString host,
	       CString to,
	       CString from,
	       int port,
	       CString subject,
	       CString message,
	       BOOL needAuth,
	       CString user,
	       CString juniper,
		   BOOL usessl);

    //{{AFX_DATA(TrEmailAdd)
    enum { IDD = IDD_DIALOGEMAILADD };
    CButton	mAuthButton;
    CButton	mAddUpdateButton;
    CString	mFrom;
    CString	mHost;
    CString	mMessage;
    int		mPort;
    CString	mSubject;
    CString	mTo;
    BOOL	mAuthNeeded;
    CString     mUser;
    CString     mJuniper;
	BOOL    usessl;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TrEmailAdd)

  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(TrEmailAdd)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnButtonSetAuth();
    afx_msg void OnCheckAuthRequired();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREMAILADD_H__2EC3BB85_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_)
