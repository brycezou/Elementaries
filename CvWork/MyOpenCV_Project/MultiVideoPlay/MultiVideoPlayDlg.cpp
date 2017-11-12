
// MultiVideoPlayDlg.cpp : ʵ���ļ�
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


// CMultiVideoPlayDlg �Ի���

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

// CMultiVideoPlayDlg ��Ϣ�������

BOOL CMultiVideoPlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pMyDlg=this;

	HANDLE handle=CreateThread(NULL, 0, OnPlayVideos, NULL, 0, 0);
	if(!handle)
		MessageBox(_T("������Ƶ�����߳�ʧ��!"));

	CloseHandle(handle);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiVideoPlayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMultiVideoPlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMultiVideoPlayDlg::OnShowImage(IplImage * img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();   // �����ʾ�ؼ���DC
	HDC hdc = pDC->GetSafeHdc(); // ��ȡ HDC(�豸���) �����л�ͼ����
	CRect rect;
	GetDlgItem(ID)->GetClientRect( &rect );
	CvvImage cimg;
	cimg.CopyOf( img );                // ����ͼƬ
	cimg.DrawToHDC( hdc, &rect );  // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
	cimg.Destroy();
}

//pMyDlg->OnShowImage(&ipl_depth, IDC_FRAME_DEPTH);

DWORD WINAPI OnPlayVideos(LPVOID lpParam)
{
	string str_video_path("����ʶ�����˰�A.wmv");
	string str_video_path2("����ʶ�����˰�B.wmv");
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