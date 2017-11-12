
// IDCardDlg.cpp : ʵ���ļ�
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


// CIDCardDlg �Ի���
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


// CIDCardDlg ��Ϣ�������
BOOL CIDCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_srcRGB=NULL;
	m_dstRGB=NULL;
	m_intPointNum=0;
	m_bIsLoaded=FALSE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CIDCardDlg::OnPaint()
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

	if (m_srcRGB!=NULL)						//�ػ�����ͼ��
		OnShowImage(m_srcRGB, IDC_IMAGE_IN);

	for (int i=0; i<m_intPointNum; i++)		//�ػ��û�ѡȡ�ĵ�
	{
		OnMyDrawLine(m_ptArray[i], m_ptArray[i], 6);
	}

	if(m_intPointNum == 4)						//�ػ�߽���
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


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CIDCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��������ͼ��
void CIDCardDlg::OnBnClickedButtonLoadImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog hFileDlg(		TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,
										TEXT(	"����֧�ֵ�ͼ���ļ� (*.jpg;*.gif;*.bmp;*.tif;*.png...)|*.jpg;*.gif;*.bmp;*.tif;*.png; \
													 |BMPͼ���ļ�(*.bmp)|*.bmp|JPEGͼ���ļ�(*.jpg)|*.jpg|PNGͼ���ļ�(*.png)|*.png; \
													 |Tiffͼ���ļ�(*.tiff;*.tif)|*.tiff;*.tif||" ), NULL );
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof( OPENFILENAME );
	hFileDlg.m_ofn.lpstrTitle = TEXT( "��ͼ���ļ�...\0" );
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


//��MFC�ؼ�����ʾIplImage* ��ʽ��ͼƬ
void CIDCardDlg::OnShowImage(IplImage * img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();		// �����ʾ�ؼ���DC
	HDC hdc = pDC->GetSafeHdc();					// ��ȡ HDC(�豸���) �����л�ͼ����
	CRect rect;
	GetDlgItem(ID)->GetClientRect( &rect );
	CvvImage cimg;
	cimg.CopyOf( img );										 // ����ͼƬ
	cimg.DrawToHDC( hdc, &rect );					 // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
	cimg.Destroy();
}


//���û�ѡ������������̶���С
IplImage* MyResizeInputImage(IplImage *src)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(498, 308), src->depth, src->nChannels);	//������С���Ŀ��ͼ��
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}


//Ѱ�Ҷ�ֵͼ��srcBinary�п����漰�����ֵ�blobs
//�����Ƴ��ľ�ȷ���򷵻ص�rtAccurate��
IplImage* GetBlobs(IplImage *srcBinary, CvRect &rtAccurate)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	bool IsFind=false;

	IplImage *filtered_blob=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );			//Ѱ�Ҷ�ֵͼ�е���ͨ��
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, filtered_blob );		

	int minx=10000, maxx=-1, miny=10000, maxy=-1;
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();		//�õ���ǰ Blob ���������
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


//��������Ҷ�ͼ��srcGray������Ӧ����ֵͼ��
//�������ӳ�����ͼ��dstBinMat��
void getBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat)
{
	Mat srcGrayMat(srcGray);
	adaptiveThreshold(srcGrayMat, dstBinMat, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 7, 14);
}


//��������RGB��ɫͼ��srcRGB��OTSU��ֵͼ��
//�������ӳ�����ͼ��dstBinary��
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary)
{
	cvCvtColor(srcRGB, dstBinary, CV_BGR2GRAY); 
	cvThreshold(dstBinary, dstBinary, 25, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
}


//����ÿһ��С����gOrgRectBoxArray[]�ľ��������
//���������������ھ�ȷ����gAccurateRectBoxArray[]���������
//�õ��������ھ�ȷ����ľ�������gAbsoluteBoxRectArray
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
		gAbsoluteBoxRectArray[i].width=intBX;		//��ʵgAbsoluteBoxRectArray�д洢�Ĳ���������CvRect�ṹ�����Ǹýṹ�����ϽǺ����½ǵ�����ֵ
		gAbsoluteBoxRectArray[i].height=intBY;
	}
}


//�����������¼�������Ҫ����ѡ��
void CIDCardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bIsLoaded)
	{
		CRect rect;
		this->GetDlgItem(IDC_IMAGE_IN)->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		int dX=point.x-rect.left;
		int dY=point.y-rect.top;

		if(dX>=0 && dY>=0 && dX<=rect.Width() && dY<=rect.Height())		//�����λ������ͼ����ʱ
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
				MyGet4PtInImage(rect);										//����ĸ����ѡ�еĵ������Ӧ��ͼ���е�����
				MyGetSelectAeraImg(m_srcRGB, m_dstRGB);		//��ȡѡ�е��ı��������ڵ�ͼ��    
				//CvRect rtAera=MyGetCorrectImage();				//��ҵ�����е�͸�ӱ任����
				CvRect rtAera=MyGetCorrectImageByCV();			//OpenCV�е�͸�ӱ任����
				cvSetImageROI(m_dstRGB, rtAera);
				IplImage *ImgIDCard=MyResizeInputImage(m_dstRGB);
				cvResetImageROI(m_dstRGB);

				MyLabelAeras(ImgIDCard, cvGetSize(ImgIDCard).width, cvGetSize(ImgIDCard).height, FALSE);
				for (int i=0; i<NUM_BOX_ORG; i++)					//�õ��������ڵľ�ȷ����������ꣻ�洢��gAccurateRectBoxArray[]��
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
				getAbsoluteBoxRect();											//�õ��������ڵľ�ȷ���򣬾������ꣻ�洢��gAbsoluteBoxRectArray[]��

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


//����Ҽ������¼�������Ҫ��������㣬���²���
void CIDCardDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_intPointNum = 0;
	this->SendMessage(WM_PAINT);

	CDialogEx::OnRButtonDown(nFlags, point);
}


