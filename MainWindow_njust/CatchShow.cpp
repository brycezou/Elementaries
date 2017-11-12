// CatchShow.cpp : implementation file
//

#include "stdafx.h"
#include "mainwindow_njust.h"
#include "CatchShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatchShow dialog

CCatchShow::CCatchShow(CWnd* pParent /*=NULL*/)
	: CDialog(CCatchShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatchShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCatchShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchShow)
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatchShow, CDialog)
	//{{AFX_MSG_MAP(CCatchShow)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchShow message handlers

BOOL CCatchShow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect wdrect;
	this->GetClientRect(wdrect);
	m_height=wdrect.Height();
	m_width=wdrect.Width();
    this->SetTimer(1,1,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCatchShow::OnShowPicture()
{
	CClientDC  dc(this); 
    Graphics  graphics(dc.GetSafeHdc());   //   Create   a   GDI+   graphics   object 
    CRect rt;
	this->GetClientRect(rt);
    Bitmap  bitmap(L"Received.jpg"); 
    graphics.DrawImage(&bitmap,rt.left,rt.top,rt.Width(),rt.Height()); 
}

void CCatchShow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	this->KillTimer(1);
	OnShowPicture();

	CDialog::OnTimer(nIDEvent);
}

void CCatchShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	OnShowPicture();
	// Do not call CDialog::OnPaint() for painting messages
}
