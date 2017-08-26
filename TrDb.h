//  Copyright (c) 2012 TrapReceiver.com
//  All rights reserved.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef __TRDB_H__
#define __TRDB_H__

#include "SqliteConnection.h"
#include "SqliteRecordSet.h"

using namespace std;
using namespace SqliteClass;
#include <string>
#include <vector>

typedef struct watchActionMapping_
{
  int emailId;
  int exploderId;
  int executableId;
} watchActionMapping;


class TrDb
{

  private:

    bool                 mConnected;
    SqliteConnection     mConnection;
    SqliteRecordSet*     mRecordSet;

    char*                mDbFile;


    string               GetDbString(string column, string database);
    bool                 SetDbString(string column, string table, string value);

    int                  GetDbInteger(string column, string database);
    bool                 SetDbInteger(string column, string table, int value);

    // exploders
    string               mExploderDestination;
    int                  mExploderPort;
    int                  mExploderId;
    bool                 PopulateExploder();
    // email 
    string               mSmtpServerHost;
    int                  mSmtpPort;
    string               mFrom;
    string               mTo;
    string               mSubject;
    string               mMessage;
    bool                 mAuthNeeded;
    string               mAuthUser;
    string               mAuthJuniper;
    int                  mEmailId;
    bool                 mUseSSL;
    bool                 PopulateEmail();
    // executables
    int                  mExecutableId;
    string               mExecutable;
    string               mArguments;
    bool                 PopulateExecutable();
    // watches
    int                  mWatchId;
    string               mName;
    int                  mWatch;
    string               mValue;
    int                  mBools;
    bool                 mIncludeTrapData;
    int                  mSoundPlayDuration;
    string               mSoundFile;
    bool                 PopulateWatch();
    string               mEmailRecipientsList;
    string               mExploderDests;
    bool                 mPartOfGroup;
    string               mExe;
    string               mArgs;
    string               mExploderDestinations;

  protected:



  public:

    TrDb();
    ~TrDb();

    bool                 Connect(std::string dbName);

    // flusher info
    bool                 FlusherLoggingEnabled();
    void                 FlusherLoggingEnabled(bool val);
    unsigned int         FlusherInterval();
    void                 FlusherInterval(unsigned int val);
    string               FlusherLogFileName();
    void                 FlusherLogFileName(string val);
    string               FlusherFormat();
    void                 FlusherFormat(string val);
    unsigned int         FlusherMaxSize();
    void                 FlusherMaxSize(unsigned int val);
    bool                 FlusherOverwrite();
    void                 FlusherOverwrite(bool val);

    // misc
    bool                 TranslateOids();
    void                 TranslateOids(bool val);

    int                  ListenPort();
    void                 ListenPort(int val);
    bool                 ReverseLookup();
    void                 ReverseLookup(bool val);
    bool                 SmtpLogging();
    void                 SmtpLogging(bool val);
    bool                 ActionLogging();
    void                 ActionLogging(bool val);
    bool                 DoInforms();
    void                 DoInforms(bool val);

    int                  HttpPort();
    void                 HttpPort(int val);
    string               HttpUsername();
    void                 HttpUsername(string val);
    string               HttpPassword();
    void                 HttpPassword(string val);

    int                  Width();
    void                 Width(int w);

    int                  Height();
    void                 Height(int h);

    int                  xPos();
    void                 xPos(int x);

    int                  yPos();
    void                 yPos(int y);

    bool                 LoggingLogging();
    void                 LoggingLogging(bool logging);

    string               Cwd();
    void                 Cwd(string directory);

    bool                 Deduplicate();
    void                 Deduplicate(bool val);

    int                  DedupInterval();
    void                 DedupInterval(int interval);

    // passing trap data
    bool           IncludeEnvironment();
    void           IncludeEnvironment(bool val);

    bool           IncludeCmdLine();
    void           IncludeCmdLine(bool val);

