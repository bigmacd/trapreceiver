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

#include "stdafx.h"
#include "traprcvr.h"
#include "MibLoadStatus.h"
#include "MibParser.h"
#include "MibDb.h"

extern MibDb gDb;

MibLoadStatus::MibLoadStatus(CWnd* pParent)
              :CDialog(MibLoadStatus::IDD, pParent),
	       mStatus(FALSE),
	       mOtherList(NULL),
	       mSizeInitialized(FALSE)
{
  //{{AFX_DATA_INIT(MibLoadStatus)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

MibLoadStatus::MibLoadStatus(CString filename, CListBox* otherList)
//MibLoadStatus::MibLoadStatus(CString filename)
              :CDialog(MibLoadStatus::IDD, NULL),
	       mStatus(FALSE),
	       mFileName(filename),
	       mOtherList(otherList),
	       mSizeInitialized(FALSE)
{
}


void 
MibLoadStatus::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(MibLoadStatus)
  DDX_Control(pDX, IDC_LIST2, mList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MibLoadStatus, CDialog)
  //{{AFX_MSG_MAP(MibLoadStatus)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
MibLoadStatus::OnInitDialog() 
{
  // get the minimum window size
  CRect recT;
  GetWindowRect(recT);
  mMinSize = CPoint(recT.Width(), recT.Height());
  mSizeInitialized = TRUE;

  mDlgResizeHelper.Init(m_hWnd);
  mDlgResizeHelper.Fix(mList, 
		       DlgResizeHelper::kLeft, 
		       DlgResizeHelper::kNoVFix);
  mDlgResizeHelper.Fix(IDOK,
		       DlgResizeHelper::kWidthLeft,DlgResizeHelper::kHeightBottom);
		       //		       DlgResizeHelper::kNoHFix,
		       //		       DlgResizeHelper::kNoVFix);

  CDialog::OnInitDialog();

  ////////////////////////////////////////////////////////////////////
  MibParser parser;
  bool status = parser.Parse(&gDb, mFileName.GetBuffer(0));
  
  // check if it is already loaded
  if (false == status) // probably already - need better error reporting from parser.
  { 
      CString msg = "The MIB ";
      msg += parser.MibName();
      msg += " is already loaded!";
      mList.AddString(msg.GetBuffer(0));
      //      smiExit();
      return TRUE;
  }
  
  // do some other reporting back to the user
  ////////////////////////////////////////////////////////////////////

  CString msg;
  msg.Empty();

  //  msg.Format("%d Object definitions were added.", numberOfObjectDefinitions);
  msg.Format("%d variables were parsed.", parser.NumberOfVariables());
  mList.AddString(msg.GetBuffer(0));
  msg.ReleaseBuffer();

  msg.Empty();
  msg.Format("%d Object IDs were added.", parser.NumberOfObjectIds());
  mList.AddString(msg.GetBuffer(0));
  msg.ReleaseBuffer();

  mOtherList->AddString(parser.MibName());
  return TRUE; 
}

void 
MibLoadStatus::OnSize(UINT nType, int cx, int cy) 
{
  CDialog::OnSize(nType, cx, cy);
  mDlgResizeHelper.OnSize();
}

void 
MibLoadStatus::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI ) 
{
  if (mSizeInitialized)
    lpMMI->ptMinTrackSize = mMinSize;
}
