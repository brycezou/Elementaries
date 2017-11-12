
// MyKinectTryDlg.h : ͷ�ļ�
//
#pragma once

#include "NuiApi.h"
#include "opencv2/opencv.hpp"
#include "CoreOSG.h"

using namespace cv;

// CMyKinectTryDlg �Ի���
class CMyKinectTryDlg : public CDialogEx
{
// ����
public:
	CMyKinectTryDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMyKinectTryDlg();
	Mat m_depthImage;
	Mat m_skeletonImage;
	Mat m_trajectoryImage;
	Mat m_sampledImage;
	HANDLE		m_pDepthStreamHandle;
    HANDLE		m_hNextDepthFrameEvent;
	HANDLE		m_skeletonEvent;
	void OnShowImage(IplImage* img, UINT ID);

	CCoreOSG	*m_OSG;
	HANDLE		m_OSGThreadHandle;

// �Ի�������
	enum { IDD = IDD_MYKINECTTRY_MFC_OPENCV_DIALOG };

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
private:
	BOOL OnInitKinect(void);
public:
	afx_msg void OnClose();
private:
	bool OnOpenRecvSocket(int recvport);
};
