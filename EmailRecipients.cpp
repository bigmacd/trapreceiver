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
#include "EmailRecipients.h"
#include "Registry.h"

EmailRecipients::EmailRecipients(CString list)
	        :CDialog(EmailRecipients::IDD, NULL),
		 mInEmailList(list)
{
  //{{AFX_DATA_INIT(EmailRecipients)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void 
EmailRecipients::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(EmailRecipients)
  DDX_Control(pDX, IDC_LIST1, mList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EmailRecipients, CDialog)
	//{{AFX_MSG_MAP(EmailRecipients)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL 
EmailRecipients::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
//////////////////////////
// 6.40
  mList.EnableToolTips(TRUE);
//////////////////////////

  RECT rect;
  mList.GetClientRect(&rect);
  int width = (rect.right) - 5;

  mList.InsertColumn(0, "Select", LVCFMT_LEFT, 60);
  mList.InsertColumn(1, "Recipient", LVCFMT_LEFT, width - 60);
  mList.InsertColumn(2, "id", LVCFMT_LEFT, 0);

  ListView_SetExtendedListViewStyle(mList.m_hWnd, 
				    LVS_EX_CHECKBOXES);
  // | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

  Registry registry("Email");
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

      CString data = registry.to();
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);

//////////////////////////
// 6.40
      CString strItem = _T("From: ");
      strItem += _T(registry.from());
      strItem += _T(" Subject: ");
      strItem += _T(registry.subject());
      strItem += _T(" Message: ");
      strItem += _T(registry.message());
      mList.SetItemToolTipText(listLength, 1, strItem);
//////////////////////////

      data.Format("%d", registry.emailId());
      CString emailid = data.Right(6);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 2;
      lv.pszText = emailid.GetBuffer(0);
      mList.SetItem(&lv);

      if (mInEmailList.Find(emailid) != -1)
	mList.SetCheck(listLength);
    }
  }
  catch(...)
  {

  }
  return TRUE;
}

CString 
EmailRecipients::EmailList()
{
  return mOutEmailList;
}

#if 0
void    
EmailRecipients::EmailList(CString list)
{
  if (list.GetLength() > 0)
  {
    // tokenize the list first (comma separated)
    char* str = list.GetBuffer(0);
    char* comma = NULL;
    do 
    {
      comma = strchr(str, ',');

      if (comma != NULL)
	*comma = 0;

      // find the tokens in the list and check them
      int listLength = mList.GetItemCount();
      for (int x = 0; x < listLength; x++)
      {
	CString id = mList.GetItemText(x, 2);
	if (id == str)
	  mList.SetCheck(x);
      }

      if (comma != NULL)
      {
	*comma = ','; // reset to original value
	str = comma; 
	str++;
      }

    }
    while (comma != NULL);
  }
}
#endif

void EmailRecipients::OnOK() 
{
  // find all the checked items and put them into a comma separated list  
  mOutEmailList.Empty();

  BOOL itemInList = FALSE;

  int count = mList.GetItemCount();
  for (int x = 0; x < count; x++)
  {
    if (mList.GetCheck(x))
    {
      if (itemInList)
	mOutEmailList += ',';
      itemInList = TRUE;

      CString id = mList.GetItemText(x, 2);
      mOutEmailList += id;
    }
  }
  CDialog::OnOK();
}
