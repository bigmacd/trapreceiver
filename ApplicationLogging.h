#if !defined(AFX_APPLICATIONLOGGING_H__37B8311A_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_)
#define AFX_APPLICATIONLOGGING_H__37B8311A_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApplicationLogging.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ApplicationLogging dialog

class ApplicationLogging : public CDialog
{
// Construction
public:
	ApplicationLogging(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ApplicationLogging)
	enum { IDD = IDD_DIALOGSELECTDEBUG };
	BOOL	mSmtpLogging;
	BOOL	mActionsLogging;
	BOOL	mLoggingLogging;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ApplicationLogging)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ApplicationLogging)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLICATIONLOGGING_H__37B8311A_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_)
