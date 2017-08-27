
#if !defined(AFX_CONFIGURETRAPDATA_H__6CCFDDF5_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
#define AFX_CONFIGURETRAPDATA_H__6CCFDDF5_756F_11D4_873E_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class ConfigureTrapData : public CPropertyPage
{

  DECLARE_DYNCREATE(ConfigureTrapData)

  public:
    ConfigureTrapData(CWnd* pParent = NULL);   // standard constructor

    //{{AFX_DATA(ConfigureTrapData)
	enum { IDD = IDD_PROPPAGETRAPDATA };
    CButton	mVariables;
    CButton	mParms;
    BOOL	mCommandLine;
    BOOL	mEnvVars;
	//}}AFX_DATA
    

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(ConfigureTrapData)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
  protected:

    // Generated message map functions
    //{{AFX_MSG(ConfigureTrapData)
    afx_msg void OnCheckPassAsCommandLineParameters();
    afx_msg void OnCheckSetAsEnvironmentVariables();
    afx_msg void OnButtonSpecifyParameters();
    afx_msg void OnButtonSpecifyVariables();
    virtual BOOL OnInitDialog();
	afx_msg void OnButtonSpecifyData();
	afx_msg void OnCheckPassToEmail();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURETRAPDATA_H__6CCFDDF5_756F_11D4_873E_00A0247B6F8F__INCLUDED_)
