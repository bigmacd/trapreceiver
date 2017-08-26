#if !defined(AFX_RAWTRAPDATA_H__B98A85CE_00EA_11D6_8780_00A0247B6F8F__INCLUDED_)
#define AFX_RAWTRAPDATA_H__B98A85CE_00EA_11D6_8780_00A0247B6F8F__INCLUDED_

#include "Packet.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Packet.h"

class RawTrapData : public CDialog
{

  private:
    Packet* mPacket;
    // Construction
  public:
    RawTrapData(CWnd* pParent = NULL);   // standard constructor
    RawTrapData(Packet* p);

      // Dialog Data
      //{{AFX_DATA(RawTrapData)
	enum { IDD = IDD_DIALOG4 };
	CListBox	mList;
	//}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(RawTrapData)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:

    // Generated message map functions
    //{{AFX_MSG(RawTrapData)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
      };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWTRAPDATA_H__B98A85CE_00EA_11D6_8780_00A0247B6F8F__INCLUDED_)
