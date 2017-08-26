#pragma warning(disable: 4786)

#ifndef __TRDB_H__
#include "TrDb.h"
#endif

#include "stdafx.h"

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>


TrDb::TrDb()
  :mRecordSet(NULL),
   mConnected(false)
{
  char as [1024]; 
  memset(as, 0, 1024);
  GetModuleFileName(NULL, as, 1023); 
  char* last = strrchr(as, '\\');
  if (last != NULL)
    *last = 0;
  std::string filename = as;
  filename += '\\';
  filename += "traprcvr.db";
  Connect(filename);
}

TrDb::~TrDb()
{
  if (true == mConnected)
    mConnection.Close(); 
}

bool       
TrDb::Connect(std::string dbName)
{
  // open already returns true unless something strange happens.
  // see if the file exists, if not, open and then initialize
  bool exists = false;
  FILE* fp = fopen(dbName.c_str(), "r");
  if (NULL != fp)
  {
    exists = true;
    fclose(fp);
  }
      
  bool retVal = false;
  int status = mConnection.Open((char*)dbName.c_str());
  SqliteRecordSet* result = NULL;
  std::string sql;

  if (!status) 
    retVal = true;

  if (exists == false)
  {
    // flusher table
    try {
      sql = "CREATE TABLE flusher (enabled INTEGER, interval INTEGER, logfilename VARCHAR(128), format VARCHAR(128), maxSize INTEGER, overwrite INTEGER)";
      mConnection.Execute(sql.c_str(), result);
      
      sql = "INSERT INTO flusher (enabled, interval, logfilename, format, maxSize, overwrite) VALUES ('1', '15', 'traprcvr.log', '%v%i%t%o%g%s%bOTD', '10', '1')";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }


    // misc table
    try {
      sql = "CREATE TABLE misc (dbVersionMajor INTEGER, dbVersionMinor INTEGER, translateoids INTEGER, listenPort INTEGER, reverseLookup INTEGER, smtpLogging INTEGER, actionLogging INTEGER, doinforms INTEGER, httpport INTEGER, httpuser VARCHAR(64), httppport VARCHAR(64), version VARCHAR(64), port INTEGER, width INTEGER, height INTEGER, xPos INTEGER, yPos INTEGER, synchronized VARCHAR(64), loggingLogging INTEGER, cwd VARCHAR(1024), deduplicate INTEGER, dedupinterval INTEGER)";
      mConnection.Execute(sql.c_str(), result);
      
      sql = "INSERT INTO misc (dbVersionMajor, dbVersionMinor, translateoids, listenPort, reverseLookup, smtpLogging, actionLogging, doinforms, httpport, httpuser, httppport, version, port, width, height, xPos, yPos, deduplicate, dedupinterval) VALUES ('1', '0', '0', '162', '0', '0', '0', '0', '8888', 'admin', 'password', '7.11', 0, 0, 0, 0, '4.22', 0, '', 0, 60)";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }

    // passing trap data
    try {
      sql = "CREATE TABLE trapdata (includeEnvironment INTEGER, includeCommandLine INTEGER, includeEnvCommunity INTEGER, includeEnvIpAddress INTEGER, includeEnvGenericType INTEGER, includeEnvSpecificType INTEGER, includeEnvSenderOID INTEGER, includeEnvVbData INTEGER, includeEnvVbOID INTEGER, envCommunity VARCHAR(64), envIpAddress VARCHAR(64), envGenericType VARCHAR(64), envSpecificType VARCHAR(64), envSenderOID VARCHAR(64), envVbData VARCHAR(64), envVbOID VARCHAR(64), includeCmdCommunity INTEGER, includeCmdIpAddress INTEGER, includeCmdGenericType INTEGER, includeCmdSpecificType INTEGER, includeCmdSenderOID INTEGER, includeCmdVbData INTEGER, includeCmdVbOID INTEGER, cmdCommunity VARCHAR(64), cmdIpAddress VARCHAR(64), cmdGenericType VARCHAR(64), cmdSpecificType VARCHAR(64), cmdSenderOID VARCHAR(64), cmdVbData VARCHAR(64), cmdVbOID VARCHAR(64))";
      mConnection.Execute(sql.c_str(), result);
      
      sql = "INSERT INTO trapdata (includeEnvironment, includeCommandLine, includeEnvCommunity, includeEnvIpAddress, includeEnvGenericType, includeEnvSpecificType, includeEnvSenderOID, includeEnvVbData, includeEnvVbOID, envCommunity, envIpAddress, envGenericType, envSpecificType, envSenderOID, envVbData, envVbOID, includeCmdCommunity, includeCmdIpAddress, includeCmdGenericType, includeCmdSpecificType, includeCmdSenderOID, includeCmdVbData, includeCmdVbOID, cmdCommunity, cmdIpAddress, cmdGenericType, cmdSpecificType, cmdSenderOID, cmdVbData, cmdVbOID) VALUES ('0', '0', '0', '0', '0', '0', '0', '0', '0', '', '', '', '', '', '', '', '0', '0', '0', '0', '0', '0', '0', '', '', '', '', '', '', '')";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }

    // exploder table
    try {
      sql = "CREATE TABLE exploders (id INTEGER AUTO INCREMENT, destination VARCHAR(64), port INTEGER)";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }

    // email table
    try {
      sql = "CREATE TABLE emails (id INTEGER AUTO INCREMENT, smtpHost VARCHAR(64), smtpPort INTEGER, fromAddress VARCHAR(256), toAddress VARCHAR(256), subject VARCHAR(512), message VARCHAR(1024), authNeeded INTEGER, username VARCHAR(64), password VARCHAR(64), useSsl INTEGER)";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
      cout << "caught exeception" << endl;
      cout << e.GetErrDescription() << endl;
    }

    // no need to restructure this - being non-normalized is ok, 
    // otherwise I would have to change a whole lot of other code
    // just mimic the behavior of the code that uses the registry.
    // executable table
    //    try {
    //      sql = "CREATE TABLE executables (id INTEGER AUTO INCREMENT, executable VARCHAR(128), arguments VARCHAR(256))";
    //      mConnection.Execute(sql.c_str(), result);
    //    }
    //    catch (SqliteException e) {
    //    }

    // watches table
    try {
      sql = "CREATE TABLE watches (id INTEGER AUTO INCREMENT, name VARCHAR(64), watch INTEGER, value VARCHAR(256), bools INTEGER, exe VARCHAR(256), args VARCHAR(256), incTrapData INTEGER, emaildests VARCHAR(256), exploderdests VARCHAR(256), soundPlayDuration INTEGER, soundFile VARCHAR(256), partofgroup INTEGER)";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }

    // actions table
    //    try {
    // sql = "CREATE TABLE actions (watchId INTEGER, emailId INTEGER, exploderId INTEGER, executableId INTEGER)";
    //      mConnection.Execute(sql.c_str(), result);
    //    }
    //    catch (SqliteException e) {
    //    }

    // next table
  } // if (exists == false)

  mConnected = true;

  return retVal;
}

