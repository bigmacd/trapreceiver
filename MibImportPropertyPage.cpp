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
#include "MibImportPropertyPage.h"
#include "Registry.h"
#include "MibFileBrowse.h"
#include "MibLoadStatus.h"
#include "MibUnloadStatus.h"

#include "MibDb.h"
extern MibDb       gDb;


IMPLEMENT_DYNCREATE(MibImportPropertyPage, CPropertyPage)

MibImportPropertyPage::MibImportPropertyPage() 
                      :CPropertyPage(MibImportPropertyPage::IDD)
{
  //{{AFX_DATA_INIT(MibImportPropertyPage)
  mTranslateOids = FALSE;
  //}}AFX_DATA_INIT
}

MibImportPropertyPage::~MibImportPropertyPage()
{
}

void 
MibImportPropertyPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(MibImportPropertyPage)
  DDX_Control(pDX, IDC_BUTTONUNLOADMIB, mUnloadButton);
  DDX_Control(pDX, IDC_LISTLOADEDMIBS, mList);
  DDX_Check(pDX, IDC_CHECKTRANSLATEOIDS, mTranslateOids);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MibImportPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(MibImportPropertyPage)
	ON_BN_CLICKED(IDC_BUTTONIMPORTMIB, OnButtonImportMib)
	ON_BN_CLICKED(IDC_BUTTONUNLOADMIB, OnButtonUnloadMib)
	ON_LBN_SELCHANGE(IDC_LISTLOADEDMIBS, OnSelchangeListLoadedMibs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
MibImportPropertyPage::OnApply() 
{
  Registry registry;
  registry.translateOids(mTranslateOids);

	
  return CPropertyPage::OnApply();
}

BOOL 
MibImportPropertyPage::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
  Registry registry;
  try
  {
    mTranslateOids = registry.translateOids();
  }
  catch(...)
  {
    registry.translateOids(FALSE);
  }

  bool status = gDb.GetAllMIBs();
  while (true == status)
  {
    mList.AddString(gDb.MibName().c_str());
    status = gDb.Next(MIBDATASET);
  }
#if 0
  unsigned int numR = gTrapMap->NumberOfRecords();
  for (unsigned int x = 0; x < numR; x++)
  {
    EbRecord* pRecord = gTrapMap->GetRecord(x);
    if (mList.FindString(-1, pRecord->GetFieldString(1)) == LB_ERR)
      mList.AddString(pRecord->GetFieldString(1));
  }

  numR = gOidMap->NumberOfRecords();
  for (x = 0; x < numR; x++)
  {
    EbRecord* pRecord = gOidMap->GetRecord(x);
    if (mList.FindString(-1, pRecord->GetFieldString(1)) == LB_ERR)
      mList.AddString(pRecord->GetFieldString(1));
  }

  numR = gEnumMap->NumberOfRecords();
  for (x = 0; x < numR; x++)
  {
    EbRecord* pRecord = gEnumMap->GetRecord(x);
    if (mList.FindString(-1, pRecord->GetFieldString(1)) == LB_ERR)
      mList.AddString(pRecord->GetFieldString(1));
  }
#endif
  mUnloadButton.EnableWindow(FALSE);
  UpdateData(FALSE);
  return TRUE; 

}

void 
MibImportPropertyPage::OnButtonImportMib() 
{
  MibFileBrowse dlg;
  int result = dlg.DoModal();
  if (result == IDOK)
  {
    CString fileName = dlg.mMibFilename;
    if (fileName.GetLength() > 0)
    {
      MibLoadStatus mls(fileName, &mList);
      //      MibLoadStatus mls(fileName);
      mls.DoModal();
      if (mls.mStatus == TRUE)
	if (mList.FindString(-1, mls.mMibName.GetBuffer(0)) == LB_ERR)
	  mList.AddString(mls.mMibName.GetBuffer(0));
    }
  }
}

void 
MibImportPropertyPage::OnButtonUnloadMib() 
{
  CString mib;
  int index = mList.GetCurSel();
  mList.GetText(index, mib);
  if (mib.GetLength() > 0)
  {
    MibUnloadStatus mus(mib, &mList);
    //    MibUnloadStatus mus(mib);
    mus.DoModal();
    if (mus.mStatus == TRUE)
      mList.DeleteString(index);
  }
}

void 
MibImportPropertyPage::OnSelchangeListLoadedMibs() 
{
  if (mList.GetCurSel() == -1)
    mUnloadButton.EnableWindow(FALSE);  
  else
    mUnloadButton.EnableWindow(TRUE); 
}
