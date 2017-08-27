
#pragma warning(disable: 4786)

#include "ActionThread.h"
#include "..\Registry.h"
//#include "Queue.h"
#include "Packet.h"
//#include "..\TrActionAdd.h"
#include "UdpClient.h"
#include "TrapMatcher.h"
#include "VarbindList.h"
#include "VbPair.h"
#include "IpAddrVarbind.h"
#include <process.h>
#include <mmsystem.h>
#include "..\eventlogmessage.h"

#include "Mibdb.h"
#include "trDeDup.h"

#include <fstream>
using namespace std;

ActionThread::ActionThread(Packet* p, char* timestamp, unsigned int which)  // 0 = service, 1 = gui
             :Thread("ActionThread"),
	      mPacket(p),
	      mSmtp(NULL),
	      mWhich(which)
{
  if (timestamp != NULL)
    strcpy_s(mTimestamp, 127, timestamp);
}

ActionThread::~ActionThread()
{
  if (mSmtp != NULL)
    delete mSmtp;
  if (mPacket != NULL)
    delete mPacket;
}

void
ActionThread::main()
{
  Registry registry;
  CString currDir;
  BOOL doLogging = FALSE;
  TrDeDup* dedupdb = NULL;
  
  BOOL doTranslation = registry.translateOids();

  try { doLogging = registry.actionLogging(); }
  catch(...) { registry.actionLogging(FALSE); }    

  //  try { currDir = registry.cwd(); }
  //  catch(...) { }

  //  if (currDir.GetLength() > 0)
  //    currDir += "\\TrapRcvr.actions.log";
  //  else
  //    currDir = "TrapRcvr.actions.log";

  /////////////////////////////////////////////////////////////////////////////////////////
  // deduplication support
  BOOL dedup = FALSE;
  int interval = 60;
  try {
    dedup = registry.deduplicate();
  }
  catch(...) {
    registry.deduplicate(dedup);
  }
  try {
    interval = registry.dedupinterval();
  }
  catch(...) {
    registry.dedupinterval(interval);
  }
  /////////////////////////////////////////////////////////////////////////////////////////

  // let's try connecting to the mib database
  MibDb mDb;
  CString d = registry.cwd();
  d += "\\Mibdb.db";
  mDb.Connect(d.GetBuffer(0));

  std::ofstream ofile;

  // new in 8.2, use windows temp for files
  char path[2048];
  GetTempPath(MAX_PATH, path);
  currDir = path;
  currDir += "TrapRcvr.actions.log";

  if (doLogging == TRUE)
  {
    CString s = "Action Logging: ";
    BOOL error = false;
    try {
      ofile.open(currDir, ios::out | ios::app);
    }
    catch(...) {
      s += "caught exception trying to open ";
      s += currDir;
      error = TRUE;
    }

    if (ofile.bad()) 
      { s += " stream failbit is set ("; s += currDir; s += ")"; error = TRUE; }
    if (!ofile.is_open()) 
      { s += " did not open ("; s += currDir; s += ")"; error = TRUE; }
    ofile.clear();

    if (error) {
      eventlogmessage e;
      e.evtlog((const char *)(s.GetBuffer(0)));
      doLogging = FALSE;
    }
  }

  try
  {
    registry.ReOpen("Actions");
    unsigned int count = registry.getCount();

    if (mPacket != NULL)
    {
      if (doLogging == TRUE)
	ofile << "processing " << count << " actions" << endl;

      for (unsigned int x = 0; x < count; x++)
      {
	registry.ReOpen(x);
	BOOL incTrapData = FALSE;
	try
	{
	  incTrapData = registry.includeTrapData();
	}
	catch(...)
	{
	  registry.includeTrapData(FALSE);
	}

	if (doLogging == TRUE)
	  ofile << "this action " << (incTrapData ? "includes " : "does not include ") << "trap data" << endl;
	TrapMatcher match;

        BOOL matches = match.Matches(mPacket, x);

	if (matches == TRUE)
	{

	  // see if this same trap was received just a little while ago
	  BOOL goodtogo = TRUE;

	  if (TRUE == dedup) {
	    dedupdb = new TrDeDup();
	    if (dedupdb->IsDuplicate(mPacket->SenderIP(), mPacket->SenderOID(), interval)) {
	      if (doLogging == TRUE)
		ofile << "this trap is a duplicate, therefore no further action(s) will be performed." << endl;
	      goodtogo = FALSE;
	    }
	    else {
	      if (doLogging == TRUE)
		ofile << "Deduplication is enable, however this trap is NOT a duplicate." << endl;
	    }
	  }
	  //
	  /////////////////////////////////////////////////////////////////////////////////////////
	  // if (1) { 
	  if (goodtogo) { 

	    if (dedupdb != NULL)
	      delete dedupdb;

	    if (doLogging == TRUE)
	      ofile << "this action's watch condition matches the incoming trap's data" << endl;

	    BOOL done = FALSE;
	    int bools = registry.bools();
	    
	    BOOL emailStarted = FALSE;
	    //	    while (!done)
	    while (bools != 0)
	    {
#ifdef _DEBUG
	      cout << "bools is " << bools << endl;
#endif
	      // get the actions now and perform them

	      //#ifdef _DEBUG
	      //#ifdef _WINDOWS
	      //CString cs = "This is a "; 
	      //cs += (mWhich ? "gui" : "service");
	      //cs += " action execution loop";
	      //AfxMessageBox(cs);
	      //#else
	      //cout << "this is a " << (mWhich ? "gui" : "service") << " action execution loop" << endl;
	      //#endif
	      //#endif

	      if (doLogging == TRUE) {
		ofile << "this action execution loop is currently running from the " << (mWhich ? "GUI" : "service") << endl;
	      }

	      if (mWhich == 0 && (bools & 1)) // service and email
	      {
		if (emailStarted) {
		  if (mSmtp->State() == Thread::TERMINATED) {
		    if (doLogging == TRUE)
		      ofile << "tidying up the smtp mail thread" << endl;
		    done = TRUE;
		    bools &= ~1;
		  }
		}
		else {
		  if (doLogging == TRUE)
		    ofile << "found the email action set" << endl;
		  try
		  {
		    CString emailIds = registry.emailRecipients();
		    if (doLogging == TRUE)
		      ofile << "the following email ids are configured: " << emailIds << endl;
		    if (!emailStarted)
		    {
		      if (doLogging == TRUE)
			ofile << "starting the smtp mail thread" << endl;
		      mSmtp = new Smtp(mPacket, emailIds, mTimestamp);
		      mSmtp->Start();
		      emailStarted = TRUE;
		    }
		  }
		  catch(...)
		    {
		      
		    }
		}
	      } // if (mWhich == 0 && bools & 1) // service and email
	      else {
		if (doLogging == TRUE)
		  if (bools & 1) 
		    if (mWhich == 1)
		      ofile << "Email action set but this action execution loop does not process those." << endl;
		bools &= ~1;
		done = TRUE;
	      }

	      if (bools & 2) // execute
	      {
		if (doLogging == TRUE)
		  ofile << "found the execute action set" << endl;

		try  // start of execute processing
		{
		  bools &= ~2;
		  Registry r;

		  // check this action's runFrom flag.  Service = 0, GUI = 1
		  int runFrom = 1;    // default to 8.1 mode which is GUI
		  try {
		    runFrom = registry.runFrom();
		  } catch(...) { registry.runFrom(runFrom); }

		  if (doLogging == TRUE) {
		    ofile << "this executable is set to run from the Trap Receiver " << (runFrom ? "GUI" : "service") << endl;
		  }
		  //#ifdef _DEBUG
		  //#ifdef _WINDOWS
		  //		  CString cs = "Running executable from: ";
		  //cs += (runFrom ? "gui" : "service");
		  //cs += " action execution loop";
		  //		  AfxMessageBox(cs);
		  //#else
		  //		  cout << "Running executable from: " << (runFrom ? "gui" : "service") << " action execution loop" << endl;
		  //#endif
		  //#endif

		  if (mWhich == runFrom) 
		  {

		    CString envParmName;
		    CString envParmString;
		    char*   envParmBlock;
		    CString tempParmName;
		    BOOL    doDeleteNotFree = TRUE;
		    envParmString.Empty();
		    CString args = registry.arguments();
		    if (r.includeEnvironment() && incTrapData)
		    {
		      if (doLogging == TRUE)
			ofile << "including trap data as environment variables" << endl;
		      if (r.includeEnvIpAddress())
		      { 
			if (doLogging == TRUE)
			  ofile << "including trap's IP Address" << endl;
			
			tempParmName = r.envIpAddress();
			if (!tempParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "environment variable name for IP address is " << tempParmName << endl;
			  if (r.reverseLookup())
			  {
			    if (doLogging == TRUE)
			      ofile << "doing a reverse lookup on the IP address" << endl;
			    unsigned long addr = inet_addr(mPacket->SenderIP());
			    struct hostent* hp = gethostbyaddr((const char*)&addr, 
							       sizeof(unsigned long), 
							       AF_INET);
			    if (hp == NULL)
			    {
			      if (doLogging == TRUE)
				ofile << "reverse lookup failed, defaulting to IP address" << endl;
			      envParmString += tempParmName;
			      envParmString += '=';
			      envParmString += mPacket->SenderIP();
			      envParmString += '\a';
			    }
			    else
			    {
			      if (doLogging == TRUE)
				ofile << "reverse lookup succeeded, found: " << hp->h_name << endl;
			      envParmString += tempParmName;
			      envParmString += '=';
			      envParmString += hp->h_name;
			      envParmString += '\a';
			    }
			    
			  }
			  else
			  {
			    envParmString += tempParmName;
			    envParmString += '=';
			    envParmString += mPacket->SenderIP();
			    envParmString += '\a';
			  }
			} // if (!tempParmName.IsEmpty())
		      } // if (r.includeEnvIpAddress())
		      
		      if (r.includeEnvRealIpAddress())
		      { 
			if (doLogging == TRUE)
			  ofile << "including trap's Real IP Address" << endl;
		      
			tempParmName = r.envRealIpAddress();
			if (!tempParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "environment variable name for Real IP address is " << tempParmName << endl;
			  if (r.reverseLookup())
			  {
			    if (doLogging == TRUE)
			      ofile << "doing a reverse lookup on the Real IP address" << endl;
			    unsigned long addr = inet_addr(mPacket->RealIp());
			    struct hostent* hp = gethostbyaddr((const char*)&addr, 
							       sizeof(unsigned long), 
							       AF_INET);
			    if (hp == NULL)
			    {
			      if (doLogging == TRUE)
				ofile << "reverse lookup failed, defaulting to Real IP address" << endl;
			      envParmString += tempParmName;
			      envParmString += '=';
			      envParmString += mPacket->RealIp();
			      envParmString += '\a';
			    }
			    else
			    {
			      if (doLogging == TRUE)
				ofile << "reverse lookup succeeded, found: " << hp->h_name << endl;
			      envParmString += tempParmName;
			      envParmString += '=';
			      envParmString += hp->h_name;
			      envParmString += '\a';
			    }
			    
			  }
			  else
			  {
			    envParmString += tempParmName;
			    envParmString += '=';
			    envParmString += mPacket->RealIp();
			    envParmString += '\a';
			  }
			} // if (!tempParmName.IsEmpty())
		      } // if (r.includeEnvRealIpAddress())

		      if (r.includeEnvCommunity())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's community string" << endl;
			tempParmName = r.envCommunity();
			if (!tempParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "environment variable name for community string is " << tempParmName << endl;
			  envParmString += tempParmName;
			  envParmString += '=';
			  envParmString += mPacket->Community();
			  envParmString += '\a';
			}
		      } // if (r.includeEnvCommunity())
		      
		      if (mPacket->Type() == V1TRAP)
		      {
			if (r.includeEnvSenderOID())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's Sender OID" << endl;
			  tempParmName = r.envSenderOID();
			  if (!tempParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "environment variable name for Sender OID is " << tempParmName << endl;
			    envParmString += tempParmName;
			    envParmString += '=';
			    
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    // new in version 6.50 - translate cmdline parms and env variables.
			    if (TRUE == doTranslation)
			    {
			      string asdf;
			      if (doLogging == TRUE)
				ofile << "translating Sender OID" << endl;
			      bool status = mDb.FindTrap(mPacket->SenderOID(),
							 mPacket->SpecificTrapType());
			      if (true == status)
			      {
				asdf = mDb.TrapName();
				if (doLogging == TRUE)
				  ofile << "successfully translated Sender OID" << endl;
				envParmString += asdf.c_str();
			      }
			      else
			      {
				if (doLogging == TRUE)
				{
				  ofile << "failed to translate Sender OID" << endl;
				  ofile << "error is " << mDb.ErrorString() << endl;
				}
				envParmString += mPacket->SenderOID();
			      }
			    }
			    else 
			    {
			      if (doLogging == TRUE)
				ofile << "not translating Sender OID" << endl;
			      envParmString += mPacket->SenderOID();
			    }
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    envParmString += '\a';	
			  }
			}
			if (r.includeEnvGenericType())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's generic type" << endl;
			  tempParmName = r.envGenericType();
			  if (!tempParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "environment variable name for generic type is " << tempParmName << endl;
			    CString generictype;
			    generictype.Format("%d", mPacket->GenericTrapType());
			    envParmString += tempParmName;
			    envParmString += '=';
			    envParmString += generictype;
			    envParmString += '\a';
			  }
			}
			if (r.includeEnvSpecificType())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's specific type" << endl;
			  tempParmName = r.envSpecificType();
			  if (!tempParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "environment variable name for specific type is " << tempParmName << endl;
			    CString specifictype;
			    specifictype.Format("%d", mPacket->SpecificTrapType());
			    envParmString += tempParmName;
			    envParmString += '=';
			    envParmString += specifictype;
			    envParmString += '\a';
			  }
			}
		      } // if (mPacket->Type() == V1TRAP)
		      
		      int vblen = mPacket->VbListLength();
		      CString vbNum;
		      vbNum.Format("%d", vblen);
		      
		      if (r.includeEnvVbData())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's varbind data (there " << (vblen > 1 ? "are " : "is ") << vblen << " of them)" << endl;
			tempParmName = r.envVbData();
			if (!tempParmName.IsEmpty())
			{
			  for (int x = 1; x <= vblen; x++)
			  {
			    tempParmName.Format("%s%d", 
						r.envVbData(),
						x);
			    envParmString += tempParmName;
			    envParmString += '=';
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    // new in version 6.50 - translate cmdline parms and env variables.
			    if (TRUE == doTranslation)
			    {
			      char* data;
			      if (doLogging == TRUE)
				ofile << "translating vbdata number " << x << endl;
			      data = (char*)mPacket->VbData(x);
			    
			      ///////////////////////////////////////////////////////
			      // do lookup in case vbdata is an oid
			      if (mPacket->Type() != V1TRAP &&
				  !strcmp(mPacket->VbOID(x), "1.3.6.1.6.3.1.1.4.1.0"))
			      {
				if (doLogging == TRUE)
				  ofile << "Data is V2 Trap OID" << endl;
				const char* msg = NULL;
				char* oid = NULL;
				try
				{
				  int len = strlen((const char*)data);
				  if (len)
				  {
				    oid = new char[len + 1];
				    strcpy_s(oid, len+1, (const char*)data);
				    
				    char* c = strrchr(oid, '.');
				    if (c != NULL)
				    {
				      *c = 0;
				      c = strrchr(oid, '.');
				      if (c != NULL)
					*c = 0;
				    }				  
				    
				    string asdf;
				    bool status = mDb.FindOid(oid);
				    if (true == status)
				    {
				      asdf = mDb.OidsName();
				      msg = asdf.c_str();
				      
				      if (doLogging == TRUE)
					ofile << "translated vbdata number " << x << endl;
				    }
				    else
				    {
				      if (doLogging == TRUE)
				      {
					ofile << "could not translate vbdata number " << x << endl;
					ofile << "error is " << mDb.ErrorString() << endl;
				      }
				    }
				    if (msg == NULL || strlen(msg) == 0)
				    {
				      if (doLogging == TRUE)
					ofile << "translation returned no data" << endl;
				      envParmString += (char*)data;
				    }
				    else
				    {
				      if (doLogging == TRUE)
					ofile << "translation returned " << msg << endl;
				      envParmString += (char*)msg;
				    }
				    
				    if (oid != NULL)
				      delete [] oid;
				  } // if (len)
				}
				catch(...)
				{ 
				}
			      }
			      else //
			      {
				// maybe it is an object ID.  Let see if we can find it in our DB.
				if (!strcmp(mPacket->VbType(x), "OID"))
				{
				  if (doLogging == TRUE)
				    ofile << "translating vbdata that is an OID" << endl;
				  
				  const char* oiddata = NULL;
				  string asdf;
				  bool status = mDb.FindOid(data);
				  if (true == status)
				  {
				    asdf = mDb.OidsName();
				    oiddata = asdf.c_str();
				    if (doLogging == TRUE)
				      ofile << "translated vbdata (type OID) number " << x << endl;
				  }
				  else
				  {
				    if (doLogging == TRUE)
				      ofile << "could not translate vbdata (type OID) number " << x << endl;
				  }
				  if (oiddata == NULL || strlen(oiddata) == 0)
				  {
				    if (doLogging == TRUE)
				      ofile << "translation returned no data" << endl;
				    envParmString += (char*)data;
				  }
				  else
				  {
				    if (doLogging == TRUE)
				      ofile << "translated vbdata (type OID)" << endl;
				    envParmString += (char*)oiddata;
				  }
				}
				else // just some value
				{
				  ////////////////////////
				  ////////////////////////
				  ////////////////////////
				  int value = atoi((const char*)data);
				  bool status = mDb.FindEnum(mPacket->VbOID(x), value);
				  std::string ename;
				  if (true == status)
				  {
				    ename = mDb.EnumName();
				  }
				  if (ename.length() > 0)
				  {
				    if (doLogging == TRUE)
				      ofile << "translated vbdata number " << x << endl;
				    envParmString += ename.c_str();
				  }
				  else
				  {
				    if (doLogging == TRUE)
				    {
				      ofile << "could not translate vbdata number " << x << endl;
				      ofile << "error is " << mDb.ErrorString() << endl;
				    }
				    envParmString += mPacket->VbData(x);
				  }
				}
			      }
			    } // if (TRUE == doTranslation)
			    else
			    {
			      envParmString += mPacket->VbData(x);
			    }
			    envParmString += '\a';
			  } // for (int x = 1; x <= vblen; x++)
			} // if (!tempParmName.IsEmpty())
		      } // if (r.includeEnvVbData())
		      
		      if (r.includeEnvVbOID())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's varbind oids (again, there " << (vblen > 1 ? "are " : "is ") << vblen << " of them)" << endl;
			tempParmName = r.envVbOID();
			if (!tempParmName.IsEmpty())
			{
			  for (int x = 1; x <= vblen; x++)
			  {
			    tempParmName.Format("%s%d", 
						r.envVbOID(),
						x);
			    envParmString += tempParmName;
			    envParmString += '=';
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    ///////////////////////////////////////////////
			    // new in version 6.50 - translate cmdline parms and env variables.
			    if (TRUE == doTranslation)
			    {
			      if (doLogging == TRUE)
				ofile << "translating vboid number " << x << endl;

			      const char* oid  = mPacket->VbOID(x);
			      const char* data = NULL;
			      string asdf;

			      bool status = mDb.FindNearestOid(oid);
			      if (true == status)  {
				asdf = mDb.OidsName();
				data = asdf.c_str();
			      }
		 
			      // didn't find anything
			      if (data == NULL || strlen((const char*)data) == 0)  
			      {
				if (doLogging == TRUE)
				{
				  ofile << "could not translate vboid number " << x << endl;
				  ofile << "error is " << mDb.ErrorString() << endl;
				}
				envParmString += mPacket->VbOID(x);
			      }
			      else  // found it
			      {
				if (doLogging == TRUE)
				  ofile << "successfully translated vboid number " << x << endl;
				envParmString += (char*)data;
			      }
			    }
			    else // not translating oids, just use the oid
			    {
			      if (doLogging == TRUE)
				ofile << "not translating vboid" << endl;
			      envParmString += mPacket->VbOID(x);
			    }
			    envParmString += '\a';
			  }
			}
		      } // if (r.includeEnvVbData())
		      // set up the REAL environment here
		  
		      LPVOID junk = GetEnvironmentStrings();
		      // find the size of the current environment
		      int curr = 0;
		      char* t = (char*)junk;
		      while (1)
		      {
			if (*t == 0 && *(t + 1) == 0)
			{
			  curr++;
			  break;
			}
			curr++; t++;
		      }
		      
		      // find the size of the added environment data
		      int added = envParmString.GetLength();
		      
		      // get new buffer for new env block
		      envParmBlock = new char [curr + added + 1];
		      
		      // load data into new block
		      memcpy(envParmBlock, junk, curr);
		      FreeEnvironmentStrings((char*)junk);
		      
		      // fix up the added environment data (\a -> NULL);
		      t = envParmString.GetBuffer(0);
		      for (int index = 0; index < added; index++)
		      {
			if (t[index] == '\a')
			  t[index] = 0;
		      }
		      memcpy(envParmBlock + curr, t, added);
		      
		      // final null termination
		      envParmBlock[curr + added] = 0;
		      
		    } // if (r.includeEnvironment() && incTrapData)
		    else
		    {
		      if (doLogging == TRUE)
			ofile << "looks like we are not going to include the trap's data as environment variables" << endl;
		      envParmBlock = GetEnvironmentStrings();
		      doDeleteNotFree = FALSE;
		    }
		  
		    // don't know how this ended up missing, but in case
		    // it is, default it to false
		    try { r.includeCmdLine(); }
		    catch(...) { r.includeCmdLine(FALSE); }
		    
		    if (r.includeCmdLine() && incTrapData)
		    {
		      if (doLogging == TRUE)
			ofile << "including trap data as command line arguments" << endl;
		      char specific[32];
		      char generic[32];

		      if (r.includeCmdIpAddress())
		      {
			if (doLogging == TRUE)
			  ofile << "including trap's IP Address" << endl;
			args += ' ';
			envParmName = r.cmdIpAddress();
			if (!envParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "command line parameter for IP address is " << envParmName << endl;
			  args += envParmName;
			  args += ' ';
			}
			if (r.reverseLookup())
			{
			  if (doLogging == TRUE)
			    ofile << "doing a reverse lookup on the IP address" << endl;
			  unsigned long addr = inet_addr(mPacket->SenderIP());
			  struct hostent* h = gethostbyaddr((const char*)&addr, 
							    sizeof(unsigned long), 
							    AF_INET);
			  if (h == NULL)
			  {
			    if (doLogging == TRUE)
			      ofile << "reverse lookup failed, defaulting to IP address" << endl;
			    args += mPacket->SenderIP();
			  }
			  else
			  {
			    if (doLogging == TRUE)
			      ofile << "reverse lookup succeeded, found: " << h->h_name << endl;
			    args += h->h_name;
			  }
			}
			else
			  args += (char*)mPacket->SenderIP();
		      } // if (r.includeCmdIpAddress())

		      ////////////////////////////////////////////////		
		      if (r.includeCmdRealIpAddress())
		      {
			if (doLogging == TRUE)
			  ofile << "including trap's Real IP Address" << endl;
			args += ' ';
			envParmName = r.cmdRealIpAddress();
			if (!envParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "command line parameter for Real IP address is " << envParmName << endl;
			  args += envParmName;
			  args += ' ';
			}
			if (r.reverseLookup())
			{
			  if (doLogging == TRUE)
			    ofile << "doing a reverse lookup on the Real IP address" << endl;
			  unsigned long addr = inet_addr(mPacket->RealIp());
			  struct hostent* h = gethostbyaddr((const char*)&addr, 
							    sizeof(unsigned long), 
							    AF_INET);
			  if (h == NULL)
			  {
			    if (doLogging == TRUE)
			      ofile << "reverse lookup failed, defaulting to Real IP address" << endl;
			    args += mPacket->SenderIP();
			  }
			  else
			  {
			    if (doLogging == TRUE)
			      ofile << "reverse lookup succeeded, found: " << h->h_name << endl;
			    args += h->h_name;
			  }
			}
			else
			  args += (char*)mPacket->RealIp();
		      } // if (r.includeCmdIpAddress())
		      
		      if (r.includeCmdCommunity())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's community string" << endl;
			args += ' ';
			envParmName = r.cmdCommunity();
			if (!envParmName.IsEmpty())
			{
			  if (doLogging == TRUE)
			    ofile << "command line parameter for community string is " << envParmName << endl;
			  args += envParmName;
			  args += ' ';
			}
			args += (char*)mPacket->Community();
		      }
		      
		      if (mPacket->Type() == V1TRAP)
		      {
			//////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////
			// new in version 6.50 - translate cmdline parms and env variables.
			if (r.includeCmdSenderOID())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's sender OID" << endl;
			  
			  args += ' ';
			  envParmName = r.cmdSenderOID();
			  if (!envParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "command line parameter for sender OID is " << envParmName << endl;
			    args += envParmName;
			    args += ' ';
			  }
			  
			  if (TRUE == doTranslation)
			  {
			    string asdf;
			    if (doLogging == TRUE)
			      ofile << "translating Sender OID" << endl;
			    bool status = mDb.FindTrap(mPacket->SenderOID(),
						       mPacket->SpecificTrapType());
			    if (true == status)
			    {
			      asdf = mDb.TrapName();
			      if (doLogging == TRUE)
				ofile << "successfully translated Sender OID" << endl;
			      args += asdf.c_str();
			    }
			    else
			    {
			      if (doLogging == TRUE)
			      {
				ofile << "failed to translate Sender OID" << endl;
				ofile << "error is " << mDb.ErrorString() << endl;
			      }
			      args += (char*)mPacket->SenderOID();
			    }
			  }
			  else 
			  {
			    if (doLogging == TRUE)
			      ofile << "not translating Sender OID" << endl;
			    args += (char*)mPacket->SenderOID();
			  }
			} // if (r.includeCmdSenderOID())
			//////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////

			if (r.includeCmdGenericType())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's generic type" << endl;
			  args += ' ';
			  envParmName = r.cmdGenericType();
			  if (!envParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "command line parameter for generic type is " << envParmName << endl;
			    args += envParmName;
			    args += ' ';
			  }
			  sprintf_s(generic, 32,  
				    "%d",
				    mPacket->GenericTrapType());
			  args += generic;
			}
			if (r.includeCmdSpecificType())
			{
			  if (doLogging == TRUE)
			    ofile << "including the trap's specific type" << endl;
			  args += ' ';
			  envParmName = r.cmdSpecificType();
			  if (!envParmName.IsEmpty())
			  {
			    if (doLogging == TRUE)
			      ofile << "command line parameter for specific type is " << envParmName << endl;
			    args += envParmName;
			    args += ' ';
			  }
			  sprintf_s(specific, 32,
				    "%d",
				    mPacket->SpecificTrapType());
			  args += specific;
			}
		      } // if (mPacket->Type() == V1TRAP)


		      int vblen = mPacket->VbListLength();

		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      // new in version 6.50 - translate cmdline parms and env variables.
		      if (r.includeCmdVbData())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's varbind data (there " << (vblen > 1 ? "are " : "is ") << vblen << " of them)" << endl;
			envParmName = r.cmdVbData();
			std::string ename;
			char* data;
			
			for (int x = 1; x <= vblen; x++)
			{
			  args += ' ';
			  char parmNumber[32];
			  if (!envParmName.IsEmpty())
			  {
			    sprintf_s(parmNumber, 32, "%d", x);
			    args += envParmName;
			    args += parmNumber;
			    args += ' ';
			  }
			  data = (char*)mPacket->VbData(x);
			  if (doLogging == TRUE)
			    ofile << "current varbind data is: " << data << endl;
			  
			  if (TRUE == doTranslation)
			  {
			    if (doLogging == TRUE)
			      ofile << "translating vbdata number " << x << endl;
			    
			    ///////////////////////////////////////////////////////
			    // do lookup in case vbdata is an oid
			    if (mPacket->Type() != V1TRAP &&
				!strcmp(mPacket->VbOID(x), "1.3.6.1.6.3.1.1.4.1.0"))
			    {
			      if (doLogging == TRUE)
				ofile << "Data is V2 Trap OID" << endl;
			      const char* msg = NULL;
			      char* oid = NULL;
			      try
			      {
				int len = strlen((const char*)data);
				if (len)
				{
				  oid = new char[len + 1];
				  strcpy_s(oid, len+1, (const char*)data);
				  
				  char* c = strrchr(oid, '.');
				  if (c != NULL)
				  {
				    *c = 0;
				    c = strrchr(oid, '.');
				    if (c != NULL)
				      *c = 0;
				  }				  
				  
				  string asdf;
				  bool status = mDb.FindOid(oid);
				  if (true == status)
				  {
				    asdf = mDb.OidsName();
				    if (doLogging == TRUE)
				      ofile << "translated vbdata number " << x << endl;
				    msg = asdf.c_str();
				  }
				  else
				  {
				    if (doLogging == TRUE)
				    {
				      ofile << "could not translate vbdata number " << x << endl;
				      ofile << "error is " << mDb.ErrorString() << endl;
				    }
				  }
				  if (msg == NULL || strlen(msg) == 0)
				  {
				    if (doLogging == TRUE)
				      ofile << "translation returned no data" << endl;
				  }
				  else
				  {
				    if (doLogging == TRUE)
				      ofile << "translation returned " << msg << endl;
				    data = (char*)msg;
				  }
				
				  if (oid != NULL)
				    delete [] oid;
				} // if (len)
			      }
			      catch(...)
				{ 
				}
			    }
			    else
			    {
			      // maybe it is an object ID.  Let see if we can find it in our DB.
			      if (!strcmp(mPacket->VbType(x), "OID"))
			      {
				if (doLogging == TRUE)
				  ofile << "translating vbdata that is an OID" << endl;
				
				const char* oiddata = NULL;
				
				string asdf;
				bool status = mDb.FindOid(data);
				if (true == status)
				{
				  asdf = mDb.OidsName();
				  if (doLogging == TRUE)
				    ofile << "translated vbdata (type OID) number " << x << endl;
				  oiddata = asdf.c_str();
				}
				else
				{
				  if (doLogging == TRUE)
				    ofile << "could not translate vbdata (type OID) number " << x << endl;
				}
				if (oiddata == NULL || strlen(oiddata) == 0)
				{
				  if (doLogging == TRUE)
				    ofile << "translation returned no data" << endl;
				}
				else
				{
				  if (doLogging == TRUE)
				    ofile << "translated vbdata (type OID) as: " << oiddata << endl;
				  data = (char*)oiddata;
				}
			      }
			      else // just some value
			      {
				
				/////////////////////////////
				/////////////////////////////
				/////////////////////////////
				int value = atoi((const char*)data);
				bool status = mDb.FindEnum(mPacket->VbOID(x), value);
				if (true == status)
				{
				  ename = mDb.EnumName();
				}
				if (ename.length() > 0)
				{
				  if (doLogging == TRUE)
				    ofile << "translated vbdata number " << x << endl;
				  data = (char*)ename.c_str();
				}
				else
				{
				  if (doLogging == TRUE)
				  {
				    ofile << "could not translate vbdata number " << x << endl;
				    ofile << "error is " << mDb.ErrorString() << endl;
				  }
				}
			      }
			    }
			  } // if (TRUE == doTranslation)
			  
			  bool addQuotes = FALSE;
			  if (strchr((const char*)data, ' '))
			    addQuotes = TRUE;
			  if (addQuotes)
			    args += "\"";
			  args += data;
			  if (addQuotes)
			    args += "\"";
			  
			} // for (int x = 1; x <= vblen; x++)
		      }  // if (r.includeCmdVbData())
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      // new in version 6.50 - translate cmdline parms and env variables.
		      if (r.includeCmdVbOID())
		      {
			if (doLogging == TRUE)
			  ofile << "including the trap's varbind oids (again, there " << (vblen > 1 ? "are " : "is ") << vblen << " of them)" << endl;
			envParmName = r.cmdVbOID();
			for (int x = 1; x <= vblen; x++)
			{
			  args += ' ';
			  if (!envParmName.IsEmpty())
			  {
			    CString tempCmdOptionName;
			    tempCmdOptionName.Format("%s%d", 
						     envParmName,
						     x);
			    args += tempCmdOptionName;
			    args += ' ';
			  }
			  
			  if (TRUE == doTranslation)
			  {
			    if (doLogging == TRUE)
			      ofile << "translating vboid number " << x << endl;

			    const char* oid  = mPacket->VbOID(x);
			    const char* data = NULL;
			    string asdf;

			    bool status = mDb.FindNearestOid(oid);
			    if (true == status) {
			      asdf = mDb.OidsName();
			      data = asdf.c_str();
			    }
			    
			    // didn't find anything
			    if (data == NULL || strlen((const char*)data) == 0)  
			    {
			      if (doLogging == TRUE)
			      {
				ofile << "could not translate vboid number " << x << endl;
				ofile << "error is " << mDb.ErrorString() << endl;
			      }
			      args += mPacket->VbOID(x);
			    }
			    else  // found it, now add any trimmed off sub-ids
			    {
			      if (doLogging == TRUE)
				ofile << "successfully translated vboid number " << x << endl;
			      args += (char*)data;
			    }
			  }
			  else // not translating oids, just use the oid
			  {
			    if (doLogging == TRUE)
			      ofile << "not translating vboid" << endl;
			    args += mPacket->VbOID(x);
			  }
			} // for (int x = 0; x < 7; x++)
		      } // if (r.includeCmdVbOID())
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      //////////////////////////////////////////////////////////////////
		      
		      if (doLogging == TRUE)
			ofile << "final argument list is <" << args << '>' << endl;
		    } // if (r.includeCmdLine() && incTrapData)
		    else
		    {
		      if (doLogging == TRUE)
			ofile << "looks like we are not going to include the trap's data as command line parameters" << endl;
		      doDeleteNotFree = FALSE;
		    }
		    
		    CString prog = registry.executable();
		    if (doLogging == TRUE)		  
		      ofile << "configured executable is: " << prog << endl;
		    char p2[2048];
		    memset(p2, 0, 2048);
		    GetShortPathName(prog.GetBuffer(0), p2, 2048);
		    
		    if (doLogging == TRUE)
		      ofile << "executable's short name is: " << p2 << endl;
		    prog = p2;
		    prog += ' ';
		    prog += args;
		    PROCESS_INFORMATION p;
		    STARTUPINFO s;
		    GetStartupInfo(&s);
		    //		s.cb = sizeof(s);
		    //		s.dwFlags = STARTF_USESHOWWINDOW;
		    //		s.wShowWindow = SW_MINIMIZE;
		    DWORD rx = CreateProcess(0, 
					     prog.GetBuffer(0), 
					     0, 
					     0, 
					     FALSE,
					     CREATE_NEW_CONSOLE,
					     (void*)envParmBlock,
					     0,
					     &s, 
					     &p);
		    if (rx != 0)
		    {
		      CloseHandle(p.hProcess);
		      CloseHandle(p.hThread);
		    }
		    else {
		      if (doLogging == TRUE)
		      {
			DWORD xxx = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				      NULL,
				      xxx,
				      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				      (LPTSTR) &lpMsgBuf,
				      0, NULL);
			ofile << "result of CreateProcess: <" << (char*)lpMsgBuf << '>' << endl;
		      }
		    }
		    
		    if (doDeleteNotFree)
		      delete [] envParmBlock;
		    else
		      FreeEnvironmentStrings(envParmBlock);
		    
		  } // if (mWhich == runFrom)
		} // try  // start of execute processing
		catch(...)  { }
	      } // if (bools & 2)
	      
	      if (mWhich == 0 && bools & 4) // service and explode
	      {

#ifdef _WIN32
		WSADATA wsaData; 
		int err; 
		OSVERSIONINFO osVer;
		memset(&osVer, 0, sizeof(OSVERSIONINFO));
		osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osVer);
		if (osVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
		  err = WSAStartup(0x202, &wsaData);
		  if (err == WSAVERNOTSUPPORTED)
		    err = WSAStartup(0x200, &wsaData);
		  if (err == WSAVERNOTSUPPORTED)
		    err = WSAStartup(0x101, &wsaData);
		}
		else
		  if (osVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		  {
		    err = WSAStartup(0x200, &wsaData);
		    if (err == WSAVERNOTSUPPORTED)
		      err = WSAStartup(0x101, &wsaData);
		  }
		  else
		    err = 99;
#endif

		if (doLogging == TRUE)
		  ofile << "found the explode action set" << endl;
		bools &= ~4;
		try
		{
		  CString expDests = registry.exploderDestinations();
		  BOOL addVarbind = FALSE;
		  Registry rr1;
		  try {
		    addVarbind = rr1.addVarbinds();
		  }
		  catch(...) {}
		  
		  Registry r("Exploder");
		  unsigned long count = r.getCount();
		  
		  if (doLogging == TRUE)
		    ofile << "processing " << count << " exploder destinations" << endl;
		  for (unsigned long x = 0; x < count; x++)
		  {
		    r.ReOpen(x);
		    int id = r.exploderId();
		    CString c;
		    c.Format("%d", id);
		    c = c.Right(6);
		    
		    if (expDests.Find(c) == -1)
		    {
		      if (doLogging == TRUE)
			ofile << "exploder id:" << id << " not part of this action" << endl;
		      continue;
		    }
		    
		    if (doLogging == TRUE)
		    {
		      ofile << "exploder id:" << id << " is part of this action" << endl;
		      ofile << "destination is " << r.exploderDestination() << endl;
		      ofile << "port is " << r.exploderPort() << endl;
		    }

		    // see if we should attach a varbind pair that is the agent address
		    BOOL found = FALSE; 
		    VbPair* newVbp = NULL;
		    // first it has to be one of these
		    if (mPacket->Type() == V2TRAP || mPacket->Type() == INFORMPDU) {
		      // and this option is active
		      if (addVarbind == TRUE) {
			if (doLogging == TRUE)
			  ofile << "going to add the SnmpTrapAddress.0 varbind" << endl;
			// check if the agentaddr varbind exists already first
			VbPair* vbp = mPacket->VbList()->FirstVbPair();
			while (vbp != NULL) {
			  OidVarbind* ovb = vbp->OIDVarbind();
			  if (!strcmp(ovb->Printable(), "1.3.6.1.6.3.18.1.3.0")) {
			    if (doLogging) 
			    found = TRUE;
			    break;
			  }
			  vbp = vbp->Next;
			}
			if (!found) {
			    if (doLogging) 
			      ofile << "Did not find the varbind 1.3.6.1.6.3.18.1.3.0 present, adding..." << endl;
			  // didn't find it so add it
			  OidVarbind* addrOidVb = new OidVarbind((char*)"1.3.6.1.6.3.18.1.3.0");
			  IpAddrVarbind* ipVb = new IpAddrVarbind((char*)mPacket->RealIp());
			  newVbp = new VbPair(addrOidVb, ipVb);
			  mPacket->Add(newVbp);
			} // if (!found)
		      } // if (addvarbind == TRUE)
		    } // if (mPacket->Type() == V2TRAP || mPacket->Type() == INFORMPDU) {
		    UdpClient udp;
		    udp.Host(r.exploderDestination().GetBuffer(0));
		    udp.Port(r.exploderPort());
		    int sentBytes = udp.Send(mPacket);
		    if (doLogging == TRUE) {
		      ofile << "Send says it sent " << sentBytes << " of data" << endl;
		      ofile << "Udp error code is " << udp.ErrorCode() << endl;
		    }
		    // take the vbpair back off
		    if (addVarbind == TRUE) {    // if we were supposed to add it
		      if (!found) {              // and we did not find it there already
			VbPair* vbp = mPacket->VbList()->FirstVbPair();
			while (vbp != NULL) {
			  if (vbp->Next == newVbp) {
			    vbp->Next = NULL;
			    break;
			  }
			}
			delete newVbp;                   // delete it
		      }
		    }
		  } // for (unsigned long x = 0; x < count; x++)
		}
		catch(...)
		{

		}
	      } // if (bools & 4)
	      else {
		if (doLogging == TRUE)
		  if (bools & 4) 
		    if (mWhich == 1)
		      ofile << "Explode action set but this action execution loop does not process those." << endl;
		bools &= ~4;
	      }

	      if (bools & 8) // page
	      {
		bools &= ~8;
	      }
	      
	      if (bools & 16) // discard
	      {
		if (doLogging == TRUE)
		  ofile << "found the discard action set, so this is an error" << endl;
		bools &= ~16;
	      }

	      if (mWhich == 1 && bools & 32) // gui and sound
	      {
		if (doLogging == TRUE)
		  ofile << "found the sound action set" << endl;
		bools &= ~32;
		int duration = registry.soundPlayDuration();
		CString file = registry.soundFile();
		if (doLogging == TRUE)
		{
		  ofile << "duration is " << duration << endl;
		  ofile << "sound file is " << file << endl;
		}
		int flags = SND_FILENAME;
		if (duration > 0)
		  flags |= SND_LOOP | SND_ASYNC;
		else
		  flags |= SND_SYNC;
		PlaySound(file, NULL, flags);
		if (duration > 0)
		{
		  Sleep(duration * 1000);
		  PlaySound(NULL, NULL, 0);
		}
	      }
	      else
		bools &= ~32;
	    } // while (!done)
	  } // if (TRUE == dedup and !mDedupDb.IsDuplicate())
	} // if (matches == TRUE)	  
	else
	{
	  if (doLogging == TRUE)
	    ofile << "this action's watch condition does not match the incoming trap's data" << endl;
	}
      } // for (unsigned int x = 0; x < count; x++)
    } // if (mPacket != NULL)
  }
  catch(...)
  {

  }
  if (doLogging == TRUE)
    ofile.close();
  Thread::Exit();
}


