// MasterOrSlave.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "MasterOrSlave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMasterOrSlave dialog


CMasterOrSlave::CMasterOrSlave(CWnd* pParent /*=NULL*/)
	: CDialog(CMasterOrSlave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMasterOrSlave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMasterOrSlave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMasterOrSlave)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMasterOrSlave, CDialog)
	//{{AFX_MSG_MAP(CMasterOrSlave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMasterOrSlave message handlers

HANDLE hNoteBook;
void CMasterOrSlave::OnCancel() 
{
	// TODO: Add extra cleanup here
//	ShellExecute(NULL,_T("open"),_T("CESDC_NoteBook.exe"),NULL,NULL,SW_SHOW);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	//初始化变量
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;

	//打程序
	BOOL fRet=CreateProcess(NULL,
							"CESDC_NoteBook.exe",
							NULL,
							NULL,
							FALSE,
							NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
							NULL,
							NULL,
							&si,
							&pi);
	if(!fRet)
	{
	}
	else
	{
		hNoteBook=pi.hProcess;
	}

	CDialog::OnCancel();
}

BOOL CMasterOrSlave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LONG Style=::GetWindowLong(this->m_hWnd,GWL_STYLE);
	Style &=~WS_SYSMENU;
	::SetWindowLong(this->m_hWnd,GWL_STYLE,Style);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

