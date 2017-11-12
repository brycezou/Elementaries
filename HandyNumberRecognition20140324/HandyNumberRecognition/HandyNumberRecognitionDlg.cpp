
// HandyNumberRecognitionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HandyNumberRecognition.h"
#include "HandyNumberRecognitionDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "opencv2/opencv.hpp"
#include <fstream>
#include "svm.h"

using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHandyNumberRecognitionDlg 对话框


CPen gPen(PS_DOT, 15, RGB(0, 255, 0));
BOOL IsLButtonDown = FALSE;
CPoint prePoint;
CPtrArray *gPointList;
CPtrArray gListList;
Mat gImage;		
struct svm_model* model;


void cvEmptyMat(Mat &image)
{
	for (int i=0; i<image.rows; i++)
	{
		uchar *depthhead = image.ptr<uchar>(i);//point of the row i , one uchar indicates one color
		for (int j=0; j<image.cols; j++)
		{
			depthhead[3*j] = 0;
			depthhead[3*j+1] = 0;
			depthhead[3*j+2] = 0;
		}
	}
}


void getBinaryImageOTSU(IplImage*srcRGB, IplImage*dstBinary)
{
	cvCvtColor(srcRGB,dstBinary,CV_BGR2GRAY);
	cvThreshold(dstBinary,dstBinary, 25, 255, CV_THRESH_OTSU);
}


void getSVMfile()
{
	char strFileName[50];
	std::ofstream outfile;
	outfile.open("Train4SVM.txt");
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			outfile<<i<<"\t";
			sprintf(strFileName, "Sam4Train\\%d\\%d (%d).jpg", i, i, j);
			IplImage *binary = cvLoadImage(strFileName, CV_LOAD_IMAGE_GRAYSCALE);
			for(int y = 0; y<binary->height; y++)
			{
				for(int x = 0; x<binary->width; x++)
				{
					outfile<<(y*binary->width+x+1)<<":"<<(cvGet2D(binary, y, x).val[0] > 128 ? 1 : 0)<<"\t";                                
				}      
			}
			outfile<<std::endl;
			cvReleaseImage(&binary);
		}
	}
	outfile.close();
}


CHandyNumberRecognitionDlg::CHandyNumberRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandyNumberRecognitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHandyNumberRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHandyNumberRecognitionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_RECOGNIZE, &CHandyNumberRecognitionDlg::OnBnClickedButtonRecognize)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CHandyNumberRecognitionDlg::OnBnClickedButtonClear)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHandyNumberRecognitionDlg 消息处理程序

