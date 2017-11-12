
// my_OSG_MFC_tryDlg.h : 头文件
//

#pragma once


// C_OSG_MFC_Dlg 对话框
class C_OSG_MFC_Dlg : public CDialogEx
{
// 构造
public:
	C_OSG_MFC_Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_OSG_MFC_TRY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton();
};
