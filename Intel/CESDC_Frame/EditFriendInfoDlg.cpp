// EditFriendInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "EditFriendInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditFriendInfoDlg dialog


CEditFriendInfoDlg::CEditFriendInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFriendInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditFriendInfoDlg)
	m_IP = _T("");
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void CEditFriendInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFriendInfoDlg)
	DDX_Text(pDX, IDC_EDIT_IP, m_IP);
	DDV_MaxChars(pDX, m_IP, 20);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditFriendInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CEditFriendInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditFriendInfoDlg message handlers
