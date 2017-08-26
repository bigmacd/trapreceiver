//  Copyright (c) 2012 TrapReceiver.com
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
#define __REGISTRYV2_H__

#include "stdafx.h"
#include "TrDb.h"

  //#define KEY  HKEY_CURRENT_USER
  //#define ROOT "SOFTWARE\\Network Computing Technologies\\TrapRcvr\\4.22"
#define KEY  HKEY_LOCAL_MACHINE
#define ROOT "SOFTWARE\\Network Computing Technologies\\TrapRcvr\\4.22"

class Registry
{


  private:
    TrDb           mDb;
    HKEY           mKey;
    long           mR;
    BOOL           mCreateFlag;
    char*          mNewKey;
    
    void           CreateKeys();

    CString        GetRegSz(char* which);
    void           SetRegSz(char* which, CString data);

    DWORD          GetRegDw(char* which);
    void           SetRegDw(char* which, unsigned long data);

    CString        IndexToName(int which);

  protected:



  public:

    Registry(char* subkey = NULL);
    Registry(char* subkey, int which);
    ~Registry();

    void           ReOpen(char* which, BOOL add = FALSE);
    void           ReOpen(int which);

    unsigned long  getCount();

    // misc
    CString        version();
    void           version(const char* Inversion);

    int            port();
    void           port(int portNumber);

    CString        filename();
    void           filename(CString fileName);

    int            filesize();
    void           filesize(int size);

    BOOL           overWrite();
    void           overWrite(BOOL overwrite);

    BOOL           logging();
    void           logging(BOOL log);

    CString        formatString();
    void           formatString(CString fs);

    BOOL           trayIcon();
    void           trayIcon(BOOL tray);

    BOOL           reverseLookup();
    void           reverseLookup(BOOL rev);

    BOOL           informs();
    void           informs(BOOL inf);

    int            flushInterval();
    void           flushInterval(int interval);

    int            pingTimeout();
    void           pingTimeout(int pingtimeout);

    BOOL           translateOids();
    void           translateOids(BOOL xlat);

    int            width();
    void           width(int w);

    int            height();
    void           height(int h);

    int            xPos();
    void           xPos(int x);

    int            yPos();
    void           yPos(int y);

    CString        synchronized();
    void           synchronized(CString version);

    BOOL           smtpLogging();
    void           smtpLogging(BOOL logging);

    BOOL           actionLogging();
    void           actionLogging(BOOL logging);

    BOOL           loggingLogging();
    void           loggingLogging(BOOL logging);

    CString        cwd();
    void           cwd(CString directory);

    BOOL           deduplicate();
    void           deduplicate(BOOL val);

    int            dedupinterval();
    void           dedupinterval(int val);

    // passing trap data
    BOOL           includeEnvironment();
    void           includeEnvironment(BOOL doInclude);

    BOOL           includeCmdLine();
    void           includeCmdLine(BOOL doInclude);

    // env
    BOOL           includeEnvCommunity();
    void           includeEnvCommunity(BOOL community);
    BOOL           includeEnvIpAddress();
    void           includeEnvIpAddress(BOOL ipaddress);
    BOOL           includeEnvGenericType();
    void           includeEnvGenericType(BOOL generictype);
    BOOL           includeEnvSpecificType();
    void           includeEnvSpecificType(BOOL specifictype);
    BOOL           includeEnvSenderOID();
    void           includeEnvSenderOID(BOOL senderoid);
    BOOL           includeEnvVbData();
    void           includeEnvVbData(BOOL vbdata);
    BOOL           includeEnvVbOID();
    void           includeEnvVbOID(BOOL vboid);

    CString        envCommunity();
    void           envCommunity(CString community);
    CString        envIpAddress();
    void           envIpAddress(CString ipaddress);
    CString        envGenericType();
    void           envGenericType(CString generictype);
    CString        envSpecificType();
    void           envSpecificType(CString specifictype);
    CString        envSenderOID();
    void           envSenderOID(CString senderoid);
    CString        envVbData();
    void           envVbData(CString vbdata);
    CString        envVbOID();
    void           envVbOID(CString vboid);


