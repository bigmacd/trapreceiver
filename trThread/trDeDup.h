//  Copyright (c) 2010 TrapReceiver.com
//  All rights reserved.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef __TRDEDUP_H__
#define __TRDEDUP_H__

#include "SqliteConnection.h"
#include "SqliteRecordSet.h"

using namespace std;
using namespace SqliteClass;
#include <string>
#include <sstream>

class TrDeDup
{

  private:

    bool                 mConnected;
    SqliteConnection     mConnection;
    SqliteRecordSet*     mRecordSet;

    char*                mDbFile;


  protected:



  public:

    TrDeDup();
    ~TrDeDup();

    bool                 Connect(std::string dbName);

    bool                 IsDuplicate(std::string source, std::string oid, int interval);

};

#endif
