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
#include "MibUnloadStatus.h"

#include "MibDb.h"
extern MibDb       gDb;

MibUnloadStatus::MibUnloadStatus(CWnd* pParent /*=NULL*/)
	        :CDialog(MibUnloadStatus::IDD, pParent)
{
}

MibUnloadStatus::MibUnloadStatus(CString mib, CListBox* otherList)
  //MibUnloadStatus::MibUnloadStatus(CString mib)
	        :CDialog(MibUnloadStatus::IDD, NULL),
		 mMib(mib),
		 mStatus(FALSE),
		 mSizeInitialized(FALSE),
		 mOtherList(otherList)
{
}


void MibUnloadStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MibUnloadStatus)
	DDX_Control(pDX, IDC_LIST2, mList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MibUnloadStatus, CDialog)
  //{{AFX_MSG_MAP(MibUnloadStatus)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL 
MibUnloadStatus::OnInitDialog() 
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

  std::string mib = mMib.GetBuffer(0);
  gDb.DeleteMib(mib);
  mStatus = TRUE;
  mList.AddString("Mib successfully unloaded.");

#if 0
  unsigned int tRecNum = 0;
  unsigned int oRecNum = 0;
  unsigned int eRecNum = 0;

  CString where;
  where.Empty();
  try
  {
    where = "Error processing Trap Definitions";
    tRecNum = gTrapMap->DeleteRecords(mMib.GetBuffer(0));
    where = "Error processing Object Definitions";
    oRecNum = gOidMap->DeleteRecords(mMib.GetBuffer(0));
    where = "Error processing Enumeration Definitions";
    eRecNum = gEnumMap->DeleteRecords(mMib.GetBuffer(0));
    mStatus = TRUE;

    CString msg;
    msg.Format("%d Trap definitions were removed.", tRecNum);
    mList.AddString(msg.GetBuffer(0));
    msg.ReleaseBuffer();
    
    msg.Empty();
    msg.Format("%d Object Definitions were removed.", oRecNum);
    mList.AddString(msg.GetBuffer(0));
    msg.ReleaseBuffer();
    
    msg.Empty();
    msg.Format("%d Enumeration Defintions were removed.", eRecNum);
    mList.AddString(msg.GetBuffer(0));
    msg.ReleaseBuffer();
  }
  catch(...)
  {
    mList.AddString(where.GetBuffer(0));
  }
#endif	
  return TRUE;
}

void 
MibUnloadStatus::OnSize(UINT nType, int cx, int cy) 
{
  CDialog::OnSize(nType, cx, cy);
  mDlgResizeHelper.OnSize();
}

void 
MibUnloadStatus::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI ) 
{
  if (mSizeInitialized)
    lpMMI->ptMinTrackSize = mMinSize;
}

