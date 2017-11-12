#if !defined(AFX_MYDIALOG_H__FDF36D0A_4E75_4D5B_942F_F6378A17E427__INCLUDED_)
#define AFX_MYDIALOG_H__FDF36D0A_4E75_4D5B_942F_F6378A17E427__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog

class CMyDialog : public CDialog
{
// Construction
public:
	CMyDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDialog)
	enum { IDD = IDD_DIALOG_MY };
	CListBox	m_mylist;
	CString	m_strList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonChange();
	afx_msg void OnButtonPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG_H__FDF36D0A_4E75_4D5B_942F_F6378A17E427__INCLUDED_)
