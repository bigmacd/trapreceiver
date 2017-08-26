
#include "stdafx.h"
#include "traprcvr.h"
#include "ConfigureSound.h"

ConfigureSound::ConfigureSound(CWnd* pParent /*=NULL*/)
               :CDialog(ConfigureSound::IDD, pParent)
{
  //{{AFX_DATA_INIT(ConfigureSound)
  mDuration = 1;
  mMode = 0;
  mFilename = _T("");
  //}}AFX_DATA_INIT
}
  
ConfigureSound::ConfigureSound(int duration,
			       CString filename)
               :CDialog(ConfigureSound::IDD, NULL),
		mDuration(duration),
		mFilename(filename),
		mMode(0)
{
  if (mDuration > 0)
    mMode = 1;
}
  

void 
ConfigureSound::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ConfigureSound)
  DDX_Control(pDX, IDC_SLIDERDURATION, mDurationSlider);
  DDX_Control(pDX, IDC_STATICDURATIONLABEL, mDurationLabelWindow);
  DDX_Slider(pDX, IDC_SLIDERDURATION, mDuration);
  DDX_Radio(pDX, IDC_RADIOONCE, mMode);
  DDX_Text(pDX, IDC_EDITFILENAME, mFilename);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfigureSound, CDialog)
  //{{AFX_MSG_MAP(ConfigureSound)
  ON_BN_CLICKED(IDC_RADIOONCE, OnRadioOnce)
  ON_BN_CLICKED(IDC_RADIOLOOP, OnRadioLoop)
  ON_WM_HSCROLL()
  ON_BN_CLICKED(IDC_BUTTONBROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
ConfigureSound::OnInitDialog() 
{
  CDialog::OnInitDialog();

  mDurationSlider.SetRange(1, 30, TRUE);
  if (mMode == 1) 
  {
    CString label;
    label.Format("%d Second", mDuration);
    mDurationLabelWindow.SetWindowText(label);
    mDurationSlider.ShowWindow(SW_SHOW);
    mDurationLabelWindow.ShowWindow(SW_SHOW);
  }
  else
  {
    mDurationSlider.ShowWindow(SW_HIDE);
    mDurationLabelWindow.ShowWindow(SW_HIDE);
  }

  return TRUE;
}

void 
ConfigureSound::OnRadioOnce() 
{
  mMode = 0;
  mDurationSlider.ShowWindow(SW_HIDE);
  mDurationLabelWindow.ShowWindow(SW_HIDE);	
}

void 
ConfigureSound::OnRadioLoop() 
{
  mMode = 1;
  
  if (mDuration == 0)
    mDuration = 1;

  CString label;
  label.Format("%d Second", mDuration);

  if (mDuration > 1)
    label += "s";

  mDurationSlider.ShowWindow(SW_SHOW);
  mDurationLabelWindow.ShowWindow(SW_SHOW);

  mDurationLabelWindow.SetWindowText(label);

  mDurationSlider.SetPos(mDuration);
}

void 
ConfigureSound::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  mDuration = ((CSliderCtrl*)pScrollBar)->GetPos();

  CString label;
  label.Format("%d Second", mDuration);

  if (mDuration > 1)
    label += "s";

  mDurationLabelWindow.SetWindowText(label);
  mDurationSlider.SetPos(mDuration);
	
  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void 
ConfigureSound::OnButtonBrowse() 
{
  char dir[256];
  GetCurrentDirectory(255, dir);

  CString browseTitle = "Sound File";
  CFileDialog dlg(TRUE,
		  _T("wav"),
		  "",
		  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		  _T("Sound Files (*.wav)|*.wav|All Files|*.*||"));
  dlg.m_ofn.lpstrTitle = browseTitle;
  if (dlg.DoModal() == IDOK)
  {
    mFilename = dlg.GetPathName();
  }
  UpdateData(FALSE);
  SetCurrentDirectory(dir);	
}

void 
ConfigureSound::OnOK() 
{
  CDialog::OnOK();
  if (mMode == 0)
    mDuration = 0;
}
