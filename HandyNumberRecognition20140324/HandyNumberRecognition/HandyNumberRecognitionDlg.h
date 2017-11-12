
// HandyNumberRecognitionDlg.h : 头文件
//

#pragma once
#include "CvvImage.h"
#include "opencv2/opencv.hpp"

using namespace cv;

// CHandyNumberRecognitionDlg 对话框
class CHandyNumberRecognitionDlg : public CDialogEx
{
// 构造
public:
	CHandyNumberRecognitionDlg(CWnd* pParent = NULL);	// 标准构造函数
	CDC *pDC;

// 对话框数据
	enum { IDD = IDD_HANDYNUMBERRECOGNITION_DIALOG };

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void OnRefreshWindow(void);
	void OnShowImage(IplImage* img, UINT ID);
	afx_msg void OnBnClickedButtonRecognize();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
