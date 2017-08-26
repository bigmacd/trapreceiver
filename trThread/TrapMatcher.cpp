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
#include "TrapMatcher.h"
#endif

#include "..\Registry.h"
#include "Packet.h"
#include "..\TrActionAdd.h"

#define GREATERTHAN 0
#define EQUALS 1
#define LESSTHAN 2
#define GTEQ 3
#define LTEQ 4
#define NOTEQUALS 5

TrapMatcher::TrapMatcher()
{

}

TrapMatcher::~TrapMatcher()
{

}

BOOL
TrapMatcher::Matches(Packet* p, int index)
{
  BOOL retVal =  FALSE;


  Registry registry("Actions");
  int currentWatch = 0;
  int numberOfWatches = registry.getCount();

  registry.ReOpen(index);

  if (registry.partOfGroup())
    return FALSE; // flag the caller that this isn't anything to worry about

  // have to match all the actions that have the same name
  CString groupName = registry.name();

  BOOL done = FALSE;

  do {

    BOOL processing = FALSE;
    BOOL matches = FALSE;
    int watch = registry.watch();
    CString value = registry.value();
    switch (watch)  {
    case COMMUNITY:
      if (!strcmp(p->Community(), value.GetBuffer(0)))
	matches = TRUE;
      break;
    case GENERIC:
      if (p->GenericTrapType() == atoi(value.GetBuffer(0)))
	matches = TRUE;	     
      break;
    case SENDERIP:
      if (!strcmp(p->SenderIP(), value.GetBuffer(0)))
	matches = TRUE;
      break;
    case SENDEROID:
      matches = WildcardMatch(value.GetBuffer(0),
			      (char*)(p->SenderOID()),
			      TRUE);
      //     if (!strcmp(p->SenderOID(), value.GetBuffer(0)))
      //       matches = TRUE;
      break;
    case SPECIFIC:
      if (p->SpecificTrapType() == atoi(value.GetBuffer(0)))
	matches = TRUE;	     
      break;
    case VARBINDOID:   
    {
      ////////////////////////////////////////////////////////
      // new code
      ////////////////////////////////////////////////////////
      // hack up the registry data
      CString vboid;
      CString vbdata;
      CString oPerator = ">";
      vboid.Empty();
      vbdata.Empty();
      int comparator = GREATERTHAN;
      int firstPos = value.Find(":");
      int secondPos;
      BOOL doNewWay = TRUE;

      if (firstPos != -1)
	secondPos = value.Find(":", firstPos+1);
      else
      {
	// this code implements existing functionality
	// (we previously did not allow colon separated
	// specification for Varbind OID watches.
	doNewWay = FALSE;
	int len = p->VbListLength();
	for (int x = 1; x <= len; x++)
	{
	  matches = WildcardMatch(value.GetBuffer(0),
				  (char*)(p->VbOID(x)),
				  TRUE);
	  if (TRUE == matches)
	    break;
	}
      }

      if (TRUE == doNewWay) {
	vboid = value.Left(firstPos);
	if (secondPos != -1)      {
	  vbdata = value.Mid(firstPos + 1, (secondPos - 1) - firstPos);
	  oPerator = value.Right((value.GetLength() - secondPos) - 1);
	}
	else
	  // no operator specified
	  vbdata = value.Mid(firstPos + 1);
      
	if (oPerator.GetLength())      {
	  if (oPerator == "=")
	    comparator = EQUALS;
	  else
	    if (oPerator == "<")
	      comparator = LESSTHAN;
	    else
	      if (oPerator == ">=")
		comparator = GTEQ;
	      else
		if (oPerator == "<=")
		  comparator = LTEQ;
		else
		  if (oPerator == "!=")
		    comparator = NOTEQUALS;
	}
	int len = p->VbListLength();
	CString cIncomingOid;
	for (int x = 1; x <= len; x++)      {
	  cIncomingOid = p->VbOID(x);
	  if (WildcardMatch(vboid.GetBuffer(0), cIncomingOid.GetBuffer(0), FALSE)) {
	    ///////////////////////////////////////////////////////
	    CString cIncomingData;
	    long data;
	    long incomingData;
	    BOOL isNumeric = TRUE;
	    if (!strcmp(p->VbType(x), "String")
		|| !strcmp(p->VbType(x), "OID")
		|| !strcmp(p->VbType(x), "IpAddress"))	  {
	      cIncomingData = p->VbData(x);
	      isNumeric = FALSE;
	    }
	    else	  {
	      // numeric only
	      data = atol(vbdata.GetBuffer(0));
	      incomingData = atoi(p->VbData(x));
	    }
	    ///////////////////////////////////////////////////////
	    switch (comparator)	  {
	      case GREATERTHAN:
		if (isNumeric)	    {
		  if (incomingData > data)
		    matches = true;
		}
		else	    {
		  if (cIncomingData > vbdata)
		    matches = true;
		}
		break;
	      case EQUALS:
		if (isNumeric)	    {
		  if (incomingData == data)
		    matches = true;
		}
		else	    {
		  //		if (cIncomingData == vbdata)
		  //		  matches = true;
		  matches = WildcardMatch(vbdata.GetBuffer(0), 
					  cIncomingData.GetBuffer(0), 
					  FALSE);
		}
		break;
	      case LESSTHAN:
		if (isNumeric)	    {
		  if (incomingData < data)
		    matches = true;
		}
		else	    {
		  if (cIncomingData < vbdata)
		    matches = true;
		}
		break;
	      case GTEQ:
		if (isNumeric)	    {
		  if (incomingData >= data)
		    matches = true;
		}
		else	    {
		  if (cIncomingData >= vbdata)
		    matches = true;
		}
		break;
	      case LTEQ:
		if (isNumeric)	    {
		  if (incomingData <= data)
		    matches = true;
		}
		else	    {
		  if (cIncomingData <= vbdata)
		    matches = true;
		}
		break;
	      case NOTEQUALS:
		if (isNumeric)	    {
		  if (incomingData != data)
		    matches = true;
		}
		else	    {
		  if (cIncomingData != vbdata)
		    matches = true;
		}
		break;
	    } // switch (comparator)
	  } // if (WildcardMatch(vboid.GetBuffer(0), cIncomingOid.GetBuffer(0), FALSE)) 
	} // for (int x = 1; x <= len; x++)
      } // if (TRUE == doNewWay)
    } // case VARBINDOID
    break;
    case VARBINDVALUE:    {
      // hack up the registry data
      CString vboid;
      CString vbdata;
      CString oPerator = ">";
      vboid.Empty();
      vbdata.Empty();
      int comparator = GREATERTHAN;
      int firstPos = value.Find(":");
      int secondPos;
      if (firstPos != -1)
	secondPos = value.Find(":", firstPos+1);
      else
	break; // bad data
      
      vboid = value.Left(firstPos);
      if (secondPos != -1)      {
	vbdata = value.Mid(firstPos + 1, (secondPos - 1) - firstPos);
	oPerator = value.Right((value.GetLength() - secondPos) - 1);
      }
      else
	// no operator specified
	vbdata = value.Mid(firstPos + 1);
      
      if (oPerator.GetLength())      {
	if (oPerator == "=")
	  comparator = EQUALS;
	else
	  if (oPerator == "<")
	    comparator = LESSTHAN;
	  else
	    if (oPerator == ">=")
	      comparator = GTEQ;
	    else
	      if (oPerator == "<=")
		comparator = LTEQ;
	      else
		if (oPerator == "!=")
		  comparator = NOTEQUALS;
      }
      int len = p->VbListLength();
      for (int x = 1; x <= len; x++)      {
	if (!strcmp(p->VbOID(x), vboid.GetBuffer(0)))	{
	  ///////////////////////////////////////////////////////
	  CString cIncomingData;
	  long data;
	  long incomingData;
	  BOOL isNumeric = TRUE;
	  if (!strcmp(p->VbType(x), "String")
	      || !strcmp(p->VbType(x), "OID")
	      || !strcmp(p->VbType(x), "IpAddress"))	  {
	    cIncomingData = p->VbData(x);
	    isNumeric = FALSE;
	  }
	  else	  {
	    /////////////////////////////////////////////////////
	    //	     long data = atol(vbdata.GetBuffer(0));
	    //	     long incomingData = atoi(p->VbData(x));
	    // numeric only
	    data = atol(vbdata.GetBuffer(0));
	    incomingData = atoi(p->VbData(x));
	    /////////////////////////////////////////////////////
	  }
	  ///////////////////////////////////////////////////////
	  switch (comparator)	  {
	    case GREATERTHAN:
	      if (isNumeric)	    {
		if (incomingData > data)
		  matches = true;
	      }
	      else	    {
		if (cIncomingData > vbdata)
		  matches = true;
	      }
	      break;
	    case EQUALS:
	      if (isNumeric)	    {
		if (incomingData == data)
		  matches = true;
	      }
	      else	    {
		//		if (cIncomingData == vbdata)
		//		  matches = true;
		matches = WildcardMatch(vbdata.GetBuffer(0), 
					cIncomingData.GetBuffer(0), 
					FALSE);
	      }
	      break;
	    case LESSTHAN:
	      if (isNumeric)	    {
		if (incomingData < data)
		  matches = true;
	      }
	      else	    {
		if (cIncomingData < vbdata)
		  matches = true;
	      }
	      break;
	    case GTEQ:
	      if (isNumeric)	    {
		if (incomingData >= data)
		  matches = true;
	      }
	      else	    {
		if (cIncomingData >= vbdata)
		  matches = true;
	      }
	      break;
	    case LTEQ:
	      if (isNumeric)	    {
		if (incomingData <= data)
		  matches = true;
	      }
	      else	    {
		if (cIncomingData <= vbdata)
		  matches = true;
	      }
	      break;
	    case NOTEQUALS:
	      if (isNumeric)	    {
		if (incomingData != data)
		  matches = true;
	      }
	      else	    {
		if (cIncomingData != vbdata)
		  matches = true;
	      }
	      break;
	  } // switch (comparator)
	} // if (!strcmp(p->VbOID(x), vboid.GetBuffer(0))) 
      } // for (int x = 1; x >= len; x++)
    } // case VARBINDVALUE:
    break;
    } // switch (watch)

    // even if this is part of a group (which is ANDed), if one is false
    // the whole thing is false
    if (FALSE == matches) {
      retVal = FALSE;
      done = TRUE;
    }
    else {
      // look through registry for other actions with the same name
      retVal = matches;
      while (currentWatch < numberOfWatches)   // find next matching name
      {
	if (currentWatch == index)  {  // we already did this one
	  currentWatch++; // so skip it
	  continue;
	}

	registry.ReOpen(currentWatch);
	if (registry.partOfGroup() && registry.name() == groupName) {
	  processing = TRUE;
	  currentWatch++; // process this watch/action now
	  break;
	}
	// not skipped and not processed, look at next one
	currentWatch++;
      }
      if (currentWatch == numberOfWatches && !processing) 
	done = TRUE;
    }
  } while (!done); 
  return retVal;
}


