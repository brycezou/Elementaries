#if !defined(AFX_EDITFRIENDINFODLG_H__50B15441_39EF_4CE0_A413_1EFDCEA5124C__INCLUDED_)
#define AFX_EDITFRIENDINFODLG_H__50B15441_39EF_4CE0_A413_1EFDCEA5124C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditFriendInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditFriendInfoDlg dialog

class CEditFriendInfoDlg : public CDialog
{
// Construction
public:
	CEditFriendInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditFriendInfoDlg)
	enum { IDD = IDD_DIALOG_ADD_FRIEND };
	CString	m_IP;
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFriendInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditFriendInfoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFRIENDINFODLG_H__50B15441_39EF_4CE0_A413_1EFDCEA5124C__INCLUDED_)
