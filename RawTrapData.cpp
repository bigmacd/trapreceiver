
#include "stdafx.h"
#include "traprcvr.h"
#include "RawTrapData.h"

#define BYTESPER 17

RawTrapData::RawTrapData(CWnd* pParent /*=NULL*/)
            :CDialog(RawTrapData::IDD, pParent)
{
  //{{AFX_DATA_INIT(RawTrapData)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

RawTrapData::RawTrapData(Packet* p)
            :CDialog(RawTrapData::IDD, NULL),
	     mPacket(p)
{
}


void RawTrapData::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(RawTrapData)
  DDX_Control(pDX, IDC_LIST1, mList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RawTrapData, CDialog)
  //{{AFX_MSG_MAP(RawTrapData)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
RawTrapData::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if (mPacket != NULL)
  {
    unsigned int l = mPacket->TotalLength();
    unsigned char* c = new unsigned char[l];
    mPacket->Build(c);

    CString lineMsg;
    char    line[128];
    unsigned int x = 0;
    for (x = 0; x < l; x++)
    {
      memset(line, 0, 128);
      if (x && ((x % BYTESPER) == 0))
      {
	lineMsg += "  "; // printf("  ");
	unsigned int y = (x - BYTESPER);
	for (; y < x; y++)
	  // printf("%c", isprint(c[y]) ? c[y] : '.');
	  lineMsg += (char)(isprint(c[y]) ? c[y] : '.');
	mList.AddString(lineMsg.GetBuffer(0));
	lineMsg.ReleaseBuffer();
	lineMsg.Empty();
	// printf("\r\n");
      }
      sprintf_s(line, "%02x ", c[x]);
      lineMsg += line;
      //    printf("%02x ", c[x]);	
    }

    // ascii out the last charaters
    lineMsg.Empty();
    int bytesLeft = x % BYTESPER;
    int spacesNeeded = ((BYTESPER - bytesLeft) * 3) + 2;
    for (int z = 0; z < spacesNeeded; z++)
      lineMsg += "  "; // printf(" ");

    unsigned int y = x - bytesLeft;
    for (; y < x; y++)
      //    printf("%c", isprint(c[y]) ? c[y] : '.');
      lineMsg += (char)isprint((c[y]) ? c[y] : '.');
    // printf("\r\n\r\n");
    mList.AddString(lineMsg.GetBuffer(0));

    delete [] c;
  }
  else
    mList.AddString("No Trap Data");
  return TRUE;
}
