#if !defined(AFX_TREXPLODER_H__60C32D72_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TREXPLODER_H__60C32D72_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrExploder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TrExploder dialog

class TrExploder : public CPropertyPage
{
	DECLARE_DYNCREATE(TrExploder)
private:

	int GetCurrentIndex();

// Construction
public:
	TrExploder();
	~TrExploder();

// Dialog Data
	//{{AFX_DATA(TrExploder)
	enum { IDD = IDD_PROPPAGEEXPLODERS };
	CListCtrl	mList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TrExploder)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(TrExploder)
	afx_msg void OnButtonadd();
	afx_msg void OnButtondelete();
	afx_msg void OnButtonmodify();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREXPLODER_H__60C32D72_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
