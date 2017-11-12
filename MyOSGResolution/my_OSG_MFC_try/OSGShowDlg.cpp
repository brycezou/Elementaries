// OSGShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "my_OSG_MFC_try.h"
#include "OSGShowDlg.h"
#include "afxdialogex.h"


// COSGShowDlg �Ի���

IMPLEMENT_DYNAMIC(COSGShowDlg, CDialogEx)

COSGShowDlg::COSGShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COSGShowDlg::IDD, pParent)
{

}

COSGShowDlg::~COSGShowDlg()
{
}

void COSGShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COSGShowDlg, CDialogEx)
END_MESSAGE_MAP()


// COSGShowDlg ��Ϣ�������


BOOL COSGShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_OSG = new CCoreOSG(GetDlgItem(IDC_RECTANGLE)->m_hWnd);
	m_OSG->InitOSG();
	m_OSGThreadHandle = (HANDLE)_beginthread(&CCoreOSG::Render, 0, m_OSG) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
