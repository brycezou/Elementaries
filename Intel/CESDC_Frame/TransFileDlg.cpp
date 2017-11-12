// TransFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "TransFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransFileDlg dialog


CTransFileDlg::CTransFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_FileName="";
}


void CTransFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransFileDlg)
	DDX_Control(pDX, IDC_LIST_FILE_PATH, m_ListFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransFileDlg, CDialog)
	//{{AFX_MSG_MAP(CTransFileDlg)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_FILE, OnButtonChooseFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransFileDlg message handlers

void CTransFileDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	char filename[MAX_PATH];
	DragQueryFile(hDropInfo,0,filename,MAX_PATH);
	this->m_ListFileName.DeleteAllItems();
	m_FileName=filename;
	this->m_ListFileName.InsertItem(0,m_FileName,0);

	CDialog::OnDropFiles(hDropInfo);
}

BOOL CTransFileDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->hwnd==m_ListFileName.m_hWnd) && (pMsg->message==WM_DROPFILES))
	{
		OnDropFiles((HDROP)pMsg->wParam);
		return TRUE;
	}
	//防止按Esc键关闭对话框
	if (pMsg->message==WM_KEYDOWN && (int)pMsg->wParam==VK_ESCAPE)
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTransFileDlg::OnButtonChooseFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fDlg (TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	if(fDlg.DoModal()==IDOK)
	{
		this->m_ListFileName.DeleteAllItems();
		m_FileName=fDlg.GetPathName();
		this->m_ListFileName.InsertItem(0,m_FileName,0);
	}	
}
