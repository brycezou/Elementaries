// CESDC_ServProcessDlg.h : header file
//

#if !defined(AFX_CESDC_SERVPROCESSDLG_H__9D6101EC_6AB6_48D0_B7BD_1FDE74FB9CFD__INCLUDED_)
#define AFX_CESDC_SERVPROCESSDLG_H__9D6101EC_6AB6_48D0_B7BD_1FDE74FB9CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCESDC_ServProcessDlg dialog

class CCESDC_ServProcessDlg : public CDialog
{
// Construction
public:
	void OnWakeUpMainWnd();
	CCESDC_ServProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCESDC_ServProcessDlg)
	enum { IDD = IDD_CESDC_SERVPROCESS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_ServProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCESDC_ServProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_SERVPROCESSDLG_H__9D6101EC_6AB6_48D0_B7BD_1FDE74FB9CFD__INCLUDED_)
