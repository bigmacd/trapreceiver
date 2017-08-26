#if !defined(AFX_EMAILRECIPIENTS_H__F94B0815_B9AA_11D6_879A_00A0247B6F8F__INCLUDED_)
#define AFX_EMAILRECIPIENTS_H__F94B0815_B9AA_11D6_879A_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmailRecipients.h : header file
//

//////////////////////////
// 6.40
#include "ToolTipListCtrl.h"
//////////////////////////

/////////////////////////////////////////////////////////////////////////////
// EmailRecipients dialog

class EmailRecipients : public CDialog
{
  private:
    
    CString mInEmailList;
    CString mOutEmailList;

// Construction
public:
	EmailRecipients(CString list);

        CString EmailList();

// Dialog Data
	//{{AFX_DATA(EmailRecipients)
	enum { IDD = IDD_DIALOGEMAILSELECTION };

//////////////////////////
// 6.40
  //	CListCtrl	mList;
	CToolTipListCtrl	mList;
//////////////////////////

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EmailRecipients)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EmailRecipients)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILRECIPIENTS_H__F94B0815_B9AA_11D6_879A_00A0247B6F8F__INCLUDED_)
