// TrapRcvrDlg.h : header file
//

#if !defined(AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TrapRcvrThread.h"
#include "ActionThread.h"
#include "FlusherThread.h"
#include "ActionMgrThread.h"

/////////////////////////////////////////////////////////////////////////////
// TrapRcvrDlg dialog

class TrapRcvrDlg : public CDialog
{

  private:

    TrapRcvrThread*  mTrapRcvrThread;
    FlusherThread*   mFlusherThread;
    ActionMgrThread* mActionMgrThread;
    int              mThreadCount;
    BOOL             mDoReverse;

// Construction
public:
	TrapRcvrDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(TrapRcvrDlg)
	enum { IDD = IDD_TRAPRCVR_DIALOG };
	CListCtrl	mList;
	CString	mSource;
	CString	mTrapCount;
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
	//{{AFX_MSG(TrapRcvrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnRightClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAPRCVRDLG_H__60C32D65_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)

