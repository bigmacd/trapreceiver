
#include "stdafx.h"
#include "TrapRcvr.h"
#include "Packet.h"
#include "Mibdb.h"
//#include "NtPacket.h"


class DetailDlg : public CDialog
{

  private:

    Packet*        mPacket;
    void           UpdateFromTrap(Packet* packet);
    CListCtrl*     mTrapList;
    int            mCurrentIndex;
    unsigned char  _mTrapType;
    
    //    MibDb          mDb;

  public:
	
    DetailDlg(CWnd* pParent = NULL);	// standard constructor
    DetailDlg(Packet* pkt, CListCtrl* list, int index, unsigned char trapType);
	      
    ~DetailDlg();
    //{{AFX_DATA(DetailDlg)
    enum { IDD = IDD_TRAPRCVRDTL_DIALOG };
    CEdit	mGTypeErrorStatus;
    CEdit	mTimestampErrorIndex;
    CEdit	mSTypeErrorStatus;
    CEdit	mSendersOIDLabel;
    CButton	mPrev;
    CButton	mNext;
    CListCtrl	mList;
    CString	mCommunity;
    CString	mIpAddress;
    CString	mSenderOid;
    CString     mSpecificTypeString;
    CString     mTimeStampString;
    CString     mTrapTypeString;
    CString	mTrapType;
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(DetailDlg)
  protected:
      virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

      HICON m_hIcon;

      // Generated message map functions
      //{{AFX_MSG(DetailDlg)
      virtual BOOL OnInitDialog();
      virtual void OnOK();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonShowRaw();
	//}}AFX_MSG
      DECLARE_MESSAGE_MAP()
};

