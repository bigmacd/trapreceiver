#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <afxwin.h>
#include <fstream.h>


class Config
{

  private:
    
    BOOL         mGotConfig;

    ifstream     mConfigFile;
    char         mLineBuffer[4096];
    char*        mLineBuf;
    
    HANDLE       mLock;

    void         Rewind();

    const char*  Search(char* param);

    // returns how many match the param
    unsigned int SearchAll(char* param);

    // returns an individual one of the group
    const char*  SearchOne(char* param, int which);

    void         Lock();
    void         Unlock();


  protected:



  public:

    Config();
    ~Config();

      int     TrapPort();
      int     Mode();
      CString ModeText();

      int     ActionCount();
      CString WatchType(int which);
      CString WatchValue(int which);

};
#endif


