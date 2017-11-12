
// my_OSG_MFC_tryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "my_OSG_MFC_try.h"
#include "my_OSG_MFC_tryDlg.h"
#include "afxdialogex.h"
#include "OSGShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C_OSG_MFC_Dlg �Ի���

C_OSG_MFC_Dlg::C_OSG_MFC_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C_OSG_MFC_Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C_OSG_MFC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C_OSG_MFC_Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON, &C_OSG_MFC_Dlg::OnBnClickedButton)
END_MESSAGE_MAP()


// C_OSG_MFC_Dlg ��Ϣ�������
BOOL C_OSG_MFC_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C_OSG_MFC_Dlg::OnPaint()
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
HCURSOR C_OSG_MFC_Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C_OSG_MFC_Dlg::OnBnClickedButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COSGShowDlg dlg;
	dlg.DoModal();
}
