
#include "stdafx.h"
#include "TrLogging.h"
#include "Registry.h"
#include "ApplicationLogging.h"

IMPLEMENT_DYNCREATE(TrLogging, CPropertyPage)

TrLogging::TrLogging() 
          :CPropertyPage(TrLogging::IDD)
{
  //{{AFX_DATA_INIT(TrLogging)
  mLogFileName = _T("");
  mSChar = _T("");
  mSliderMinutes = _T("");
  mSliderUnits = _T("");
  mRadioAppendOnOff = -1;
  mRadioLoggingOnOff = -1;
  mFormatString = _T("");
  //}}AFX_DATA_INIT
}

TrLogging::~TrLogging()
{
}

void 
TrLogging::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TrLogging)
  DDX_Control(pDX, IDC_SLIDERFLUSHINTERVAL, mSlider);
  DDX_Text(pDX, IDC_EDIT1, mLogFileName);
  DDX_Text(pDX, IDC_EDITSEPARATORCHAR, mSChar);
  DDX_Text(pDX, IDC_STATICNUMBEROFMINUTES, mSliderMinutes);
  DDV_MaxChars(pDX, mSliderMinutes, 2);
  DDX_Text(pDX, IDC_STATICUNITS, mSliderUnits);
  DDV_MaxChars(pDX, mSliderUnits, 7);
  DDX_Radio(pDX, IDC_RADIOAPPEND, mRadioAppendOnOff);
  DDX_Radio(pDX, IDC_RADIOLOGGINGON, mRadioLoggingOnOff);
  DDX_Text(pDX, IDC_EDITLOGFILEFORMAT, mFormatString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrLogging, CPropertyPage)
  //{{AFX_MSG_MAP(TrLogging)
  ON_BN_CLICKED(IDC_BUTTONBROWSE, OnButtonBrowse)
  ON_WM_HSCROLL()
  ON_BN_CLICKED(IDC_RADIOAPPEND, OnRadioappend)
  ON_BN_CLICKED(IDC_RADIOLOGGINGOFF, OnRadiologgingoff)
  ON_BN_CLICKED(IDC_RADIOLOGGINGON, OnRadiologgingon)
  ON_BN_CLICKED(IDC_RADIOOVERWRITE, OnRadiooverwrite)
	ON_BN_CLICKED(IDC_BUTTONDEBUG, OnButtonDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL TrLogging::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
  mSlider.SetRangeMax(15, TRUE);
  mSlider.SetRange(1, 15, TRUE);
  mSlider.SetTicFreq(1);  
  Registry registry;
  try
  {
    mLogFileName = registry.filename();
    int minutes = registry.flushInterval();
    mSlider.SetPos(minutes);
    mSliderUnits = "Minute";
    if (minutes > 1)
      mSliderUnits += "s";
    char sliderText[32];
    sprintf_s(sliderText, 32, "%d", minutes);
    mSliderMinutes = sliderText;
    
    if (registry.logging())
      mRadioLoggingOnOff = 0;
    else
      mRadioLoggingOnOff = 1;
    
    
    if (registry.overWrite())
      mRadioAppendOnOff = 1;
    else
      mRadioAppendOnOff = 0;
  }
  catch(...)
  {
    
  }
  try
  {
    mFormatString = registry.formatString();
  }
  catch(...)
  {
    mFormatString = "%v%i%t%o%g%s%bOTD";
    registry.formatString(mFormatString);
  }

  try {
    mSChar = registry.separatorchar();
  }
  catch(...) {
    mSChar = " ";
    registry.formatString(mSChar);
  }

  CEdit* cedit = (CEdit*)GetDlgItem(IDC_EDITSEPARATORCHAR);
  if (cedit != NULL)
    cedit->SetLimitText(1);

  UpdateData(FALSE);
  return TRUE;
}

void 
TrLogging::OnButtonBrowse() 
{
  char dir[256];
  GetCurrentDirectory(255, dir);

  CString browseTitle = "Log to File";
  CFileDialog dlg(TRUE,
		  _T("log"),
		  "TrapRcvr.log",
		  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		  _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files|*.*||"));
  dlg.m_ofn.lpstrTitle = browseTitle;
  if (dlg.DoModal() == IDOK)
  {
    mLogFileName = dlg.GetPathName();
    SetModified(TRUE);
  }
  UpdateData(FALSE);
  SetCurrentDirectory(dir);
}


void 
TrLogging::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  int newPos = ((CSliderCtrl*)pScrollBar)->GetPos();
  char cNewPos[32];
  sprintf_s(cNewPos, 32, "%d", newPos);
  mSliderMinutes = cNewPos;

  mSliderUnits.Empty();
  mSliderUnits = "Minute";
  if (newPos > 1)
    mSliderUnits += "s";

  UpdateData(FALSE);	

  SetModified(TRUE);
  CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void TrLogging::OnRadioappend() 
{
  try
  {
    Registry registry;
    if (registry.overWrite())
      SetModified(TRUE);
  }
  catch(...)
  {

  }
}

void TrLogging::OnRadiooverwrite() 
{
  try
  {
    Registry registry;
    if (!registry.overWrite())
      SetModified(TRUE);
  }
  catch(...)
  {

  }
}

void 
TrLogging::OnRadiologgingoff() 
{
  try
  {
    Registry registry;
    if (registry.logging())
      SetModified(TRUE);
  }
  catch(...)
  {

  }
}

void TrLogging::OnRadiologgingon() 
{
  try 
  {
    Registry registry;
    if (!registry.logging())
      SetModified(TRUE);
  }
  catch(...)
  {

  }
}


BOOL 
TrLogging::OnApply() 
{
  try
  {
    Registry registry;
    registry.logging(!mRadioLoggingOnOff);
    registry.overWrite(mRadioAppendOnOff);
    registry.filename(mLogFileName);
    if (mSChar.GetLength() == 0)
      mSChar = ' '; // has to be at least a space
    registry.separatorchar(mSChar);
    registry.flushInterval(atoi(mSliderMinutes.GetBuffer(0)));
    registry.formatString(mFormatString);
  }
  catch(...)
  {

  }
  return CPropertyPage::OnApply();
}

void 
TrLogging::OnCancel() 
{
  CPropertyPage::OnCancel();
}

void 
TrLogging::OnOK() 
{
  CPropertyPage::OnOK();
}

BOOL 
TrLogging::OnKillActive() 
{
  return CPropertyPage::OnKillActive();
}



void TrLogging::OnButtonDebug() 
{
  ApplicationLogging dlg;
  dlg.DoModal();
}
