
#include "stdafx.h"
#include "TrAlert.h"


TrAlert::TrAlert(CWnd* pParent /*=NULL*/)
	: CDialog(TrAlert::IDD, pParent)
{
  //{{AFX_DATA_INIT(TrAlert)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void 
TrAlert::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrAlert)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrAlert, CDialog)
  //{{AFX_MSG_MAP(TrAlert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void TrAlert::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
