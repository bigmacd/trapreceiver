
#if !defined(AFX_TRALERT_H__C54DADC1_3499_11D1_9448_00A0247B6F8F__INCLUDED_)
#define AFX_TRALERT_H__C54DADC1_3499_11D1_9448_00A0247B6F8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class TrAlert : public CDialog
{

// Construction
public:
	TrAlert(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TrAlert)
	enum { IDD = IDD_TRALERT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrAlert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrAlert)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRALERT_H__C54DADC1_3499_11D1_9448_00A0247B6F8F__INCLUDED_)