int
TrDb::GetDbInteger(string column, string database)
{
  int retVal = -1;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    sql = "select ";
    sql += column;
    sql += " from ";
    sql += database;
    mConnection.Execute(sql.c_str(), result);
    if (result && !result->IsEmpty())
    {
      result->MoveFirst();
      //      string count = result->FieldValue(1);
      string count = result->FieldValue(0);
      retVal = atoi(count.c_str());
    }
  }
  catch (SqliteException e) {
    retVal = -1;
  }
  return retVal;
}

string
TrDb::GetDbString(string column, string database)
{
  string retVal;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    sql = "select ";
    sql += column;
    sql += " from ";
    sql += database;
    mConnection.Execute(sql.c_str(), result);
    if (result && !result->IsEmpty())
    {
      result->MoveFirst();
      //      string count = result->FieldValue(1);
      retVal = result->FieldValue(0);
    }
  }
  catch (SqliteException e) {
  }
  return retVal;
}

bool
TrDb::SetDbInteger(string column, string table, int value)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    sql = "update ";
    sql += table;
    sql += " set ";
    sql += column;
    sql += "=";
    sql += value;

    mConnection.Execute(sql.c_str(), result);
    if (result && !result->IsEmpty())
    {
      retVal = true;
    }
  }
  catch (SqliteException e) { }
  return retVal;
}

