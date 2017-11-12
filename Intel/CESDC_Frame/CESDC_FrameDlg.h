// CESDC_FrameDlg.h : header file
//

#if !defined(AFX_CESDC_FRAMEDLG_H__49021C47_D3A6_4AA2_9F46_9E3C15588088__INCLUDED_)
#define AFX_CESDC_FRAMEDLG_H__49021C47_D3A6_4AA2_9F46_9E3C15588088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCESDC_FrameDlg dialog
#define MASTER_BUFFER_SIZE 32768
#define FILE_BUFFER_SIZE   32768
class CFriendInfo;
class CCESDC_FrameDlg : public CDialog
{
// Construction
public:
	BOOL OnFindProcessName(CString strPross);
	void OnBeginWhenStart();
	void OnRestartWhenExitMeeting();
	void OnMenuExit();
	void OnMenuShow();
	BOOL OnOpenAsServer(int transRecv);
	CString OnGetFileName(CString);
	void OnSendFile();
    SOCKET m_sockRecvFile;
	BOOL OnOpenReceiveFileSocket(int recvport);
	CString m_SendFileName;
	CString m_SendFileIP;
	void OnCaptureFrame();
	void OnInitialNameList();
	void OnInitialListFrame();
	CFriendInfo *pfInfo;
	CCESDC_FrameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCESDC_FrameDlg)
	enum { IDD = IDD_CESDC_FRAME_DIALOG };
	CListCtrl	m_memberlist;
	CListCtrl	m_friendnamelist;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_FrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_NotifyIcon;

	// Generated message map functions
	//{{AFX_MSG(CCESDC_FrameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAddFriend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonDeleteFriend();
	afx_msg void OnClickListFriendName(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMeeting();
	afx_msg void OnButtonStopMeeting();
	afx_msg void OnButtonAddMember();
	afx_msg void OnButtonStartMeeting();
	afx_msg void OnButtonSendFile();
	afx_msg void OnButtonMyId();
	afx_msg void OnButtonDeleteMember();
	afx_msg void OnClickListMemberName(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_FRAMEDLG_H__49021C47_D3A6_4AA2_9F46_9E3C15588088__INCLUDED_)
