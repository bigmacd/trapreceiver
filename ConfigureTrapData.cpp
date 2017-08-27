
#include "stdafx.h"
#include "traprcvr.h"
#include "ConfigureTrapData.h"
#include "EnvVarsDlg.h"
#include "CmdLineDlg.h"
#include "Registry.h"

IMPLEMENT_DYNCREATE(ConfigureTrapData, CPropertyPage)

ConfigureTrapData::ConfigureTrapData(CWnd* pParent /*=NULL*/)
	          :CPropertyPage(ConfigureTrapData::IDD)
{
  //{{AFX_DATA_INIT(ConfigureTrapData)
  mCommandLine = FALSE;
  mEnvVars = FALSE;
	//}}AFX_DATA_INIT
}


void 
ConfigureTrapData::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ConfigureTrapData)
	DDX_Control(pDX, IDC_BUTTONSPECIFYVARIABLES, mVariables);
	DDX_Control(pDX, IDC_BUTTONSPECIFYPARAMETERS, mParms);
  DDX_Check(pDX, IDC_CHECKPASSASCOMMANDLINEPARAMETERS, mCommandLine);
  DDX_Check(pDX, IDC_CHECKSETASENVIRONMENTVARIABLES, mEnvVars);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfigureTrapData, CPropertyPage)
  //{{AFX_MSG_MAP(ConfigureTrapData)
  ON_BN_CLICKED(IDC_CHECKPASSASCOMMANDLINEPARAMETERS, OnCheckPassAsCommandLineParameters)
  ON_BN_CLICKED(IDC_CHECKSETASENVIRONMENTVARIABLES, OnCheckSetAsEnvironmentVariables)
  ON_BN_CLICKED(IDC_BUTTONSPECIFYPARAMETERS, OnButtonSpecifyParameters)
  ON_BN_CLICKED(IDC_BUTTONSPECIFYVARIABLES, OnButtonSpecifyVariables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void 
ConfigureTrapData::OnCheckPassAsCommandLineParameters() 
{
  UpdateData(TRUE);
  mParms.EnableWindow(mCommandLine);
  Registry r;
  r.includeCmdLine(mCommandLine);
  SetModified(TRUE);
}

void 
ConfigureTrapData::OnCheckSetAsEnvironmentVariables() 
{
  UpdateData(TRUE);
  mVariables.EnableWindow(mEnvVars);
  Registry r;
  r.includeEnvironment(mEnvVars);
  SetModified(TRUE);
}

void 
ConfigureTrapData::OnButtonSpecifyParameters() 
{
  CmdLineDlg dlg;
  int result = dlg.DoModal();
  if (result == IDOK)
  {
    SetModified(TRUE);
    Registry r;
    r.includeCmdCommunity(dlg.mCommunity);
    r.includeCmdIpAddress(dlg.mIpAddress);
    r.includeCmdRealIpAddress(dlg.mRealIpAddress);
    r.includeCmdGenericType(dlg.mGenericType);
    r.includeCmdSpecificType(dlg.mSpecificType);
    r.includeCmdSenderOID(dlg.mSenderOID);
    r.includeCmdVbData(dlg.mVbData);
    r.includeCmdVbOID(dlg.mVbOID);
    if (dlg.mCommunity)
      r.cmdCommunity(dlg.mCommunityString);
    if (dlg.mGenericType)
      r.cmdGenericType(dlg.mGenericTypeString);
    if (dlg.mIpAddress)
      r.cmdIpAddress(dlg.mIpAddressString);
    if (dlg.mRealIpAddress)
      r.cmdRealIpAddress(dlg.mRealIpAddressString);
    if (dlg.mSenderOID)
      r.cmdSenderOID(dlg.mSenderOIDString);
    if (dlg.mSpecificType)
      r.cmdSpecificType(dlg.mSpecificTypeString);
    if (dlg.mVbData)
      r.cmdVbData(dlg.mVbDataString);
    if (dlg.mVbOID)
      r.cmdVbOID(dlg.mVbOIDString);
  }  
}

void 
ConfigureTrapData::OnButtonSpecifyVariables() 
{
  EnvVarsDlg dlg;
  int result = dlg.DoModal();
  if (result == IDOK)
  {
    SetModified(TRUE);
    Registry r;
    r.includeEnvCommunity(dlg.mCommunity);
    r.includeEnvIpAddress(dlg.mIpAddress);
    r.includeEnvRealIpAddress(dlg.mRealIpAddress);
    r.includeEnvGenericType(dlg.mGenericType);
    r.includeEnvSpecificType(dlg.mSpecificType);
    r.includeEnvSenderOID(dlg.mSenderOID);
    r.includeEnvVbData(dlg.mVbData);
    r.includeEnvVbOID(dlg.mVbOID);
    if (dlg.mCommunity)
      r.envCommunity(dlg.mCommunityString);
    if (dlg.mGenericType)
      r.envGenericType(dlg.mGenericTypeString);
    if (dlg.mIpAddress)
      r.envIpAddress(dlg.mIpAddressString);
    if (dlg.mRealIpAddress)
      r.envRealIpAddress(dlg.mRealIpAddressString);
    if (dlg.mSenderOID)
      r.envSenderOID(dlg.mSenderOIDString);
    if (dlg.mSpecificType)
      r.envSpecificType(dlg.mSpecificTypeString);
    if (dlg.mVbData)
      r.envVbData(dlg.mVbDataString);
    if (dlg.mVbOID)
      r.envVbOID(dlg.mVbOIDString);
  }  	
}

BOOL 
ConfigureTrapData::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
  Registry r;
  try
  {
    mCommandLine = r.includeCmdLine();
    mParms.EnableWindow(mCommandLine);
    mEnvVars = r.includeEnvironment();
    mVariables.EnableWindow(mEnvVars);
  }
  catch(int x)
  {
    if (x == 0)
    {
      r.includeCmdLine(FALSE);
      r.includeEnvironment(FALSE);
    }
  }	
  UpdateData(FALSE);
  return TRUE;
}

