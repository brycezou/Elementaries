
// IDCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IDCardCorrectAndRecognize.h"
#include "IDCardDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"
#include "labelAeras.h"
#include "BlobResult.h"
#include "blob.h"

using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIDCardDlg 对话框
CIDCardDlg::CIDCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIDCardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CIDCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIDCardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CIDCardDlg::OnBnClickedButtonLoadImage)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CIDCardDlg 消息处理程序
BOOL CIDCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_srcRGB=NULL;
	m_dstRGB=NULL;
	m_intPointNum=0;
	m_bIsLoaded=FALSE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CIDCardDlg::OnPaint()
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

	if (m_srcRGB!=NULL)						//重绘输入图像
		OnShowImage(m_srcRGB, IDC_IMAGE_IN);

	for (int i=0; i<m_intPointNum; i++)		//重绘用户选取的点
	{
		OnMyDrawLine(m_ptArray[i], m_ptArray[i], 6);
	}

	if(m_intPointNum == 4)						//重绘边界线
	{			
		for(int i = 0 ; i < 3 ; i++)
			OnMyDrawLine(m_ptArray[i], m_ptArray[i+1], 2);
		OnMyDrawLine(m_ptArray[3], m_ptArray[0], 2);
	}
	else if(m_intPointNum > 1 && m_intPointNum < 4)
	{
		for(int i = 0 ; i < m_intPointNum-1 ; i++)
			OnMyDrawLine(m_ptArray[i], m_ptArray[i+1], 2);
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIDCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//加载输入图像
void CIDCardDlg::OnBnClickedButtonLoadImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog hFileDlg(		TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,
										TEXT(	"所有支持的图像文件 (*.jpg;*.gif;*.bmp;*.tif;*.png...)|*.jpg;*.gif;*.bmp;*.tif;*.png; \
													 |BMP图像文件(*.bmp)|*.bmp|JPEG图像文件(*.jpg)|*.jpg|PNG图像文件(*.png)|*.png; \
													 |Tiff图像文件(*.tiff;*.tif)|*.tiff;*.tif||" ), NULL );
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof( OPENFILENAME );
	hFileDlg.m_ofn.lpstrTitle = TEXT( "打开图像文件...\0" );
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if ( hFileDlg.DoModal() == IDOK )
	{
		if (m_srcRGB!=NULL)
			cvReleaseImage(&m_srcRGB);
		if (m_dstRGB!=NULL)
			cvReleaseImage(&m_dstRGB);

		CString path = hFileDlg.GetPathName();
		for (int i=0; i<path.GetLength(); i++)
		{
			m_strInFilePath[i]=path.GetAt(i);
		}
		m_strInFilePath[path.GetLength()]='\0';

		m_srcRGB=cvLoadImage(m_strInFilePath, CV_LOAD_IMAGE_COLOR);
		OnShowImage(m_srcRGB, IDC_IMAGE_IN);
		m_bIsLoaded=TRUE;
		m_intPointNum=0;
	}
}


//在MFC控件上显示IplImage* 格式的图片
void CIDCardDlg::OnShowImage(IplImage * img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();		// 获得显示控件的DC
	HDC hdc = pDC->GetSafeHdc();					// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(ID)->GetClientRect( &rect );
	CvvImage cimg;
	cimg.CopyOf( img );										 // 复制图片
	cimg.DrawToHDC( hdc, &rect );					 // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
	cimg.Destroy();
}


//将用户选定区域调整到固定大小
IplImage* MyResizeInputImage(IplImage *src)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(498, 308), src->depth, src->nChannels);	//构造缩小后的目标图象
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}


