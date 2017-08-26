//#pragma warning(disable: 4786)

#include "..\stdafx.h"

#ifndef __TRDEDUP_H__
#include "TrDeDup.h"
#endif

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>




TrDeDup::TrDeDup()
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
  filename += "trdedup.db";
  Connect(filename);
}

TrDeDup::~TrDeDup()
{
  if (true == mConnected)
    mConnection.Close(); 
}

bool       
TrDeDup::Connect(std::string dbName)
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
    try {
      sql = "CREATE TABLE dd (ip VARCHAR(128), oid VARCHAR(512), last datetime)";
      mConnection.Execute(sql.c_str(), result);
    }
    catch (SqliteException e) {
    }
  } // if (exists == false)
  //  else { // always start with an empty database
  //    try {
  //      sql = "delete from dd";
  //      mConnection.Execute(sql.c_str(), result);
  //    }
  //    catch (SqliteException e) {
  //    }
  //  }
  mConnected = true;

  return retVal;
}

bool                 
TrDeDup::IsDuplicate(std::string source, std::string oid, int interval)
{
  bool retVal = false;
  SqliteRecordSet* result = NULL;

  stringstream ss;
  ss << interval;
  
  // first, prune
  string pruneString = "delete from dd where (select datetime(last, '+";
  pruneString += ss.str();
  pruneString += " minutes') as ll where datetime('now') > ll)";
  try {
    mConnection.Execute(pruneString.c_str(), result);
  }
  catch (SqliteException e) {
    cout << "caught prune exeception" << endl;
    cout << e.GetErrDescription() << endl;
  }


  // now the query to see if it is a duplicate
  string query = "select datetime(last, '+";
  query += ss.str();
  query += " minutes') as ll, datetime('now') as nn, oid, ip from dd where nn < ll and ip = '";
  query += source;
  query += "' and oid = '";
  query += oid;
  query += "'";
  try {
    mConnection.Execute(query.c_str(), result);
    if (result && !result->IsEmpty()) {
      retVal = true;
    }
  }
  catch (SqliteException e) {
    cout << "query is <" << query << '>' << endl;
    cout << "caught select exeception" << endl;
    cout << e.GetErrDescription() << endl;
  }

  // if not a duplicate, insert this one:
  if (false == retVal) {
    string insert = "insert into dd (ip, oid, last) values ('";
    insert += source;
    insert += "', '";
    insert += oid;
    insert += "', datetime('now'))";
    try {
      mConnection.Execute(insert.c_str(), result);
    }
    catch (SqliteException e) {
      cout << "insert is <" << insert << '>' << endl;
      cout << "caught insert exeception" << endl;
      cout << e.GetErrDescription() << endl;
    }
  }

  return retVal;
}

