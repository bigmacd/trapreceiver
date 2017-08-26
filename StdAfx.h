// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifndef WINVER
#define WINVER 0x0502
#endif

#if !defined(AFX_STDAFX_H__60C32D67_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
#define AFX_STDAFX_H__60C32D67_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxsock.h>		// MFC socket extensions

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#define WM_TRAY_NOTIFY WM_APP+1111

#endif // !defined(AFX_STDAFX_H__60C32D67_2D6F_11D3_ABB9_00C04F6BC4E1__INCLUDED_)
