// MyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CVPlayFoundation.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog


CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDialog)
	m_strList = _T("");
	//}}AFX_DATA_INIT
}


void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialog)
	DDX_Control(pDX, IDC_LIST_TEST, m_mylist);
	DDX_LBString(pDX, IDC_LIST_TEST, m_strList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	//{{AFX_MSG_MAP(CMyDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialog message handlers

BOOL CMyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_mylist.AddString("A");
	m_mylist.AddString("BB");
	((CListBox *)GetDlgItem(IDC_LIST_TEST))->AddString("CCC");
	((CListBox *)GetDlgItem(IDC_LIST_TEST))->AddString("DDDD");
    m_mylist.SetCurSel(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyDialog::OnButtonChange() 
{
	// TODO: Add your control notification handler code here
	static CRect rectLarge,rectSmall;
	if(rectLarge.IsRectEmpty())
	{
		this->GetWindowRect(&rectLarge);
		rectSmall.left=rectLarge.left ;
		rectSmall.top=rectLarge.top ;
		rectSmall.right=rectLarge.right ;
		CRect rect;
		GetDlgItem(IDC_FRAME)->GetWindowRect(&rect);
		rectSmall.bottom =rect.bottom ;
	}

	CString str;
	GetDlgItem(IDC_BUTTON_CHANGE)->GetWindowText(str);
	if(str=="ÊÕËõ <<")
	{
		GetDlgItem(IDC_BUTTON_CHANGE)->SetWindowText("À©Õ¹ >>");
	    SetWindowPos(NULL,rectSmall.left,rectSmall.top,rectSmall.Width(),rectSmall.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHANGE)->SetWindowText("ÊÕËõ <<");
        MoveWindow(rectLarge.left,rectLarge.top,rectLarge.Width(),rectLarge.Height());
    }
	
}

void CMyDialog::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	m_mylist.AddString("EEEEE");
	MessageBox(this->m_strList);
//	this->UpdateData(FALSE);
}
