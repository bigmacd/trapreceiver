#pragma warning(disable: 4786)

//  Copyright (c) 1997-2002 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "DetailDlg.h"
#include "vbs.h"
#include <mgmtapi.h>
#include "Resource.h"
#include "Registry.h"
#include "RawTrapData.h"
#include "Mibdb.h"

extern MibDb gDb;

DetailDlg::DetailDlg(CWnd* pParent /*=NULL*/)
   	    :CDialog(DetailDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(DetailDlg)
  mCommunity = _T("");
  mIpAddress = _T("");
  mSenderOid = _T("");
  mSpecificTypeString = _T("");
  mTimeStampString = _T("");
  mTrapTypeString = _T("");
  mTrapType = _T("");
  //}}AFX_DATA_INIT
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}
  
DetailDlg::DetailDlg(Packet* pkt, CListCtrl* list, int index, unsigned char trapType)
          :CDialog(DetailDlg::IDD, NULL),
	   mPacket(pkt),
	   mTrapList(list),
	   mCurrentIndex(index),
	   _mTrapType(trapType)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  if (trapType == V1TRAP)
    mTrapType = "SNMPv1";
  else
    if (trapType == V2TRAP)
      mTrapType = "SNMPv2c";
    else
      if (trapType == INFORMPDU)
	mTrapType = "INFORM";
}

DetailDlg::~DetailDlg()
{
}


void 
DetailDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(DetailDlg)
  DDX_Control(pDX, IDC_EDITTRAPTYPEREQUESTID, mGTypeErrorStatus);
  DDX_Control(pDX, IDC_EDITTIMESTAMPERRORINDEX, mTimestampErrorIndex);
  DDX_Control(pDX, IDC_EDITSPECIFICTYPEERRORSTATUS, mSTypeErrorStatus);
  DDX_Control(pDX, IDC_EDITSENDERSOID, mSendersOIDLabel);
  DDX_Control(pDX, IDC_BUTTONPREV, mPrev);
  DDX_Control(pDX, IDC_BUTTONNEXT, mNext);
  DDX_Control(pDX, IDC_LIST, mList);
  DDX_Text(pDX, IDC_EDITCOMMUNITY, mCommunity);
  DDX_Text(pDX, IDC_EDITIPADDRESS, mIpAddress);
  DDX_Text(pDX, IDC_EDITSENDEROID, mSenderOid);
  DDX_Text(pDX, IDC_EDITSPECIFICTYPE, mSpecificTypeString);
  DDX_Text(pDX, IDC_EDITTIMESTAMP, mTimeStampString);
  DDX_Text(pDX, IDC_EDITTRAPTYPE, mTrapTypeString);
  DDX_Text(pDX, IDC_EDITTYPE, mTrapType);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DetailDlg, CDialog)
  //{{AFX_MSG_MAP(DetailDlg)
  ON_BN_CLICKED(IDC_BUTTONNEXT, OnButtonNext)
  ON_BN_CLICKED(IDC_BUTTONPREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTONSHOWRAW, OnButtonShowRaw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
DetailDlg::OnInitDialog()
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

  if (_mTrapType == V1TRAP)
  {
    mGTypeErrorStatus.SetWindowText("Trap Type");
    mTimestampErrorIndex.SetWindowText("TimeStamp");
    mSTypeErrorStatus.SetWindowText("Specific Type");
    mSendersOIDLabel.SetWindowText("Sender OID");
  }
  else
  {
    mGTypeErrorStatus.SetWindowText("Request ID");
    mTimestampErrorIndex.SetWindowText("Error Status");
    mSTypeErrorStatus.SetWindowText("Error Index");

    mSendersOIDLabel.ShowWindow(SW_HIDE);
    CWnd* c = GetDlgItem(IDC_EDITSENDEROID);
    if (c != NULL)
      c->ShowWindow(SW_HIDE);
  }

  return TRUE; 
}

void 
DetailDlg::OnOK() 
{

  CDialog::OnOK();
}

