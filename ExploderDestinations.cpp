//  Copyright (c) 2002-2003 Network Computing Technologies, Inc.
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
#include "ExploderDestinations.h"
#include "Registry.h"


ExploderDestinations::ExploderDestinations(CString list)
                     :CDialog(ExploderDestinations::IDD, NULL),
		      mInExploderList(list)
{
  //{{AFX_DATA_INIT(ExploderDestinations)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void 
ExploderDestinations::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ExploderDestinations)
  DDX_Control(pDX, IDC_LIST1, mList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExploderDestinations, CDialog)
  //{{AFX_MSG_MAP(ExploderDestinations)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
ExploderDestinations::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  RECT rect;
  mList.GetClientRect(&rect);
  int width = (rect.right) - 5;

  mList.InsertColumn(0, "Select", LVCFMT_LEFT, 60);
  mList.InsertColumn(1, "Destination", LVCFMT_LEFT, width - 60);
  mList.InsertColumn(2, "id", LVCFMT_LEFT, 0);

  ListView_SetExtendedListViewStyle(mList.m_hWnd, 
				    LVS_EX_CHECKBOXES);
  // | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

  Registry registry("Exploder");
  int count = (int)registry.getCount();

  try
  {
    for (int listLength = 0; listLength < count; listLength++)
    {
      LV_ITEM lv;
      lv.mask = LVIF_TEXT;

      registry.ReOpen(listLength);
      lv.iItem = listLength;

      lv.mask = LVIF_TEXT;
      lv.iSubItem = 0;
      lv.pszText = "";
      mList.InsertItem(&lv);

      CString data = registry.exploderDestination();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

      data.Format("%d", registry.exploderId());
      CString exploderid = data.Right(6);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 2;
      lv.pszText = exploderid.GetBuffer(0);
      mList.SetItem(&lv);

      if (mInExploderList.Find(exploderid) != -1)
	mList.SetCheck(listLength);
    }
  }
  catch(...)
  {

  }
  return TRUE;
}

void 
ExploderDestinations::OnOK() 
{
  // find all the checked items and put them into a comma separated list  
  mOutExploderList.Empty();

  BOOL itemInList = FALSE;

  int count = mList.GetItemCount();
  for (int x = 0; x < count; x++)
  {
    if (mList.GetCheck(x))
    {
      if (itemInList)
	mOutExploderList += ',';
      itemInList = TRUE;

      CString id = mList.GetItemText(x, 2);
      mOutExploderList += id;
    }
  }
  CDialog::OnOK();
}

CString 
ExploderDestinations::ExploderList()
{
  return mOutExploderList;
}
