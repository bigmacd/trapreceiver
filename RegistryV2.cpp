//  Copyright (c) 2013 TrapReceiver.com
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


#ifndef __REGISTRYV2_H__
#include "RegistryV2.h"
#endif

#include "errno.h"

Registry::Registry(char* subkey)
         :mNewKey(NULL),
	  mKey(NULL),
	  mCreateFlag(FALSE)
{
  ReOpen(subkey);
}

Registry::Registry(char* subkey, int which)
         :mNewKey(NULL),
	  mKey(NULL),
	  mCreateFlag(FALSE)
{
  ReOpen(subkey);
  ReOpen(which);
}

Registry::~Registry()
{
  if (mNewKey)
    delete [] mNewKey;
  RegCloseKey(mKey);
}

//    CString cValue;
//    cValue = "TrapRcvr.log";
//    filename(cValue);
//    port(162);
//    flushInterval(15);
//    overWrite(TRUE);
//    logging(FALSE);
//    cValue = "%v%i%t%o%g%s%bOTD";
//    formatString(cValue);
//    reverseLookup(FALSE);
//    informs(FALSE);
//    filesize(10);
//    trayIcon(FALSE);
//    version(versionString());
//    pingTimeout(1000);
//    translateOids(FALSE);
//    includeTrapData(FALSE);
//    includeEnvironment(FALSE);
//    includeCmdLine(FALSE);
//    smtpLogging(FALSE);

//    char as [256]; 
//    GetModuleFileName(NULL, as, 255); 
//    char* last = strrchr(as, '\\');
//    if (last != NULL)
//      *last = 0;
//    cwd(as);

}

void
Registry::ReOpen(char* which, BOOL add)
{  
  // what is this one?  do an order by id and insert if not found?  prolly.
}

void
Registry::ReOpen(int which)
{
  // do an order by id?  prolly.
}

// misc

CString
Registry::version()
{
  CString val = mDb.VersionString().c_str();
  return val;
}

void           
Registry::version(const char* Inversion)
{
}

int        
Registry::port()
{
  return mDb.ListenPort();
}

void           
Registry::port(int portNumber)
{
  mDb.ListenPort(portNumber);
}

