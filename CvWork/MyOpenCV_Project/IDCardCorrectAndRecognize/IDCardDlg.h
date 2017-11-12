
// IDCardDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;


// CIDCardDlg �Ի���
class CIDCardDlg : public CDialogEx
{
// ����
public:
	CIDCardDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IDCARDCORRECTANDRECOGNIZE_DIALOG };

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
	afx_msg void OnBnClickedButtonLoadImage();

private:
	IplImage *m_srcRGB;
	IplImage *m_dstRGB;
	char m_strInFilePath[MAX_PATH];
	int m_intPointNum;
	CPoint m_ptArray[4];
	CPoint m_PointInImage[4];
	BOOL m_bIsLoaded;
	void OnShowImage(IplImage * img, UINT ID);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void OnMyDrawLine(CPoint startPt, CPoint stopPt, int lineWidth);
	BOOL MyJudgePtInRect(CPoint P1, CPoint P2, CPoint P3, CPoint P4, CPoint Pt);
	double MyGetTriangleArea(CPoint a, CPoint b, CPoint c);
	void MyGetPerspectiveTransform(CPoint * srcPoint, CPoint * dstPoint, double * h);
	double * MyGaussianEli(double * c, int n);
	void MyWarpPerspective(IplImage * dst, CPoint * srcPoint, CPoint * dstPoint, double * h);
	void MyGet4PtInImage(CRect rt);
	void MyGetSelectAeraImg(IplImage * src, IplImage * dst);
	CvRect MyGetCorrectImage(void);
	CvRect MyGetCorrectImageByCV(void);
};
