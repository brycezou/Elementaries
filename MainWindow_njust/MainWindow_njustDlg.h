// MainWindow_njustDlg.h : header file
//

#if !defined(AFX_MAINWINDOW_NJUSTDLG_H__B1C43C0A_F312_41E8_A2C5_0C57EA7B0FFD__INCLUDED_)
#define AFX_MAINWINDOW_NJUSTDLG_H__B1C43C0A_F312_41E8_A2C5_0C57EA7B0FFD__INCLUDED_
#include "ParameterSetDlg.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainWindow_njustDlg dialog
#include "afxinet.h"
#include "afxtempl.h"
class CMainWindow_njustDlg : public CDialog
{
// Construction
public:
	CParameterSetDlg pSdlg; 
	CMainWindow_njustDlg(CWnd* pParent = NULL);	// standard constructor
    CInternetSession* m_pInetSession;	//会话对象
// Dialog Data
	//{{AFX_DATA(CMainWindow_njustDlg)
	enum { IDD = IDD_MAINWINDOW_NJUST_DIALOG };
	CListCtrl	m_InstrList;
	CString	m_addr;
	CString	m_sysState;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWindow_njustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMainWindow_njustDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPara();
	afx_msg void OnButtonInstruct();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonDelete();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonBack();
	afx_msg void OnButtonGrasp();
	afx_msg void OnButtonStop();
	afx_msg void OnClickListInstrction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowOnlyPicture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnInitialListFrame();
	void OnCaptureFrame();
	void OnInitialMain();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWINDOW_NJUSTDLG_H__B1C43C0A_F312_41E8_A2C5_0C57EA7B0FFD__INCLUDED_)