//寻找二值图像srcBinary中可能涉及到文字的blobs
//将估计出的精确区域返回到rtAccurate中
IplImage* GetBlobs(IplImage *srcBinary, CvRect &rtAccurate)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	bool IsFind=false;

	IplImage *filtered_blob=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );			//寻找二值图中的连通域
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, filtered_blob );		

	int minx=10000, maxx=-1, miny=10000, maxy=-1;
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();		//得到当前 Blob 的外包矩形
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if ( wi*he > Width*Height/1.0/200 )		
		{
			//currentBlob->FillBlob( filtered_blob, CV_RGB(0, 0, 255));
			//cvRectangle(filtered_blob, cvPoint(rct.x, rct.y), cvPoint(rct.x+rct.width, rct.y+rct.height), cvScalar(0, 255, 0), 1);
			if(rct.x<minx)	minx=rct.x;
			if(rct.y<miny)	miny=rct.y;
			if(rct.x+rct.width>maxx)	maxx=rct.x+rct.width;
			if(rct.y+rct.height>maxy)	maxy=rct.y+rct.height;
			IsFind=true;
		}
	}

	if (IsFind)
	{
		rtAccurate.x=minx;
		rtAccurate.y=miny;
		rtAccurate.width=maxx-minx;
		rtAccurate.height=maxy-miny;
	}
	else
	{
		rtAccurate.x=0;
		rtAccurate.y=0;
		rtAccurate.width=srcBinary->width;
		rtAccurate.height=srcBinary->height;
	}

	return filtered_blob;
}


//计算输入灰度图像srcGray的自适应化二值图像
//将结果反映到输出图像dstBinMat中
void getBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat)
{
	Mat srcGrayMat(srcGray);
	adaptiveThreshold(srcGrayMat, dstBinMat, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 7, 14);
}


