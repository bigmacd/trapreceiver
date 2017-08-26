#if !defined(AFX_TRMISC_H__60C32D7B_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRMISC_H__60C32D7B_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrMisc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TrMisc dialog

class TrMisc : public CPropertyPage
{
    DECLARE_DYNCREATE(TrMisc)

      // Construction
  public:
    TrMisc();
    ~TrMisc();

    // Dialog Data
    //{{AFX_DATA(TrMisc)
    enum { IDD = IDD_PROPPAGEMISC };
    CSpinButtonCtrl	mSpinPort;
    CSpinButtonCtrl	mSpinDedup;
    CSpinButtonCtrl	mSpinPingTimeout;
    CSpinButtonCtrl	mSpinLogFileSize;
    CString	mLogFileSize;
    CString	mPingTimeout;
    CString	mPortNumber;
    CString	mDedupNumber;
    BOOL	mCheckRev;
    BOOL	mCheckDoDedup;
    BOOL	mCheckTray;
    BOOL	mCheckInforms;
    BOOL	mCheckAddVarbinds;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(TrMisc)
  public:
    virtual BOOL OnApply();
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(TrMisc)
    afx_msg void OnCheckrev();
    afx_msg void OnCheckDoDedup();
    afx_msg void OnChecktray();
    afx_msg void OnDeltaposSpinlogfilesize(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinpingtimeout(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinport(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinDedup(NMHDR* pNMHDR, LRESULT* pResult);
    virtual BOOL OnInitDialog();
    afx_msg void OnCheckInforms();
    afx_msg void OnCheckAddVarbinds();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

      };

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRMISC_H__60C32D7B_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