BOOL CHandyNumberRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pDC = GetDC();
	pDC->SelectObject(&gPen);
	//getSVMfile();
	model = svm_load_model("res\\svm_classify.model");		//model为训练得到的svm模型, 该句加载svm模型
	this->SetTimer(1, 2, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHandyNumberRecognitionDlg::OnPaint()
{
	OnBnClickedButtonClear();

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
HCURSOR CHandyNumberRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHandyNumberRecognitionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	this->GetDlgItem(IDC_RECTANGLE)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int dX = point.x-rect.left;
	int dY = point.y-rect.top;

	if(dX >= 0 && dY >= 0 && dX <= rect.Width() && dY <= rect.Height())
	{
		prePoint = point;
		gPointList = new CPtrArray();
		gPointList->RemoveAll();
		IsLButtonDown = TRUE;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CHandyNumberRecognitionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IsLButtonDown = FALSE;

	CRect rect;
	this->GetDlgItem(IDC_RECTANGLE)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int dX = point.x-rect.left;
	int dY = point.y-rect.top;

	if(dX >= 0 && dY >= 0 && dX <= rect.Width() && dY <= rect.Height())
	{
		gListList.Add(gPointList);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CHandyNumberRecognitionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsLButtonDown)
	{
		CRect rect;
		this->GetDlgItem(IDC_RECTANGLE)->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		int dX = point.x-rect.left;
		int dY = point.y-rect.top;

		if(dX >= 0 && dY >= 0 && dX <= rect.Width() && dY <= rect.Height())
		{
			pDC->MoveTo(prePoint);
			pDC->LineTo(point);
			prePoint = point;

			CPoint* pt = new CPoint(point);	
			gPointList->Add(pt);
		}
		else
			IsLButtonDown = FALSE;
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void CHandyNumberRecognitionDlg::OnRefreshWindow(void)
{
	CRect rect;
	this->GetDlgItem(IDC_RECTANGLE)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(0, 0, 0));
	dc.SelectObject(&pen);
	dc.SelectObject(&brush);
	dc.Rectangle(rect);

	this->GetDlgItem(IDC_SMALL)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	dc.Rectangle(rect);

	this->GetDlgItem(IDC_RESULT)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	dc.Rectangle(rect);
}


void CHandyNumberRecognitionDlg::OnShowImage(IplImage* img, UINT ID)
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


//int nCount4SavePicture = 0;	/////////////////////////////////////////////////////////////
//char strPicName[20];			/////////////////////////////////////////////////////////////
void CHandyNumberRecognitionDlg::OnBnClickedButtonRecognize()
{
	// TODO: 在此添加控件通知处理程序代码
	if (gListList.GetSize() == 0)
	{
		MessageBox(_T("请书写待识别字符(0-9)!"));
		return;
	}
	this->GetDlgItem(IDC_BUTTON_RECOGNIZE)->EnableWindow(FALSE);

	CPtrArray *pta; 
	CPoint *ppt;
	int minX=50000, minY=50000, maxX=-1, maxY=-1;		

	for (int i=0; i < gListList.GetSize(); i++ )
	{
		pta = (CPtrArray *)gListList.GetAt(i);
		for (int j=0; j < pta->GetSize(); j++)
		{
			ppt = (CPoint*)pta->GetAt(j);
			if (ppt->x < minX)		minX = ppt->x;
			if (ppt->y < minY)		minY = ppt->y;
			if (ppt->x > maxX)		maxX = ppt->x;
			if (ppt->y > maxY)	maxY = ppt->y;	
		}
	}

	gImage.create(maxY-minY+1, maxX-minX+1, CV_8UC3); 
	cvEmptyMat(gImage);

	CPoint *ppt2;
	for (int i=0; i < gListList.GetSize(); i++ )
	{
		pta = (CPtrArray *)gListList.GetAt(i);
		for (int j=0; j < pta->GetSize()-1; j++)
		{
			ppt = (CPoint*)pta->GetAt(j);
			ppt2 = (CPoint*)pta->GetAt(j+1);	
			line(gImage, cvPoint(ppt->x-minX, ppt->y-minY), cvPoint(ppt2->x-minX, ppt2->y-minY), cvScalar(255, 0, 255), 8); 
		}
	}

	IplImage ipl_image = gImage;
	IplImage *dstImage = cvCreateImage(cvSize(32, 32), (&ipl_image)->depth, (&ipl_image)->nChannels);
	cvResize(&ipl_image, dstImage, CV_INTER_LINEAR);
	IplImage *binary = cvCreateImage(cvGetSize(dstImage), IPL_DEPTH_8U, 1);
	getBinaryImageOTSU(dstImage, binary);

	//sprintf(strPicName, "0%d.jpg", ++nCount4SavePicture);	/////////////////////////////////////////////////////////////
	//cvSaveImage(strPicName, binary);									/////////////////////////////////////////////////////////////

	struct svm_node *node = (struct svm_node *) malloc(1025*sizeof(struct svm_node));	//svm节点(index:value)
	for(int y = 0; y<binary->height; y++)
	{
		for(int x = 0; x<binary->width; x++)
		{
			int index = y*binary->width+x;
			node[index].index = index+1;
			node[index].value = (cvGet2D(binary, y, x).val[0] > 128 ? 1 : 0);                                
		}      
	}
	node[1024].index = -1;
	char chPicName[20];
	int intPredict = (int)svm_predict(model, node);
	sprintf(chPicName, "res\\%d.jpg", (intPredict == 10 ? 1 : intPredict));
	IplImage *result = cvLoadImage(chPicName, CV_LOAD_IMAGE_COLOR);
	OnShowImage(result, IDC_RESULT);
	cvReleaseImage(&result);
	free(node);

	cvReleaseImage(&binary);
	OnShowImage(dstImage, IDC_SMALL);
	cvReleaseImage(&dstImage);
	gImage.release();

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i=0; i < gListList.GetSize(); i++ )
	{
		pta = (CPtrArray *)gListList.GetAt(i);
		pta->RemoveAll();
	}
	gListList.RemoveAll();
	////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void CHandyNumberRecognitionDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetDlgItem(IDC_BUTTON_RECOGNIZE)->EnableWindow(TRUE);
	CPtrArray *pta; 
	for (int i=0; i < gListList.GetSize(); i++ )
	{
		pta = (CPtrArray *)gListList.GetAt(i);
		pta->RemoveAll();
	}
	gListList.RemoveAll();
	OnRefreshWindow();
}


void CHandyNumberRecognitionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->KillTimer(1);
	OnRefreshWindow();

	CDialogEx::OnTimer(nIDEvent);
}
