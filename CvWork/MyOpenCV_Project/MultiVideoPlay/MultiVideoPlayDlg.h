
// MultiVideoPlayDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;


// CMultiVideoPlayDlg �Ի���
class CMultiVideoPlayDlg : public CDialogEx
{
// ����
public:
	CMultiVideoPlayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTIVIDEOPLAY_DIALOG };

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
	void OnShowImage(IplImage * img, UINT ID);
};
