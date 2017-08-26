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

#if !defined(AFX_MIBFILEBROWSE_H__EE123FA3_F4BC_11D5_877D_00A0247B6F8F__INCLUDED_)
#define AFX_MIBFILEBROWSE_H__EE123FA3_F4BC_11D5_877D_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MibFileBrowse : public CDialog
{
  // Construction
  public:
    MibFileBrowse(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(MibFileBrowse)
    enum { IDD = IDD_DIALOGLOADMIBFILE };
    CString	mMibFilename;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(MibFileBrowse)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(MibFileBrowse)
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
      };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIBFILEBROWSE_H__EE123FA3_F4BC_11D5_877D_00A0247B6F8F__INCLUDED_)