bool
TrDb::SetDbString(string column, string table, string value)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;
  try {
    std::string sql;
    sql = "update ";
    sql += table;
    sql += " set ";
    sql += column;
    sql += "='";
    sql += value;
    sql += "'";

    mConnection.Execute(sql.c_str(), result);
    if (result)
    {
      retVal = true;
    }
  }
  catch (SqliteException e) { }
  return retVal;
}

bool                 
TrDb::FlusherLoggingEnabled()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("enabled", "flusher");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void                 
TrDb::FlusherLoggingEnabled(bool val)
{

}


  // flusher info
unsigned int         
TrDb::FlusherInterval()
{
  return (unsigned int) GetDbInteger("interval", "flusher");
}

void                 
TrDb::FlusherInterval(unsigned int val)
{

}

string               
TrDb::FlusherLogFileName()
{
  return GetDbString("logfilename", "flusher");
}

void
TrDb::FlusherLogFileName(string val)
{

}

string
TrDb::FlusherFormat()
{
  return GetDbString("format", "flusher");
}

void
TrDb::FlusherFormat(string val)
{

}

unsigned int
TrDb::FlusherMaxSize()
{
  return (unsigned int) GetDbInteger("maxSize", "flusher");
}

void
TrDb::FlusherMaxSize(unsigned int val)
{

}

bool
TrDb::FlusherOverwrite()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("overwrite", "flusher");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void
TrDb::FlusherOverwrite(bool val)
{

}

 // misc
int                  
TrDb::Width()
{
  return GetDbInteger("width", "misc");
}

void                 
TrDb::Width(int w)
{
}

int                  
TrDb::Height()
{
  return GetDbInteger("height", "misc");
}

void                 
TrDb::Height(int h)
{
}

int                  
TrDb::xPos()
{
  return GetDbInteger("xpos", "misc");
}

void                 
TrDb::xPos(int x)
{
}

int                  
TrDb::yPos()
{
  return GetDbInteger("ypos", "misc");
}

void                 
TrDb::yPos(int y)
{
}

