
#ifndef __REGISTRY_H__
#include "Registry.h"
#endif

#include "errno.h"

Registry::Registry(char* subkey)
         :mNewKey(NULL),
	  mKey(NULL),
	  mCreateFlag(FALSE)
{
  ReOpen(subkey);

  if (mR == ENOENT)
    CreateKeys();

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

void
Registry::CreateKeys()
{
  DWORD dwDisp;

  mR = 
    RegCreateKeyEx(KEY,
		   ROOT,
		   (DWORD)0,
		   NULL,
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   NULL,
		   &mKey,
		   &dwDisp);
  if (mR == ERROR_SUCCESS)
  {
    CString cValue;
    cValue = "TrapRcvr.log";
    filename(cValue);
    separatorchar(" ");
    port(162);
    flushInterval(15);
    overWrite(TRUE);
    logging(FALSE);
    cValue = "%v%i%t%o%g%s%bOTD";
    formatString(cValue);
    reverseLookup(FALSE);
    informs(FALSE);
    addVarbinds(FALSE);
    filesize(10);
    trayIcon(FALSE);
    version(versionString());
    pingTimeout(1000);
    translateOids(FALSE);
    includeTrapData(FALSE);
    includeEnvironment(FALSE);
    includeCmdLine(FALSE);
    smtpLogging(FALSE);

    char as [256]; 
    GetModuleFileName(NULL, as, 255); 
    char* last = strrchr(as, '\\');
    if (last != NULL)
      *last = 0;
    cwd(as);
    deduplicate(FALSE);
    dedupinterval(60);

    RegCloseKey(mKey);
    mKey = NULL;
  }

  mR = 
    RegCreateKeyEx(KEY,
		   ROOT"\\Exploder",
		   (DWORD)0,
		   NULL,
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   NULL,
		   &mKey,
		   &dwDisp);
  if (mR == ERROR_SUCCESS)
  {
    RegCloseKey(mKey);
    mKey = NULL;
  }

  mR = 
    RegCreateKeyEx(KEY,
		   ROOT"\\Actions",
		   (DWORD)0,
		   NULL,
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   NULL,
		   &mKey,
		   &dwDisp);
  if (mR == ERROR_SUCCESS)
  {
    RegCloseKey(mKey);
    mKey = NULL;
  }

  mR = 
    RegCreateKeyEx(KEY,
		   ROOT"\\Email",
		   (DWORD)0,
		   NULL,
		   REG_OPTION_NON_VOLATILE,
		   KEY_ALL_ACCESS,
		   NULL,
		   &mKey,
		   &dwDisp);
  if (mR == ERROR_SUCCESS)
  {
    RegCloseKey(mKey);
    mKey = NULL;
  }

  // change for groups in 7.47
  //  mR = 
  //    RegCreateKeyEx(KEY,
  //		   ROOT"\\ActionGroups",
  //		   (DWORD)0,
  //		   NULL,
  //		   REG_OPTION_NON_VOLATILE,
  //		   KEY_ALL_ACCESS,
  //		   NULL,
  //		   &mKey,
  //		   &dwDisp);
  //  if (mR == ERROR_SUCCESS)
  //  {
  //    RegCloseKey(mKey);
  //    mKey = NULL;
  //  }

}

void
Registry::ReOpen(char* which, BOOL add)
{  
  int size = 0;
  if (add && mNewKey)
    size = strlen(mNewKey) + 1;
  else
    size = strlen(ROOT) + 1;

  if (which != NULL)
    size += (strlen(which) + 1);

  char* tempKey = NULL;
  if (add && mNewKey)
  {
    tempKey = new char [strlen(mNewKey) + 1];
    strcpy_s(tempKey, strlen(mNewKey)+1, mNewKey);
  }

  if (mNewKey)
    delete [] mNewKey;

  mNewKey = new char [size];
  memset(mNewKey, 0, size);

  if (add && tempKey)
    strcpy_s(mNewKey, size, tempKey);
  else
    strcpy_s(mNewKey, size, ROOT);

  if (add && tempKey)
    delete [] tempKey;

  if (which != NULL)
  {
    strcat_s(mNewKey, size, "\\");
    strcat_s(mNewKey, size, which);
  }

  if (mKey)
  {
    RegCloseKey(mKey);
    mKey = NULL;
  }
  mR = 
    RegOpenKeyEx(KEY,
		 mNewKey,
		 (DWORD)0, 
		 KEY_ALL_ACCESS,
		 &mKey);

  if (mR == ENOENT && mCreateFlag == TRUE)
  {
    DWORD dwDisp;

    mR = 
      RegCreateKeyEx(KEY,
		     mNewKey,
		     (DWORD)0,
		     NULL,
		     REG_OPTION_NON_VOLATILE,
		     KEY_ALL_ACCESS,
		     NULL,
		     &mKey,
		     &dwDisp);  
  }
}

void
Registry::ReOpen(int which)
{
  CString number;
  CString format;
  CString leafKey;

  number = "\\";
  format.Format("%d", which);
  number += format;

  leafKey = mNewKey;
  leafKey += number;

  RegCloseKey(mKey);
  mKey = NULL;

  mR = 
    RegOpenKeyEx(KEY,
		 leafKey,
		 (DWORD)0, 
		 KEY_ALL_ACCESS,
		 &mKey);

  if (mR == ENOENT && mCreateFlag == TRUE)
  {
    DWORD dwDisp;
    
    mR = 
      RegCreateKeyEx(KEY,
		     leafKey,
		     (DWORD)0,
		     NULL,
		     REG_OPTION_NON_VOLATILE,
		     KEY_ALL_ACCESS,
		     NULL,
		     &mKey,
		     &dwDisp);  
  }
}

CString
Registry::GetRegSz(char* which)
{
  DWORD dwSize = 2048;
  char buf[2048];
  memset(buf, 0, 2048);
  unsigned long type = REG_SZ;
  mR = 
  RegQueryValueEx(mKey, 
		  which,
		  (DWORD*)0,
		  &type,
		  (unsigned char*)&buf,
		  &dwSize);
  if (mR != ERROR_SUCCESS)
    throw(0);
  CString retVal = buf;
  return retVal;
}

void
Registry::SetRegSz(char* which, CString data)
{
  mR = RegSetValueEx(mKey, 
		     which,
		     (DWORD)0,
		     REG_SZ, 
		     (const unsigned char*)data.GetBuffer(0),
		     ((DWORD)data.GetLength() + 1));
}

DWORD
Registry::GetRegDw(char* which)
{
  DWORD dwSize = 4;
  DWORD buf = 0;
  unsigned long type = REG_DWORD;
  mR = 
  RegQueryValueEx(mKey, 
		  which,
		  (DWORD*)0,
		  &type,
		  (unsigned char*)&buf,
		  &dwSize);
  if (mR != ERROR_SUCCESS)
    throw(0);
  int retVal = (int)buf;
  return retVal;
}

void
Registry::SetRegDw(char* which, unsigned long data)
{
  DWORD val = data;
  RegSetValueEx(mKey, 
		which,
		(DWORD)0,
		REG_DWORD, 
		(const unsigned char*)&val,
		sizeof(DWORD));
}

CString        
Registry::filename()
{
  return GetRegSz("LogFileName");
}

void           
Registry::filename(CString fileName)
{
  SetRegSz("LogFileName", fileName);
}

CString        
Registry::separatorchar()
{
  return GetRegSz("sepchar");
}

void           
Registry::separatorchar(CString sepchar)
{
  SetRegSz("sepchar", sepchar);
}

CString
Registry::version()
{
  return GetRegSz("Version");
}

void           
Registry::version(const char* Inversion)
{
  CString v = Inversion;
  SetRegSz("Version", v);
}

int        
Registry::port()
{
  return (int)GetRegDw("TrapPort");
}

void           
Registry::port(int portNumber)
{
  SetRegDw("TrapPort", (DWORD)portNumber);
}

int        
Registry::filesize()
{
  return (int)GetRegDw("filesize");
}

void           
Registry::filesize(int size)
{
  SetRegDw("filesize", (DWORD)size);
}

int        
Registry::pingTimeout()
{
  return (int)GetRegDw("pingtimeout");
}

void           
Registry::pingTimeout(int pingtimeout)
{
  SetRegDw("pingtimeout", (DWORD)pingtimeout); 
}

BOOL           
Registry::translateOids()
{
  return (BOOL)GetRegDw("translateOids");
}

void           
Registry::translateOids(BOOL xlat)
{
  SetRegDw("translateOids", xlat);
}

int            
Registry::width()
{
  return (int)GetRegDw("width");
}

void           
Registry::width(int w)
{
  SetRegDw("width", (DWORD)w); 
}

int            
Registry::height()
{
  return (int)GetRegDw("height");
}

void           
Registry::height(int h)
{
  SetRegDw("height", (DWORD)h); 
}

int            
Registry::xPos()
{
  return (int)GetRegDw("xPos");
}

void           
Registry::xPos(int x)
{
  SetRegDw("xPos", (DWORD)x); 
}

int            
Registry::yPos()
{
  return (int)GetRegDw("yPos");
}

void           
Registry::yPos(int y)
{
  SetRegDw("yPos", (DWORD)y); 
}

CString        
Registry::synchronized()
{
  return GetRegSz("syncVersion");
}

void           
Registry::synchronized(CString version)
{
  SetRegSz("syncVersion", version);
}

BOOL           
Registry::smtpLogging()
{
  return (BOOL)GetRegDw("smtpLogging");
}

void           
Registry::smtpLogging(BOOL logging)
{
  SetRegDw("smtpLogging", (DWORD)logging); 
}

BOOL           
Registry::actionLogging()
{
  return (BOOL)GetRegDw("actionLogging");
}

void           
Registry::actionLogging(BOOL logging)
{
  SetRegDw("actionLogging", (DWORD)logging); 
}

void           
Registry::loggingLogging(BOOL logging)
{
  SetRegDw("loggingLogging", (DWORD)logging); 
}

BOOL           
Registry::loggingLogging()
{
  return (BOOL)GetRegDw("loggingLogging");
}

CString        
Registry::cwd()
{
  return GetRegSz("cwd");
}

void           
Registry::cwd(CString directory)
{
  SetRegSz("cwd", directory);
}

BOOL
Registry::deduplicate()
{
  return (BOOL)GetRegDw("deduplicate");
}

void           
Registry::deduplicate(BOOL val)
{
  SetRegDw("deduplicate", (DWORD)val);
}

int            
Registry::dedupinterval()
{
  return GetRegDw("dedupinterval");
}

void           
Registry::dedupinterval(int val)
{
  SetRegDw("dedupinterval", (DWORD)val); 
}

BOOL           
Registry::trayIcon()
{
  return (BOOL)GetRegDw("trayIcon");
}

void           
Registry::trayIcon(BOOL tray)
{
  SetRegDw("trayIcon", (DWORD)tray); 
}

BOOL           
Registry::reverseLookup()
{
  return (BOOL)GetRegDw("reverseLookup");
}

void           
Registry::reverseLookup(BOOL rev)
{
  SetRegDw("reverseLookup", (DWORD)rev); 
}

BOOL           
Registry::informs()
{
  return (BOOL)GetRegDw("informs");
}

void           
Registry::informs(BOOL inf)
{
  SetRegDw("informs", (DWORD)inf); 
}

BOOL           
Registry::addVarbinds()
{
  return (BOOL)GetRegDw("addvarbinds");
}

void           
Registry::addVarbinds(BOOL addVbs)
{
  SetRegDw("addvarbinds", (DWORD)addVbs); 
}

BOOL
Registry::overWrite()
{
  return (BOOL)GetRegDw("OverWrite");
}

void           
Registry::overWrite(BOOL overwrite)
{
  SetRegDw("OverWrite", (DWORD)overwrite);
}

BOOL
Registry::logging()
{
  return (BOOL)GetRegDw("LogToFile");
}

void           
Registry::logging(BOOL log)
{
  SetRegDw("LogToFile", (DWORD)log);
}

CString        
Registry::formatString()
{
  return GetRegSz("FormatString");
}

void           
Registry::formatString(CString fs)
{
  SetRegSz("FormatString", fs);
}

int        
Registry::flushInterval()
{
  return (int)GetRegDw("FlushInterval");
}

void           
Registry::flushInterval(int interval)
{
  SetRegDw("FlushInterval", (DWORD)interval);
}


// actions
// change for groups in 7.47
BOOL           
Registry::partOfGroup()
{
  return (BOOL)GetRegDw("partOfGroup");
}

void           
Registry::partOfGroup(BOOL value)
{
  SetRegDw("partOfGroup", (DWORD)value);
}

CString        
Registry::name()
{
  return GetRegSz("name");
}

void           
Registry::name(CString inName)
{
  SetRegSz("name", inName);
}

int
Registry::watch()
{
  return (int)GetRegDw("watch");
}

void           
Registry::watch(int inWatch)
{
  SetRegDw("watch", inWatch);
}

CString        
Registry::value()
{
  return GetRegSz("value");
}

void           
Registry::value(CString inValue)
{
  SetRegSz("value", inValue);
}

int            
Registry::bools()
{
  return (int)GetRegDw("bools");
}

void           
Registry::bools(int inBools)
{
  SetRegDw("bools", (DWORD)inBools);
}

CString        
Registry::executable()
{
  return GetRegSz("exe");
}

void           
Registry::executable(CString exe)
{
  SetRegSz("exe", exe); 
}

int            
Registry::runFrom() {
  return (int)GetRegDw("runfrom");
}

void           
Registry::runFrom(int which) {
  SetRegDw("runfrom", (DWORD)which);
}


BOOL           
Registry::includeTrapData()
{
  return (BOOL)GetRegDw("includeTrapData");
}

void           
Registry::includeTrapData(BOOL doInclude)
{
  SetRegDw("includeTrapData", (DWORD)doInclude);
}

CString        
Registry::arguments()
{
  return GetRegSz("args");
}

void           
Registry::arguments(CString exe)
{
  SetRegSz("args", exe);
}

BOOL           
Registry::includeEnvironment()
{
  return (BOOL)GetRegDw("includeEnvironment");
}
void           
Registry::includeEnvironment(BOOL doInclude)
{
  SetRegDw("includeEnvironment", (DWORD)doInclude); 
}

BOOL           
Registry::includeCmdLine()
{
  return (BOOL)GetRegDw("includeCmdLine");
}

void           
Registry::includeCmdLine(BOOL doInclude)
{
  SetRegDw("includeCmdLine", (DWORD)doInclude); 
}

CString        
Registry::emailRecipients()
{
  return GetRegSz("emailDest");
}

void           
Registry::emailRecipients(CString dest)
{
  SetRegSz("emailDest", dest);
}

int            
Registry::soundPlayDuration()
{
  return (int)GetRegDw("soundDuration");
}

void           
Registry::soundPlayDuration(int seconds)
{
  SetRegDw("soundDuration", (DWORD)seconds);
}

CString        
Registry::soundFile()
{
  return GetRegSz("soundFile");
}

void           
Registry::soundFile(CString filename)
{
  SetRegSz("soundFile", filename);
}

CString        
Registry::exploderDestinations()
{
  return GetRegSz("exploderDest");
}

void           
Registry::exploderDestinations(CString dest)
{
  SetRegSz("exploderDest", dest);
}

BOOL           
// change for groups in 7.47
//Registry::insertAction(int     inWatch,
Registry::insertAction(CString inName,
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
		       BOOL    partofgroup,
		       int     requiredesktop)
{
  BOOL retVal = FALSE;
  ReOpen("Actions");
  mCreateFlag = TRUE;
  int index = 0;
  //  BOOL found = find(inWatch,
  BOOL found = find(inName,
		    inWatch,
		    inValue,
		    index);
  //  if (!found && !partofgroup)
  if (1)
  {
    ReOpen("Actions");
    ReOpen(index);
// change for groups in 7.47
    name(inName);
    watch(inWatch);
    value(inValue);
    bools(inBools);
    executable(exe);
    arguments(args);
    includeTrapData(inTrapData);
    emailRecipients(emailDests);
    exploderDestinations(exploderDests);
    soundPlayDuration(soundduration);
    soundFile(soundfile);
    partOfGroup(partofgroup);
    try {
      runFrom(requiredesktop);
    }
    catch(...) { 
      runFrom(1);
    }
    retVal = TRUE;
  }
  mCreateFlag = FALSE;
  return retVal;
}

void           
Registry::deleteAction(int which)
{
  ReOpen("Actions");
  CString number = IndexToName(which);
  mR = RegDeleteKey(mKey,
		    number);
}

void           
Registry::deleteActions()
{
  ReOpen("Actions");
  unsigned long count = getCount();
  if (count)
  {
    for (; count; count--)
      deleteAction(count);
    deleteAction(count);  // delete zero too
  }
}
  // exploders

CString        
Registry::exploderDestination()
{
  return GetRegSz("Destination");
}

void           
Registry::exploderDestination(CString dest)
{
  SetRegSz("Destination", dest);
}

int        
Registry::exploderPort()
{
  return (int)GetRegDw("Port");
}

void           
Registry::exploderPort(int portNumber)
{
  SetRegDw("Port",(DWORD)portNumber);
}

int            
Registry::exploderId()
{
  return (int)GetRegDw("exploderId");
}

void           
Registry::exploderId(int id)
{
  SetRegDw("exploderId", (DWORD)id); 
}

CString        
Registry::smtpServerHost()
{
  return GetRegSz("Host");
}

void           
Registry::smtpServerHost(CString string)
{
  SetRegSz("Host", string);
}

int            
Registry::smtpPort()
{
  return (int)GetRegDw("Port");
}

void           
Registry::smtpPort(int port)
{
  SetRegDw("Port", (DWORD)port); 
}

CString       
Registry::from()
{
  return GetRegSz("From");
}

void           
Registry::from(CString string)
{
  SetRegSz("From", string);
}

CString        
Registry::to()
{
  return GetRegSz("To");
}

void           
Registry::to(CString string)
{
  SetRegSz("To", string);
}

CString        
Registry::subject()
{
  return GetRegSz("Subject");
}

void           
Registry::subject(CString string)
{
  SetRegSz("Subject", string);
}

CString        
Registry::message()
{
  return GetRegSz("Message");
}

void           
Registry::message(CString string)
{
  SetRegSz("Message", string);
}

BOOL           
Registry::authNeeded()
{
  return (BOOL)GetRegDw("hassneezed");
}

void           
Registry::authNeeded(BOOL useAuth)
{
  SetRegDw("hassneezed", (DWORD)useAuth); 
}

CString        
Registry::authUser()
{
  return GetRegSz("braindead");
}

void           
Registry::authUser(CString user)
{
  SetRegSz("braindead", user);
}
    
CString        
Registry::authJuniper()
{
  return GetRegSz("lobotomy");
}

void           
Registry::authJuniper(CString juniper)
{
  SetRegSz("lobotomy", juniper);
}

int            
Registry::emailId()
{
  return (int)GetRegDw("emailId");
}

void           
Registry::emailId(int id)
{
  SetRegDw("emailId", (DWORD)id); 
}

void
Registry::useSSL(BOOL usessl)
{
  SetRegDw("usessl", (DWORD)usessl);
}

BOOL
Registry::useSSL()
{
  return (BOOL)GetRegDw("usessl");
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
  BOOL retVal = FALSE;
  ReOpen("Email");
  mCreateFlag = TRUE;
  int index = 0;
  BOOL found = find(serverHost,
		    smtpFrom,
		    smtpTo, 
		    smtpSubject,
		    index);
  if (!found)
  {
    ReOpen("Email");
    ReOpen(index);
    smtpServerHost(serverHost);
    smtpPort(port);
    from(smtpFrom);
    to(smtpTo);
    subject(smtpSubject);
    message(smtpMessage);
    authNeeded(useAuth);
    authUser(authuser);
    authJuniper(authjuniper);
    //    int x = time(0);
    //    emailId(x);
    emailId(id);
	useSSL(useSsl);
    retVal = TRUE;
  }
  mCreateFlag = FALSE;
  return retVal;
}

void
Registry::deleteEmail(int which)
{
  ReOpen("Email");
  CString number = IndexToName(which);
  mR = RegDeleteKey(mKey,
		    number);
}

void
Registry::deleteEmails()
{
  ReOpen("Email");
  unsigned long count = getCount();
  if (count)
  {
    for (; count; count--)
      deleteEmail(count);
    deleteEmail(count); // delete zero too
  }
}

BOOL
Registry::insertExploder(CString exploderdestination,
			 int     exploderport,
			 int     id)
{
  BOOL retVal = FALSE;
  ReOpen("Exploder");
  mCreateFlag = TRUE;
  int index = 0;
  BOOL found = find(exploderdestination,
		    exploderport,
		    index);
  if (!found)
  {
    ReOpen("Exploder");
    ReOpen(index);
    exploderDestination(exploderdestination);
    exploderPort(exploderport);
    exploderId(id);
    retVal = TRUE;
  }
  mCreateFlag = FALSE;
  return retVal;
}

void
Registry::deleteExploder(int which)
{
  ReOpen("Exploder");
  CString number = IndexToName(which);
  mR = RegDeleteKey(mKey,
		    number);
}

void
Registry::deleteExploders()
{
  ReOpen("Exploder");
  unsigned long count = getCount();
  if (count)
  {
    for (; count; count--)
      deleteExploder(count);
    deleteExploder(count); // delete zero too
  }
}


unsigned long
Registry::getCount()
{
  unsigned long count = 0;
  mR = RegQueryInfoKey(mKey,
		       NULL,
		       NULL,
		       NULL,
		       &count,
		       NULL,
		       NULL,
		       NULL,
		       NULL,
		       NULL,
		       NULL,
		       NULL);
  return count;
}

BOOL           
//Registry::find(int     inWatch, 
Registry::find(CString inName,
	       int     inWatch, 
	       CString inValue, 
	       int& index)
{
  BOOL retVal = FALSE;
  ReOpen("Actions");
  unsigned long count = getCount();

  for (index = 0; index < (int)count;)
  {
    try 
    {
      ReOpen("Actions");
      char nameKey[256];
      LONG result = RegEnumKey(mKey,
			       (unsigned long)index++,
			       nameKey,
			       256);
      if (result == ERROR_SUCCESS)
      {
	ReOpen(nameKey, TRUE);
	int alreadyWatch = watch();
	CString alreadyValue = value();
	CString alreadyName = name();
	if (inWatch == alreadyWatch && 
	    inValue == alreadyValue &&
	    inName == alreadyName)
	{
	  retVal = TRUE;
	  break;
	}
      }
    }
    catch(...)
    {
    }
  }
  return retVal;
}

BOOL           
Registry::findWatchByName(CString inName)
{
  BOOL retVal = FALSE;
  ReOpen("Actions");
  unsigned long count = getCount();

  for (int index = 0; index < (int)count;)
  {
    try 
    {
      ReOpen("Actions");
      char nameBuf[256];
      LONG result = RegEnumKey(mKey,
			       (unsigned long)index++,
			       nameBuf,
			       256);
      if (result == ERROR_SUCCESS)
      {
	ReOpen(nameBuf, TRUE);
	//	CString alreadyName = name();
	CString alreadyName = "";
	if (inName == alreadyName)
	{
	  retVal = TRUE;
	  break;
	}
      }
    }
    catch(...)
    {
    }
  }
  return retVal;
}


BOOL           
Registry::find(CString exploderdestination,
	       int exploderport,
	       int& index)
{
  BOOL retVal = FALSE;
  ReOpen("Exploder");
  unsigned long count = getCount();

  for (index = 0; index < (int)count;)
  {
    try
    {
      ReOpen("Exploder");
      char name[256];
      LONG result = RegEnumKey(mKey,
			       (unsigned long)index++,
			       name,
			       256);
      if (result == ERROR_SUCCESS)
      {
	ReOpen(name, TRUE);
	CString dest = exploderDestination();
	int port = exploderPort();
	if (dest == exploderdestination && port == exploderport)
	{
	  retVal = TRUE;
	  break;
	}
      }
    }
    catch(...)
    {
    }
  }
  return retVal;
}
 
BOOL           
Registry::find(CString serverHost,
	       CString smtpFrom,
	       CString smtpTo, 
	       CString smtpSubject,
	       int& index)
{
  BOOL retVal = FALSE;
  ReOpen("Email");
  unsigned long count = getCount();

  for (index = 0; index < (int)count;)
  {
    try
    {
      ReOpen("Email");
      char name[256];
      LONG result = RegEnumKey(mKey,
			       (unsigned long)index++,
			       name,
			       256);
      if (result == ERROR_SUCCESS)
      {
	ReOpen(name, TRUE);
	CString host = smtpServerHost();
	CString sFrom = from();
	CString sTo = to();
	CString cSubject = subject();
	if (host == serverHost &&
	    sFrom == smtpFrom &&
	    sTo == smtpTo &&
	    cSubject == smtpSubject)
	{
	  retVal = TRUE;
	  break;
	}
      }
    }
    catch(...)
    {
    }
  }
  return retVal;
}

CString
Registry::IndexToName(int which)
{
  char name[256];
  memset(name, 0, 256);
  LONG result = RegEnumKey(mKey,
			   which,
			   name,
			   256);
  CString retVal = name;
  return retVal;
}

// env

BOOL           
Registry::includeEnvCommunity()
{
  return (BOOL)GetRegDw("includeEnvCommunity");
}

void           
Registry::includeEnvCommunity(BOOL community)
{
  SetRegDw("includeEnvCommunity", (DWORD)community); 
}

BOOL           
Registry::includeEnvIpAddress()
{
  return (BOOL)GetRegDw("includeEnvIpAddress");
}

void           
Registry::includeEnvIpAddress(BOOL ipaddress)
{
  SetRegDw("includeEnvIpAddress", (DWORD)ipaddress); 
}
//
BOOL           
Registry::includeEnvRealIpAddress()
{
  return (BOOL)GetRegDw("includeEnvRealIpAddress");
}

void           
Registry::includeEnvRealIpAddress(BOOL ipaddress)
{
  SetRegDw("includeEnvRealIpAddress", (DWORD)ipaddress); 
}
//
BOOL           
Registry::includeEnvGenericType()
{
  return (BOOL)GetRegDw("includeEnvGenericType");
}

void           
Registry::includeEnvGenericType(BOOL generictype)
{
  SetRegDw("includeEnvGenericType", (DWORD)generictype); 
}

BOOL           
Registry::includeEnvSpecificType()
{
  return (BOOL)GetRegDw("includeEnvSpecificType");
}

void           
Registry::includeEnvSpecificType(BOOL specifictype)
{
  SetRegDw("includeEnvSpecificType", (DWORD)specifictype); 
}

BOOL           
Registry::includeEnvSenderOID()
{
  return (BOOL)GetRegDw("includeEnvSenderOID");
}

void           
Registry::includeEnvSenderOID(BOOL senderoid)
{
  SetRegDw("includeEnvSenderOID", (DWORD)senderoid); 
}

BOOL           
Registry::includeEnvVbData()
{
  return (BOOL)GetRegDw("includeEnvVbData");
}

void           
Registry::includeEnvVbData(BOOL vbdata)
{
  SetRegDw("includeEnvVbData", (DWORD)vbdata); 
}

BOOL           
Registry::includeEnvVbOID()
{
  return (BOOL)GetRegDw("includeEnvVbOID");
}

void           
Registry::includeEnvVbOID(BOOL vboid)
{
  SetRegDw("includeEnvVbOID", (DWORD)vboid); 
}


CString        
Registry::envCommunity()
{
  return GetRegSz("envCommunity");
}

void           
Registry::envCommunity(CString community)
{
  SetRegSz("envCommunity", community);
}

CString        
Registry::envIpAddress()
{
  return GetRegSz("envIpAddress");
}

void           
Registry::envIpAddress(CString ipaddress)
{
  SetRegSz("envIpAddress", ipaddress);
}

CString        
Registry::envRealIpAddress()
{
  return GetRegSz("envRealIpAddress");
}

void           
Registry::envRealIpAddress(CString ipaddress)
{
  SetRegSz("envRealIpAddress", ipaddress);
}
//
CString        
Registry::envGenericType()
{
  return GetRegSz("envGenericType");
}

void           
Registry::envGenericType(CString generictype)
{
  SetRegSz("envGenericType", generictype );
}

CString        
Registry::envSpecificType()
{
  return GetRegSz("envSpecificType");
}

void           
Registry::envSpecificType(CString specifictype)
{
  SetRegSz("envSpecificType", specifictype);
}

CString        
Registry::envSenderOID()
{
  return GetRegSz("envSenderOID");
}

void           
Registry::envSenderOID(CString senderoid)
{
  SetRegSz("envSenderOID", senderoid);
}

CString        
Registry::envVbData()
{
  return GetRegSz("envVbData");
}

void           
Registry::envVbData(CString vbdata)
{
  SetRegSz("envVbData", vbdata);
}

CString        
Registry::envVbOID()
{
  return GetRegSz("envVbOID");
}

void           
Registry::envVbOID(CString vboid)
{
  SetRegSz("envVbOID", vboid);
}

  // cmd
BOOL           
Registry::includeCmdCommunity()
{
  return (BOOL)GetRegDw("includeCmdCommunity");
}

void           
Registry::includeCmdCommunity(BOOL community)
{
  SetRegDw("includeCmdCommunity", (DWORD)community); 
}

BOOL           
Registry::includeCmdIpAddress()
{
  return (BOOL)GetRegDw("includeCmdIpAddress");
}

void           
Registry::includeCmdIpAddress(BOOL ipaddress)
{
  SetRegDw("includeCmdIpAddress", (DWORD)ipaddress); 
}

BOOL           
Registry::includeCmdRealIpAddress()
{
  return (BOOL)GetRegDw("includeCmdRealIpAddress");
}

void           
Registry::includeCmdRealIpAddress(BOOL ipaddress)
{
  SetRegDw("includeCmdRealIpAddress", (DWORD)ipaddress); 
}

//
BOOL           
Registry::includeCmdGenericType()
{
  return (BOOL)GetRegDw("includeCmdGenericType");
}

void           
Registry::includeCmdGenericType(BOOL generictype)
{
  SetRegDw("includeCmdGenericType", (DWORD)generictype); 
}

BOOL           
Registry::includeCmdSpecificType()
{
  return (BOOL)GetRegDw("includeCmdSpecificType");
}

void           
Registry::includeCmdSpecificType(BOOL specifictype)
{
  SetRegDw("includeCmdSpecificType", (DWORD)specifictype); 
}

BOOL           
Registry::includeCmdSenderOID()
{
  return (BOOL)GetRegDw("includeCmdSenderOID");
}

void           
Registry::includeCmdSenderOID(BOOL senderoid)
{
  SetRegDw("includeCmdSenderOID", (DWORD)senderoid); 
}

BOOL           
Registry::includeCmdVbData()
{
  return (BOOL)GetRegDw("includeCmdVbData");
}

void           
Registry::includeCmdVbData(BOOL vbdata)
{
  SetRegDw("includeCmdVbData", (DWORD)vbdata); 
}

BOOL           
Registry::includeCmdVbOID()
{
  return (BOOL)GetRegDw("includeCmdVbOID");
}

void           
Registry::includeCmdVbOID(BOOL vboid)
{
  SetRegDw("includeCmdVbOID", (DWORD)vboid); 
}


CString        
Registry::cmdCommunity()
{
  return GetRegSz("cmdCommunity");
}

void           
Registry::cmdCommunity(CString community)
{
  SetRegSz("cmdCommunity", community);
}

CString        
Registry::cmdIpAddress()
{
  return GetRegSz("cmdIpAddress");
}

void           
Registry::cmdIpAddress(CString ipaddress)
{
  SetRegSz("cmdIpAddress", ipaddress);
}

CString        
Registry::cmdRealIpAddress()
{
  return GetRegSz("cmdRealIpAddress");
}

void           
Registry::cmdRealIpAddress(CString ipaddress)
{
  SetRegSz("cmdRealIpAddress", ipaddress);
}

//
CString        
Registry::cmdGenericType()
{
  return GetRegSz("cmdGenericType");
}

void           
Registry::cmdGenericType(CString generictype)
{
  SetRegSz("cmdGenericType", generictype );
}

CString        
Registry::cmdSpecificType()
{
  return GetRegSz("cmdSpecificType");
}

void           
Registry::cmdSpecificType(CString specifictype)
{
  SetRegSz("cmdSpecificType", specifictype);
}

CString        
Registry::cmdSenderOID()
{
  return GetRegSz("cmdSenderOID");
}

void           
Registry::cmdSenderOID(CString senderoid)
{
  SetRegSz("cmdSenderOID", senderoid);
}

CString        
Registry::cmdVbData()
{
  return GetRegSz("cmdVbData");
}

void           
Registry::cmdVbData(CString vbdata)
{
  SetRegSz("cmdVbData", vbdata);
}

CString        
Registry::cmdVbOID()
{
  return GetRegSz("cmdVbOID");
}

void           
Registry::cmdVbOID(CString vboid)
{
  SetRegSz("cmdVbOID", vboid);
}
 

// for action groups

CString        
Registry::andGroups()
{
  return GetRegSz("ag");
}

void           
Registry::andGroups(CString inAndGroups)
{
  SetRegSz("ag", inAndGroups);
}

CString        
Registry::orGroups()
{
  return GetRegSz("og");
}

void           
Registry::orGroups(CString inOrGroups)
{
  SetRegSz("og", inOrGroups);
}

BOOL           
Registry::find(CString inAndGroups,
	       CString inOrGroups,
	       int& index)
{
  BOOL retVal = FALSE;
  ReOpen("ActionGroups");
  unsigned long count = getCount();

  for (index = 0; index < (int)count;)
  {
    try
    {
      ReOpen("ActionGroups");
      char name[256];
      LONG result = RegEnumKey(mKey,
			       (unsigned long)index++,
			       name,
			       256);
      if (result == ERROR_SUCCESS)
      {
	ReOpen(name, TRUE);
	CString thisAndGroup = andGroups();
	CString thisOrGroup = orGroups();
	if (thisAndGroup == inAndGroups &&
	    thisOrGroup == inOrGroups)
	{
	  retVal = TRUE;
	  break;
	}
      }
    }
    catch(...)
    {
    }
  }
  return retVal;
}

BOOL           
Registry::insertGroup(CString inAndGroups,
		      CString inOrGroups)
{
  BOOL retVal = FALSE;
  ReOpen("ActionGroups");
  mCreateFlag = TRUE;
  int index = 0;
  BOOL found = find(inAndGroups,
		    inOrGroups,
		    index);
  if (!found)
  {
    ReOpen("ActionGroups");
    ReOpen(index);
    andGroups(inAndGroups);
    orGroups(inOrGroups);
    retVal = TRUE;
  }
  mCreateFlag = FALSE;
  return retVal;

}

void           
Registry::deleteGroup(int which)
{
  ReOpen("ActionGroups");
  CString number = IndexToName(which);
  mR = RegDeleteKey(mKey,
		    number);
}

void           
Registry::deleteGroups()
{
  ReOpen("ActionGroups");
  unsigned long count = getCount();
  if (count)
  {
    for (; count; count--)
      deleteGroup(count);
    deleteGroup(count); // delete zero too
  }

}







