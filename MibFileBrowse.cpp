
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



