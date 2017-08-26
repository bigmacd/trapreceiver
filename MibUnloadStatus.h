//  Copyright (c) 1997-2001 Network Computing Technologies, Inc.
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
//

#include "DlgResizeHelper.h"

#if !defined(AFX_MIBUNLOADSTATUS_H__15C17FFA_FBAA_11D5_877E_00A0247B6F8F__INCLUDED_)
#define AFX_MIBUNLOADSTATUS_H__15C17FFA_FBAA_11D5_877E_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MibUnloadStatus : public CDialog
{
  private:
    CString         mMib;
    CListBox*       mOtherList;
    DlgResizeHelper mDlgResizeHelper;
    CPoint          mMinSize;
    BOOL            mSizeInitialized;

    // Construction
  public:
    MibUnloadStatus(CWnd* pParent = NULL);   // standard constructor
    MibUnloadStatus(CString mib, CListBox* list);
  //    MibUnloadStatus(CString mib);

    BOOL mStatus;
    // Dialog Data
    //{{AFX_DATA(MibUnloadStatus)
    enum { IDD = IDD_DIALOG3 };
    CListBox	mList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(MibUnloadStatus)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:

    // Generated message map functions
    //{{AFX_MSG(MibUnloadStatus)
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI );
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
      };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIBUNLOADSTATUS_H__15C17FFA_FBAA_11D5_877E_00A0247B6F8F__INCLUDED_)
