
// my_OSG_MFC_tryDlg.h : ͷ�ļ�
//

#pragma once


// C_OSG_MFC_Dlg �Ի���
class C_OSG_MFC_Dlg : public CDialogEx
{
// ����
public:
	C_OSG_MFC_Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_OSG_MFC_TRY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton();
};