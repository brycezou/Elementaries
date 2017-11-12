// CVPlayFoundation.h : main header file for the CVPLAYFOUNDATION application
//

#if !defined(AFX_CVPLAYFOUNDATION_H__97BBEE4B_665B_41ED_8ED4_F6D9BF48418C__INCLUDED_)
#define AFX_CVPLAYFOUNDATION_H__97BBEE4B_665B_41ED_8ED4_F6D9BF48418C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationApp:
// See CVPlayFoundation.cpp for the implementation of this class
//

class CCVPlayFoundationApp : public CWinApp
{
public:
	CCVPlayFoundationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVPlayFoundationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCVPlayFoundationApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVPLAYFOUNDATION_H__97BBEE4B_665B_41ED_8ED4_F6D9BF48418C__INCLUDED_)
