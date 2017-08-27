
#if !defined(AFX_TREXPLODERADDDLG_H__2EC3BB87_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_)
#define AFX_TREXPLODERADDDLG_H__2EC3BB87_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class TrExploderAddDlg : public CDialog
{

  private:

    BOOL    mInChangeMode;
    int     mExploderNumber;

  public:
	
    TrExploderAddDlg(CWnd* pParent = NULL);
    TrExploderAddDlg(CString host, 
		     CString port);

    //    int     doModal(int nothing);

    //{{AFX_DATA(TrExploderAddDlg)
    enum { IDD = IDD_DIALOGEXPLODERADD };
    CButton	mAddUpdateButton;
    CString	mHost;
    CString	mPort;
	//}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TrExploderAddDlg)

  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(TrExploderAddDlg)
    virtual BOOL OnInitDialog();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonAdd();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREXPLODERADDDLG_H__2EC3BB87_E55A_11D2_8A9B_00A0247B6F8F__INCLUDED_)