BOOL           
Registry::translateOids()
{
  BOOL retVal = FALSE;
  bool x = mDb.TranslateOids();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::translateOids(BOOL xlat)
{
  bool val = false;
  if (TRUE == xlat)
    val = true;
  mDb.TranslateOids(val);
}

int            
Registry::width()
{
  return mDb.Width();
}

void           
Registry::width(int w)
{
  mDb.Width(w);
}

int            
Registry::height()
{
  return mDb.Height();
}

void           
Registry::height(int h)
{
  mDb.Height(h);
}

int            
Registry::xPos()
{
  return mDb.xPos();
}

void           
Registry::xPos(int x)
{
  mDb.xPos(x);
}

int            
Registry::yPos()
{
  return mDb.yPos();
}

void           
Registry::yPos(int y)
{
  mDb.yPos(y);
}

CString        
Registry::synchronized()
{
  CString val = ""; // mDb.Synchronized().c_str();
  return val;
}

void           
Registry::synchronized(CString version)
{
  // do nothing
}

BOOL           
Registry::smtpLogging()
{
  BOOL retVal = FALSE;
  bool x = mDb.SmtpLogging();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::smtpLogging(BOOL logging)
{
  bool val = false;
  if (TRUE == logging)
    val = true;
  mDb.SmtpLogging(val);
}

BOOL           
Registry::actionLogging()
{
  BOOL retVal = FALSE;
  bool x = mDb.ActionLogging();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::actionLogging(BOOL logging)
{
  bool val = false;
  if (TRUE == logging)
    val = true;
  mDb.ActionLogging(val);
}

BOOL           
Registry::loggingLogging()
{
  BOOL retVal = FALSE;
  bool x = mDb.LoggingLogging();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::loggingLogging(BOOL logging)
{
  bool val = false;
  if (TRUE == logging)
    val = true;
  mDb.LoggingLogging(val);
}

CString        
Registry::cwd()
{
  CString val = mDb.Cwd().c_str();
  return val;
}

void           
Registry::cwd(CString directory)
{
  string val = directory.GetBuffer(0);
  mDb.Cwd(val);
}

BOOL           
Registry::deduplicate()
{
  BOOL retVal = FALSE;
  bool x = mDb.Deduplicate();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::deduplicate(BOOL val)
{
  bool temp = false;
  if (TRUE == val)
    temp = true;
  mDb.Deduplicate(temp);
}

int            
Registry::dedupinterval()
{
  int retVal = 0;
  unsigned int x = mDb.DedupInterval();
  retVal = (int)x;
  return retVal;
}

void           
Registry::dedupinterval(int val)
{
}

BOOL           
Registry::reverseLookup()
{
  BOOL retVal = FALSE;
  bool x = mDb.ReverseLookup();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::reverseLookup(BOOL rev)
{
  bool val = false;
  if (TRUE == rev)
    val = true;
  mDb.ReverseLookup(val);
}

BOOL           
Registry::informs()
{
  BOOL retVal = FALSE;
  bool x = mDb.DoInforms();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::informs(BOOL inf)
{
  bool val = false;
  if (TRUE == inf)
    val = true;
  mDb.DoInforms(val);
}

// logging

BOOL
Registry::overWrite()
{
  BOOL retVal = FALSE;
  bool x = mDb.FlusherOverwrite();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::overWrite(BOOL overwrite)
{
  bool val = false;
  if (TRUE == overwrite)
    val = true;
  mDb.FlusherOverwrite(val);
}

BOOL
Registry::logging()
{
  BOOL retVal = FALSE;
  bool x = mDb.FlusherLoggingEnabled();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::logging(BOOL log)
{
  bool val = false;
  if (TRUE == log)
    val = true;
  mDb.FlusherLoggingEnabled(val);
}

CString        
Registry::formatString()
{
  string val = mDb.FlusherFormat();
  CString retVal = val.c_str();
  return retVal;
}

void           
Registry::formatString(CString fs)
{
  string val = fs.GetBuffer(0);
  mDb.FlusherFormat(val);
}

int        
Registry::flushInterval()
{
  int retVal = 0;
  unsigned int x = mDb.FlusherInterval();
  retVal = (int)x;
  return retVal;
}

void           
Registry::flushInterval(int interval)
{
  mDb.FlusherInterval((unsigned int)interval);
}


int        
Registry::filesize()
{
  int retVal = 0;
  unsigned int x = mDb.FlusherMaxSize();
  retVal = (int)x;
  return retVal;
}

void           
Registry::filesize(int size)
{
  mDb.FlusherMaxSize((unsigned int)size);
}

CString        
Registry::filename()
{
  CString retVal = mDb.FlusherLogFileName().c_str();
  return retVal;
}

void           
Registry::filename(CString fileName)
{
  string val = fileName.GetBuffer(0);
  mDb.FlusherLogFileName(val);
}

// actions
// change for groups in 7.47
CString        
Registry::name()
{
  CString retVal = mDb.Name().c_str();
  return retVal;
}

//void           
//Registry::name(CString inName)
//{
//  string val = inName.GetBuffer(0);
//  mDb.Name(val);
//}

int
Registry::watch()
{
  int retVal = 0;
  unsigned int x = mDb.Watch();
  retVal = (int)x;
  return retVal;
}

//void           
//Registry::watch(int inWatch)
//{
//  mDb.Watch((unsigned int)size);
//}

CString        
Registry::value()
{
  CString retVal = mDb.Value().c_str();
  return retVal;
}

//void           
//Registry::value(CString inValue)
//{
//  string val = inValue.GetBuffer(0);
//  mDb.Value(val);
//}

int            
Registry::bools()
{
  int retVal = 0;
  unsigned int x = mDb.Bools();
  retVal = (int)x;
  return retVal;
}

//void           
//Registry::bools(int inBools)
//{
//  mDb.Bools((unsigned int)size);
//}

CString        
Registry::executable()
{
  CString retVal = mDb.Executable().c_str();
  return retVal;
}

//void           
//Registry::executable(CString exe)
//{
//  string val = exe.GetBuffer(0);
//  mDb.Executable(val);
//}

BOOL           
Registry::includeTrapData()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeTrapData();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

BOOL           
Registry::partOfGroup()
{
  BOOL retVal = FALSE;
  bool x = mDb.PartOfGroup();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

//void           
//Registry::includeTrapData(BOOL doInclude)
//{
//  bool val = false;
//  if (TRUE == doInclude)
//    val = true;
//  mDb.IncludeTrapData(val);
//}

CString        
Registry::arguments()
{
  CString retVal = mDb.Arguments().c_str();
  return retVal;
}

//void           
//Registry::arguments(CString args)
//{
//  string val = args.GetBuffer(0);
//  mDb.Arguments(val);
//}

BOOL           
Registry::includeEnvironment()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvironment();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvironment(BOOL doInclude)
{
  bool val = false;
  if (TRUE == doInclude)
    val = true;
  mDb.IncludeEnvironment(val);
}

BOOL           
Registry::includeCmdLine()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdLine();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdLine(BOOL doInclude)
{
  bool val = false;
  if (TRUE == doInclude)
    val = true;
  mDb.IncludeCmdLine(val);
}

CString        
Registry::emailRecipients()
{
  CString retVal = mDb.EmailRecipients().c_str();
  return retVal;
}

//void           
//Registry::emailRecipients(CString dest)
//{
//  string val = dest.GetBuffer(0);
//  mDb.EmailRecipients(val);
//}

int            
Registry::soundPlayDuration()
{
  int retVal = 0;
  unsigned int x = mDb.SoundPlayDuration();
  retVal = (int)x;
  return retVal;
}

//void           
//Registry::soundPlayDuration(int seconds)
//{
//  mDb.SoundPlayDuration((unsigned int)seconds);
//}
  
CString        
Registry::soundFile()
{
  CString retVal = mDb.SoundFile().c_str();
  return retVal;
}

//void           
//Registry::soundFile(CString filename)
//{
//  string val = filename.GetBuffer(0);
//  mDb.SoundFile(val);
//}

CString        
Registry::exploderDestinations()
{
  CString retVal = mDb.ExploderDestinations().c_str();
  return retVal;
}

//void           
//Registry::exploderDestinations(CString dest)
//{
//  string val = dest.GetBuffer(0);
//  mDb.ExploderDestinations(val);
//}

BOOL           
// change for groups in 7.47
Registry::insertAction(CString inName,
		       int     inWatch,
		       CString inValue,
		       int     inBools,
		       CString inExe,
		       CString inArgs,
		       BOOL    inTrapData,
		       CString inEmailDests,
		       CString inExploderDests,
		       int     soundduration,
		       CString inSoundfile,
		       BOOL    inPartOfGroup)
{
  string name = inName.GetBuffer(0);
  string value = inValue.GetBuffer(0);
  string exe = inExe.GetBuffer(0);
  string args = inArgs.GetBuffer(0);
  string emaildests = inEmailDests.GetBuffer(0);
  string exploderdests = inExploderDests.GetBuffer(0);
  string soundfile = inSoundfile.GetBuffer(0);
  bool trapdata = false;
  if (TRUE == inTrapData)
    trapdata = true;

  bool partofgroup = false;
  if (TRUE == inPartOfGroup)
    partofgroup = true;

  bool retVal = mDb.InsertWatch(name,
				inWatch,
				value,
				inBools,
				exe,
				args,
				trapdata,
				emaildests,
				exploderdests,
				soundduration,
				soundfile,
				partofgroup);
  return retVal;
}

//void           
//Registry::deleteAction()
//{
  // not needed
//}

void           
Registry::deleteActions()
{
  mDb.DeleteWatches();
}


// exploders

CString        
Registry::exploderDestination()
{
  CString val = mDb.ExploderDestination().c_str();
  return val;
}

int        
Registry::exploderPort()
{
  int retVal = 0;
  unsigned int x = mDb.ExploderPort();
  retVal = (int)x;
  return retVal;
}

int            
Registry::exploderId()
{
  int retVal = 0;
  unsigned int x = mDb.ExploderId();
  retVal = (int)x;
  return retVal;
}

BOOL
Registry::insertExploder(CString exploderdestination,
			 int     exploderport,
			 int     id)
{
  return TRUE;
}

//void
//Registry::deleteExploder(int which)
//{
//
//}

void
Registry::deleteExploders()
{
  mDb.DeleteExploders();
}


// email destinations
CString        
Registry::smtpServerHost()
{
  CString val = mDb.SmtpServerHost().c_str();
  return val;
}

int            
Registry::smtpPort()
{
  int retVal = 0;
  unsigned int x = mDb.SmtpPort();
  retVal = (int)x;
  return retVal;
}

CString       
Registry::from()
{
  CString val = mDb.From().c_str();
  return val;
}

CString        
Registry::to()
{
  CString val = mDb.To().c_str();
  return val;
}

CString        
Registry::subject()
{
  CString val = mDb.Subject().c_str();
  return val;
}

CString        
Registry::message()
{
  CString val = mDb.Message().c_str();
  return val;
}

BOOL           
Registry::authNeeded()
{
  BOOL retVal = FALSE;
  bool x = mDb.AuthNeeded();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

CString        
Registry::authUser()
{
  CString val = mDb.AuthUser().c_str();
  return val;
}

CString        
Registry::authJuniper()
{
  CString val = mDb.AuthJuniper().c_str();
  return val;
}

int            
Registry::emailId()
{
  int retVal = 0;
  unsigned int x = mDb.EmailId();
  retVal = (int)x;
  return retVal;
}

BOOL
Registry::useSSL()
{
  BOOL retVal = FALSE;
  bool x = mDb.UseSSL();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

BOOL
Registry::insertEmail(CString serverHost,
		      int port,
		      CString smtpFrom,
		      CString smtpTo,
		      CString smtpSubject,
		      CString smtpMessage,
		      BOOL useAuth,
		      CString authuser,
		      CString authjuniper,
		      int id,
			  BOOL useSsl)
{
  return FALSE;
}

//void
//Registry::deleteEmail(int which)
//{
//
//}

void
Registry::deleteEmails()
{
  mDb.DeleteEmails();
}

unsigned long
Registry::getCount()
{
  return 0;
}

BOOL           
Registry::find(int     inWatch, 
	       CString inValue, 
	       int& index)
{
  return FALSE;
}

BOOL           
Registry::findWatchByName(CString inName)
{
  return FALSE;
}


BOOL           
Registry::find(CString exploderdestination,
	       int exploderport,
	       int& index)
{
  return FALSE;
}
 
BOOL           
Registry::find(CString serverHost,
	       CString smtpFrom,
	       CString smtpTo, 
	       CString smtpSubject,
	       int& index)
{
  return FALSE;
}

CString
Registry::IndexToName(int which)
{
  CString retval;
  return retval;
}

// env
//sql = "CREATE TABLE trapdata (includeEnvironment INTEGER, includeCommandLine INTEGER, includeEnvCommunity INTEGER, includeEnvIpAddress INTEGER, includeEnvGenericType INTEGER, includeEnvSpecificType INTEGER, includeEnvSenderOID INTEGER, includeEnvVbData INTEGER, includeEnvVbOID INTEGER, envCommunity VARCHAR(64), envIpAddress VARCHAR(64), envGenericType VARCHAR(64), envSpecificType VARCHAR(64), envSenderOID VARCHAR(64), envVbData VARCHAR(64), envVbOID VARCHAR(64), includeCmdCommunity INTEGER, includeCmdIpAddress INTEGER, includeCmdGenericType INTEGER, includeCmdSpecificType INTEGER, includeCmdSenderOID INTEGER, includeCmdVbData INTEGER, includeCmdVbOID INTEGER, cmdCommunity VARCHAR(64), cmdIpAddress VARCHAR(64), cmdGenericType VARCHAR(64), cmdSpecificType VARCHAR(64), cmdSenderOID VARCHAR(64), cmdVbData VARCHAR(64), cmdVbOID VARCHAR(64))";
BOOL           
Registry::includeEnvCommunity()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvCommunity();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvCommunity(BOOL community)
{
  bool val = false;
  if (TRUE == community)
    val = true;
  mDb.IncludeEnvCommunity(val);
}

BOOL           
Registry::includeEnvIpAddress()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvIpAddress();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvIpAddress(BOOL ipaddress)
{
  bool val = false;
  if (TRUE == ipaddress)
    val = true;
  mDb.IncludeEnvIpAddress(val);
}

BOOL           
Registry::includeEnvGenericType()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvGenericType();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvGenericType(BOOL generictype)
{
  bool val = false;
  if (TRUE == generictype)
    val = true;
  mDb.IncludeEnvGenericType(val);
}

BOOL           
Registry::includeEnvSpecificType()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvSpecificType();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvSpecificType(BOOL specifictype)
{
  bool val = false;
  if (TRUE == specifictype)
    val = true;
  mDb.IncludeEnvSpecificType(val);
}

BOOL           
Registry::includeEnvSenderOID()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvSenderOID();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvSenderOID(BOOL senderoid)
{
  bool val = false;
  if (TRUE == senderoid)
    val = true;
  mDb.IncludeEnvSenderOID(val);
}

BOOL           
Registry::includeEnvVbData()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvVbData();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvVbData(BOOL vbdata)
{
  bool val = false;
  if (TRUE == vbdata)
    val = true;
  mDb.IncludeEnvVbData(val);
}

BOOL           
Registry::includeEnvVbOID()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeEnvVbOID();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeEnvVbOID(BOOL vboid)
{
  bool val = false;
  if (TRUE == vboid)
    val = true;
  mDb.IncludeEnvVbOID(val);
}


CString        
Registry::envCommunity()
{
  CString val = mDb.EnvCommunity().c_str();
  return val;
}

void           
Registry::envCommunity(CString community)
{
}

CString        
Registry::envIpAddress()
{
  CString val = mDb.EnvIpAddress().c_str();
  return val;
}

void           
Registry::envIpAddress(CString ipaddress)
{
}

CString        
Registry::envGenericType()
{
  CString val = mDb.EnvGenericType().c_str();
  return val;
}

void           
Registry::envGenericType(CString generictype)
{
}

CString        
Registry::envSpecificType()
{
  CString val = mDb.EnvSpecificType().c_str();
  return val;
}

void           
Registry::envSpecificType(CString specifictype)
{
}

CString        
Registry::envSenderOID()
{
  CString val = mDb.EnvSenderOID().c_str();
  return val;
}

void           
Registry::envSenderOID(CString senderoid)
{
}

CString        
Registry::envVbData()
{
  CString val = mDb.EnvVbData().c_str();
  return val;
}

void           
Registry::envVbData(CString vbdata)
{
}

CString        
Registry::envVbOID()
{
  CString val = mDb.EnvVbOID().c_str();
  return val;
}

void           
Registry::envVbOID(CString vboid)
{
}

// cmd
BOOL           
Registry::includeCmdCommunity()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdCommunity();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdCommunity(BOOL community)
{

  bool val = false;
  if (TRUE == community)
    val = true;
  mDb.IncludeCmdCommunity(val);
}

BOOL           
Registry::includeCmdIpAddress()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdIpAddress();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdIpAddress(BOOL ipaddress)
{
  bool val = false;
  if (TRUE == ipaddress)
    val = true;
  mDb.IncludeCmdIpAddress(val);
}

BOOL           
Registry::includeCmdGenericType()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdGenericType();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdGenericType(BOOL generictype)
{
  bool val = false;
  if (TRUE == generictype)
    val = true;
  mDb.IncludeCmdGenericType(val);
}

BOOL           
Registry::includeCmdSpecificType()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdSpecificType();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdSpecificType(BOOL specifictype)
{
  bool val = false;
  if (TRUE == specifictype)
    val = true;
  mDb.IncludeCmdSpecificType(val);
}

BOOL           
Registry::includeCmdSenderOID()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdSenderOID();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdSenderOID(BOOL senderoid)
{
  bool val = false;
  if (TRUE == senderoid)
    val = true;
  mDb.IncludeCmdSenderOID(val);
}

BOOL           
Registry::includeCmdVbData()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdVbData();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdVbData(BOOL vbdata)
{
  bool val = false;
  if (TRUE == vbdata)
    val = true;
  mDb.IncludeCmdVbData(val);
}

BOOL           
Registry::includeCmdVbOID()
{
  BOOL retVal = FALSE;
  bool x = mDb.IncludeCmdVbOID();
  if (true == x)
    retVal = TRUE;
  return retVal;
}

void           
Registry::includeCmdVbOID(BOOL vboid)
{
  bool val = false;
  if (TRUE == vboid)
    val = true;
  mDb.IncludeCmdVbOID(val);
}


CString        
Registry::cmdCommunity()
{
  CString val = mDb.CmdCommunity().c_str();
  return val;
}

void           
Registry::cmdCommunity(CString community)
{
}

CString        
Registry::cmdIpAddress()
{
  CString val = mDb.CmdIpAddress().c_str();
  return val;
}

void           
Registry::cmdIpAddress(CString ipaddress)
{
}

CString        
Registry::cmdGenericType()
{
  CString val = mDb.CmdGenericType().c_str();
  return val;
}

void           
Registry::cmdGenericType(CString generictype)
{
}

CString        
Registry::cmdSpecificType()
{
  CString val = mDb.CmdSpecificType().c_str();
  return val;
}

void           
Registry::cmdSpecificType(CString specifictype)
{
}

CString        
Registry::cmdSenderOID()
{
  CString val = mDb.CmdSenderOID().c_str();
  return val;
}

void           
Registry::cmdSenderOID(CString senderoid)
{
}

CString        
Registry::cmdVbData()
{
  CString val = mDb.CmdVbData().c_str();
  return val;
}

void           
Registry::cmdVbData(CString vbdata)
{
}

CString        
Registry::cmdVbOID()
{
  CString val = mDb.CmdVbOID().c_str();
  return val;
}

void           
Registry::cmdVbOID(CString vboid)
{
}
 
