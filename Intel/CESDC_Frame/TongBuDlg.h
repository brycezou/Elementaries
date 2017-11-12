#if !defined(AFX_TONGBUDLG_H__7B32FD15_F6BF_43B8_9EB0_D11970679DAD__INCLUDED_)
#define AFX_TONGBUDLG_H__7B32FD15_F6BF_43B8_9EB0_D11970679DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TongBuDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTongBuDlg dialog
#define SLAVE_BUFFER_SIZE 32768
class CFriendInfo;
class CTongBuDlg : public CDialog
{
// Construction
public:
	void OnCreateMyDoctor();
	void OnShowPicture();
	int m_width;
	int m_height;
	HBITMAP hBitmap;
	SOCKET m_socksev;
	BOOL OnInitialAsServer(int recvport);

	CTongBuDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTongBuDlg)
	enum { IDD = IDD_DIALOG_TONG_BU };
	CStatic	m_Picture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTongBuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTongBuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TONGBUDLG_H__7B32FD15_F6BF_43B8_9EB0_D11970679DAD__INCLUDED_)
