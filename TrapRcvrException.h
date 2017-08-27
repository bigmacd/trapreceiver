
#ifndef __TRAPRCVREXCEPTION_H__
#define __TRAPRCVREXCEPTION_H__

enum TraprcvrExceptionCode
{
  TESTMESSAGE = 0,
  NULLMESSAGE,
  MAXTRAPRCVREXCEPTION
};

static  char* mMessages[] = 
{
  // parsing errors
  "Test",
  
  // build errors
  "",
  
  // component errors
  ""
};


class TrapRcvrException
{

  private:

    int     mCode;

  protected:



  public:

    TrapRcvrException(int code);
    ~TrapRcvrException();

};
#endif
