// MyAddr.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "MyAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyAddr dialog


CMyAddr::CMyAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CMyAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyAddr)
	m_EditMyIp = _T("");
	//}}AFX_DATA_INIT
}


void CMyAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyAddr)
	DDX_Text(pDX, IDC_EDIT_MY_IPADDR, m_EditMyIp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyAddr, CDialog)
	//{{AFX_MSG_MAP(CMyAddr)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyAddr message handlers

BOOL CMyAddr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetTimer(1,1,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyAddr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	this->KillTimer(1);
	this->m_EditMyIp=OnGetMyIP();
	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}

CString CMyAddr::OnGetMyIP()
{
	char szhostname[128];
    CString str;
	//获得主机名
	if(gethostname(szhostname,128)==0)
	{
		// 获得主机ip地址
		struct hostent * phost;
		int i;
		phost=gethostbyname(szhostname);
		i=0;
		int j;
		int h_length=4;
		for(j=0;j<h_length;j++)
		{
			CString addr;		
			if(j>0)
				str+=".";
			
			addr.Format("%u", (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
			str+=addr;
		}
	}
	return str;
}
