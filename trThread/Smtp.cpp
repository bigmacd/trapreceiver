#pragma warning(disable:4786)

#ifndef __SMTP_H__
#include "Smtp.h"
#endif

#include "..\Registry.h"
#include <time.h>
#include "Base64.h"

#include <winsock2.h>

#include <fstream>
using namespace std;

#include "..\eventlogmessage.h"


BOOL doTranslation = FALSE;
BOOL doLogging = FALSE;
std::ofstream ofile;

Smtp::Smtp()
     :Thread("Smtp"),
      mPacket(NULL)
{
}

Smtp::~Smtp()
{
}

Smtp::Smtp(Packet* packet, CString emailIds, char* timestamp)
     :Thread("Smtp"),
      mPacket(packet),
      mEmailIds(emailIds)
{
  if (timestamp != NULL)
    strcpy_s(mTimestamp, 127, timestamp);
}


void
Smtp::main()
{
  //  MibDb mDb;
  Registry registry;
  doTranslation = registry.translateOids();
  //  if (doTranslation == TRUE)

  // let's try connecting to the mib database
  CString d = registry.cwd();
  CString cwd = d;
  d += "\\Mibdb.db";
  mDb.Connect(d.GetBuffer(0));
  
  try
  {
    try {
      doLogging = registry.smtpLogging();
    }
    catch(...) {
      registry.smtpLogging(doLogging);
    }

    CString currDir;
  
    // new in 8.2, use windows temp for files
    char path[2048];
    GetTempPath(MAX_PATH, path);
    currDir = path;
    currDir += "TrapRcvr.smtp.log";

    if (doLogging == TRUE) {
      CString s = "Email Logging: ";
      BOOL error = false;
      try {
		ofile.open(currDir, ios::out | ios::app);
      }
      catch(...) {
		s += "caught exception trying to open ";
		s += currDir;
		error = TRUE;
      }

      if (ofile.bad()) { 
		  s += " stream failbit is set ("; s += currDir; s += ")"; error = TRUE; 
	  }
      if (!ofile.is_open()) { 
		  s += " did not open ("; s += currDir; s += ")"; error = TRUE; 
	  }
      ofile.clear();

      if (error) {
		eventlogmessage e;
		e.evtlog((const char *)(s.GetBuffer(0)));
		doLogging = FALSE;
      }

    }

    registry.ReOpen("Email");
    unsigned long count = registry.getCount();

    if (doLogging == TRUE)
      ofile << "processing " << count << " email destinations" << endl;

    for (unsigned long x = 0; x < count; x++)
    {
      registry.ReOpen(x);
      int id = registry.emailId();
      CString c;
      c.Format("%d", id);
      c = c.Right(6);
      if (mEmailIds.Find(c) == -1)
      {
		if (doLogging == TRUE)
			ofile << "email id:" << id << " not part of this action" << endl;
		continue;
      }

      if (doLogging == TRUE)
      {
		ofile << "email id:" << id << " part of this action" << endl;      
		ofile << "destination is " << registry.smtpServerHost() << endl;
		ofile << "destination port is " << registry.smtpPort() << endl;
		ofile.flush();
      }

	  /*
	  sendemail.exe -f martin.cooley@gmail.com -t martin.cooley@gmail.com -u test
	  -m "testing 1, 2, 3", -s smtp.gmail.com:587 -xu martin.cooley@gmail.com -xp "dcpk lsqp iawt ikmi"
	  -o tls=yes
	  */
      
      //  Set the SMTP login/password (if required)
      CBase64 cb62;
      cb62.Decode(registry.authUser().GetBuffer(0));
      CString uname;
      uname = cb62.DecodedMessage();

      cb62.Decode(registry.authJuniper().GetBuffer(0));
      CString pwd;
      pwd = cb62.DecodedMessage();

	  CString dest = registry.smtpServerHost(); dest += ":"; 
	  CString port;
	  port.Format("%d", registry.smtpPort());
	  dest += port;
	  BOOL usessl = FALSE;
	  try {
		usessl = registry.useSSL();
	  }
	  catch(...) {
		registry.useSSL(usessl);
	  }
	  if (usessl) 		
		  if (doLogging == TRUE)
			ofile << "setting ssl mode on connection" << endl;

	  CString tls = "-o tls="; tls += (usessl ? "yes" : "no");
	 
      CString Subject = registry.subject();
      CString Message = registry.message();
      CString realSubject;
      CString realMessage;
      if (mPacket == NULL)
      {
	    realMessage = Message;
	    realSubject = Subject;
      }
      else
      {
	    realMessage = ExpandKeywords(Message);
	    realSubject = ExpandKeywords(Subject);
      }

      if (doLogging == TRUE)
      {
		ofile << "translated subject: " << realSubject << endl;
		ofile << "translated message: " << realMessage << endl;
		ofile << "From: " << registry.from() << endl;
		ofile << "To: " << registry.to() << endl;
		ofile.flush();
      }        

	  ////////////////////////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////////////////////////
	   // assemble the above command line
	  CString args;
	  args = "-q "; 
	  args += "-l "; args += currDir; 
	  // -f registry.from()
	  args += " -f "; args += registry.from();
	  // -t registry.to()
	  args += " -t "; args += registry.to();
	  // -u (mPacket == NULL ? registry.subject() : ExpandKeywords(registry.subject())
	  args += " -u \""; args += realSubject; args += "\"";
	  // -m (mPacket == NULL ? registry.message() : ExpandKeywords(registry.message())
	  args += " -m \""; args += realMessage; args += "\"";
	  // -s dest
	  args += " -s "; args += dest;
	  // -xu uname
	  args += " -xu "; args += uname;
	  // -xp pwd
	  args += " -xp "; args += pwd;
	  // tls
	  args += " "; args += tls;

	  CString prog = cwd;
	  prog += "\\sendEmail.exe";
	  char p2[2048];
      memset(p2, 0, 2048);
	  GetShortPathName(prog, p2, 2048);
	  if (doLogging == TRUE)
	     ofile << "executable's short name is: " << p2 << endl;
	  prog = p2;
	  prog += ' ';
	  prog += args;
	  PROCESS_INFORMATION p;
	  STARTUPINFO s;
	  //GetStartupInfo(&s);
	  DWORD rx = CreateProcess(0, 
					  	       prog.GetBuffer(0), 
							   0, 
					           0, 
							   FALSE,
							   CREATE_NO_WINDOW,
							   NULL,
							   0,
							   &s, 
					           &p);
      if (rx != 0)  {
		 CloseHandle(p.hProcess);
		 CloseHandle(p.hThread);
      }
	  else {
	     if (doLogging == TRUE) {
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
      ////////////////////////////////////////////////////////////////////////////////////
	  ////////////////////////////////////////////////////////////////////////////////////
    } //     for (unsigned long x = 0; x < count; x++)
      
    if (doLogging == TRUE)
    {
      ofile << "Done with email action processing" << endl;
      ofile.close();
    }
  } // try
  catch(...)
  {
    if (doLogging == TRUE)
    {
      ofile << "Caught an unknown exception!" << endl;
      ofile.close();
    }
      
  }
  Thread::Exit();
} // ::main()

CString
Smtp::ExpandKeywords(CString message)
{
  CString realMessage;
  realMessage.Empty();

  char* str = message.GetBuffer(0);
  int len = strlen(str);
  while (len > 0)
  {
    BOOL zero = FALSE;	      
    char* end = str;
    if (*end != '%')
    {
      while (len > 0)
      {
	end++; len--;
	if (*end == '%')
	{
	  *end = 0;
	  zero = TRUE;
	  break;
	}
      }
    }
    // check to see if a predefined
    // argument was used
    char specific[32];
    char genericx[32];
    if (!strncmp(str, "%SENDERIP%", 10))
    {
      len -= 10; end += 10;

      Registry r;
      if (r.reverseLookup())
      {
	unsigned long addr = inet_addr(mPacket->SenderIP());
	struct hostent* hp = gethostbyaddr((const char*)&addr, 
					   sizeof(unsigned long), 
					   AF_INET);
	if (hp == NULL)
	{
	  realMessage += mPacket->SenderIP();
	  realMessage += "-u";
	}
	else
	  realMessage += hp->h_name;
      }
      else
	realMessage += (char*)mPacket->SenderIP();
    }
    else
      if (!strncmp(str, "%SENDEROID%", 11))
      {
	string asdf;
	try   {
	  len -= 11; end += 11;
	  if (doTranslation == TRUE)   {
	    bool status = false;
	    if (mPacket->Type() == V1TRAP)  {
	      status = mDb.FindTrap(mPacket->SenderOID(), 			
				    mPacket->SpecificTrapType());
	      if (true == status) {
		if (TRUE == doLogging)
		  ofile << "found sender OID in database" << endl;
		asdf = mDb.TrapName();
		realMessage += (char*)asdf.c_str();
	      }
	      else {
		if (TRUE == doLogging)  {
		  ofile << "could not find sender OID in database" << endl;
		  ofile << "error is " << mDb.ErrorString() << endl;
		}
		realMessage += (char*)mPacket->SenderOID();
	      }
	    }
	    else   {
	      int specificType;
	      char* oid = new char[strlen(mPacket->SenderOID()) + 1];
	      memset(oid, 0, strlen(mPacket->SenderOID() + 1));
	      strcpy_s(oid, strlen(mPacket->SenderOID())+1, mPacket->SenderOID());
	      
	      // manipulate the sender oid to remove the two subidentifiers
	      char* c = strrchr(oid, '.');
	      if (NULL != c) {
		char* lastSub = c + 1;
		specificType = atoi(lastSub);
		*c = 0;
		c = strrchr(oid, '.');
		if (NULL != c)  {
		  *c = 0;
		  status = mDb.FindTrap(oid, 
					specificType);
		  if (true == status)	{
		    if (TRUE == doLogging)
		      ofile << "found sender OID in database" << endl;
		    asdf = mDb.TrapName();
		    realMessage += (char*)asdf.c_str();
		  }
		  else {
		    if (TRUE == doLogging)  {
		      ofile << "could not find sender OID in database" << endl;
		      ofile << "error is " << mDb.ErrorString() << endl;
		    }
		    realMessage += (char*)mPacket->SenderOID();
		  }
		} // if (NULL != c)
	      } // if (NULL != c)
	      delete [] oid;
	    }
	  } // if (doTranslation == TRUE)
	  else
	    realMessage += (char*)mPacket->SenderOID();
	}
	catch(...)   {   }
      } // if (!strncmp(str, "%SENDEROID%", 11))

      else
	if (!strncmp(str, "%COMMUNITY%", 11))
	{
	  len -= 11; end += 11;
	  realMessage += 
	    (char*)mPacket->Community();
	}
	else
	  if (!strncmp(str, "%GENERICTYPE%", 13))
	  {
	    len -= 13; end += 13;
	    sprintf_s(genericx, 32,
		    "%d",
		    mPacket->GenericTrapType());
	    realMessage += genericx;
	  }
	  else
	    if (!strncmp(str, "%SPECIFICTYPE%", 14))
	    {
	      len -= 14; end += 14;			
	      sprintf_s(specific, 32,
		      "%d",
		      mPacket->SpecificTrapType());
	      realMessage += specific;
	    }
	    else
	      if (!strncmp(str, "%VBOID", 6))
	      {
		char* percentSign = strchr(str + 1, '%');
		// there has to be more
		if (percentSign > (str + 6))
		{
		  if (percentSign <= (str + len))
		  {
		    *percentSign = 0;
		    int which = atoi(str + 6);
		    char asterisk = *(str + 6);
		    if (asterisk == '*') // new feature 3/15/2017 - does all varbind oids
		      which = -1;
		    ///////////////////////////////////////////////////////////////////
		    realMessage += doVbOidLookups(which, doTranslation, doLogging); 
		    ///////////////////////////////////////////////////////////////////
		    len -= ((percentSign - str) + 1);
		    end += ((percentSign - str) + 1);
		    *percentSign = '%';
		  }
		  else
		    len = 0;  // when too far
		}
		else
		{ len -= 7; end += 7; }
	      }
	      else
		if (!strncmp(str, "%VBDATA", 7)) {
		  char* percentSign = strchr(str + 1, '%');
		  // there has to be more
		  if (percentSign > (str + 7))
		  {
		    if (percentSign <= (str + len))
		    {
		      *percentSign = 0;
		      int which = atoi(str + 7);
		      int vblen = mPacket->VbListLength();
		      char* data = NULL;
		      std::string ename;
		      if (which > 0 && which <= vblen)		      
		      {
			if (TRUE == doTranslation)
			{
			  if (doLogging == TRUE)
			    ofile << "translating vbdata number " << which << endl;
			  data = (char*)mPacket->VbData(which);
			  ///////////////////////////////////////////////////////
			  // do lookup in case vbdata is an oid
			  if (mPacket->Type() != V1TRAP &&
			      !strcmp(mPacket->VbOID(which), "1.3.6.1.6.3.1.1.4.1.0"))
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
				    ofile << "translated vbdata number " << which << endl;
				  //				  msg = mDb.OidsName().c_str();
				  msg = asdf.c_str();
				}
				else
				{
				  if (doLogging == TRUE)
				  {
				    ofile << "could not translate vbdata number " << which << endl;
				    ofile << "error is " << mDb.ErrorString() << endl;
				  }
				}
				if (msg == NULL || strlen(msg) == 0)
				{
				  if (doLogging == TRUE)
				    ofile << "translation returned no data" << endl;
				  realMessage += (char*)data;
				}
				else
				{
				  if (doLogging == TRUE)
				    ofile << "translation returned " << msg << endl;
				  realMessage += (char*)msg;
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
			    if (!strcmp(mPacket->VbType(which), "OID"))
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
				  ofile << "translated vbdata (type OID) number " << which << endl;
				//				oiddata = mDb.OidsName().c_str();
				oiddata = asdf.c_str();
			      }
			      else
			      {
				if (doLogging == TRUE)
				  ofile << "could not translate vbdata (type OID) number " << which << endl;
			      }
			      if (oiddata == NULL || strlen(oiddata) == 0)
			      {
				if (doLogging == TRUE)
				  ofile << "translation returned no data" << endl;
				realMessage += (char*)data;
			      }
			      else
			      {
				if (doLogging == TRUE)
				  ofile << "translated vbdata (type OID)" << endl;
				realMessage += (char*)oiddata;
			      }
			    }
			    else // just some value
			    {
			      ///////////////////////////////////////////////////////
			      int value = atoi((const char*)data);
			      bool status = mDb.FindEnum(mPacket->VbOID(which), value);
			      if (true == status)
			      {
				ename = mDb.EnumName();
			      }
			      if (ename.length() > 0)
			      {
				if (doLogging == TRUE)
				  ofile << "translated vbdata number " << which << endl;
				realMessage += ename.c_str();
			      }
			      else
			      {
				if (doLogging == TRUE)
				{
				  ofile << "could not translate vbdata number " << which << endl;
				  ofile << "error is " << mDb.ErrorString() << endl;
				}
				realMessage += mPacket->VbData(which);
			      }

			    }
			  }
			  
			} // if (TRUE == doTranslation)
			else
			{
			  realMessage += mPacket->VbData(which);
			}
			
		      } // if (which > 0 && which <= vblen)
		      len -= ((percentSign - str) + 1);
		      end += ((percentSign - str) + 1);
		      *percentSign = '%';
		    }
		    else
		      len = 0;  // when too far
		  }
		  else
		  { len -= 8; end += 8; }
		} // if (!strncmp(str, "%VBDATA", 7))
		else 
		  if (!strncmp(str, "%TIMESTAMP%", 11))
		  {
		    len -= 11; end += 11;
		    realMessage += mTimestamp;
		  }
		  else
		    if (!strncmp(str, "%REALIP%", 8)) {
		      len -= 8; end += 8;
		      
		      Registry r;
		      if (r.reverseLookup())
		      {
			unsigned long addr = inet_addr(mPacket->RealIp());
			struct hostent* hp = gethostbyaddr((const char*)&addr, 
							   sizeof(unsigned long), 
							   AF_INET);
			if (hp == NULL)
			{
			  realMessage += mPacket->RealIp();
			  realMessage += "-u";
			}
			else
			  realMessage += hp->h_name;
		      }
		      else
			realMessage += (char*)mPacket->RealIp();
		    }
		    else
		    {
		      if (end == str && *end == '%')
		      {
			// found a misspelled % demarc 
			// find the end
			end = strchr(str + 1, '%');
			if (end == NULL)
			  len = 0; // no closing %, get out
			else
			{
			  end++; // past the closing %
			  len -= end - str;
			}
		      }
		      else
			realMessage += str;
		    }

    if (zero == TRUE)
      *end = '%';
    str = end;
  } // while (len)


  // version 6.0 - allow for embedded CR and LF
  // convert the string "\r" or "\n" to '\r' or '\n'
  // will have to add a space too (two characters)
  char* charstr = realMessage.GetBuffer(0);
  char* specChar;
  while ((specChar = strstr(charstr, "\\r")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\r';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\R")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\r';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\n")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\n';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\N")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\n';
  }

  return realMessage;
}


