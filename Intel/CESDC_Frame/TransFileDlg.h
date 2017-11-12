#if !defined(AFX_TRANSFILEDLG_H__BD903CC7_BF5B_4BC2_9BDE_8C63369567A8__INCLUDED_)
#define AFX_TRANSFILEDLG_H__BD903CC7_BF5B_4BC2_9BDE_8C63369567A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransFileDlg dialog

class CTransFileDlg : public CDialog
{
// Construction
public:
	CString   m_FileName;
	CTransFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransFileDlg)
	enum { IDD = IDD_DIALOG_SEND_FILE };
	CListCtrl	m_ListFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransFileDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransFileDlg)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonChooseFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFILEDLG_H__BD903CC7_BF5B_4BC2_9BDE_8C63369567A8__INCLUDED_)