    // env
    bool           IncludeEnvCommunity();
    void           IncludeEnvCommunity(bool val);
    bool           IncludeEnvIpAddress();
    void           IncludeEnvIpAddress(bool val);
    bool           IncludeEnvGenericType();
    void           IncludeEnvGenericType(bool val);
    bool           IncludeEnvSpecificType();
    void           IncludeEnvSpecificType(bool val);
    bool           IncludeEnvSenderOID();
    void           IncludeEnvSenderOID(bool val);
    bool           IncludeEnvVbData();
    void           IncludeEnvVbData(bool val);
    bool           IncludeEnvVbOID();
    void           IncludeEnvVbOID(bool val);
    string         EnvCommunity();
    void           EnvCommunity(string val);
    string         EnvIpAddress();
    void           EnvIpAddress(string val);
    string         EnvGenericType();
    void           EnvGenericType(string val);
    string         EnvSpecificType();
    void           EnvSpecificType(string val);
    string         EnvSenderOID();
    void           EnvSenderOID(string val);
    string         EnvVbData();
    void           EnvVbData(string val);
    string         EnvVbOID();
    void           EnvVbOID(string val);


    // cmd
    bool           IncludeCmdCommunity();
    void           IncludeCmdCommunity(bool val);
    bool           IncludeCmdIpAddress();
    void           IncludeCmdIpAddress(bool val);
    bool           IncludeCmdGenericType();
    void           IncludeCmdGenericType(bool val);
    bool           IncludeCmdSpecificType();
    void           IncludeCmdSpecificType(bool val);
    bool           IncludeCmdSenderOID();
    void           IncludeCmdSenderOID(bool val);
    bool           IncludeCmdVbData();
    void           IncludeCmdVbData(bool val);
    bool           IncludeCmdVbOID();
    void           IncludeCmdVbOID(bool val);
    string         CmdCommunity();
    void           CmdCommunity(string val);
    string         CmdIpAddress();
    void           CmdIpAddress(string val);
    string         CmdGenericType();
    void           CmdGenericType(string val);
    string         CmdSpecificType();
    void           CmdSpecificType(string val);
    string         CmdSenderOID();
    void           CmdSenderOID(string val);
    string         CmdVbData();
    void           CmdVbData(string val);
    string         CmdVbOID();
    void           CmdVbOID(string val);

    // for exploding
    string         ExploderDestination();
    int            ExploderPort();
    int            ExploderId();
    bool           FindExploder(string exploderdestination,
			int exploderport,
			int& index);
    bool           InsertExploder(string exploderdestination,
				  int     exploderport);
    void           DeleteExploder(int which);
    void           DeleteExploders();
    bool           GetByExploderId(int id);

  // for emails
    string         SmtpServerHost();
    int            SmtpPort();
    string         From();
    string         To();
    string         Subject();
    string         Message();
    bool           AuthNeeded();
    string         AuthUser();
    string         AuthJuniper();
    int            EmailId();
    bool           UseSSL();
    bool           Find(string serverHost,
			string smtpFrom,
			string smtpTo, 
			string smtpSubject,
			int& index);
    bool           InsertEmail(string serverHost,
			       int port,
			       string smtpFrom,
			       string smtpTo,
			       string smtpSubject,
			       string smtpMessage,
			       bool useAuth,
			       string authuser,
			       string authjuniper,
			       int     id);

    void           DeleteEmail(int which);
    void           DeleteEmails();

    // executables
    int            ExecutableId();
    string         Executable();
    string         Arguments();
    bool           InsertExecutable(string executable,
				    string arguments);
    void           DeleteExecutable(int which);
    void           DeleteExecutables();
    bool           GetByExecutableId(int id);

    // watches
    int            WatchId();
    string         Name();
    int            Watch();
    string         Value();
    int            Bools();
    bool           IncludeTrapData();
    int            SoundPlayDuration();
    string         SoundFile();
    string         EmailRecipients();
    string         ExploderDestinations();
    bool           PartOfGroup();
    bool           GetWatches();
    bool           Find(int inWatch, 
			string inValue, 
			int& index);

    bool           FindWatchByName(string inName);

    bool           InsertWatch(string inName,
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
			       bool   partOfGroup);

    // don't know what I was thinking
    //    bool           InsertWatch(string inName,
    //			       int    inWatch,
    //			       string inValue,
    //			       int    inBools,
    //			       bool   inTrapData,
    //			       int    soundduration,
    //			       string soundfile);

    void           DeleteWatch(int which);
    void           DeleteWatches();


    // relationships between actions and things to do
    std::vector<watchActionMapping> GetAllActions(int watchId);

    // version
    string          VersionString() { return "8.00"; }

    // go to the next row
    bool               NextExploder();
    bool               NextEmail();
    bool               NextWatch();
    bool               NextExecutable();
};


#endif
