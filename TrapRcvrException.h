//  Copyright (c) 1997 Network Computing Technologies, Inc.
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
