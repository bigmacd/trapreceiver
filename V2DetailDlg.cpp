
#include "stdafx.h"
#include "traprcvr.h"
#include "V2DetailDlg.h"
#include "vbs.h"

V2DetailDlg::V2DetailDlg(CWnd* pParent /*=NULL*/)
	    :CDialog(V2DetailDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(V2DetailDlg)
  mErrorIndex = 0;
  mErrorStatus = 0;
  mIpAddress = _T("");
  mRequestId = 0;
  mCommunity = _T("");
  //}}AFX_DATA_INIT
}

V2DetailDlg::V2DetailDlg(Packet* pkt, CListCtrl* list, int index)
            :CDialog(V2DetailDlg::IDD, NULL),
	     mPacket(pkt),
	     mTrapList(list),
	     mCurrentIndex(index)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

V2DetailDlg::~V2DetailDlg()
{
}

void V2DetailDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(V2DetailDlg)
	DDX_Control(pDX, IDC_BUTTONPREV, mPrev);
	DDX_Control(pDX, IDC_BUTTONNEXT, mNext);
  DDX_Control(pDX, IDC_LIST1, mList);
  DDX_Text(pDX, IDC_EDITERRORINDEX, mErrorIndex);
  DDX_Text(pDX, IDC_EDITERRORSTATUS, mErrorStatus);
  DDX_Text(pDX, IDC_EDITIPADDRESS, mIpAddress);
  DDX_Text(pDX, IDC_EDITREQUESTID, mRequestId);
	DDX_Text(pDX, IDC_EDITCOMMUNITY, mCommunity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(V2DetailDlg, CDialog)
  //{{AFX_MSG_MAP(V2DetailDlg)
	ON_BN_CLICKED(IDC_BUTTONNEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTONPREV, OnButtonPrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL 
V2DetailDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  RECT rect;
  mList.GetClientRect(&rect);
  int width = (rect.right / 2) - 35;
  mList.InsertColumn(0, "OID", LVCFMT_CENTER, width);
  mList.InsertColumn(1, "Type", LVCFMT_CENTER, 70);
  mList.InsertColumn(2, "Value", LVCFMT_CENTER, width);
 
  UpdateFromTrap(mPacket);	
	
  int count = mTrapList->GetItemCount();
  if (mCurrentIndex == 0)
    mNext.EnableWindow(FALSE);
  if (mCurrentIndex == (count - 1))
    mPrev.EnableWindow(FALSE);

  return TRUE;
}

void 
V2DetailDlg::OnOK() 
{
  CDialog::OnOK();
}

void
V2DetailDlg::UpdateFromTrap(Packet* trappdu)
{
  mIpAddress = trappdu->SenderIP();
  mCommunity = trappdu->Community();
  mErrorIndex = trappdu->ErrorIndex();
  mErrorStatus = trappdu->ErrorStatus();
  mRequestId = trappdu->RequestId();

  mList.DeleteAllItems();
  LV_ITEM lv;
  lv.mask = LVIF_TEXT;
  int itemNumber = 0;

  VarbindList* vblist = trappdu->VbList();
  VbPair* vbp = vblist->FirstVbPair();
  while(vbp != NULL)
  {    
    OidVarbind* ovb = vbp->OIDVarbind();
    if (ovb)
    {
      // oid, type, value
      lv.iItem = itemNumber;
      lv.iSubItem = 0;
      
      char* data = (char*)ovb->Printable();
      lv.pszText = data;
      mList.InsertItem(&lv);
      
      Varbind* vb = vbp->VarBind();
      if (vb)
      {
	lv.iSubItem = 1;
	lv.pszText = (char*)vb->TypeString();
	mList.SetItem(&lv); 
	
	lv.iSubItem = 2;
	data = (char*)vb->Printable();
	lv.pszText = data;
	mList.SetItem(&lv);
	itemNumber++;
      }
    }
    vbp=vbp->Next;
  }
  UpdateData(FALSE);  
}

void 
V2DetailDlg::OnButtonNext() 
{
  BOOL disabled = FALSE;
  mPrev.EnableWindow(TRUE);
  if (--mCurrentIndex == 0)
  {
    // disable button
    mNext.EnableWindow(FALSE);
    disabled = TRUE;
  }

  mTrapList->SetItemState(mCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
  Packet* p = (Packet*)mTrapList->GetItemData(mCurrentIndex);
  if (p != NULL)
  {
    if (p->Type() == V2TRAP || p->Type() == INFORMPDU)
      UpdateFromTrap(p);
    else
      if (!disabled)
	OnButtonPrev();
  }
}

void 
V2DetailDlg::OnButtonPrev() 
{
  BOOL disabled = FALSE;
  mNext.EnableWindow(TRUE);
  int count = mTrapList->GetItemCount();
  if (++mCurrentIndex == (count - 1))
  {
    // disable button
    mPrev.EnableWindow(FALSE);
    disabled = TRUE;
  }

  mTrapList->SetItemState(mCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
  Packet* p = (Packet*)mTrapList->GetItemData(mCurrentIndex);
  if (p != NULL)
  {
    if (p->Type() == V2TRAP || p->Type() == INFORMPDU)
      UpdateFromTrap(p);
    else
      if (!disabled)
	OnButtonNext();
  }	
}

