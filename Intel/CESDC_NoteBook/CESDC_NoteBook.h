// CESDC_NoteBook.h : main header file for the CESDC_NOTEBOOK application
//

#if !defined(AFX_CESDC_NOTEBOOK_H__F2D3A741_E61F_4BA7_B5D0_E881C43E6966__INCLUDED_)
#define AFX_CESDC_NOTEBOOK_H__F2D3A741_E61F_4BA7_B5D0_E881C43E6966__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookApp:
// See CESDC_NoteBook.cpp for the implementation of this class
//

class CCESDC_NoteBookApp : public CWinApp
{
public:
	CCESDC_NoteBookApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_NoteBookApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCESDC_NoteBookApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_NOTEBOOK_H__F2D3A741_E61F_4BA7_B5D0_E881C43E6966__INCLUDED_)
