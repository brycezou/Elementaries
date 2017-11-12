// Ex003.h : main header file for the EX003 application
//

#if !defined(AFX_EX003_H__B9AB57A7_B801_49CF_8155_EEAE95400C3D__INCLUDED_)
#define AFX_EX003_H__B9AB57A7_B801_49CF_8155_EEAE95400C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEx003App:
// See Ex003.cpp for the implementation of this class
//

class CEx003App : public CWinApp
{
public:
	CEx003App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx003App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEx003App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX003_H__B9AB57A7_B801_49CF_8155_EEAE95400C3D__INCLUDED_)
