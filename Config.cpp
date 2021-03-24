#ifndef __CONFIG_H__
#include "Config.h"
#endif

#ifdef _WIN32
#include <sstream>
#endif

using namespace std;

Config::Config()
       :mGotConfig(FALSE)
{
  mConfigFile.open("traprcvr.dat", ios::in);
  if (mConfigFile.good())
    mGotConfig = TRUE;
  //  int x = mutex_init(&mLock, USYNC_THREAD, NULL);
  //  if (x != 0)
  //    mGotConfig = FALSE;
}

Config::~Config()
{
  mConfigFile.close();
  //  mutex_destroy(&mLock);
}

int     
Config::TrapPort()
{
  return 162;
}

int     
Config::Mode()
{
  return 200;
}

CString 
Config::ModeText()
{
  return "Port 162";
}

int     
Config::ActionCount()
{
return 0;
}

CString 
Config::WatchType(int which)
{
  CString a;
  return a;
}

CString 
Config::WatchValue(int which)
{
  CString a;
  return a;
}







  // private stuff
void
Config::Lock()
{
  //  mutex_lock(&mLock);
}

void
Config::Unlock()
{
  //  mutex_unlock(&mLock);
}

void
Config::Rewind()
{
  mConfigFile.seekg(0);
  mConfigFile.clear();
}

const char*
Config::Search(char* param)
{
  mLineBuf = NULL;
    
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (mLineBuf = strstr(mLineBuffer, param))
    {
      mLineBuf += strlen(param);
      while (*mLineBuf == ' ' || *mLineBuf == '\t')
	mLineBuf++;
      break;
    }
  }
  return (const char*)mLineBuf;
}

unsigned int
Config::SearchAll(char* param)
{
  unsigned int retVal = 0;
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (strstr(mLineBuffer, param) != NULL)
      retVal++;
  }
  return retVal;
}

const char*
Config::SearchOne(char* param, int which)
{
  mLineBuf = NULL;
    
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (mLineBuf = strstr(mLineBuffer, param))
    {
      mLineBuf += strlen(param);
      while (*mLineBuf == ' ' || *mLineBuf == '\t')
	mLineBuf++;
      if (!which--)
	break;
    }
  }
  return (const char*)mLineBuf;
}