//计算输入RGB彩色图像srcRGB的OTSU二值图像
//将结果反映到输出图像dstBinary中
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary)
{
	cvCvtColor(srcRGB, dstBinary, CV_BGR2GRAY); 
	cvThreshold(dstBinary, dstBinary, 25, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
}


//根据每一个小区域gOrgRectBoxArray[]的绝对坐标和
//该区域内文字所在精确区域gAccurateRectBoxArray[]的相对坐标
//得到文字所在精确区域的绝对坐标gAbsoluteBoxRectArray
void getAbsoluteBoxRect()
{
	int intSX, intSY, intBX, intBY;
	for (int i=0; i<NUM_BOX_ORG; i++)
	{
		intSX=gOrgRectBoxArray[i].x+gAccurateRectBoxArray[i].x;
		intSY=gOrgRectBoxArray[i].y+gAccurateRectBoxArray[i].y;
		intBX=intSX+gAccurateRectBoxArray[i].width;
		intBY=intSY+gAccurateRectBoxArray[i].height;
		gAbsoluteBoxRectArray[i].x=intSX;
		gAbsoluteBoxRectArray[i].y=intSY;
		gAbsoluteBoxRectArray[i].width=intBX;		//其实gAbsoluteBoxRectArray中存储的不是真正的CvRect结构，而是该结构的左上角和右下角的坐标值
		gAbsoluteBoxRectArray[i].height=intBY;
	}
}


//鼠标左键按下事件处理，主要用来选点
void CIDCardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsLoaded)
	{
		CRect rect;
		this->GetDlgItem(IDC_IMAGE_IN)->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		int dX=point.x-rect.left;
		int dY=point.y-rect.top;

		if(dX>=0 && dY>=0 && dX<=rect.Width() && dY<=rect.Height())		//当鼠标位于输入图像上时
		{
			if ( m_intPointNum < 4 ) 
			{			
				m_ptArray[ m_intPointNum ] = point ;
				OnMyDrawLine(point, point, 12);
				if(m_intPointNum >= 1)
					OnMyDrawLine(m_ptArray[m_intPointNum-1], m_ptArray[m_intPointNum], 2);
				m_intPointNum++;		
			}

			if(m_intPointNum == 4)
			{
				OnMyDrawLine(m_ptArray[3], m_ptArray[0], 2);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_dstRGB=cvCloneImage(m_srcRGB);
				cvZero(m_dstRGB);
				MyGet4PtInImage(rect);										//求出四个鼠标选中的点坐标对应到图像中的坐标
				MyGetSelectAeraImg(m_srcRGB, m_dstRGB);		//截取选中的四边形区域内的图像    
				//CvRect rtAera=MyGetCorrectImage();				//毕业论文中的透视变换方法
				CvRect rtAera=MyGetCorrectImageByCV();			//OpenCV中的透视变换方法
				cvSetImageROI(m_dstRGB, rtAera);
				IplImage *ImgIDCard=MyResizeInputImage(m_dstRGB);
				cvResetImageROI(m_dstRGB);

				MyLabelAeras(ImgIDCard, cvGetSize(ImgIDCard).width, cvGetSize(ImgIDCard).height, FALSE);
				for (int i=0; i<NUM_BOX_ORG; i++)					//得到文字所在的精确区域，相对坐标；存储在gAccurateRectBoxArray[]中
				{
					cvSetImageROI(ImgIDCard, gOrgRectBoxArray[i] );
					//OnShowImage(ImgIDCard, IDC_IMAGE_OUT);
					IplImage *binary=cvCreateImage(cvGetSize(ImgIDCard), IPL_DEPTH_8U, 1);
					getBinaryImageOTSU(ImgIDCard, binary);
					IplImage *filtered_blob=GetBlobs(binary, gAccurateRectBoxArray[i]);
					//cvShowImage( "blobs", filtered_blob);
					cvReleaseImage(&filtered_blob);
					cvReleaseImage(&binary);
					cvResetImageROI(ImgIDCard);
				}
				getAbsoluteBoxRect();											//得到文字所在的精确区域，绝对坐标；存储在gAbsoluteBoxRectArray[]中

				for (int i=0; i<NUM_BOX_ORG; i++)
				{
					cvRectangle(ImgIDCard, cvPoint(gAbsoluteBoxRectArray[i].x, gAbsoluteBoxRectArray[i].y), cvPoint(gAbsoluteBoxRectArray[i].width, gAbsoluteBoxRectArray[i].height), cvScalar(0, 0, 255), 1);	
				}
				OnShowImage(ImgIDCard, IDC_IMAGE_OUT);

				cvReleaseImage(&ImgIDCard);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


//鼠标右键按下事件处理，主要用来清除点，重新操作
void CIDCardDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_intPointNum = 0;
	this->SendMessage(WM_PAINT);

	CDialogEx::OnRButtonDown(nFlags, point);
}


//在两点之间画一条线段
void CIDCardDlg::OnMyDrawLine(CPoint startPt, CPoint stopPt, int lineWidth)
{
	CDC *pDC = GetDC(); 
	CPen pen(PS_SOLID, lineWidth, RGB (255, 0, 0)); 
	CPen* pOldPen = pDC->SelectObject (&pen); 

	pDC->MoveTo(startPt);
	pDC->LineTo(stopPt);

	pDC->SelectObject(pOldPen);
	ReleaseDC(pDC);
}


//求出四个鼠标选中的点坐标对应到图像中的坐标
void CIDCardDlg::MyGet4PtInImage(CRect rect)
{
	//计算显示窗与实际图像的尺寸比例
	double dbWidthRate = m_srcRGB->width/1.0/rect.Width();
	double dbHeightRate = m_srcRGB->height/1.0/rect.Height();
	for(int i = 0 ; i < 4 ; i++)
	{
		m_PointInImage[i].x = MIN(int(m_ptArray[i].x * dbWidthRate), m_srcRGB->width);
		m_PointInImage[i].y = MIN(int(m_ptArray[i].y * dbHeightRate), m_srcRGB->height);
	}
}


//截取选中的四边形区域内的图像     
void CIDCardDlg::MyGetSelectAeraImg(IplImage * src, IplImage * dst)
{
	CPoint ptTemp;
	for (int i = 0; i < src->height; i++ )
	{
		for (int j = 0; j < src->width; j++ )
		{
			ptTemp.x = j;
			ptTemp.y = i;
			BOOL bIsInRect = MyJudgePtInRect(m_PointInImage[0] , m_PointInImage[1] , m_PointInImage[2] , m_PointInImage[3] , ptTemp);
			if( bIsInRect == TRUE )
			{
				(dst->imageData+i*dst->widthStep)[3*j]=((uchar *)(src->imageData+i*src->widthStep))[3*j];			//第i行第j列第R通道
				(dst->imageData+i*dst->widthStep)[3*j+1]=((uchar *)(src->imageData+i*src->widthStep))[3*j+1];	//第i行第j列第G通道
				(dst->imageData+i*dst->widthStep)[3*j+2]=((uchar *)(src->imageData+i*src->widthStep))[3*j+2];	//第i行第j列第B通道
			}
		}
	}
}


//判断点是否在凸四边形区域内
BOOL CIDCardDlg::MyJudgePtInRect(CPoint P1, CPoint P2, CPoint P3, CPoint P4, CPoint Pt)
{
	double dTriangle = MyGetTriangleArea(P1, P2, Pt)+MyGetTriangleArea(P2, P3, Pt)+MyGetTriangleArea(P3, P4, Pt)+MyGetTriangleArea(P4, P1, Pt);   
	double dQuadrangle = MyGetTriangleArea(P1, P2, P3)+MyGetTriangleArea(P3, P4, P1);                

	if(fabs(dTriangle-dQuadrangle) < 1e-6 )
		return TRUE;
	else 
		return FALSE;
}


//计算三角形面积
double CIDCardDlg::MyGetTriangleArea(CPoint a, CPoint b, CPoint c)
{
	double result = fabs((a.x * b.y + b.x * c.y + c.x * a.y - b.x * a.y -c.x * b.y - a.x * c.y )/2.0);
	return result;
}


CvRect CIDCardDlg::MyGetCorrectImage(void)
{
	int minx, miny, maxx, maxy;			//计算被截图像的一个较小的外接矩形
	minx = m_PointInImage[0].x;
	miny = m_PointInImage[0].y;
	maxx = m_PointInImage[0].x;
	maxy = m_PointInImage[0].y;

	for( int i = 0; i < 4; i++)
	{
		if(m_PointInImage[i].x < minx)		minx = m_PointInImage[i].x;
		if(m_PointInImage[i].y < miny)		miny = m_PointInImage[i].y;
		if(m_PointInImage[i].x > maxx)		maxx = m_PointInImage[i].x;
		if(m_PointInImage[i].y > maxy)		maxy = m_PointInImage[i].y;
	}

	CvRect rtAera;		
	rtAera.x=minx;
	rtAera.y=miny;
	rtAera.width=maxx-minx;
	rtAera.height=maxy-miny;

	//透视变换 
	CPoint srcPoint[4];   
	CPoint dstPoint[4];   

	srcPoint[0].x=(m_PointInImage[0].x);	srcPoint[0].y=(m_PointInImage[0].y);
	srcPoint[1].x=(m_PointInImage[1].x);	srcPoint[1].y=(m_PointInImage[1].y);   
	srcPoint[2].x=(m_PointInImage[2].x);	srcPoint[2].y=(m_PointInImage[2].y);   
	srcPoint[3].x=(m_PointInImage[3].x);	srcPoint[3].y=(m_PointInImage[3].y);   

	dstPoint[0].x=(minx);		dstPoint[0].y=(miny);   
	dstPoint[1].x=(minx);		dstPoint[1].y=(maxy);   
	dstPoint[2].x=(maxx);	dstPoint[2].y=(maxy);       
	dstPoint[3].x=(maxx);	dstPoint[3].y=(miny); 	

	double h[8] = {0., 0., 0., 0., 0., 0., 0.,0.};
	MyGetPerspectiveTransform(srcPoint, dstPoint, h);								//获取双线性空间变换参数 
	MyWarpPerspective(m_dstRGB, srcPoint, dstPoint, h);     //对畸变图像中每个坐标进行双线性空间变换

	return rtAera;
}


void CIDCardDlg::MyGetPerspectiveTransform(CPoint * srcPoint, CPoint * dstPoint, double * h)
{
	double a[4][5];							//存放系数
	double b[4][5];
	double *h1, *h2;

	for(int i = 0; i < 4; i++)
	{
		a[i][0] = double(dstPoint[i].x);
		a[i][1] = double(dstPoint[i].y);
		a[i][2] = double(dstPoint[i].x * dstPoint[i].y);
		a[i][3] = 1.;
		a[i][4] = double(srcPoint[i].x);
	}

	for(int i = 0; i < 4; i++)
	{
		b[i][0] = double(dstPoint[i].x);
		b[i][1] = double(dstPoint[i].y);
		b[i][2] = double(dstPoint[i].x * dstPoint[i].y);
		b[i][3] = 1.;
		b[i][4] = double(srcPoint[i].y);
	}

	h1 = MyGaussianEli(a[0], 4);
	h2 = MyGaussianEli(b[0], 4);

	for(int i = 0; i < 4; i++)	h[i] = h1[i];
	for(int i = 4; i < 8; i++)	h[i] = h2[i-4];
}


//高斯消元法解线性方程组
double * CIDCardDlg::MyGaussianEli(double * c, int n)
{
	int i, j, k, t;   
	double *x, p; 
	x=(double *)malloc(n*sizeof(double));   

	for(i=0; i<=n-2; i++)   
	{   
		k=i;   
		for(j=i+1; j<=n-1; j++)   
			if(fabs(*(c+j*(n+1)+i))>(fabs(*(c+k*(n+1)+i))))
				k=i;   
		if(k!=i)   
			for(j=i; j<=n; j++)   
			{   
				p=*(c+i*(n+1)+j);   
				*(c+j*(n+1)+j)=*(c+k*(n+1)+j);   
				*(c+k*(n+1)+j)=p;   
			}   
			for(j=i+1;j<=n-1;j++)   
			{   
				p=(*(c+j*(n+1)+i))/(*(c+i*(n+1)+i));   
				for(t=i;t<=n;t++)   
					*(c+j*(n+1)+t)-=p*(*(c+i*(n+1)+t));   
			}   
	} 

	for(i=n-1; i>=0; i--)   
	{   
		for(j=n-1; j>=i+1; j--)   
			(*(c+i*(n+1)+n))-=x[j]*(*(c+i*(n+1)+j));   
		x[i]=*(c+i*(n+1)+n)/(*(c+i*(n+1)+i));   
	}   

	return x;   
}


void CIDCardDlg::MyWarpPerspective(IplImage * dst, CPoint * srcPoint, CPoint * dstPoint, double * h)
{
	IplImage *tImg=cvCloneImage(dst);
	cvZero(tImg);

	//双线性插值法
	for( int j = dstPoint[0].y; j < dstPoint[2].y; j++ )
	{
		for ( int i =dstPoint[0].x; i < dstPoint[2].x; i++ )
		{
			double ti = h[0] * i + h[1] * j + h[2] * i * j + h[3];			//存放临时坐标
			double tj = h[4] * i + h[5] * j + h[6] * i * j + h[7];

			int ii = int(ti);		//纠正前的扭曲图像的横坐标和纵坐标
			int jj = int(tj);
			double u = ti - int(ti);
			double v = tj - int(tj);

			(tImg->imageData+j*tImg->widthStep)[3*i] =		(1-u)*(1-v)*(dst->imageData+jj*dst->widthStep)[3*ii] +			//第jj行第ii列第R通道
				(1-u)*v*(dst->imageData+(jj+1)*dst->widthStep)[3*ii] + 
				u*(1-v)*(dst->imageData+jj*dst->widthStep)[3*(ii+1)] + 
				u*v*(dst->imageData+(jj+1)*dst->widthStep)[3*(ii+1)];
			(tImg->imageData+j*tImg->widthStep)[3*i+1]=	(1-u)*(1-v)*(dst->imageData+jj*dst->widthStep)[3*ii+1] + 
				(1-u)*v*(dst->imageData+(jj+1)*dst->widthStep)[3*ii+1] + 
				u*(1-v)*(dst->imageData+jj*dst->widthStep)[3*(ii+1)+1] + 
				u*v*(dst->imageData+(jj+1)*dst->widthStep)[3*(ii+1)+1];
			(tImg->imageData+j*tImg->widthStep)[3*i+2] = (1-u)*(1-v)*(dst->imageData+jj*dst->widthStep)[3*ii+2] + 
				(1-u)*v*(dst->imageData+(jj+1)*dst->widthStep)[3*ii+2] + 
				u*(1-v)*(dst->imageData+jj*dst->widthStep)[3*(ii+1)+2] + 
				u*v*(dst->imageData+(jj+1)*dst->widthStep)[3*(ii+1)+2];
		}
	}

	for (int i = 0; i < dst->height; i++ )
	{
		for (int j = 0; j < dst->width; j++ )
		{
			(dst->imageData+i*dst->widthStep)[3*j]=((uchar *)(tImg->imageData+i*tImg->widthStep))[3*j];
			(dst->imageData+i*dst->widthStep)[3*j+1]=((uchar *)(tImg->imageData+i*tImg->widthStep))[3*j+1];
			(dst->imageData+i*dst->widthStep)[3*j+2]=((uchar *)(tImg->imageData+i*tImg->widthStep))[3*j+2];
		}
	}

	cvReleaseImage(&tImg);
}


//OpenCV中的透视变换
CvRect CIDCardDlg::MyGetCorrectImageByCV(void)
{	
	int minx, miny, maxx, maxy;			//计算被截图像的一个较小的外接矩形
	minx = m_PointInImage[0].x;
	miny = m_PointInImage[0].y;
	maxx = m_PointInImage[0].x;
	maxy = m_PointInImage[0].y;

	for( int i = 0; i < 4; i++)
	{
		if(m_PointInImage[i].x < minx)		minx = m_PointInImage[i].x;
		if(m_PointInImage[i].y < miny)		miny = m_PointInImage[i].y;
		if(m_PointInImage[i].x > maxx)		maxx = m_PointInImage[i].x;
		if(m_PointInImage[i].y > maxy)		maxy = m_PointInImage[i].y;
	}

	CvRect rtAera;		
	rtAera.x=minx;
	rtAera.y=miny;
	rtAera.width=maxx-minx;
	rtAera.height=maxy-miny;

	//透视变换 
	CvPoint2D32f srcQuad[4];
	CvPoint2D32f dstQuad[4];
	CvMat* warp_matrix = cvCreateMat(3, 3, CV_32FC1);

	srcQuad[0].x=(m_PointInImage[0].x);	srcQuad[0].y=(m_PointInImage[0].y);
	srcQuad[1].x=(m_PointInImage[1].x);	srcQuad[1].y=(m_PointInImage[1].y);   
	srcQuad[2].x=(m_PointInImage[2].x);	srcQuad[2].y=(m_PointInImage[2].y);   
	srcQuad[3].x=(m_PointInImage[3].x);	srcQuad[3].y=(m_PointInImage[3].y);   

	dstQuad[0].x=(minx);		dstQuad[0].y=(miny);   
	dstQuad[1].x=(minx);		dstQuad[1].y=(maxy);   
	dstQuad[2].x=(maxx);	dstQuad[2].y=(maxy);       
	dstQuad[3].x=(maxx);	dstQuad[3].y=(miny); 	

	cvGetPerspectiveTransform(srcQuad, dstQuad, warp_matrix);
	cvWarpPerspective(m_srcRGB, m_dstRGB, warp_matrix);

	return rtAera;
}
