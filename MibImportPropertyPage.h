#if !defined(AFX_MIBIMPORTPROPERTYPAGE_H__74AB3AEF_89A8_11D5_8769_00A0247B6F8F__INCLUDED_)
#define AFX_MIBIMPORTPROPERTYPAGE_H__74AB3AEF_89A8_11D5_8769_00A0247B6F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class MibImportPropertyPage : public CPropertyPage
{
    DECLARE_DYNCREATE(MibImportPropertyPage)

// Construction
public:
    MibImportPropertyPage();
    ~MibImportPropertyPage();

// Dialog Data
    //{{AFX_DATA(MibImportPropertyPage)
    enum { IDD = IDD_PROPPAGEMIBS };
    CButton	mUnloadButton;
    CListBox	mList;
    BOOL	mTranslateOids;
    //}}AFX_DATA
    
    
    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(MibImportPropertyPage)
  public:
    virtual BOOL OnApply();
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(MibImportPropertyPage)
    virtual BOOL OnInitDialog();
	afx_msg void OnButtonImportMib();
	afx_msg void OnButtonUnloadMib();
	afx_msg void OnSelchangeListLoadedMibs();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
      
};
  
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
  
#endif // !defined(AFX_MIBIMPORTPROPERTYPAGE_H__74AB3AEF_89A8_11D5_8769_00A0247B6F8F__INCLUDED_)
