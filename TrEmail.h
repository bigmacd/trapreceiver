//  Copyright (c) 1997 Network Computing Technologies, Inc.
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

#if !defined(AFX_TREMAIL_H__3C49E6C4_E492_11D2_8A9B_00A0247B6F8F__INCLUDED_)
#define AFX_TREMAIL_H__3C49E6C4_E492_11D2_8A9B_00A0247B6F8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class TrEmail : public CPropertyPage
{
  DECLARE_DYNCREATE(TrEmail)

  private:

    int  GetCurrentIndex();
    void DoModify(int currentIndex);

  // Construction
  public:

    TrEmail();
    ~TrEmail();

    // Dialog Data
    //{{AFX_DATA(TrEmail)
    enum { IDD = IDD_PROPPAGEEMAIL };
    CListCtrl	mList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(TrEmail)
	public:
	virtual BOOL OnApply();
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(TrEmail)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonModify();
	afx_msg void OnDblClkListActions(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREMAIL_H__3C49E6C4_E492_11D2_8A9B_00A0247B6F8F__INCLUDED_)
