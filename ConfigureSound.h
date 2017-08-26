#if !defined(AFX_CONFIGURESOUND_H__37B8311B_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_)
#define AFX_CONFIGURESOUND_H__37B8311B_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class ConfigureSound : public CDialog
{
// Construction
public:
	ConfigureSound(CWnd* pParent = NULL);   // standard constructor
	ConfigureSound(int duration, 
		       CString filename);

// Dialog Data
    //{{AFX_DATA(ConfigureSound)
    enum { IDD = IDD_DIALOGSOUNDPLAY };
    CSliderCtrl	mDurationSlider;
    CStatic	mDurationLabelWindow;
    int		mDuration;
    int		mMode;
    CString	mFilename;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(ConfigureSound)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
  protected:
    
    // Generated message map functions
    //{{AFX_MSG(ConfigureSound)
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioOnce();
    afx_msg void OnRadioLoop();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnButtonBrowse();
	virtual void OnOK();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
  
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURESOUND_H__37B8311B_C2D0_11D6_879D_00A0247B6F8F__INCLUDED_)
