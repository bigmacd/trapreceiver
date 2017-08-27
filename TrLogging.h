
#if !defined(AFX_TRLOGGING_H__6F404552_A6E1_11D1_81D2_006097C511C2__INCLUDED_)
#define AFX_TRLOGGING_H__6F404552_A6E1_11D1_81D2_006097C511C2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class TrLogging : public CPropertyPage
{
  DECLARE_DYNCREATE(TrLogging)

  private:




  public:
	
    TrLogging();
    ~TrLogging();


    //{{AFX_DATA(TrLogging)
    enum { IDD = IDD_PROPPAGELOGGING };
    CSliderCtrl	mSlider;
    CString	mLogFileName;
    CString	mSChar;
    CString	mSliderMinutes;
    CString	mSliderUnits;
    int		mRadioAppendOnOff;
    int		mRadioLoggingOnOff;
    CString	mFormatString;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(TrLogging)
  public:
    virtual BOOL OnApply();
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnKillActive();
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL


  protected:
    // Generated message map functions
    //{{AFX_MSG(TrLogging)
    afx_msg void OnButtonBrowse();
    virtual BOOL OnInitDialog();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnRadioappend();
    afx_msg void OnRadiologgingoff();
    afx_msg void OnRadiologgingon();
    afx_msg void OnRadiooverwrite();
	afx_msg void OnButtonDebug();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRLOGGING_H__6F404552_A6E1_11D1_81D2_006097C511C2__INCLUDED_)


