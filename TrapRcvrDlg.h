
#if !defined(AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "QueueServer.h"
#include "LogQueueClient.h"
#include "AMTQueueClient.h"
#include "DlgResizeHelper.h"
#include "ActionMgrThread.h"
#include "TrayNot.h"

class TrapRcvrDlg : public CDialog
{

  private:

    int              mThreadCount;
    BOOL             mDoReverse;
    QueueServer*     mQueue;
    LogQueueClient   mLogQueue;    
    AMTQueueClient   mAMTQueue;    
    BOOL             mSizeInitialized;
    ActionMgrThread* actionMgrThread;

  public:

    TrapRcvrDlg(CWnd* pParent = NULL);	// standard constructor
    
    DlgResizeHelper mDlgResizeHelper;

    CTrayNot* mTray;
    BOOL m_bHidden;
    HICON m_pIconList[3];

  // Dialog Data
  //{{AFX_DATA(TrapRcvrDlg)
    enum { IDD = IDD_TRAPRCVR_DIALOG };
    CListCtrl	mList;
    CString	mSource;
    CString	mTrapCount;
    CPoint      mMinSize;
    //}}AFX_DATA
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TrapRcvrDlg)

  protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual void OnOK();
    virtual void OnCancel();

    //{{AFX_MSG(TrapRcvrDlg)
    afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButton1();
    afx_msg void OnRightClickList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLinksList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI );

    //////////////////////////////////////////////////////////////////
    // tray messages
    afx_msg LONG OnTrayNotify ( WPARAM wParam, LPARAM lParam ) ;
    afx_msg void OnTrayRestore();
    afx_msg void OnHide();
    afx_msg void OnUnHide();
    afx_msg void OnDestroy();
    //
    //////////////////////////////////////////////////////////////////

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)


