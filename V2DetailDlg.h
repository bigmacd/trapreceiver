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

#if !defined(AFX_V2DETAILDLG_H__6CCFDDF4_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
#define AFX_V2DETAILDLG_H__6CCFDDF4_756F_11D4_873E_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrapRcvr.h"
#include "Packet.h"


class V2DetailDlg : public CDialog
{
  private:

    Packet*        mPacket;
    void           UpdateFromTrap(Packet* packet);
    CListCtrl*     mTrapList;
    int            mCurrentIndex;

  public:
    V2DetailDlg(CWnd* pParent = NULL);   // standard constructor
    V2DetailDlg(Packet* pkt, CListCtrl* list, int index);

    ~V2DetailDlg();
    //{{AFX_DATA(V2DetailDlg)
	enum { IDD = IDD_TRAPRCVRDTLV2_DIALOG };
	CButton	mPrev;
	CButton	mNext;
    CListCtrl	mList;
    int		mErrorIndex;
    int		mErrorStatus;
    CString	mIpAddress;
    int		mRequestId;
    CString	mCommunity;
	//}}AFX_DATA
    
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(V2DetailDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    HICON m_hIcon;
    // Implementation
  protected:
    
    // Generated message map functions
    //{{AFX_MSG(V2DetailDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
  
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_V2DETAILDLG_H__6CCFDDF4_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