    // cmd
    BOOL           includeCmdCommunity();
    void           includeCmdCommunity(BOOL community);
    BOOL           includeCmdIpAddress();
    void           includeCmdIpAddress(BOOL ipaddress);
    BOOL           includeCmdGenericType();
    void           includeCmdGenericType(BOOL generictype);
    BOOL           includeCmdSpecificType();
    void           includeCmdSpecificType(BOOL specifictype);
    BOOL           includeCmdSenderOID();
    void           includeCmdSenderOID(BOOL senderoid);
    BOOL           includeCmdVbData();
    void           includeCmdVbData(BOOL vbdata);
    BOOL           includeCmdVbOID();
    void           includeCmdVbOID(BOOL vboid);

    CString        cmdCommunity();
    void           cmdCommunity(CString community);
    CString        cmdIpAddress();
    void           cmdIpAddress(CString ipaddress);
    CString        cmdGenericType();
    void           cmdGenericType(CString generictype);
    CString        cmdSpecificType();
    void           cmdSpecificType(CString specifictype);
    CString        cmdSenderOID();
    void           cmdSenderOID(CString senderoid);
    CString        cmdVbData();
    void           cmdVbData(CString vbdata);
    CString        cmdVbOID();
    void           cmdVbOID(CString vboid);

    // actions
    BOOL           partOfGroup();
    void           partOfGroup(BOOL value);
    
    CString        name();
    //    void           name(CString inName);

    int            watch();
    //    void           watch(int inWatch);

    CString        value();
    //    void           value(CString inValue);

    int            bools();
    //    void           bools(int inBools);

    CString        executable();
    //    void           executable(CString exe);

    BOOL           includeTrapData();
    //    void           includeTrapData(BOOL doInclude);

    CString        arguments();
    //    void           arguments(CString args);

    CString        emailRecipients();
    //    void           emailRecipients(CString dest);

    CString        exploderDestinations();
    //    void           exploderDestinations(CString dest);

    int            soundPlayDuration();
    //    void           soundPlayDuration(int seconds);

    CString        soundFile();
    //    void           soundFile(CString filename);

    BOOL           find(int inWatch, 
			CString inValue, 
			int& index);

    BOOL           findWatchByName(CString inName);

// change for groups in 7.47
//    BOOL           insertAction(int     inWatch,
    BOOL           insertAction(CString inName,
				int     inWatch,
				CString inValue,
				int     inBools,
				CString exe,
				CString args,
				BOOL    inTrapData,
				CString emailDests,
				CString exploderDests,
				int     soundduration,
				CString soundfile,
				BOOL    partofgroup);

    //    void           deleteAction(int which);
    void           deleteActions();

    // for exploding
    CString        exploderDestination();
    void           exploderDestination(CString dest);

    int            exploderPort();
    void           exploderPort(int portNumber);

    int            exploderId();
    void           exploderId(int id);

    BOOL           find(CString exploderdestination,
			int exploderport,
			int& index);

    BOOL           insertExploder(CString exploderdestination,
				  int     exploderport,
				  int     id);
    void           deleteExploder(int which);
    void           deleteExploders();


  // for emails
    CString        smtpServerHost();
    void           smtpServerHost(CString string);

    int            smtpPort();
    void           smtpPort(int port);

    CString        from();
    void           from(CString string);

    CString        to();
    void           to(CString string);

    CString        subject();
    void           subject(CString string);

    CString        message();
    void           message(CString string);

    BOOL           authNeeded();
    void           authNeeded(BOOL useAuth);

    CString        authUser();
    void           authUser(CString user);
    
    CString        authJuniper();
    void           authJuniper(CString juniper);

    int            emailId();
    void           emailId(int id);

	BOOL           useSSL();
	void           useSSL(BOOL usessl);

    BOOL           find(CString serverHost,
			CString smtpFrom,
			CString smtpTo, 
			CString smtpSubject,
			int& index);

    BOOL           insertEmail(CString serverHost,
			       int port,
			       CString smtpFrom,
			       CString smtpTo,
			       CString smtpSubject,
			       CString smtpMessage,
			       BOOL useAuth,
			       CString authuser,
			       CString authjuniper,
			       int     id,
				   BOOL    useSsl);

    void           deleteEmail(int which);
    void           deleteEmails();

    char*          versionString() { return "7.47"; }

      // for action groups
    CString        andGroups();
    void           andGroups(CString inAndGroups);

    CString        orGroups();
    void           orGroups(CString inOrGroups);

    BOOL           find(CString inAndGroups,
			CString inOrGroups,
			int& index);

    BOOL           insertGroup(CString inAndGroups,
			       CString inOrGroups);
    void           deleteGroup(int which);
    void           deleteGroups();


};
#endif