BOOL          
TrapMatcher::WildcardMatch (char* pattern, char* eventData, BOOL caseSensitive)
{
  /*  See if a string matches a wildcard specification that uses * or ?
      (like "*.txt"), and return TRUE or FALSE, depending on the result.
      There's also a TRUE/FALSE parameter you use to indicate whether
      the match should be case-sensitive or not.  */
  
  char wcChar;
  char strChar;
  // use the starMatchesZero variable to determine whether an asterisk
  // matches zero or more characters (TRUE) or one or more characters
  // (FALSE)
  int starMatchesZero = TRUE;


  while ((strChar = *eventData) && (wcChar = *pattern))
  {
    // we only want to advance the pointers if we successfully assigned
    // both of our char variables, so we'll do it here rather than in the
    // loop condition itself
    *eventData++;
    *pattern++;
    
    // if this isn't a case-sensitive match, make both chars uppercase
    // (thanks to David John Fielder (Konan) at http://innuendo.ev.ca
    // for pointing out an error here in the original code)
    if (!caseSensitive)
    {
      wcChar = toupper(wcChar);
      strChar = toupper(strChar);
    }
    
    // check the wcChar against our wildcard list
    switch (wcChar)
    {
      // an asterisk matches zero or more characters
      case '*' :
	// do a recursive call against the rest of the string,
        // until we've either found a match or the string has
	// ended
	if (starMatchesZero)
	  *eventData--;

	while (*eventData)
	{
	  if (WildcardMatch(pattern, eventData++, caseSensitive))
	    return TRUE;
	}

	break;

	// a question mark matches any single character
      case '?' :
	break;

	// if we fell through, we want an exact match
      default :
	if (wcChar != strChar)
	  return FALSE;
	break;
    }
    
  }

  // if we have any asterisks left at the end of the wildcard string, we can
  // advance past them if starMatchesZero is TRUE (so "blah*" will match "blah")
  while ((*pattern) && (starMatchesZero))
  {
    if (*pattern == '*')
      pattern++;
    else
      break;
  }
	
  // if we got to the end but there's still stuff left in either of our strings,
  // return false; otherwise, we have a match
  if ((*eventData) || (*pattern))
    return FALSE;
  else
    return TRUE;
}