bool                 
TrDb::LoggingLogging()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("logginglogging", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void                 
TrDb::LoggingLogging(bool logging)
{
}

string               
TrDb::Cwd()
{
  return GetDbString("cwd", "misc");
}

void                 
TrDb::Cwd(string directory)
{
}

bool                 
TrDb::Deduplicate()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("deduplicate", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void                 
TrDb::Deduplicate(bool val)
{

}

int                  
TrDb::DedupInterval()
{
  return GetDbInteger("dedupinterval", "misc");
}

void                 
TrDb::DedupInterval(int interval)
{
}

bool                 
TrDb::TranslateOids()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("translateoids", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void                 
TrDb::TranslateOids(bool val)
{

}

int    
TrDb::ListenPort() 
{
  return GetDbInteger("listenPort", "misc");
}

void   
TrDb::ListenPort(int val) 
{

}

bool   
TrDb::ReverseLookup() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("reverseLookup", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::ReverseLookup(bool val) 
{

}

bool   
TrDb::SmtpLogging() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("smtpLogging", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::SmtpLogging(bool val) 
{

}

bool   
TrDb::ActionLogging() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("actionLogging", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::ActionLogging(bool val) 
{

}

bool                 
TrDb::DoInforms()
{
  bool retVal = true;
  int isEnabled = GetDbInteger("doinforms", "misc");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void                 
TrDb::DoInforms(bool val)
{

}

int                  
TrDb::HttpPort()
{
  return (int) GetDbInteger("httpport", "misc");
}

void                 
TrDb::HttpPort(int val)
{

}

string               
TrDb::HttpUsername()
{
  return GetDbString("httpuser", "misc");
}

void                 
TrDb::HttpUsername(string val)
{

}

string               
TrDb::HttpPassword()
{
  return GetDbString("httppport", "misc");
}

void                 
TrDb::HttpPassword(string val)
{

}

///////////////////////////////////
///////////////////////////////////
// dbversion
///////////////////////////////////
///////////////////////////////////

// passing trap data
bool   
TrDb::IncludeEnvironment() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvironment", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvironment(bool val) 
{

}


bool   
TrDb::IncludeCmdLine() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCommandLine", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdLine(bool val) 
{

}

// env
bool   
TrDb::IncludeEnvCommunity() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvCommunity", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvCommunity(bool val) 
{

}

bool   
TrDb::IncludeEnvIpAddress() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvIpAddress", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvIpAddress(bool val) 
{

}

bool   
TrDb::IncludeEnvGenericType() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvGenericType", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvGenericType(bool val) 
{

}

bool   
TrDb::IncludeEnvSpecificType() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvSpecificType", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvSpecificType(bool val) 
{

}

bool   
TrDb::IncludeEnvSenderOID() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvSenderOID", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvSenderOID(bool val) 
{

}

bool   
TrDb::IncludeEnvVbData() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvVbData", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvVbData(bool val) 
{

}

bool   
TrDb::IncludeEnvVbOID() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeEnvVbOID", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeEnvVbOID(bool val) 
{

}


string 
TrDb::EnvCommunity() 
{
  return GetDbString("envCommunity", "trapdata");
}

void   
TrDb::EnvCommunity(string val) 
{

}

string 
TrDb::EnvIpAddress() 
{
  return GetDbString("envIpAddress", "trapdata");
}

void   
TrDb::EnvIpAddress(string val) 
{

}

string 
TrDb::EnvGenericType() 
{
  return GetDbString("envGenericType", "trapdata");
}

void   
TrDb::EnvGenericType(string val) 
{

}

string 
TrDb::EnvSpecificType() 
{
  return GetDbString("envSpecificType", "trapdata");
}

void   
TrDb::EnvSpecificType(string val) 
{

}

string 
TrDb::EnvSenderOID() 
{
  return GetDbString("envSenderOID", "trapdata");
}

void   
TrDb::EnvSenderOID(string val) 
{

}

string 
TrDb::EnvVbData() 
{
  return GetDbString("envVbData", "trapdata");
}

void   
TrDb::EnvVbData(string val) 
{

}

string 
TrDb::EnvVbOID() 
{
  return GetDbString("envVbOID", "trapdata");
}

void   
TrDb::EnvVbOID(string val) 
{

}



    // cmd
bool   
TrDb::IncludeCmdCommunity() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdCommunity", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdCommunity(bool val) 
{

}

bool   
TrDb::IncludeCmdIpAddress() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdIpAddress", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdIpAddress(bool val) 
{

}

bool   
TrDb::IncludeCmdGenericType() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdGenericType", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdGenericType(bool val) 
{

}

bool   
TrDb::IncludeCmdSpecificType() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdSpecificType", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdSpecificType(bool val) 
{

}

bool   
TrDb::IncludeCmdSenderOID() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdSenderOID", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdSenderOID(bool val) 
{

}

bool   
TrDb::IncludeCmdVbData() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdVbData", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdVbData(bool val) 
{

}

bool   
TrDb::IncludeCmdVbOID() 
{
  bool retVal = true;
  int isEnabled = GetDbInteger("includeCmdVbOID", "trapdata");
  if (!isEnabled) 
    retVal = false;
  return retVal;
}

void   
TrDb::IncludeCmdVbOID(bool val) 
{

}


string 
TrDb::CmdCommunity() 
{
  return GetDbString("cmdCommunity", "trapdata");
}

void   
TrDb::CmdCommunity(string val) 
{

}

string 
TrDb::CmdIpAddress() 
{
  return GetDbString("cmdIpAddress", "trapdata");
}

void   
TrDb::CmdIpAddress(string val) 
{

}

string 
TrDb::CmdGenericType() 
{
  return GetDbString("cmdGenericType", "trapdata");
}

void   
TrDb::CmdGenericType(string val) 
{

}

string 
TrDb::CmdSpecificType() 
{
  return GetDbString("cmdSpecificType", "trapdata");
}

void   
TrDb::CmdSpecificType(string val) 
{

}

string 
TrDb::CmdSenderOID() 
{
  return GetDbString("cmdSenderOID", "trapdata");
}

void   
TrDb::CmdSenderOID(string val) 
{

}

string 
TrDb::CmdVbData() 
{
  return GetDbString("cmdVbData", "trapdata");
}

void   
TrDb::CmdVbData(string val) 
{

}

string 
TrDb::CmdVbOID() 
{
  return GetDbString("cmdVbOID", "trapdata");
}

void   
TrDb::CmdVbOID(string val) 
{

}

/////// exploders
string 
TrDb::ExploderDestination() 
{
  return GetDbString("destination", "exploders");
}

int    
TrDb::ExploderPort() 
{
  return GetDbInteger("port", "exploders");
}

int    
TrDb::ExploderId() 
{
  return GetDbInteger("id", "exploders");
}


bool   
TrDb::FindExploder(string exploderdestination,
			int exploderport,
			int& index) 
{
  return false;
}


bool   
TrDb::InsertExploder(string exploderdestination,
		     int     exploderport)
{
  return false;
}

void   
TrDb::DeleteExploder(int which) 
{

}

bool           
TrDb::GetByExploderId(int id)
{
  bool retVal = false;

  try {
    char temp[128];
    sprintf(temp, "select * from exploders where id = %d", id);
    mConnection.Execute(temp, mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();
      retVal = PopulateExploder();
    }      
  } 
  catch (SqliteException e) {
  }
  return retVal;
}

void   
TrDb::DeleteExploders() 
{
  SqliteRecordSet* result = NULL;
  try {
    std::string sql = "delete from exploders";
    mConnection.Execute(sql.c_str(), result);
  }
  catch (SqliteException e) {
  }
}

// go to the next row
bool       
TrDb::NextExploder()
{
  bool retVal = false;

  if (mRecordSet && !mRecordSet->IsEmpty())
    mRecordSet->MoveNext();
  if (false == mRecordSet->EndOfRecordSet)
  {
    retVal = PopulateExploder();
  }
  return retVal;
}

bool	
TrDb::PopulateExploder()
{
  bool retVal = false;
  try {
    string data          = mRecordSet->FieldValue(0); // id
    mExploderId          = atoi(data.c_str());
    mExploderDestination = mRecordSet->FieldValue(1); // destination
    data                 = mRecordSet->FieldValue(2); // port
    mExploderPort        = atoi(data.c_str());
    retVal = true;
  }
  catch (SqliteException e) {
  }
  return retVal;
}
/////// end of exploders

/////// emails
string 
TrDb::SmtpServerHost() 
{
  return mSmtpServerHost;
}

int    
TrDb::SmtpPort() 
{
  return mSmtpPort;
}

string 
TrDb::From() 
{
  return mFrom;
}

string 
TrDb::To() 
{
  return mTo;
}

string 
TrDb::Subject() 
{
  return mSubject;
}

string 
TrDb::Message() 
{
  return mMessage;
}

bool   
TrDb::AuthNeeded() 
{
  return mAuthNeeded;
}

string 
TrDb::AuthUser() 
{
  return mAuthUser;
}

string 
TrDb::AuthJuniper() 
{
  return mAuthJuniper;
}

int    
TrDb::EmailId() 
{
  return mEmailId;
}

bool
TrDb::UseSSL()
{
  return mUseSSL;
}

bool   
TrDb::Find(string serverHost,
	   string smtpFrom,
	   string smtpTo, 
	   string smtpSubject,
	   int& index) 
{
  return false;
}


bool   
TrDb::InsertEmail(string serverHost,
		  int port,
		  string smtpFrom,
		  string smtpTo,
		  string smtpSubject,
		  string smtpMessage,
		  bool useAuth,
		  string authuser,
		  string authjuniper,
		  int     id) 
{
  return false;
}


void   
TrDb::DeleteEmail(int which) 
{

}

void   
TrDb::DeleteEmails() 
{
  SqliteRecordSet* result = NULL;
  try {
    std::string sql = "delete from emails";
    mConnection.Execute(sql.c_str(), result);
  }
  catch (SqliteException e) {
  }
}

// go to the next row
bool       
TrDb::NextEmail()
{
  bool retVal = false;

  if (mRecordSet && !mRecordSet->IsEmpty())
    mRecordSet->MoveNext();
  if (false == mRecordSet->EndOfRecordSet)
  {
    retVal = PopulateEmail();
  }
  return retVal;
}

bool	
TrDb::PopulateEmail()
{
  bool retVal = false;
  try {
    string data          = mRecordSet->FieldValue(0); // id
    mEmailId             = atoi(data.c_str());
    mSmtpServerHost      = mRecordSet->FieldValue(1); // smtpHost
    data                 = mRecordSet->FieldValue(2); // smtpPort
    mSmtpPort            = atoi(data.c_str());
    mFrom                = mRecordSet->FieldValue(3); // from
    mTo                  = mRecordSet->FieldValue(4); // to
    mSubject             = mRecordSet->FieldValue(5); // subject
    mMessage             = mRecordSet->FieldValue(6); // message
    data                 = mRecordSet->FieldValue(7); // authNeeded
    int numData          = atoi(data.c_str());
    if (0 == numData)
      mAuthNeeded = false;
    else
      mAuthNeeded = true;
    mAuthUser            = mRecordSet->FieldValue(8); // username
    mAuthJuniper         = mRecordSet->FieldValue(9); // password 
    retVal = true;
  }
  catch (SqliteException e) {
  }
  return retVal;
}
/////// end of emails


/////// executables
int            
TrDb::ExecutableId()
{
  return mExecutableId;
}

string         
TrDb::Executable()
{
  return mExecutable;
}

string         
TrDb::Arguments()
{
  return mArguments;
}

bool           
TrDb::InsertExecutable(string executable, string arguments)
{
  return false;
}

void           
TrDb::DeleteExecutable(int which)
{

}

void           
TrDb::DeleteExecutables()
{
  SqliteRecordSet* result = NULL;
  try {
    std::string sql = "delete from executables";
    mConnection.Execute(sql.c_str(), result);
  }
  catch (SqliteException e) {
  }
}

bool           
TrDb::GetByExecutableId(int id)
{
  bool retVal = false;

  try {
    char temp[128];
    sprintf(temp, "select * from executables where id = %d", id);
    mConnection.Execute(temp, mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();
      retVal = PopulateExecutable();
    }      
  } 
  catch (SqliteException e) {
  }
  return retVal;
}

bool                 
TrDb::PopulateExecutable()
{
  bool retVal = false;
  try {
    string data          = mRecordSet->FieldValue(0); // id
    mExecutableId        = atoi(data.c_str());
    mExecutable          = mRecordSet->FieldValue(1); // executable
    mArguments           = mRecordSet->FieldValue(2); // arguments
    retVal = true;
  }
  catch (SqliteException e) {
  }
  return retVal;
}

bool               
TrDb::NextExecutable()
{
  bool retVal = false;

  if (mRecordSet && !mRecordSet->IsEmpty())
    mRecordSet->MoveNext();
  if (false == mRecordSet->EndOfRecordSet)
  {
    retVal = PopulateExecutable();
  }
  return retVal;
}

/////// end of executables

/////// watches
int
TrDb::WatchId()
{
  return mWatchId;
}

string 
TrDb::Name() 
{
  return mName;
}

int    
TrDb::Watch() 
{
  return mWatch;
}

string 
TrDb::Value() 
{
  return mValue;
}

int    
TrDb::Bools() 
{
  return mBools;
}

bool   
TrDb::IncludeTrapData() 
{
  return mIncludeTrapData;
}

bool
TrDb::PartOfGroup()
{
  return mPartOfGroup;
}

int    
TrDb::SoundPlayDuration() 
{
  return mSoundPlayDuration;
}

string 
TrDb::SoundFile() 
{
  return mSoundFile;
}

string         
TrDb::EmailRecipients()
{
  return mEmailRecipientsList;
}

string         
TrDb::ExploderDestinations()
{
  return mExploderDestinations;
}

bool           
TrDb::GetWatches()
{ // returns true if there are watches configured.  Access watch data using getters

  bool retVal = false;

  try {
    string sql = "select * from watches";
    mConnection.Execute(sql.c_str(), mRecordSet);
    if (mRecordSet && !mRecordSet->IsEmpty())
    {
      mRecordSet->MoveFirst();
      retVal = PopulateWatch();
    }      
  } 
  catch (SqliteException e) {
  }
  return retVal;
}

bool   
TrDb::Find(int inWatch, 
	   string inValue, 
	   int& index) 
{
  return false;
}


bool   
TrDb::FindWatchByName(string inName) 
{
  return false;
}


bool   
TrDb::InsertWatch(string inName,
		  int    inWatch,
		  string inValue,
		  int    inBools,
		  string exe,
		  string args,
		  bool   inTrapData,
		  string emaildests,
		  string exploderdests,
		  int    soundduration,
		  string soundfile,
		  bool   partOfGroup)
{
  return false;
}


void   
TrDb::DeleteWatch(int which) 
{

}

void   
TrDb::DeleteWatches() 
{
  SqliteRecordSet* result = NULL;
  try {
    std::string sql = "delete from watches";
    mConnection.Execute(sql.c_str(), result);
  }
  catch (SqliteException e) {
  }
}

// go to the next row
bool       
TrDb::NextWatch()
{
  bool retVal = false;

  if (mRecordSet && !mRecordSet->IsEmpty())
    mRecordSet->MoveNext();
  if (false == mRecordSet->EndOfRecordSet)
  {
    retVal = PopulateWatch();
  }
  return retVal;
}

bool	
TrDb::PopulateWatch()
{
  // watches
  string               mSoundFile;
  bool                 PopulateAction();

  bool retVal = false;
  try {
    int numData;
    string data           = mRecordSet->FieldValue(0); // id
    mWatchId              = atoi(data.c_str());
    mName                 = mRecordSet->FieldValue(1); // name
    data                  = mRecordSet->FieldValue(2); // watch
    mWatch                = atoi(data.c_str());
    mValue                = mRecordSet->FieldValue(3); // value
    data                  = mRecordSet->FieldValue(4); // bools
    mExe                  = mRecordSet->FieldValue(5); // exe
    mArgs                 = mRecordSet->FieldValue(6); // args
    mBools = atoi(data.c_str());
    data                  = mRecordSet->FieldValue(7); // include trap data

    numData               = atoi(data.c_str());
    mIncludeTrapData = true;
    if (0 == numData)
      mIncludeTrapData = false;

    mEmailRecipientsList  = mRecordSet->FieldValue(8);
    mExploderDests        = mRecordSet->FieldValue(9);
    data                  = mRecordSet->FieldValue(10); // sound duration
    mSoundPlayDuration    = atoi(data.c_str());
    mSoundFile            = mRecordSet->FieldValue(11); // sound file
    data                  = mRecordSet->FieldValue(12);
    
    numData               = atoi(data.c_str());
    mPartOfGroup          = true;
    if (0 == numData)
      mPartOfGroup = false;

    mExploderDestinations = mRecordSet->FieldValue(13);

    retVal = true;
  }
  catch (SqliteException e) {
  }
  return retVal;
}
/////// end of actions

/////// watch/action mappings
std::vector<watchActionMapping> 
TrDb::GetAllActions(int watchId)
{
  std::vector<watchActionMapping> retVal;
  bool status = false;

  SqliteRecordSet* result = NULL;

  try {
    char temp[128];
    sprintf(temp, "select * from actions where watchId = '%d'", watchId);
    std::string sql = temp;
    mConnection.Execute(sql.c_str(), result);
  }
  catch (SqliteException e) {
  }

  // have to move to the first one
  if (result && !result->IsEmpty())
  {
    result->MoveFirst();
    status = true;
  }

  watchActionMapping wap;
  wap.emailId = -1;
  wap.exploderId = -1;
  wap.executableId = -1;

  while (status == true)
  {
    string data               = result->FieldValue(1); // emailId
    wap.emailId               = atoi(data.c_str());
    data                      = result->FieldValue(2); // exploderId
    wap.exploderId            = atoi(data.c_str());
    data                      = result->FieldValue(3); // executableId
    wap.executableId          = atoi(data.c_str());

    retVal.push_back(wap);

    result->MoveNext();
    if (true == result->EndOfRecordSet)
      status = false;

  }
  
  return retVal;
}
/////// end of watch/action mappings