CString
Smtp::doVbOidLookups(int which, BOOL doTranslation, BOOL doLogging)
{
  CString realMessage;
  realMessage.Empty();
  BOOL done = FALSE;
  BOOL once = TRUE;
  int vblen = mPacket->VbListLength();

  while (!done) {

    if (once) {
      if (which != -1) done = TRUE; // one time, just this one 'which'
      else which = 1; // loop through them all, building up realMessage
      once = FALSE;
    }

    if (which > 0 && which <= vblen) {
      if (doTranslation == TRUE)  {
	const char* oid  = mPacket->VbOID(which);

	if (TRUE == doLogging)
	  ofile << "looking up oid (" << oid << ") in database" << endl;
      
	const char* data = NULL;
	string asdf;
      
	bool status = mDb.FindNearestOid(oid);
	if (true == status)  {
	  asdf = mDb.OidsName();
	  data = asdf.c_str();
	  if (TRUE == doLogging)  {
	    ofile << "found OID " << oid << " in database as " << data << endl;
	  }
	}
      
	// didn't find anything
	if (data == NULL || strlen((const char*)data) == 0)  {
	  if (doLogging == TRUE) {
	    ofile << "could not translate vboid " << oid << endl;
	    ofile << "error is " << mDb.ErrorString() << endl;
	  }
	  realMessage += mPacket->VbOID(which);
	}
	else { // found it, now add any trimmed off sub-ids 
	  if (doLogging == TRUE)
	    ofile << "successfully translated vboid " << oid << endl;
	  
	  realMessage += (char*)data;
	}
      } // if (doTranslation == TRUE)
      else  {
	if (TRUE == doLogging)
	  ofile << "not looking up oids in database" << endl;
	realMessage += mPacket->VbOID(which);
      }
    } // if (which > 0 && which <= vblen)
    which++;
    if (which > vblen)
      done = TRUE;
    else
      realMessage += "\r\n";
  } // while (!done)
  return realMessage;
}





