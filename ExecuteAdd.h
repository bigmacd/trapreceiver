#if !defined(AFX_EXECUTEADD_H__8167C60F_5272_11D3_8AB5_00A0247B6F8F__INCLUDED_)
#define AFX_EXECUTEADD_H__8167C60F_5272_11D3_8AB5_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class ExecuteAdd : public CDialog
{
  public:
    ExecuteAdd(CWnd* pParent = NULL);
    ExecuteAdd(CString exe, 
	       CString args,
	       BOOL includeTrapData,
	       int requireDesktop);

    // Dialog Data
    //{{AFX_DATA(ExecuteAdd)
    enum { IDD = IDD_DIALOG1 };
    CString	mArguments;
    CString	mExecutable;
    BOOL	mAddTrapData;
    int         mRequireDesktop;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(ExecuteAdd)

  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:

    // Generated message map functions
    //{{AFX_MSG(ExecuteAdd)
    afx_msg void OnButtonbrowse();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXECUTEADD_H__8167C60F_5272_11D3_8AB5_00A0247B6F8F__INCLUDED_)