void
DetailDlg::UpdateFromTrap(Packet* trappdu)
{

  mCommunity.Empty();
  mCommunity = trappdu->Community();

  _mTrapType = trappdu->Type();
  if (_mTrapType == V1TRAP)
  {
    mGTypeErrorStatus.SetWindowText("Trap Type");
    mTimestampErrorIndex.SetWindowText("TimeStamp");
    mSTypeErrorStatus.SetWindowText("Specific Type");
    mSendersOIDLabel.SetWindowText("Sender OID");
    mSendersOIDLabel.ShowWindow(SW_SHOW);
    CWnd* c = GetDlgItem(IDC_EDITSENDEROID);
    if (c != NULL)
      c->ShowWindow(SW_SHOW);

    mTrapType = "SNMPv1";
  }
  else
  {
    mGTypeErrorStatus.SetWindowText("Request ID");
    mTimestampErrorIndex.SetWindowText("Error Status");
    mSTypeErrorStatus.SetWindowText("Error Index");

    mSendersOIDLabel.ShowWindow(SW_HIDE);
    CWnd* c = GetDlgItem(IDC_EDITSENDEROID);
    if (c != NULL)
      c->ShowWindow(SW_HIDE);
  }
  if (_mTrapType == V2TRAP)
    mTrapType = "SNMPv2c";
  else
    if (_mTrapType == INFORMPDU)
      mTrapType = "INFORM";
  
  unsigned char* data = NULL;
  Registry r;
  if (r.translateOids())
  {
    const char* sendOid = trappdu->SenderOID();
    if (sendOid != NULL) {
      string asdf;
      //bool status = gDb.FindOid(trappdu->SenderOID());
	  bool status = gDb.FindOid(sendOid);
      if (true == status)
	asdf = gDb.OidsName();
	data = (unsigned char*)asdf.c_str();
    }
    else data = (unsigned char*)"null";
  }
  //    data = (unsigned char*)gTrapMap->LookUpTrapName(trappdu->SenderOID(),
  //					       trappdu->SpecificTrapType());

  if (data == NULL || strlen((char*)data) == 0)
    data = (unsigned char*)(trappdu->pdu()->SenderOID());

  mSenderOid = data;

  data = (unsigned char*)(trappdu->pdu()->SenderIP());
  mIpAddress = data;
  if (_mTrapType == V1TRAP)
  {
    mTrapTypeString.Format("%d", trappdu->pdu()->GenericTrapType());
    mSpecificTypeString.Format("%d", trappdu->pdu()->SpecificTrapType());
    
    unsigned int t = trappdu->pdu()->TimeTicks();
    char timeVal[64];
    memset(timeVal, 0, 64);
    int days = t/(100*60*60*24);
    t -= days*(100*60*60*24);
    int hrs = t/(100*60*60);
    t -= hrs*(100*60*60);
    int mins = t/(100*60);
    t -= mins*(100*60);
    int secs = t/100;
    t -= secs*100;
    mTimeStampString.Format("%d days %02dh:%02dm:%02d.%02ds", days, hrs, mins, secs, t);
  }
  else
  {
    mTrapTypeString.Format("%d", trappdu->pdu()->RequestId());
    mSpecificTypeString.Format("%d", trappdu->pdu()->ErrorStatus());
    mTimeStampString.Format("%d", trappdu->pdu()->ErrorIndex());
  }

  mList.DeleteAllItems();
  LV_ITEM lv;
  lv.mask = LVIF_TEXT;
  int itemNumber = 0;

  VarbindList* vblist = trappdu->VbList();
  VbPair* vbp = vblist->FirstVbPair();
  while(vbp != NULL)
  {    
    // first process the OID part of the varbind
    OidVarbind* ovb = vbp->OIDVarbind();
    if (ovb)
    {
      // oid, type, value
      lv.iItem = itemNumber;
      lv.iSubItem = 0;
      const char* oiddata = NULL;
      const char* oid  = ovb->Printable();
      string asdf;
      try
      {
	if (r.translateOids())
	{
	  bool status = gDb.FindNearestOid(oid);
	  if (true == status)  {
	    asdf = gDb.OidsName();
	    oiddata = asdf.c_str();
	  }
	}
      }
      catch(...)
      {

      }

      if (oiddata == NULL || strlen(oiddata) == 0)
      {
	lv.pszText = (char*)ovb->Printable();
	mList.InsertItem(&lv);
      }
      else
      {
	  lv.pszText = (char*)oiddata;
	  mList.InsertItem(&lv);
      }

      // now process the rest of the varbind
      Varbind* vb = vbp->VarBind();
      if (vb)
      {
	// first the easy part, the type
	lv.iSubItem = 1;
	lv.pszText = (char*)vb->TypeString();
	mList.SetItem(&lv); 

	// now for the more difficult part, the data
	lv.iSubItem = 2;
	char timeVal[64];

	// let's deal with timetick data right away
	if (!strcmp(vb->TypeString(), "TimeTick"))
	{
	  unsigned int t = (unsigned int)atoi(vb->Printable());
	  memset(timeVal, 0, 64);
	  int days = t/(100*60*60*24);
	  t -= days*(100*60*60*24);
	  int hrs = t/(100*60*60);
	  t -= hrs*(100*60*60);
	  int mins = t/(100*60);
	  t -= mins*(100*60);
	  int secs = t/100;
	  t -= secs*100;
	  sprintf_s(timeVal, 63, "%d days %02dh:%02dm:%02d.%02ds", days, hrs, mins, secs, t);
	  lv.pszText = timeVal;
	  mList.SetItem(&lv);
	}
	else
	{
	  // ok, not timetick data, maybe it is the sender's oid in 
	  // an SNMPv2c trap or inform
	  if (trappdu->Type() != V1TRAP &&
	      !strcmp(ovb->Printable(), "1.3.6.1.6.3.1.1.4.1.0"))
	  {
	    const char* vbdata = vb->Printable();
	    const char* msg = NULL;
	    char* oid = NULL;
	    try
	    {
	      int len = strlen(vbdata);
	      if (len)
	      {
		oid = new char[len + 1];
		strcpy_s(oid, len+1, vbdata);

		char* c = strrchr(oid, '.');
		if (c != NULL)
		{
		  *c = 0;
		  c = strrchr(oid, '.');
		  if (c != NULL)
		  {
		    *c = 0;
		    if (r.translateOids())
		    {
		      //		      msg = gTrapMap->LookUpTrapName(oid,
		      //				       trappdu->SpecificTrapType());
		      string asdf;
		      bool status = gDb.FindOid(oid);
		      if (true == status)
		      {
			asdf = gDb.OidsName();
			//			msg = gDb.OidsName().c_str();
			msg = asdf.c_str();
		      }
		    }
		    if (msg == NULL || strlen(msg) == 0)
		      lv.pszText = (char*)vbdata;
		    else
		      lv.pszText = (char*)msg;
		    mList.SetItem(&lv);
		    if (oid != NULL)
		      delete [] oid;
		  } // if (c != NULL)
		} // if (c != NULL)
	      } // if (len)
	    }
	    catch(...)
	    {
	      
	    }
	  }
	  else
	  {
	    // maybe it is an object ID.  Let see if we can find it in our DB.
	    if (!strcmp(vb->TypeString(), "OID"))
	    {
	      const char* oiddata = NULL;
	      const char* oid = vb->Printable();
	      string asdf;
	      if (r.translateOids())
	      {
		bool status = gDb.FindOid(oid);
		if (true == status)
		{
		  asdf = gDb.OidsName();
		  //		  oiddata = gDb.OidsName().c_str();
		  oiddata = asdf.c_str();
		}
	      }
	      if (oiddata == NULL || strlen(oiddata) == 0)
		lv.pszText = (char*)oid;
	      else
		lv.pszText = (char*)oiddata;
	      mList.SetItem(&lv);	      
	    }
	    else
	    {
	      ///////////////////////////////////////////////////////////////////////////
	      // finally, not timetick, not sender's oid, and not an OID. Just try 
	      // to look its enumerated value if we know the name (oiddata)
	      const char* ename = NULL;
	      string asdf;
	      data = (unsigned char*)vb->Printable();
	      int value = atoi((const char*)data);
	      bool status = gDb.FindEnum(oid, value);
	      if (true == status)
	      {
		asdf = gDb.EnumName();
		//		ename = gDb.EnumName().c_str();
		ename = asdf.c_str();
	      }
	      if (ename != NULL && strlen(ename) > 0)
		lv.pszText = (char*)ename;
	      else
		lv.pszText = (char*)data;
	      mList.SetItem(&lv);
#if 0
	      data = (unsigned char*)vb->Printable();
	      if (oiddata != NULL && strlen(oiddata) > 0)
	      {
		// we know the name, maybe it is enumerated
		const char* ename = NULL;
		try
		{
		  // try lookup based on name of varbind
		  unsigned long ldata = atol((const char*)data);
		  if (r.translateOids())
		  {
		    
		    ename = gEnumMap->LookUpEnumName(oiddata, (int)ldata);
		  }
		}
		catch(...)
		{
	    
		}
		if (ename != NULL && strlen(ename) > 0)
		  lv.pszText = (char*)ename;
		else
		  lv.pszText = (char*)data;
		mList.SetItem(&lv);
	      }
	      else
	      {
		// don't know the name, just use the value
		lv.pszText = (char*)data;
		mList.SetItem(&lv);
	      }
#endif
	      ///////////////////////////////////////////////////////////////////////////
	    }
	  }
	}
	itemNumber++;
      }
    }
    vbp=vbp->Next;
  }
  UpdateData(FALSE);  
}

