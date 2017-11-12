// CESDC_Frame.h : main header file for the CESDC_FRAME application
//

#if !defined(AFX_CESDC_FRAME_H__C26B62BE_E454_4771_9C1A_4216DDA5865B__INCLUDED_)
#define AFX_CESDC_FRAME_H__C26B62BE_E454_4771_9C1A_4216DDA5865B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCESDC_FrameApp:
// See CESDC_Frame.cpp for the implementation of this class
//

class CCESDC_FrameApp : public CWinApp
{
public:
	BOOL FirstInstance();
	CCESDC_FrameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_FrameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCESDC_FrameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_FRAME_H__C26B62BE_E454_4771_9C1A_4216DDA5865B__INCLUDED_)
