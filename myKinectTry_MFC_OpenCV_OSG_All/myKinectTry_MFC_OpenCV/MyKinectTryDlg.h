
// MyKinectTryDlg.h : 头文件
//
#pragma once

#include "NuiApi.h"
#include "opencv2/opencv.hpp"
#include "CoreOSG.h"

using namespace cv;

// CMyKinectTryDlg 对话框
class CMyKinectTryDlg : public CDialogEx
{
// 构造
public:
	CMyKinectTryDlg(CWnd* pParent = NULL);	// 标准构造函数
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

// 对话框数据
	enum { IDD = IDD_MYKINECTTRY_MFC_OPENCV_DIALOG };

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
private:
	BOOL OnInitKinect(void);
public:
	afx_msg void OnClose();
private:
	bool OnOpenRecvSocket(int recvport);
};
