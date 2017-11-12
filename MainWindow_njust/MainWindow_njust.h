// MainWindow_njust.h : main header file for the MAINWINDOW_NJUST application
//

#if !defined(AFX_MAINWINDOW_NJUST_H__C136A82A_1440_4857_8643_A7BD6E0F6672__INCLUDED_)
#define AFX_MAINWINDOW_NJUST_H__C136A82A_1440_4857_8643_A7BD6E0F6672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMainWindow_njustApp:
// See MainWindow_njust.cpp for the implementation of this class
//

class CMainWindow_njustApp : public CWinApp
{
public:
	CMainWindow_njustApp();
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWindow_njustApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMainWindow_njustApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWINDOW_NJUST_H__C136A82A_1440_4857_8643_A7BD6E0F6672__INCLUDED_)
