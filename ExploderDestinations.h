#if !defined(AFX_EXPLODERDESTINATIONS_H__24DB3435_936A_47A5_9F30_3C05E244D1B8__INCLUDED_)
#define AFX_EXPLODERDESTINATIONS_H__24DB3435_936A_47A5_9F30_3C05E244D1B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExploderDestinations.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ExploderDestinations dialog

class ExploderDestinations : public CDialog
{

  private:
  
    CString mInExploderList;
    CString mOutExploderList;

  public:

    ExploderDestinations(CString list); 

    CString ExploderList();

// Dialog Data
	//{{AFX_DATA(ExploderDestinations)
	enum { IDD = IDD_DIALOGEXPLODERSELECTION };
	CListCtrl	mList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExploderDestinations)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ExploderDestinations)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLODERDESTINATIONS_H__24DB3435_936A_47A5_9F30_3C05E244D1B8__INCLUDED_)
