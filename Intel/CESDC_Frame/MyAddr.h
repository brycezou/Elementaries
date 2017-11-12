#if !defined(AFX_MYADDR_H__0E3D073D_524B_4595_932B_63CCACB9AB5D__INCLUDED_)
#define AFX_MYADDR_H__0E3D073D_524B_4595_932B_63CCACB9AB5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyAddr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyAddr dialog

class CMyAddr : public CDialog
{
// Construction
public:
	CString OnGetMyIP();
	CMyAddr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyAddr)
	enum { IDD = IDD_DIALOG_MY_IP };
	CString	m_EditMyIp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAddr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyAddr)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYADDR_H__0E3D073D_524B_4595_932B_63CCACB9AB5D__INCLUDED_)
