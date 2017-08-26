//  Copyright (c) 2001 Network Computing Technologies, Inc.
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

#ifndef __TRAPMATCHER_H__
#define __TRAPMATCHER_H__

//#include "compat.h"
#include "..\stdafx.h"

class Packet;

class TrapMatcher
{

  private:

    BOOL  WildcardMatch (char* pattern, char* eventData, BOOL caseSensitive);

  protected:


  public:

    TrapMatcher();
    ~TrapMatcher();

    BOOL Matches(Packet* p, int index);


};
#endif
