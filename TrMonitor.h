
#if !defined(AFX_TRMONITOR_H__5C7A6A11_36DB_11D1_944A_00A0247B6F8F__INCLUDED_)
#define AFX_TRMONITOR_H__5C7A6A11_36DB_11D1_944A_00A0247B6F8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include <winsvc.h>

#define TRMONITORTIMERID  12344

class TrMonitor : public CDialog
{
private:

  unsigned long  mWaitHint;
  HCURSOR        mCursor;

  SC_HANDLE       mScManager;
  SC_HANDLE         mService;
  SERVICE_STATUS mScs;


// Construction
public:
	TrMonitor(unsigned long waitHint);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TrMonitor)
	enum { IDD = IDD_DIALOGMONITOR };
  //	CButton	mButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrMonitor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrMonitor)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_TRMONITOR_H__5C7A6A11_36DB_11D1_944A_00A0247B6F8F__INCLUDED_)
