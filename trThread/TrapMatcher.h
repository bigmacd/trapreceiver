
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