//������֮�仭һ���߶�
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


//����ĸ����ѡ�еĵ������Ӧ��ͼ���е�����
void CIDCardDlg::MyGet4PtInImage(CRect rect)
{
	//������ʾ����ʵ��ͼ��ĳߴ����
	double dbWidthRate = m_srcRGB->width/1.0/rect.Width();
	double dbHeightRate = m_srcRGB->height/1.0/rect.Height();
	for(int i = 0 ; i < 4 ; i++)
	{
		m_PointInImage[i].x = MIN(int(m_ptArray[i].x * dbWidthRate), m_srcRGB->width);
		m_PointInImage[i].y = MIN(int(m_ptArray[i].y * dbHeightRate), m_srcRGB->height);
	}
}


//��ȡѡ�е��ı��������ڵ�ͼ��     
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
				(dst->imageData+i*dst->widthStep)[3*j]=((uchar *)(src->imageData+i*src->widthStep))[3*j];			//��i�е�j�е�Rͨ��
				(dst->imageData+i*dst->widthStep)[3*j+1]=((uchar *)(src->imageData+i*src->widthStep))[3*j+1];	//��i�е�j�е�Gͨ��
				(dst->imageData+i*dst->widthStep)[3*j+2]=((uchar *)(src->imageData+i*src->widthStep))[3*j+2];	//��i�е�j�е�Bͨ��
			}
		}
	}
}


//�жϵ��Ƿ���͹�ı���������
BOOL CIDCardDlg::MyJudgePtInRect(CPoint P1, CPoint P2, CPoint P3, CPoint P4, CPoint Pt)
{
	double dTriangle = MyGetTriangleArea(P1, P2, Pt)+MyGetTriangleArea(P2, P3, Pt)+MyGetTriangleArea(P3, P4, Pt)+MyGetTriangleArea(P4, P1, Pt);   
	double dQuadrangle = MyGetTriangleArea(P1, P2, P3)+MyGetTriangleArea(P3, P4, P1);                

	if(fabs(dTriangle-dQuadrangle) < 1e-6 )
		return TRUE;
	else 
		return FALSE;
}


//�������������
double CIDCardDlg::MyGetTriangleArea(CPoint a, CPoint b, CPoint c)
{
	double result = fabs((a.x * b.y + b.x * c.y + c.x * a.y - b.x * a.y -c.x * b.y - a.x * c.y )/2.0);
	return result;
}


CvRect CIDCardDlg::MyGetCorrectImage(void)
{
	int minx, miny, maxx, maxy;			//���㱻��ͼ���һ����С����Ӿ���
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

	//͸�ӱ任 
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
	MyGetPerspectiveTransform(srcPoint, dstPoint, h);								//��ȡ˫���Կռ�任���� 
	MyWarpPerspective(m_dstRGB, srcPoint, dstPoint, h);     //�Ի���ͼ����ÿ���������˫���Կռ�任

	return rtAera;
}


void CIDCardDlg::MyGetPerspectiveTransform(CPoint * srcPoint, CPoint * dstPoint, double * h)
{
	double a[4][5];							//���ϵ��
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


//��˹��Ԫ�������Է�����
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

	//˫���Բ�ֵ��
	for( int j = dstPoint[0].y; j < dstPoint[2].y; j++ )
	{
		for ( int i =dstPoint[0].x; i < dstPoint[2].x; i++ )
		{
			double ti = h[0] * i + h[1] * j + h[2] * i * j + h[3];			//�����ʱ����
			double tj = h[4] * i + h[5] * j + h[6] * i * j + h[7];

			int ii = int(ti);		//����ǰ��Ť��ͼ��ĺ������������
			int jj = int(tj);
			double u = ti - int(ti);
			double v = tj - int(tj);

			(tImg->imageData+j*tImg->widthStep)[3*i] =		(1-u)*(1-v)*(dst->imageData+jj*dst->widthStep)[3*ii] +			//��jj�е�ii�е�Rͨ��
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


//OpenCV�е�͸�ӱ任
CvRect CIDCardDlg::MyGetCorrectImageByCV(void)
{	
	int minx, miny, maxx, maxy;			//���㱻��ͼ���һ����С����Ӿ���
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

	//͸�ӱ任 
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
