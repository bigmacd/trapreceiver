// TrapRcvr.h : main header file for the TRAPRCVR application
//

#if !defined(AFX_TRAPRCVR_H__60C32D63_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_TRAPRCVR_H__60C32D63_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// TrapRcvrApp:
// See TrapRcvr.cpp for the implementation of this class
//

class TrapRcvrApp : public CWinApp
{

  private:

    BOOL            ScmForTrapsIsRunning();
    BOOL            StartupScmForTraps();
    void            UpdateForExploderDestinations();
    void            UpdateForActionLogic();
    void            Elevate();
    BOOL            IsRunAsAdmin();

public:

    TrapRcvrApp();
    BOOL            mServiceIsRunning;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrapRcvrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(TrapRcvrApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAPRCVR_H__60C32D63_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)

extern TrapRcvrApp theApp;
