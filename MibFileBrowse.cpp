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
#include "MibFileBrowse.h"

MibFileBrowse::MibFileBrowse(CWnd* pParent /*=NULL*/)
              :CDialog(MibFileBrowse::IDD, pParent)
{
  //{{AFX_DATA_INIT(MibFileBrowse)
  mMibFilename = _T("");
  //}}AFX_DATA_INIT
}


void MibFileBrowse::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(MibFileBrowse)
  DDX_Text(pDX, IDC_EDITMIBFILENAME, mMibFilename);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MibFileBrowse, CDialog)
  //{{AFX_MSG_MAP(MibFileBrowse)
	ON_BN_CLICKED(IDC_BUTTONBROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void 
MibFileBrowse::OnButtonBrowse() 
{
  char dir[256];
  GetCurrentDirectory(255, dir);

  CString browseTitle = "Mib File Import";
  CFileDialog dlg(TRUE,
		  _T("mib"),
		  "",
		  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		  _T("MIB Files (*.mib, *.my)|*.mib;*.my|Text File (*.txt)|*.txt|All Files|*.*||"));
  dlg.m_ofn.lpstrTitle = browseTitle;
  if (dlg.DoModal() == IDOK)
  {
    mMibFilename = dlg.GetPathName();
  }
  UpdateData(FALSE);

  SetCurrentDirectory(dir);
  
}



