#if !defined(AFX_TRACTIONS_H__60C32D6F_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRACTIONS_H__60C32D6F_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrActions.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// TrActions dialog

class TrActions : public CPropertyPage
{
	DECLARE_DYNCREATE(TrActions)
private:

	int GetCurrentIndex();
        void DoModify(int currentIndex);

// Construction
public:
	TrActions();
	~TrActions();

// Dialog Data
	//{{AFX_DATA(TrActions)
	enum { IDD = IDD_PROPPAGEACTIONS };
	CListCtrl	mList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TrActions)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(TrActions)
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonadd();
	afx_msg void OnButtondelete();
	afx_msg void OnButtonmodify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACTIONS_H__60C32D6F_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