void 
DetailDlg::OnButtonNext() 
{
  mPrev.EnableWindow(TRUE);
  if (--mCurrentIndex == 0)
    // disable button
    mNext.EnableWindow(FALSE);

  mTrapList->SetItemState(mCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
  Packet* p = (Packet*)mTrapList->GetItemData(mCurrentIndex);
  if (p != NULL)
    UpdateFromTrap(p);
}

void 
DetailDlg::OnButtonPrev() 
{
  mNext.EnableWindow(TRUE);
  int count = mTrapList->GetItemCount();
  if (++mCurrentIndex == (count - 1))
    // disable button
    mPrev.EnableWindow(FALSE);

  mTrapList->SetItemState(mCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
  Packet* p = (Packet*)mTrapList->GetItemData(mCurrentIndex);
  if (p != NULL)
    UpdateFromTrap(p);
}

#if 0
void
DetailDlg::UpdateFromTrap(NtPacket* trappdu)
{
  char data[1024];
  unsigned int length;

  mCommunity = "n/a";

  memset(data, 0, 1024);
  length = trappdu->Enterprise()->idLength;
  for (unsigned int x = 0; x < length; x++)
  {
    char number[32];
    sprintf(number, "%d", trappdu->Enterprise()->ids[x]);
    strcat(data, number);
    if (length > 1)
      strcat(data, ".");
  }
  mSenderOid = data;

  memset(data, 0, 1024);
  length = trappdu->IPAddress()->length;
  for (x = 0; x < length; x++)
  {
    char number[32];
    sprintf(number, "%d", trappdu->IPAddress()->stream[x]);
    strcat(data, number);
    if (length > 1)
      strcat(data, ".");
  }
  mIpAddress = data;
  
  mTrapTypeString.Empty();
  mTrapTypeString.Format("%d", *(trappdu->Generictrap()));

  mSpecificTypeString.Empty();
  mSpecificTypeString.Format("%d", *(trappdu->Specifictrap()));

  mTimeStampString.Empty();
  mTimeStampString.Format("%d", *(trappdu->Timestamp()));

  mList.DeleteAllItems();
  LV_ITEM lv;
  lv.mask = LVIF_TEXT;
  int itemNumber = 0;

  int vblen = trappdu->Variablebindings()->len;
  for(int i = 0; i < vblen; i++)
  {    
    RFC1157VarBind varb;
    varb = trappdu->Variablebindings()->list[i];

    // oid, type, value
    lv.iItem = itemNumber;
    lv.iSubItem = 0;
    
    char* data;
    SnmpMgrOidToStr(&varb.name, &data);
    lv.pszText = data;
    mList.InsertItem(&lv);
    if (data)
      SNMP_free(data);

    switch (varb.value.asnType)
    {
      case ASN_INTEGER:                 // unsigned longs
      {
	lv.iSubItem = 1;
	lv.pszText = "Integer";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char [128];
	sprintf(data, "%ld", varb.value.asnValue);
	lv.pszText = data;
	mList.SetItem(&lv);
	delete [] data;
	break;
      }

      case ASN_OCTETSTRING:             // asnoctectstrings
      {
	lv.iSubItem = 1;
	lv.pszText = "String";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char[256];
	memset(data, 0, 256);
	length = varb.value.asnValue.string.length;
	for (unsigned int i = 0; i < length; i++)
	{
	  char number[32];
	  memset(number, 0, 32);
	  sprintf(number, "%d", varb.value.asnValue.string.stream[i]);
	  strcat(data, number);
	  if (length > 1)
	    strcat(data, ".");
	}
	lv.pszText = data;
	mList.SetItem(&lv);
	break;
      }

      case ASN_OBJECTIDENTIFIER:        // use ids member
      {
	lv.iSubItem = 1;
	lv.pszText = "OID";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	SnmpMgrOidToStr(&varb.value.asnValue.object, &data);
	lv.pszText = data;
	mList.SetItem(&lv);
	if (data)
	  SNMP_free(data);
	break;
      }

      case ASN_RFC1155_IPADDRESS:       // use stream member
      {
	lv.iSubItem = 1;
	lv.pszText = "IP Address";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char[256];
	memset(data, 0, 256);
	length = varb.value.asnValue.address.length;
	for (unsigned int i = 0; i < length; i++)
	{
	  char number[32];
	  memset(number, 0, 32);
	  sprintf(number, "%d", varb.value.asnValue.address.stream[i]);
	  strcat(data, number);
	  if (length > 1)
	    strcat(data, ".");
	}
	lv.pszText = data;
	mList.SetItem(&lv);
	break;
      }

      case ASN_RFC1155_COUNTER:
      {
	lv.iSubItem = 1;
	lv.pszText = "Counter";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char [128];
	sprintf(data, "%ld", varb.value.asnValue);
	lv.pszText = data;
	mList.SetItem(&lv);
	delete [] data;
	break;
      }

      case ASN_RFC1155_GAUGE:
      {
	lv.iSubItem = 1;
	lv.pszText = "Gauge";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char [128];
	sprintf(data, "%ld", varb.value.asnValue);
	lv.pszText = data;
	mList.SetItem(&lv);
	delete [] data;
	break;
      }

      case ASN_RFC1155_TIMETICKS:
      {
	lv.iSubItem = 1;
	lv.pszText = "Timeticks";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char [128];
	sprintf(data, "%ld", varb.value.asnValue);
	lv.pszText = data;
	mList.SetItem(&lv);
	delete [] data;
	break;
      }

      case ASN_RFC1155_OPAQUE:
      {
	lv.iSubItem = 1;
	lv.pszText = "Opaque";
	mList.SetItem(&lv); 
	lv.iSubItem = 2;
	data = new char[256];
	length = varb.value.asnValue.arbitrary.length;
	for (unsigned int i = 0; i < length; i++)
	{
	  char number[32];
	  sprintf(number, "%d", varb.value.asnValue.arbitrary.stream[i]);
	  strcat(data, number);
	}
	lv.pszText = data;
	mList.SetItem(&lv);
	break;
      }
    }
  }
  UpdateData(FALSE);  
}

#endif


void DetailDlg::OnButtonShowRaw() 
{
  RawTrapData dlg(mPacket);
  dlg.DoModal();
}
