
// MultiVideoPlayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiVideoPlay.h"
#include "MultiVideoPlayDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiVideoPlayDlg 对话框

DWORD WINAPI OnPlayVideos(LPVOID lpParam);


CMultiVideoPlayDlg::CMultiVideoPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiVideoPlayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiVideoPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiVideoPlayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


CMultiVideoPlayDlg *pMyDlg;

// CMultiVideoPlayDlg 消息处理程序

BOOL CMultiVideoPlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pMyDlg=this;

	HANDLE handle=CreateThread(NULL, 0, OnPlayVideos, NULL, 0, 0);
	if(!handle)
		MessageBox(_T("创建视频播放线程失败!"));

	CloseHandle(handle);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiVideoPlayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMultiVideoPlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMultiVideoPlayDlg::OnShowImage(IplImage * img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();   // 获得显示控件的DC
	HDC hdc = pDC->GetSafeHdc(); // 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(ID)->GetClientRect( &rect );
	CvvImage cimg;
	cimg.CopyOf( img );                // 复制图片
	cimg.DrawToHDC( hdc, &rect );  // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
	cimg.Destroy();
}

//pMyDlg->OnShowImage(&ipl_depth, IDC_FRAME_DEPTH);

DWORD WINAPI OnPlayVideos(LPVOID lpParam)
{
	string str_video_path("手势识别有人版A.wmv");
	string str_video_path2("手势识别有人版B.wmv");
	CvCapture* captrue = cvCreateFileCapture(str_video_path.c_str());
	CvCapture* captrue2 = cvCreateFileCapture(str_video_path2.c_str());
	IplImage* input_image = cvQueryFrame(captrue);
	IplImage* input_image2 = cvQueryFrame(captrue2);
	while (input_image || input_image2)
	{
		if (input_image)
		{
			pMyDlg->OnShowImage(input_image, IDC_VIDEO1);
			input_image=cvQueryFrame(captrue);
		}
		if (input_image2)
		{
			pMyDlg->OnShowImage(input_image2, IDC_VIDEO2);
			input_image2=cvQueryFrame(captrue2);
		}
		Sleep(25);
	}
	cvReleaseCapture(&captrue);
	cvReleaseCapture(&captrue2);
	cvReleaseImage(&input_image);
	cvReleaseImage(&input_image2);

	return 0;
}