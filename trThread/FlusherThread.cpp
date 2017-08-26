//  Copyright (c) 2013 trapreceiver.com
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  trapreceiver.com
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  trapreceiver.com
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#pragma warning(disable:4786)

#ifndef __FLUSHERTHREAD_H__
#include "FlusherThread.h"
#endif

#include "Registry.h"
#include "vbs.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <winsock2.h>

#define GVERSION     0
#define GCOMMUNITY   1
#define GGENERIC     2
#define GSPECIFIC    3
#define GTIMETICKS   4
#define GSENDERIP    5
#define GREALIP      6
#define GSENDEROID   7
#define GVARBINDS    8
#define GVBOID       9
#define GVBTYPE      10
#define GVBDATA      11

#include "Mibdb.h"
#include "..\eventlogmessage.h"

FlusherThread::FlusherThread()
               :Thread("Flusher"),
		mFlushInterval(0),
		mTimerEvent(INVALID_HANDLE_VALUE),
		mQueue(NULL),
		mGuiQueue(NULL)
{
  try
  {
    Registry registry;
    mFlushInterval = registry.flushInterval();
    mFlushInterval *= 60000;  // minutes to milliseconds
    mTimerEvent = CreateEvent(0, FALSE, FALSE, 0);

    mQueue = new QueueServer("trap", "logger");
    if (mQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
      { eventlogmessage e; e.evtlog("Could not establish communication channel for trap logger");  }

    mGuiQueue = new QueueServer("trap", "guiToLog", TRUE);
    if (mGuiQueue->mMailSlotHandle == INVALID_HANDLE_VALUE) 
      { eventlogmessage e; e.evtlog("Could not establish communication channel for trap guiToLog");  }
    //    mQueue = new NamedPipe(TRUE);

    for (int x = 0; x < 9; x++)
      mFString[x] = -1;
    
    for (int x = 0; x < 3; x++)
      mOids[x] = -1;
    ParseFormatString();

  }
  catch(...)
  {

  }

  Registry registry;
  // let's try connecting to the mib database
  CString d = registry.cwd();
  d += "\\Mibdb.db";
  mDb.Connect(d.GetBuffer(0));
}

FlusherThread::~FlusherThread()
{
  try { CloseHandle(mTimerEvent); }
  catch(...){}
  if (mQueue != NULL)
    delete mQueue;
  if (mGuiQueue != NULL)
    delete mGuiQueue;
}

BOOL doLoggingLogging = FALSE;

void
FlusherThread::main()
{
  CString currDir;
  //  {
  //    Registry registry;
  //    try {
  //      currDir = registry.cwd();
  //      currDir += "\\TrapRcvr.logging.log";
  //    }
  //    catch(...) { doLoggingLogging = FALSE; }
  //
  //  }

  // new in 8.2, use windows temp for files
  char path[2048];
  GetTempPath(MAX_PATH, path);
  currDir = path;
  currDir += "TrapRcvr.logging.log";

  while (State() != Thread::TERMINATED)
  {
    {
      Registry r;
      try  { doLoggingLogging = r.loggingLogging(); }
      catch(...)  
      {
	r.loggingLogging(doLoggingLogging);
      }	

      try {
	mSChar = r.separatorchar()[0];
      }
      catch(...) {
	mSChar = ' ';
	r.separatorchar(" ");
      }
    }

    if (doLoggingLogging == TRUE) {
      CString s = "Logging Logging: ";
      BOOL error = false;
      try {
	mLogFile.open(currDir, ios::out | ios::app);
      }
      catch(...) {
	s += "caught exception trying to open ";
	s += currDir;
	error = TRUE;
      }

      if (mLogFile.bad()) 
	{ s += " stream failbit is set ("; s += currDir; s += ")"; error = TRUE; }
      if (!mLogFile.is_open()) 
	{ s += " did not open ("; s += currDir; s += ")"; error = TRUE; }
      mLogFile.clear();
      
      if (error) {
	eventlogmessage e;
	e.evtlog((const char *)(s.GetBuffer(0)));
	doLoggingLogging = FALSE;
      }
    }

    if (doLoggingLogging == TRUE)
      mLogFile << "waiting for flush interval of " << mFlushInterval << " milliseconds" << endl;

    WaitForSingleObject(mTimerEvent, mFlushInterval);

    if (doLoggingLogging == TRUE)
      mLogFile << "done waiting " << mFlushInterval << " milliseconds" << endl;

    try 
    {
      Registry registry;
      BOOL sizeOK = TRUE;

      if (doLoggingLogging == TRUE)
      {
	if (registry.logging())
	  mLogFile << "logging is enabled" << endl;
	else
	  mLogFile << "logging is disabled" << endl;
      }

      if (registry.logging())
      {
	BOOL overwrite = registry.overWrite();
	CString filename = registry.filename();

	if (doLoggingLogging)
	{
	  mLogFile << "we will" << (overwrite ? " " : " not ") << "overwrite the file " << filename.GetBuffer(0) << endl;
	}
	
	if (!overwrite)
	{
	  if (doLoggingLogging)
	    mLogFile << "since we are not overwriting, we need to check the file size" << endl;
	  
	  // get the file size
	  struct _stat buf;
	  int result;
	  
	  result = _stat( filename.GetBuffer(0), &buf );
	  if(!result)
	  {
	    long size = (long)registry.filesize();
	    size *= 1048576; // 1Meg
	    if (buf.st_size >= size)
	    {
	      sizeOK = FALSE;
	      if (doLoggingLogging)
		mLogFile << "current file size is greater than " << size << endl;
	    }
	    else 
	      if (doLoggingLogging)
		mLogFile << "current file size is less than " << size << endl;
	  }
	} // if (!overwrite)

	int openmode = ios::out;
	if (sizeOK)
	{
	  if (!overwrite)
	    openmode |= ios::app;
	}
	else
	{
	  if (doLoggingLogging)
	    mLogFile << "since the file is too big, we will roll it now" << endl;
	  // do log rolling here
  	    bool hasExtension = false;
	    int length = filename.GetLength();

	    // save off the extenstion, if one
	    CString extension;
	    int index = filename.ReverseFind('.');
	    if (-1 != index)
	    {
	      hasExtension = true;
	      extension = filename.Right(length - (index + 1));
	      filename.SetAt(index, 0);
	      if (doLoggingLogging)
		mLogFile << "found files extension of " << extension.GetBuffer(0) << endl;
	    }
	    else 
	    {
	      if (doLoggingLogging)
		mLogFile << "did not find an extension to the file" << endl;
	      index = filename.GetLength() - 1;
	    }

	    // extract the number that trails the filename
	    index--;
	    CString numbers;
	    bool foundDigit = false;
	    while (isdigit(filename[index]))
	    {
	      foundDigit = true;
	      numbers += filename[index];
	      index--;
	    }
	    if (true == foundDigit)
	      filename.SetAt(index + 1, 0);

	    numbers.MakeReverse();

	    if (doLoggingLogging)
	      mLogFile << "found a rolled number of " << numbers.GetBuffer(0) << endl;

	    // bump up the number by one
	    int number = 0;
	    if (numbers.GetLength() > 0)
	      number = atoi(numbers.GetBuffer(0));
	    number++;

	    if (doLoggingLogging)
	      mLogFile << "going to use a roll number of " << number << endl;

 	    // put the filename back together (basename + number + extension)
	    CString newFilename;
	    if (true == hasExtension)
	      newFilename.Format("%s%03d.%s", filename, number, extension);
	    else
	      newFilename.Format("%s%03d", filename, number);
	  // write it to the registry
	    registry.filename(newFilename);
	    filename = newFilename;

	    if (doLoggingLogging)
	      mLogFile << "new rolled file is " << filename << endl;
	}

	//////////////////////////////////////////////////
	CString s = "Trap Logging: ";
	BOOL error = false;
	try {
	  mOfile.open(filename, openmode);
	}
	catch(...) {
	  s += "caught exception trying to open ";
	  s += filename;
	  error = TRUE;
	}

	if (mOfile.bad()) 
	  { s += " stream failbit is set ("; s += filename; s += ")"; error = TRUE; }
	if (!mOfile.is_open()) 
	  { s += " did not open ("; s += filename; s += ")"; error = TRUE; }
	mOfile.clear();

	if (error) {
	  eventlogmessage e;
	  e.evtlog((const char *)(s.GetBuffer(0)));
	}

	if (FALSE == error)
	//if (mOfile.good())
	//if (!mOfile.fail())
	/////////////////////////////////////////////////
	{
	  if (doLoggingLogging)
	    mLogFile << "seems we have opened the file for writing successfully" << endl;

	  unsigned char buff[4096];
	  unsigned int length = 4096;
	  memset(buff, 0, 4096);

	  if (doLoggingLogging)
	    mLogFile << "retrieving trap data from queue" << endl;

	  while (mQueue->Get(buff, length))
	  {
	    if (doLoggingLogging)
	      mLogFile << "got trap data" << endl;

	    SnmpParser parser(&buff[8], length-8);
	    Packet* p = parser.packet();
	    
	    if (p && doLoggingLogging)
	      mLogFile << "successfully parsed data into a trap" << endl;

	    struct sockaddr_in realA;
	    memset(&realA, 0, sizeof(sockaddr_in));
	    memcpy(&(realA.sin_addr.s_addr), &buff[4], 4);
	    p->RealIp(&realA);

	    if (p && (p->Type() == V2TRAP || p->Type() == INFORMPDU))
	    {
	      if (doLoggingLogging)
		mLogFile << "looks like a v2 or inform, gathering sender's IP" << endl;

	      struct sockaddr_in a;
	      memset(&a, 0, sizeof(sockaddr_in));
	      memcpy(&(a.sin_addr.s_addr), &buff[4], 4);
	      V2Notification* pdu = (V2Notification*)p->pdu();
	      pdu->SenderIP(&a);
	    }
	    
	    if (p != NULL)
	    {
	      __time32_t t = *((__time32_t*)buff);
	      struct tm* newT = NULL;
		  newT = _localtime32(&t);
	      char cTime[64];
	      memset(cTime, 0, 64);
	      strftime(cTime, 
		       64, 
		       "%X %x",
		       newT);
	      mOfile << cTime << mSChar;
	      
	      OutputTrap(p);		
	      delete p;
	    } // if (p != NULL)
	    else
	      if (doLoggingLogging)
		mLogFile << "could not transform data into a trap" << endl;
	    length = 4096;
	    memset(buff, 0, 4096);
	  } // while (mQueue->Get(buff, length))
	  
	  // now check the ack/delete queue
	  if (doLoggingLogging)
	    mLogFile << "retrieving trap data from ack/delete queue" << endl;

	  while (mGuiQueue->Get(buff, length))
	  {
	    if (doLoggingLogging)
	      mLogFile << "got ack/delete trap data" << endl;

	    SnmpParser parser(&buff[5], length-5);
	    Packet* p = parser.packet();
	    
	    if (p && doLoggingLogging)
	      mLogFile << "successfully parsed data into a trap" << endl;

	    if (p != NULL)
	    {
	      __time32_t t = *((__time32_t*)buff);
	      struct tm* newT = NULL;
		  newT = _localtime32(&t);
	      char cTime[64];
	      memset(cTime, 0, 64);
	      strftime(cTime, 
		       64, 
		       "%X %x",
		       newT);
	      mOfile << cTime ;
	      
	      // output 'A' or 'D' for Ack or Delete
	      mOfile << mSChar << buff[4] << mSChar;
	      
	      OutputTrap(p);		
	      delete p;
	    } // if (p != NULL)
	    else
	      if (doLoggingLogging)
		mLogFile << "could not transform data into a trap" << endl;

	    length = 4096;
	    memset(buff, 0, 4096);
	  } // while (mQueue->Get(buff, length))
	  
	  mOfile.flush();
	} // if (!mOfile.bad())
	else
	  if (doLoggingLogging)
	    mLogFile << "seems we could NOT open the file for writing, state is " << mOfile.bad() << endl;
      } // if (registry.logging())
      else
      {
	// purge off the queue
	unsigned char buff[4096];
	unsigned int length = 4096;
	while (mQueue->Get(buff, length))
	  length = 4096;
      }

      if (doLoggingLogging)
	mLogFile.close();

      // set up the interval in case it changed
      mOfile.close();
      mFlushInterval = registry.flushInterval();
      mFlushInterval *= 60000;  // minutes to milliseconds
    } // try
    catch(...)
    {

    }
  } // for (;;)
}

void
FlusherThread::OutputTrap(Packet* p)
{
  Registry r;
  BOOL doTranslation = r.translateOids();

  if (doLoggingLogging)
    mLogFile << "writing trap data and" << (doTranslation ? " " : " not ") << "translating OIDs" << endl;

  Pdu* thisPdu = p->pdu();
  for (int x = 0; x < 9; x++)
  {
    switch(mFString[x])
    {
     case GVERSION:   
       if (doLoggingLogging)
	 mLogFile << "writing version data" << endl;
       mOfile << p->Version() << mSChar;
       break;
     case GCOMMUNITY: 
       if (doLoggingLogging)
	 mLogFile << "writing community data" << endl;
       mOfile << p-> Community() << mSChar;
       break;
     case GGENERIC:   
       if (doLoggingLogging)
	 mLogFile << "writing generic type data" << endl;
       if (thisPdu != NULL && (p->Type() == V1TRAP))
	 mOfile << p->GenericTrapType() << mSChar;
       break;
     case GSPECIFIC:  
       if (doLoggingLogging)
	 mLogFile << "writing specific type data" << endl;
       if (thisPdu != NULL && (p->Type() == V1TRAP))
	 mOfile << p->SpecificTrapType() << mSChar;
       break;
     case GTIMETICKS: 
       if (doLoggingLogging)
	 mLogFile << "writing time data" << endl;
       if (thisPdu != NULL && (p->Type() == V1TRAP))
	 mOfile << p->TimeTicks() << mSChar;
       break;
     case GSENDERIP:  
       if (doLoggingLogging)
	 mLogFile << "writing sender IP data" << endl;
       mOfile << p->SenderIP() << mSChar;
       break;
     case GREALIP:  
       if (doLoggingLogging)
	 mLogFile << "writing real IP data" << endl;
       mOfile << p->RealIp() << mSChar;
       break;
     case GSENDEROID: 
       {
	 if (doLoggingLogging)
	   mLogFile << "writing sender OID data" << endl;
	 unsigned char* data = NULL;
	 string asdf;
	 if (TRUE == doTranslation)
	 {
	   bool status = mDb.FindTrap(p->SenderOID(),
				      p->SpecificTrapType());
	   if (true == status)
	   {
	     asdf = mDb.TrapName().c_str();
	     //	     data = (unsigned char*)mDb.TrapName().c_str();
	     data = (unsigned char*)asdf.c_str();
	   }
	   else
	     data = (unsigned char*)(p->SenderOID());
	 }
	 else 
	   data = (unsigned char*)(p->SenderOID());
	 mOfile << data << mSChar;
       }
       break;
     case GVARBINDS:     
       {
	 if (doLoggingLogging)
	   mLogFile << "writing varbind data" << endl;
	 string asdf;
	 int x = p->VbListLength();
	 for (int y = 1; y <= x; y++)
	 {
	   for (int yy = 0; yy < 3; yy++)
	   {
	     unsigned char* data = NULL;
	     if (mOids[yy] == GVBOID)
	     {
	       bool status = false;
	       if (TRUE == doTranslation)
	       {
		 const char* oid  = p->VbOID(y);
		 data = NULL;
		 status = mDb.FindNearestOid(oid);
		 if (true == status)  {
		     asdf = mDb.OidsName();
		     data = (unsigned char*)asdf.c_str();
		 }
		 
		 // didn't find anything
		 if (data == NULL || strlen((const char*)data) == 0)  {
		   data = (unsigned char*)p->VbOID(y);
		   mOfile << data << mSChar;
		 }
		 else  {
		   mOfile << data << mSChar;
		 }
	       }
	       else // not translating oids, just use the oid
	       {
		 data = (unsigned char*)p->VbOID(y);
		 mOfile << data << mSChar;
	       }
	     }
	     else
	     {

	       if (mOids[yy] == GVBTYPE)
	       {
		 mOfile << p->VbType(y) << mSChar;
	       }
	       else
	       {
		 if (mOids[yy] == GVBDATA)
		 {
		   data = (unsigned char*)p->VbData(y);
		   int value = atoi((const char*)data);
		   bool status = mDb.FindEnum(p->VbOID(y), value);
		   std::string ename;
		   if (true == status)
		   {
		     ename = mDb.EnumName();
		   }
		   if (ename.length() > 0)
		     mOfile << ename.c_str() << mSChar;
		   else
		     mOfile << data << mSChar;
		 }
		 else
		   break;
	       }
	     }
	   } // for (int yy = 0; yy < 3; yy++)
	 } // for (int y = 1; y <= x; y++)
	 break;
       }
     default:
       break;
    } // switch (mFString[x])
  } // for (int x = 0; x < 9; x++)
  mOfile << endl << endl;
}


void
FlusherThread::ParseFormatString()
{
  int pos = 0;

  Registry r;
  CString fs;
  try 
  {
    fs = r.formatString();
  }
  catch(...)
  {
    fs = "%v%i%t%o%g%s%bOTD";
    r.formatString(fs);
  }

  int len = fs.GetLength();
  char* t = fs.GetBuffer(0);

  while (len > 0 && pos < 9)
  {
    if (*t == '%')
    {
      t++; len--;
      switch (*t)
      {
        case 'v':
	  mFString[pos++] = GVERSION;
          break;
        case 'c':
	  mFString[pos++] = GCOMMUNITY;
          break;
        case 'g':
	  mFString[pos++] = GGENERIC;
          break;
        case 's':
	  mFString[pos++] = GSPECIFIC;
          break;
        case 't':
	  mFString[pos++] = GTIMETICKS;
          break;
        case 'i':
	  mFString[pos++] = GSENDERIP;
          break;
        case 'r':
	  mFString[pos++] = GREALIP;
          break;
        case 'o':
	  mFString[pos++] = GSENDEROID;
          break;
        case 'b':
          {
	    mFString[pos++] = GVARBINDS;
	    int index = 0;
            while (len > 0 && index < 3)
	    {
              t++; len--;
	      if (*t == '%') // oops, didn't expect that!
	      {
		t--;len--; // back things up and try to finish it up
		break;
	      }
 	      if (*t == 'O')
		mOids[index++] = GVBOID;
	      else
	        if (*t == 'T')
		  mOids[index++] = GVBTYPE;
	        else
	          if (*t == 'D')
		    mOids[index++] = GVBDATA;
	          else
	            break;
            } // while (len > 0 && index < 3)
	    break;
          } // case 'b':
      } // switch(*t++)
      t++; len--;
    }
    else
    {
      t++; len--;
    }
  } // while (len)
}

