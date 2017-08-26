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

#if !defined(AFX_CMDLINEDLG_H__6CCFDDF7_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
#define AFX_CMDLINEDLG_H__6CCFDDF7_756F_11D4_873E_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CmdLineDlg : public CDialog
{

  public:
    CmdLineDlg(CWnd* pParent = NULL);   // standard constructor

    //{{AFX_DATA(CmdLineDlg)
    enum { IDD = IDD_DIALOGSPECIFYCMDLINE };
    BOOL	mCommunity;
    BOOL	mGenericType;
    BOOL	mIpAddress;
    BOOL	mRealIpAddress;
    BOOL	mSenderOID;
    BOOL	mSpecificType;
    BOOL	mVbData;
    BOOL	mVbOID;
    CString	mCommunityString;
    CString	mGenericTypeString;
    CString	mIpAddressString;
    CString	mRealIpAddressString;
    CString	mSenderOIDString;
    CString	mSpecificTypeString;
    CString	mVbDataString;
    CString	mVbOIDString;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CmdLineDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

  protected:

    // Generated message map functions
    //{{AFX_MSG(CmdLineDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnCheckcommunity();
    afx_msg void OnCheckgenerictype();
    afx_msg void OnCheckipaddress();
    afx_msg void OnCheckrealipaddress();
    afx_msg void OnChecksenderoid();
    afx_msg void OnCheckspecifictype();
    afx_msg void OnCheckvbdata();
    afx_msg void OnCheckvboid();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDLINEDLG_H__6CCFDDF7_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
