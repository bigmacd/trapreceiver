
#ifndef __TRAPRCVREXCEPTION_H__
#include "TrapRcvrException.h"
#endif


TrapRcvrException::TrapRcvrException(int code)
                  :mCode(code)
{
  throw(this);
}

TrapRcvrException::~TrapRcvrException()
{

}
