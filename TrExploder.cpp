// TrExploder.cpp : implementation file
//

#include "stdafx.h"
#include "TrapRcvr.h"
#include "TrExploder.h"
#include "TrExploderAdd.h"
#include "Registry.h"

IMPLEMENT_DYNCREATE(TrExploder, CPropertyPage)

TrExploder::TrExploder() : CPropertyPage(TrExploder::IDD)
{
	//{{AFX_DATA_INIT(TrExploder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

TrExploder::~TrExploder()
{
}

void TrExploder::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrExploder)
	DDX_Control(pDX, IDC_LIST1, mList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrExploder, CPropertyPage)
	//{{AFX_MSG_MAP(TrExploder)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDELETE, OnButtondelete)
	ON_BN_CLICKED(IDC_BUTTONMODIFY, OnButtonmodify)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrExploder message handlers

void TrExploder::OnButtonadd() 
{
  int result = IDOK;
  TrExploderAddDlg dlg;
  result = dlg.DoModal();
  if (result == IDOK)
  {
    LV_ITEM lv;
    lv.mask = LVIF_TEXT;
    lv.iItem = mList.GetItemCount();
    
    lv.iSubItem = 0;
    lv.pszText = dlg.mHost.GetBuffer(0);
    mList.InsertItem(&lv);
    
    lv.mask = LVIF_TEXT;
    lv.iSubItem = 1;
    lv.pszText = dlg.mPort.GetBuffer(0);
    mList.SetItem(&lv);	
    
    lv.mask = LVIF_TEXT;
    lv.iSubItem = 2;
    CString c;
    c.Format("%d", time(0));
    lv.pszText = c.GetBuffer(0);
    mList.SetItem(&lv);	    

    SetModified(TRUE);
  }
  SetFocus();  
}

void TrExploder::OnButtondelete() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
  {
    mList.DeleteItem(currentIndex);

    SetModified(TRUE);
  }
  SetFocus();	
}

void TrExploder::OnButtonmodify() 
{
  int currentIndex = GetCurrentIndex();
  if (currentIndex >= 0)
  {
    // get the item data
    CString host = mList.GetItemText(currentIndex, 0);
    CString port = mList.GetItemText(currentIndex, 1);

    // pass it to the dlg
    TrExploderAddDlg dlg(host, port);
    int result = dlg.DoModal();
    if (result == IDOK)
    {
      LV_ITEM lv;
      lv.mask = LVIF_TEXT;
      lv.iItem = currentIndex;

      lv.iSubItem = 0;
      lv.pszText = dlg.mHost.GetBuffer(0);
      mList.SetItem(&lv);
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      lv.pszText = dlg.mPort.GetBuffer(0);
      mList.SetItem(&lv);	

      SetModified(TRUE);
    }
  }
  SetFocus();	
}

void TrExploder::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UINT nFlags;
  CPoint point;
  BOOL b = ::GetCursorPos( &point );
  mList.ScreenToClient( &point );
  int currentIndex = mList.HitTest(point, &nFlags);

  if (currentIndex >= 0)
  {
    CString host = mList.GetItemText(currentIndex, 0);
    CString port = mList.GetItemText(currentIndex, 1);

    // kick off the configure action dialog box
    TrExploderAddDlg dlg(host, port);
    int result = dlg.DoModal();
    if (result == IDOK)
    {
      LV_ITEM lv;
      lv.mask = LVIF_TEXT;
      lv.iItem = currentIndex;
      
      lv.iSubItem = 0;
      lv.pszText = dlg.mHost.GetBuffer(0);
      mList.SetItem(&lv);
      
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      lv.pszText = dlg.mPort.GetBuffer(0);
      mList.SetItem(&lv);	

      SetModified(TRUE);
    }
  } // if (currentIndex >= 0)
	
  *pResult = 0;
  SetFocus();  
}

BOOL TrExploder::OnSetActive() 
{
  return CPropertyPage::OnSetActive();
}

int 
TrExploder::GetCurrentIndex()
{
  int count = mList.GetItemCount();
  int x;
  for (x = 0; x < count; x++)
  {
    if (mList.GetItemState(x, LVIS_SELECTED) == LVIS_SELECTED)
      break;
  }
  if (x == count)
    return -1;
  else
    return x;
}

BOOL TrExploder::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();

  RECT rect;
  mList.GetClientRect(&rect);
  int width = (rect.right / 2);
  mList.InsertColumn(0, "Address", LVCFMT_CENTER, width);
  mList.InsertColumn(1, "Port", LVCFMT_CENTER, width);
  mList.InsertColumn(2, "id", LVCFMT_CENTER, 0);

  Registry registry("Exploder");
  unsigned long count = registry.getCount(); 

  LV_ITEM lv;
  lv.mask = LVIF_TEXT;
  try 
  {
    for (unsigned long x = 0; x < count; x++)
    {
      registry.ReOpen(x);
      lv.iItem = x;
      CString dest = registry.exploderDestination();
      lv.iSubItem = 0;
      lv.pszText = dest.GetBuffer(0);
      mList.InsertItem(&lv);
    
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 1;
      CString port;
      port.Format("%d", registry.exploderPort());
      lv.pszText = port.GetBuffer(0);
      mList.SetItem(&lv); 

      int id = registry.exploderId();
      CString data;
      data.Format("%d", id);
      lv.mask = LVIF_TEXT;
      lv.iSubItem = 2;
      lv.pszText = data.GetBuffer(0);
      mList.SetItem(&lv);    
    }
  }
  catch(...)
  {

  }
  return TRUE;
}

BOOL TrExploder::OnApply() 
{
  try
  {
    Registry registry("Exploder");
    unsigned long count = registry.getCount();

    //  for (unsigned long x = 0; x < count; x++)
    //    registry.deleteExploder(x);
    registry.deleteExploders();

    int listCount = mList.GetItemCount();
    for (int regX = 0; regX < listCount; regX++)
    {
      CString host = mList.GetItemText(regX, 0);
      CString port = mList.GetItemText(regX, 1);
      int     id   = atoi(mList.GetItemText(regX, 2));      

      registry.insertExploder(host,
			      atoi(port.GetBuffer(0)),
			      id);
    }
  }
  catch(...)
  {

  }
  return CPropertyPage::OnApply();
}
