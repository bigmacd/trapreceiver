#if !defined(AFX_TRACTIONADD_H__60C32D70_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRACTIONADD_H__60C32D70_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrActionAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TrActionAdd dialog
#define COMMUNITY 0
#define GENERIC 1 
#define SENDERIP 2 
#define SENDEROID 3 
#define SPECIFIC 4 
#define VARBINDOID 5 
#define VARBINDVALUE 6 

#include "stdafx.h"
#include "resource.h"


class TrActionAdd : public CDialog
{
// Construction
public:
  TrActionAdd(CWnd* pParent = NULL);
  TrActionAdd(CString name,
	      int     watch,
	      CString value,
	      int     bools,
	      CString executable,
	      CString args,
	      BOOL    includeTrapData,
	      CString emailDest,
	      CString exploderDest,
	      int     soundPlayDuration,
	      CString soundFile,
	      BOOL    partOfGroup,
	      int     requireDesktop);


// Dialog Data
  //{{AFX_DATA(TrActionAdd)
  enum { IDD = IDD_DIALOGACTIONADD };
  CButton	mButtonExplodeConfigure;
  CButton	mButtonSoundConfigure;
  CButton	mButtonEmailConfigure;
  CButton	mAddUpdateButton;
  CButton	mButtonExecute;
  BOOL	        mEmail;
  BOOL	        mExecute;
  BOOL	        mExplode;
  BOOL	        mPage;
  BOOL          mPartOfGroup;
  int           mBools;
  CString	mValue;
  CString       mExecuteProgram;
  CString       mExecuteArgs;
  BOOL          mInChangeMode;
  BOOL          mIncludeTrapData;
  int           mRequireDesktop;
  int	        mWatch;
  BOOL	        mDiscard;
  BOOL	        mSound;
  CEdit         mNameEdit;
  //}}AFX_DATA
  CString       mName;
  CString       mWatchText;
  CString       mEmailDest;
  CString       mExploderDest;
  int           mSoundPlayDuration;
  CString       mSoundFile;


  BOOL          mCheckBoxes[5];
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrActionAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrActionAdd)
	afx_msg void OnCheckemail();
	afx_msg void OnCheckexecute();
	afx_msg void OnCheckexplode();
	afx_msg void OnCheckPartOfGroup();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonemailconfigure();
	afx_msg void OnButtonexecuteconfigure();
	afx_msg void OnEditupdateCombowatch();
	afx_msg void OnSelchangeCombowatch();
	afx_msg void OnCheckDiscard();
	afx_msg void OnButtonEmailConfigure();
	afx_msg void OnButtonSoundConfigure();
	afx_msg void OnCheckPlaySound();
	afx_msg void OnButtonexplodeconfigure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACTIONADD_H__60C32D70_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
