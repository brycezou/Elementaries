#if !defined(AFX_CATCHSHOW_H__7FD2B679_9054_46C5_8E4B_DCDE9308F3A6__INCLUDED_)
#define AFX_CATCHSHOW_H__7FD2B679_9054_46C5_8E4B_DCDE9308F3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatchShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCatchShow dialog
class CCatchShow : public CDialog
{
// Construction
public:
	int m_width;
	int m_height;
	HBITMAP hBitmap;
	void OnShowPicture();
	CCatchShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCatchShow)
	enum { IDD = IDD_DIALOG_SHOW };
	CStatic	m_Picture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCatchShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSHOW_H__7FD2B679_9054_46C5_8E4B_DCDE9308F3A6__INCLUDED_)
