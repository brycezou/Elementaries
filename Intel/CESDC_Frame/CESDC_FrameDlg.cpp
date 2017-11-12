// CESDC_FrameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "CESDC_FrameDlg.h"
#include "EditFriendInfoDlg.h"
#include "FriendInfo.h"
#include "fstream.h"
#include "MasterOrSlave.h"
#include "TongBuDlg.h"
#include "math.h"
#include "MyAddr.h"
#include "TransFileDlg.h"
#include <tlhelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCESDC_FrameDlg dialog

CCESDC_FrameDlg::CCESDC_FrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCESDC_FrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCESDC_FrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCESDC_FrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCESDC_FrameDlg)
	DDX_Control(pDX, IDC_LIST_MEMBER_NAME, m_memberlist);
	DDX_Control(pDX, IDC_LIST_FRIEND_NAME, m_friendnamelist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCESDC_FrameDlg, CDialog)
	//{{AFX_MSG_MAP(CCESDC_FrameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD_FRIEND, OnButtonAddFriend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DELETE_FRIEND, OnButtonDeleteFriend)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FRIEND_NAME, OnClickListFriendName)
	ON_BN_CLICKED(IDC_BUTTON_MEETING, OnButtonMeeting)
	ON_BN_CLICKED(IDC_BUTTON_STOP_MEETING, OnButtonStopMeeting)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MEMBER, OnButtonAddMember)
	ON_BN_CLICKED(IDC_BUTTON_START_MEETING, OnButtonStartMeeting)
	ON_BN_CLICKED(IDC_BUTTON_SEND_FILE, OnButtonSendFile)
	ON_BN_CLICKED(IDC_BUTTON_MY_ID, OnButtonMyId)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MEMBER, OnButtonDeleteMember)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MEMBER_NAME, OnClickListMemberName)
	ON_COMMAND(ID_MENU_EXIT,OnMenuExit)
	ON_COMMAND(ID_MENU_SHOW,OnMenuShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCESDC_FrameDlg message handlers
CCESDC_FrameDlg *pDlg;
CPtrArray pfriendlist;
BOOL CCESDC_FrameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
//  OnBeginWhenStart();
    ShellExecute(NULL,_T("open"),_T("C:\\CESDC_Initial\\ArCoreDllClient.exe"),NULL,NULL,SW_HIDE);
    ShellExecute(NULL,_T("open"),_T("C:\\CESDC_Initial\\CESDC_light_Adjust.exe"),NULL,NULL,SW_HIDE);
	if(!OnFindProcessName("ZoomIt.exe"))
		ShellExecute(NULL,_T("open"),_T("C:\\CESDC_Initial\\ZoomIt.exe"),NULL,NULL,SW_HIDE);

	m_NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
    m_NotifyIcon.hWnd=this->GetSafeHwnd();
    strcpy(m_NotifyIcon.szTip,_T("CESDC_Frame"));
    m_NotifyIcon.uCallbackMessage=WM_ICONMESSAGE;
    m_NotifyIcon.uFlags=NIF_MESSAGE|NIF_TIP|NIF_ICON;
    m_NotifyIcon.hIcon=m_hIcon;
	m_NotifyIcon.uID=IDR_MAINFRAME;
    Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon);

	int Width=GetSystemMetrics(SM_CXSCREEN);
	int Height=GetSystemMetrics(SM_CYSCREEN);
	CRect wdrct;
	this->GetWindowRect(&wdrct);
	SetWindowPos(NULL,Width-wdrct.Width()-5,Height-wdrct.Height()-43,0,0,SWP_NOSIZE|SWP_NOZORDER);
	OnInitialListFrame();
	OnInitialNameList();
	OnOpenReceiveFileSocket(65535);
	OnOpenAsServer(30000);
	this->SetTimer(1,1,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCESDC_FrameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID==SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCESDC_FrameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCESDC_FrameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnInitialListFrame()
{
	LV_COLUMN col_name,col_ip,col_member; //先定义创建列表视图控件列；

	col_name.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	col_name.fmt=LVCFMT_CENTER; //列文本的对齐方式；
	col_name.cx=99;          //列的宽度；
	col_name.iSubItem=0;
	col_name.pszText="  好友姓名";   //列标题字符串；

    col_ip.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	col_ip.fmt=LVCFMT_CENTER;
	col_ip.cx=103;
	col_ip.iSubItem=0;
	col_ip.pszText="好友IP";

	col_member.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	col_member.fmt=LVCFMT_CENTER; //列文本的对齐方式；
	col_member.cx=99;          //列的宽度；
	col_member.iSubItem=0;
	col_member.pszText="  参与人员";   //列标题字符串；

	m_friendnamelist.InsertColumn(0,&col_name); //插入列至控件中；
	m_friendnamelist.InsertColumn(1,&col_ip);
	m_memberlist.InsertColumn(0,&col_member);
    m_memberlist.InsertColumn(1,&col_ip);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnInitialNameList()
{
	int sum=0;
	char strName[20]="\0";
    char strIP[20]="\0";
	ifstream inNumber;
    inNumber.open("Total Number.txt");
	inNumber>>sum;
	inNumber.close();

    LV_ITEM new_item; 
	new_item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	new_item.state=0;
	new_item.stateMask=0;
	new_item.iImage=0;
	new_item.iItem=0;
	new_item.iSubItem=0;

    ifstream inTotal;
	inTotal.open("Total Information.txt");
	for(int i=0;i<sum;i++)
	{
	   inTotal>>strName;
	   inTotal>>strIP;

       new_item.pszText=strName; //项目文本；
	   m_friendnamelist.InsertItem(&new_item); //向列中添加主项目；
	   m_friendnamelist.SetItemText(0,1,strIP); //添加子项目；

	   pfInfo=new CFriendInfo(strName,strIP);
	   pfriendlist.Add(pfInfo);

	}
	inTotal.close();

	int sdt;
	ifstream ifstate;
	ifstate.open("theChangingRoles.txt");
	ifstate>>sdt;
	ifstate.close();
	if(sdt==4)
	{
		char charip[20]="\0";
		char charname[20]="\0";
		int num;
		ifstream instate;
		instate.open("strAllMemberIPs.txt");
		instate>>num;
		for(int j=0;j<num;j++)
		{
			instate>>charip;
			new_item.pszText=charname; //项目文本；
	        m_memberlist.InsertItem(&new_item); //向列中添加主项目；
	        m_memberlist.SetItemText(0,1,charip); //添加子项目；
		}
        instate.close();

		ofstream ofstate;
	    ofstate.open("theChangingRoles.txt");
        ofstate<<0<<endl;
	    ofstate.close();

		this->GetDlgItem(IDC_BUTTON_ADD_MEMBER)->EnableWindow(FALSE);
	    this->GetDlgItem(IDC_BUTTON_DELETE_MEMBER)->EnableWindow(FALSE);
//		this->GetDlgItem(IDC_LIST_MEMBER_NAME)->EnableWindow(FALSE);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnButtonAddFriend() 
{
	// TODO: Add your control notification handler code here
	BOOL IsComplete=FALSE;
	WinExec(_T("osk.exe"),SW_SHOW);
    CEditFriendInfoDlg dlg;
	CString strname;
	CString strip;
    if(dlg.DoModal()==IDOK)
    {
	    strname=dlg.m_Name;
		strip=dlg.m_IP;
		if(strname.IsEmpty()||strip.IsEmpty())
		{
		    MessageBox("请输入完整的信息!");
		}
		else 
		{
			ofstream outTotal;
	        outTotal.open("Total Information.txt",ios::app);
	        outTotal<<strname<<'\t'<<strip<<'\n';
            outTotal.close();

	        pfInfo=new CFriendInfo(strname,strip);
	        pfriendlist.Add(pfInfo);

            ofstream outNumber;
	        outNumber.open("Total Number.txt");
	        outNumber<<pfriendlist.GetSize();
            outNumber.close();

			char charName[20]="\0";
		    for(int i=0;i<strname.GetLength();i++)
			{
			   charName[i]=strname.GetAt(i);
			}
		    charName[strname.GetLength()]='\0';

			LV_ITEM new_item; //然后定义创建添加项目；
 	        new_item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
 	        new_item.state=0;
 	        new_item.stateMask=0;
 	        new_item.iImage=0;
 	        new_item.iItem=0;
 	        new_item.iSubItem=0;
 	        new_item.pszText=charName; //项目文本；
 	        m_friendnamelist.InsertItem(&new_item); //向列中添加主项目；
 	        m_friendnamelist.SetItemText(0,1,strip); //添加子项目；
		}
	}	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IsChecked=FALSE;
void CCESDC_FrameDlg::OnButtonDeleteFriend() 
{
	// TODO: Add your control notification handler code here
	if(IsChecked)
	{	
	  int index=m_friendnamelist.GetNextItem(-1,LVNI_SELECTED);
	  if(index!=-1)
	  {
		CString str=m_friendnamelist.GetItemText(index,1);
	    int sum=pfriendlist.GetSize();
	    CFriendInfo pfInfo;
	    for(int i=0;i<sum;i++)
		{
	       pfInfo=*(CFriendInfo*)pfriendlist.GetAt(i);
	       if(pfInfo.m_IP==str)
		   {
			   pfriendlist.RemoveAt(i);
			   break;
		   }
		}
	
		ofstream outTotal;
	    outTotal.open("Total Information.txt");
		for(int j=0;j<sum-1;j++)
		{
			pfInfo=*(CFriendInfo*)pfriendlist.GetAt(j);
	        outTotal<<pfInfo.m_Name<<'\t'<<pfInfo.m_IP<<'\n';
		}
        outTotal.close();

		ofstream outNumber;
	    outNumber.open("Total Number.txt");
	    outNumber<<sum-1;
        outNumber.close();

        m_friendnamelist.DeleteItem(index);

	  }
	  IsChecked=FALSE;
	}
	else
		MessageBox("请选中一个好友！");
	
}
void CCESDC_FrameDlg::OnClickListFriendName(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	IsChecked=TRUE;
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int totalNumber;
BOOL IsSendFinish=TRUE;
BOOL IsMeetingMode=FALSE;
static CRect rectsmall,rectlarge;
void CCESDC_FrameDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(!IsMeetingMode)
	{
		this->KillTimer(1);
        if(rectsmall.IsRectEmpty())
		{
		    this->GetWindowRect(&rectlarge);
		    rectsmall.left=rectlarge.left;
		    rectsmall.top=rectlarge.top;
		    rectsmall.bottom=rectlarge.bottom;
		    CRect rect;
		    GetDlgItem(IDC_FRAME)->GetWindowRect(&rect);
		    rectsmall.right=rect.right;
		}
	    SetWindowPos(NULL,rectsmall.left ,rectsmall.top ,rectsmall.Width (),rectsmall.Height (),SWP_NOZORDER|SWP_NOMOVE);		
	    pDlg=this;
	}
	else
	{
		if(IsSendFinish)
			OnCaptureFrame();

	}
	CDialog::OnTimer(nIDEvent);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCESDC_FrameDlg::OnButtonMeeting() 
{
	// TODO: Add your control notification handler code here
	CMasterOrSlave dlg;
	int retval;
	if((retval=dlg.DoModal())==IDOK)
	{
		MoveWindow(rectlarge.left ,rectlarge.top ,rectlarge.Width(),rectlarge.Height());
		ofstream ofstate;
		ofstate.open("theChangingRoles.txt");
		ofstate<<1<<endl;
		ofstate.close();
	}
	else if(retval==IDCANCEL)
	{
		ShowWindow(SW_HIDE);

		ofstream ofstate;
		ofstate.open("theChangingRoles.txt");
		ofstate<<2<<endl;
		ofstate.close();

		CTongBuDlg tbdlg;
        tbdlg.DoModal();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL StopMasterThread=FALSE;
CPtrArray pSentMember;
void CCESDC_FrameDlg::OnButtonStopMeeting() 
{
	// TODO: Add your control notification handler code here
	StopMasterThread=TRUE;
	this->KillTimer(2);
	pSentMember.RemoveAll();
	WSACleanup();
	this->GetDlgItem(IDC_BUTTON_ADD_MEMBER)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTON_DELETE_MEMBER)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BUTTON_START_MEETING)->EnableWindow(TRUE);
	SetWindowPos(NULL,rectsmall.left ,rectsmall.top ,rectsmall.Width (),rectsmall.Height (),SWP_NOZORDER|SWP_NOMOVE);
	OnRestartWhenExitMeeting();
	this->SendMessage(WM_CLOSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnButtonAddMember() 
{
	// TODO: Add your control notification handler code here
	if(!IsChecked)
	{
		MessageBox("请选中一个好友！");
	}
	else
	{
		char strname[20]="\0";
	    int index=m_friendnamelist.GetNextItem(-1,LVNI_SELECTED);
	    if(index!=-1)
		{
		    CString csname=m_friendnamelist.GetItemText(index,0);
            CString csip=m_friendnamelist.GetItemText(index,1);
            for(int i=0;i<csname.GetLength();i++)
			{
				strname[i]=csname.GetAt(i);
			}
			strname[csname.GetLength()]='\0';

 	        LV_ITEM new_item; //然后定义创建添加项目；
 	        new_item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
 	        new_item.state=0;
 	        new_item.stateMask=0;
 	        new_item.iImage=0;
 	        new_item.iItem=0;
 	        new_item.iSubItem=0;
 	        new_item.pszText=strname; //项目文本；
 	        m_memberlist.InsertItem(&new_item); //向列中添加主项目；
 	        m_memberlist.SetItemText(0,1,csip); //添加子项目；
		}
		IsChecked=FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IsMemberChecked=FALSE;
void CCESDC_FrameDlg::OnButtonDeleteMember() 
{
	// TODO: Add your control notification handler code here
	if(IsMemberChecked)
	{	
	  int index=m_memberlist.GetNextItem(-1,LVNI_SELECTED);
	  if(index!=-1)
	  {
		CString str=m_memberlist.GetItemText(index,1);
        m_memberlist.DeleteItem(index);
	  }
	  IsMemberChecked=FALSE;
	}
	else
		MessageBox("请选中一个好友！");
	
}
void CCESDC_FrameDlg::OnClickListMemberName(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	IsMemberChecked=TRUE;
	*pResult=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct transSendMsg
{
	int type;
	unsigned long length;
}transSendMsg;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SOCKET sock[10];
BOOL IsPictureFull=TRUE;
char masterSendBuffer[10][MASTER_BUFFER_SIZE];
DWORD WINAPI OnConnectMembers(LPVOID lpParam)
{
	int num=(int)lpParam;
	CFriendInfo fInfo=*(CFriendInfo*)pSentMember.GetAt(num);
    WSADATA wsadata;
	while(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		if(StopMasterThread)
		{
			pSentMember.RemoveAll();
			return 0;
		}
	}
	while((sock[num]=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
	{
		if(StopMasterThread)
		{
			pSentMember.RemoveAll();
			closesocket(sock[num]);
			return 0;
		}
	}
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(32767);
	sin.sin_addr.S_un.S_addr=inet_addr(fInfo.m_IP);
	while(connect(sock[num],(sockaddr*)&sin,sizeof(sockaddr))==-1)
	{
		if(StopMasterThread)
		{
			pSentMember.RemoveAll();
			closesocket(sock[num]);
			return 0;
		}
	}
    transSendMsg sendMsg;
	unsigned long sendSize;
	for(int i=0;i<totalNumber+1;i++)
	{
		if(StopMasterThread)
		{
			pSentMember.RemoveAll();
			closesocket(sock[num]);
			return 0;
		}
		fInfo=*(CFriendInfo*)pSentMember.GetAt(i);
		sendMsg.type=127;
		sendMsg.length=strlen(fInfo.m_IP);
		send(sock[num],(const char *)&sendMsg,sizeof(sendMsg),0);
		send(sock[num],fInfo.m_IP,strlen(fInfo.m_IP),0);
	}
	while(true)
	{
		if(StopMasterThread)
		{
			pSentMember.RemoveAll();
			closesocket(sock[num]);
			return 0;
		}
		if(IsPictureFull)
		{
			IsSendFinish=FALSE;
        	HANDLE hFile=CreateFileA("Captured.bmp",GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	    	sendMsg.type=15;
	    	sendMsg.length=GetFileSize(hFile,NULL);
	    	send(sock[num],(const char *)&sendMsg,sizeof(sendMsg),0);
	    	while(true)
			{
				if(StopMasterThread)
				{
					pSentMember.RemoveAll();
					closesocket(sock[num]);
					return 0;
				}
		    	ReadFile(hFile,masterSendBuffer[num],32768,&sendSize,0);
		    	if(sendSize==0)
				{
					IsSendFinish=TRUE;
					masterSendBuffer[num][0]='\0';
					CloseHandle(hFile);
			    	break;
				}
		    	send(sock[num],masterSendBuffer[num],sendSize,0);
			}
			IsPictureFull=FALSE;
		}
	}	
	pSentMember.RemoveAll();
	closesocket(sock[num]);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnButtonStartMeeting() 
{
	// TODO: Add your control notification handler code here
	IsMeetingMode=TRUE;
	int i=0;
	CString strnum;
    CString str=m_memberlist.GetItemText(i,1);
    while(str!="")
	{
	    pfInfo=new CFriendInfo("",str);
	    pSentMember.Add(pfInfo);
		i++;
		str=m_memberlist.GetItemText(i,1);
	}
	if(i==0)
		MessageBox("请选择与会人员!");
	else
	{
		this->GetDlgItem(IDC_BUTTON_ADD_MEMBER)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON_DELETE_MEMBER)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON_START_MEETING)->EnableWindow(FALSE);
		OnCaptureFrame();
		totalNumber=pSentMember.GetSize();

		CMyAddr adrdlg;
	    pfInfo=new CFriendInfo("",adrdlg.OnGetMyIP());
	    pSentMember.Add(pfInfo);

		for(i=0;i<totalNumber;i++)
		{
			HANDLE handle=CreateThread(NULL,0,OnConnectMembers,(void*)i,0,0);
	        if(!handle)
				MessageBox("CreateThread i failed!",0,0);
		}
		ShowWindow(SW_HIDE);
		this->SetTimer(2,20,NULL);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD bitmapSize;
void CCESDC_FrameDlg::OnCaptureFrame()
{
/*	CRect wdrect;
	GetDesktopWindow()->GetClientRect(wdrect);				//获取屏幕的客户区域
	CDC* pDeskDC=GetDesktopWindow()->GetDC();		//获取桌面画布对象
	CDC  memDC;											//定义一个内存画布
	memDC.CreateCompatibleDC(pDeskDC);					//创建一个兼容的画布
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC,wdrect.Width(),wdrect.Height());	//创建兼容位图
	memDC.SelectObject(&bmp);							//选中位图对象	
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);
	memDC.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);

	bitmapSize=bitmap.bmWidthBytes*bitmap.bmHeight;
	LPSTR lpData=(LPSTR)GlobalAllocPtr(GPTR,bitmapSize);
	int panelsize=0;									//记录调色板大小
	if(bitmap.bmBitsPixel<16)							//判断是否为真彩色位图
		panelsize=pow(2,bitmap.bmBitsPixel*sizeof(RGBQUAD));

	BITMAPINFOHEADER *pBInfo=(BITMAPINFOHEADER*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
	pBInfo->biBitCount       = bitmap.bmBitsPixel;
	pBInfo->biClrImportant   = 0;
	pBInfo->biCompression    = 0;
	pBInfo->biHeight         = bitmap.bmHeight;
	pBInfo->biPlanes         = bitmap.bmPlanes;
	pBInfo->biSize           = sizeof(BITMAPINFO);
	pBInfo->biSizeImage      = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->biWidth          = bitmap.bmWidth;
	pBInfo->biXPelsPerMeter  = 0;
	pBInfo->biYPelsPerMeter  = 0;
	GetDIBits(memDC.m_hDC,bmp,0,pBInfo->biHeight,lpData,(BITMAPINFO*)pBInfo,DIB_RGB_COLORS);

	BITMAPFILEHEADER bfh;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType      = ((WORD)('M'<< 8)|'B');
	bfh.bfSize      = 54+bitmapSize;
	bfh.bfOffBits   = 54;
	CFile file;
	IsPictureFull=FALSE;
	if(file.Open("Captured.bmp",CFile::modeCreate|CFile::modeWrite))
	{
		file.WriteHuge(&bfh,sizeof(BITMAPFILEHEADER));
		file.WriteHuge(pBInfo,sizeof(BITMAPINFOHEADER));
		file.WriteHuge(lpData,bitmapSize);
		GlobalFreePtr(lpData);
		LocalFree(pBInfo);
		pDeskDC->DeleteDC();
		ReleaseDC(pDeskDC);
		bmp.DeleteObject();
		memDC.DeleteDC();
		file.Close();
		IsPictureFull=TRUE;
	}
*/
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	int Width=GetSystemMetrics(SM_CXSCREEN);
	int Height=GetSystemMetrics(SM_CYSCREEN);
	CDC tdc;
	tdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOld=tdc.SelectObject(&bmp);
	tdc.StretchBlt(0,0,Width,Height,&dc,0,0,Width,Height,SRCCOPY);
	tdc.SelectObject(pOld);

	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);
	DWORD size=bitmap.bmWidthBytes*bitmap.bmHeight;
	LPSTR lpData=(LPSTR)GlobalAllocPtr(GPTR,size);

	BITMAPINFOHEADER bih;
	bih.biBitCount=bitmap.bmBitsPixel;
	bih.biClrImportant=0;
	bih.biClrUsed=0;
	bih.biCompression=0;
	bih.biHeight=bitmap.bmHeight;
	bih.biPlanes=1;
	bih.biSize=sizeof(BITMAPINFOHEADER);
	bih.biSizeImage=size;
	bih.biWidth=bitmap.bmWidth;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	GetDIBits(dc,bmp,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);

	IsPictureFull=FALSE;
	CFile file;
	BITMAPFILEHEADER bfh;
	bfh.bfReserved1=bfh.bfReserved2=0;
	bfh.bfType=((WORD)('M'<< 8)|'B');
	bfh.bfSize=54+size;
	bfh.bfOffBits=54;
	if(file.Open("Captured.bmp",CFile::modeCreate|CFile::modeWrite))
	{
		file.WriteHuge(&bfh,sizeof(BITMAPFILEHEADER));
		file.WriteHuge(&bih,sizeof(BITMAPINFOHEADER));
		file.WriteHuge(lpData,size);
		GlobalFreePtr(lpData);
		DeleteObject(bmp);
	    DeleteObject(pOld);
	    DeleteDC(dc);
	    DeleteDC(tdc);
		GlobalFree(lpData);
		file.Close();
		IsPictureFull=TRUE;
	}
	else
	{
		GlobalFreePtr(lpData);
	    DeleteObject(bmp);
	    DeleteObject(pOld);
	    DeleteDC(dc);
	    DeleteDC(tdc);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnButtonSendFile() 
{
	// TODO: Add your control notification handler code here
	if(IsChecked)
	{
		CTransFileDlg trsDlg;
	    if(trsDlg.DoModal()==IDOK)
		{
		   if(trsDlg.m_FileName=="")
		   {
			   MessageBox("请选择发送的文件!");
		       return;
		   }
		   m_SendFileName=trsDlg.m_FileName;

		   int index=m_friendnamelist.GetNextItem(-1,LVNI_SELECTED);
	       if(index!=-1)
		   {
			   m_SendFileIP=m_friendnamelist.GetItemText(index,1);
			   OnSendFile();
		   }
		}
		IsChecked=FALSE;
	}
	else
		MessageBox("请选中一个好友!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct transFileMsg
{
	int type;
	unsigned long length;
} transFileMsg;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char RecvFileBuffer[FILE_BUFFER_SIZE];
DWORD WINAPI OnListeningFile(LPVOID lpParam)
{
	SOCKET sockRecvFile=(int)lpParam;
	transFileMsg recvfileMsg;
	int bytes;
	CString fileName;
	unsigned long recSize=0;

	bytes=recv(sockRecvFile,(char*)&recvfileMsg,sizeof(recvfileMsg),0);
    if(bytes==-1)
	{
		AfxMessageBox("接受连接失败!");
		closesocket(sockRecvFile);
		return 0;
	}
    else if(bytes!=sizeof(recvfileMsg))
	{
        AfxMessageBox("应答错误R0!");
		closesocket(sockRecvFile);
		return 0;
	}
    else if(recvfileMsg.type==0)
	{
		recv(sockRecvFile,RecvFileBuffer,recvfileMsg.length,0);
		RecvFileBuffer[recvfileMsg.length]='\0';
		if((CString)RecvFileBuffer=="SendFile")
		{
			if(AfxMessageBox("是否接收文件?", MB_YESNO)==IDYES)
			{
				BROWSEINFO bInfo;
			    bInfo.hwndOwner = pDlg->m_hWnd;
			    bInfo.pidlRoot  = NULL;
			    char filename[MAX_PATH];
		        char dir[MAX_PATH];
			    memset(filename,0,MAX_PATH);
			    memset(dir,0,MAX_PATH);
			
			    bInfo.pszDisplayName = filename;
			    bInfo.lpszTitle ="请选择文件的存储路径:";
			    bInfo.ulFlags = BIF_BROWSEINCLUDEFILES|BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS|BIF_BROWSEFORCOMPUTER;
			    bInfo.lParam  =NULL;
			    bInfo.lpfn  =NULL;
			    bInfo.iImage = 0;

			    LPITEMIDLIST lpList =  SHBrowseForFolder(&bInfo);
			
			    CString filedir;
		        if(SHGetPathFromIDList(lpList,dir))
				{
				    filedir = dir;
				    filedir += "\\";

				    recvfileMsg.type=0;
                    recvfileMsg.length=strlen("YES");
                    send(sockRecvFile,(const char *)&recvfileMsg,sizeof(recvfileMsg),0);
                    send(sockRecvFile,"YES",strlen("YES"),0);

  	                bytes=recv(sockRecvFile,(char*)&recvfileMsg,sizeof(recvfileMsg),0);
		            if(bytes==-1)
					{
					    AfxMessageBox("应答错误R2!");
					    closesocket(sockRecvFile);
		                return 0;
					}
	        	    else if(bytes!=sizeof(recvfileMsg))
					{
					    AfxMessageBox("应答错误R3!");
				        closesocket(sockRecvFile);
		                return 0;
					}
				    else if(recvfileMsg.type==0)
					{
				    	recv(sockRecvFile,RecvFileBuffer,recvfileMsg.length,0);
		                RecvFileBuffer[recvfileMsg.length]='\0';
					    fileName=RecvFileBuffer;
					}

			        bytes=recv(sockRecvFile,(char*)&recvfileMsg,sizeof(recvfileMsg),0);
		            if(bytes==-1)
					{
					    AfxMessageBox("应答错误R4!");
					    closesocket(sockRecvFile);
		                return 0;
					}
	        	    else if(bytes!=sizeof(recvfileMsg))
					{
					    AfxMessageBox("应答错误R5!");
					    closesocket(sockRecvFile);
		                return 0;
					}
					else if(recvfileMsg.type==1)
					{
		            	HANDLE hFile=CreateFile(filedir+fileName,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	            		if(hFile==INVALID_HANDLE_VALUE)
						{
		            		AfxMessageBox("Create file failed!");
		                    closesocket(sockRecvFile);
							return 0;
						}
	            		unsigned long recvSizeTotal=0,b;
	             		unsigned long recvSizeNow=0;
		            	while(recvSizeTotal<recvfileMsg.length)
						{
				            recvSizeNow=recv(sockRecvFile,RecvFileBuffer,32768,0);
				            WriteFile(hFile,RecvFileBuffer,recvSizeNow,&b,0);
			             	recvSizeTotal+=recvSizeNow;
						}
                        CloseHandle(hFile);
						closesocket(sockRecvFile);
						AfxMessageBox("文件接收完毕!");
					}
				}
				else
				{
					AfxMessageBox("您已取消接收!");
					closesocket(sockRecvFile);
					return 0;
				}
			}
	        else
			{
				recvfileMsg.type=0;
                recvfileMsg.length=strlen("NO");
                send(sockRecvFile,(const char *)&recvfileMsg,sizeof(recvfileMsg),0);
                send(sockRecvFile,"NO",strlen("NO"),0);
				closesocket(sockRecvFile);
				return 0;
			}
		}
		else
		{
			AfxMessageBox("应答错误R1!");
			closesocket(sockRecvFile);
            return 0;
		}
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI AcceptFileSock(LPVOID lpParam)
{
	SOCKET temp,socket;
	socket=(int)lpParam;
	while(true)
	{
		temp=accept(socket,NULL,NULL);
		CreateThread(NULL,0,OnListeningFile,(void*)temp,0,0);
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCESDC_FrameDlg::OnOpenReceiveFileSocket(int recvport)
{
	WSADATA wsadata;
 	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
 	{
 		AfxMessageBox("WSAStartup failed!");
 		return FALSE;
 	}
 	m_sockRecvFile=socket(AF_INET,SOCK_STREAM,0);
 	while(m_sockRecvFile==INVALID_SOCKET)
 	{
 		if(AfxMessageBox("m_sockRecvFile initial failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 
 	sockaddr_in sin;
 	sin.sin_port=htons(recvport);
 	sin.sin_family=AF_INET;
 	sin.sin_addr.s_addr=0;
 	while(bind(m_sockRecvFile,(sockaddr*)&sin,sizeof(sockaddr))==SOCKET_ERROR)
 	{
 		if(AfxMessageBox("Bind failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 	listen(m_sockRecvFile,1);
 
 	HANDLE handle=CreateThread(NULL,0,AcceptFileSock,(void*)m_sockRecvFile,0,0);
 	if(!handle)
 		MessageBox("CreateThread failed",0,0);
 
 	return TRUE;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString sendFileName;
CString sendFileIP;
char SendFileBuffer[FILE_BUFFER_SIZE];
DWORD WINAPI SendFileSock(LPVOID lpParam)
{
	SOCKET sockFile;
	WSADATA wsadata;
	while(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
	}
	while((sockFile=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
	{
	}
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(65535);
	sin.sin_addr.S_un.S_addr=inet_addr(sendFileIP);
	while(connect(sockFile,(sockaddr*)&sin,sizeof(sockaddr))==-1)
	{
	}	
	int bytes;
	unsigned long sendSize;
	transFileMsg fileMsg;

    fileMsg.type=0;
    fileMsg.length=strlen("SendFile");
    send(sockFile,(const char *)&fileMsg,sizeof(fileMsg),0);
    send(sockFile,"SendFile",strlen("SendFile"),0);

   	bytes=recv(sockFile,(char*)&fileMsg,sizeof(fileMsg),0);
	if(bytes==-1)
	{
		AfxMessageBox("创建连接失败!");
		closesocket(sockFile);
		return 0;
	}
	else if(bytes!=sizeof(fileMsg))
	{
        AfxMessageBox("对方已取消接收!");
	    closesocket(sockFile);
		return 0;
	}
	else if(fileMsg.type==0)
	{
		recv(sockFile,SendFileBuffer,fileMsg.length,0);
		SendFileBuffer[fileMsg.length]='\0';
     //	AfxMessageBox(SendFileBuffer);
		if((CString)SendFileBuffer=="NO")
		{
			AfxMessageBox("对方拒绝接收文件!");
			closesocket(sockFile);
			return 0;
		}
		else if((CString)SendFileBuffer=="YES")
		{
			CCESDC_FrameDlg cfDlg;
			CString fileName=cfDlg.OnGetFileName(sendFileName);
			fileMsg.type=0;
            fileMsg.length=strlen(fileName);
            send(sockFile,(const char *)&fileMsg,sizeof(fileMsg),0);
            send(sockFile,fileName,strlen(fileName),0);

			HANDLE hFile=CreateFileA(sendFileName,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
			fileMsg.type=1;
	    	fileMsg.length=GetFileSize(hFile,NULL);
	    	send(sockFile,(const char *)&fileMsg,sizeof(fileMsg),0);
			while(true)
			{
		    	ReadFile(hFile,SendFileBuffer,32768,&sendSize,0);
		    	if(sendSize==0)
				{
					CloseHandle(hFile);
					closesocket(sockFile);
					AfxMessageBox("文件发送完毕!");
			    	return 1;
				}
		    	send(sockFile,SendFileBuffer,sendSize,0);
			}
		}
		else
		{
			closesocket(sockFile);
			AfxMessageBox("未定义的应答信号!");
			return 0;
		}
	}
	else
	{
		closesocket(sockFile);
		AfxMessageBox("应答错误S1!");
		return 0;
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCESDC_FrameDlg::OnSendFile()
{
	sendFileName=m_SendFileName;
	sendFileIP=m_SendFileIP;
	HANDLE handle=CreateThread(NULL,0,SendFileSock,NULL,0,0);
 	if(!handle)
 		MessageBox("CreateThread failed",0,0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCESDC_FrameDlg::OnGetFileName(CString str)
{
	int pos=str.ReverseFind('\\');
	CString fName=str.Mid(pos+1,str.GetLength());
	return fName;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCESDC_FrameDlg::OnButtonMyId() 
{
	// TODO: Add your control notification handler code here
    CMyAddr adrDlg;
	adrDlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char AskAndAnswer[20];
DWORD WINAPI OnListeningRequest(LPVOID lpParam)
{
	SOCKET socket=(int)lpParam;
	transSendMsg sendMsg;
	int bytes;
	bytes=recv(socket,(char*)&sendMsg,sizeof(sendMsg),0);
    if(bytes==-1)
	{
		closesocket(socket);
		return 0;
	}
    else if(bytes!=sizeof(sendMsg))
	{
		closesocket(socket);
		return 0;
	}
    else if(sendMsg.type==127)
	{
	    recv(socket,AskAndAnswer,sendMsg.length,0);
		AskAndAnswer[sendMsg.length]='\0';
		CString str=(CString)AskAndAnswer;
		if(str=="wantobethehost")
		{
			pDlg->ShowWindow(SW_SHOW);
			if(AfxMessageBox("有人请求发言，同意?", MB_YESNO)==IDYES)
			{
             	StopMasterThread=TRUE;
             	pDlg->KillTimer(2);
             	pDlg->m_memberlist.DeleteAllItems();
				pSentMember.RemoveAll();
				sendMsg.type=127;
	            sendMsg.length=strlen("YES");
	            send(socket,(const char *)&sendMsg,sizeof(sendMsg),0);
	            send(socket,"YES",strlen("YES"),0);
				pDlg->GetDlgItem(IDC_BUTTON_STOP_MEETING)->EnableWindow(FALSE);
						
				ofstream ofstate;
		        ofstate.open("theChangingRoles.txt");
		        ofstate<<3<<endl;
		        ofstate.close();

				AfxMessageBox("请进入从机模式!");
			}
			else
			{
                pDlg->ShowWindow(SW_HIDE);
				sendMsg.type=127;
	            sendMsg.length=strlen("NO");
	            send(socket,(const char *)&sendMsg,sizeof(sendMsg),0);
	            send(socket,"NO",strlen("NO"),0);
			}
		}

	}
	closesocket(socket);
	return 1;
}

DWORD WINAPI AcceptRequest(LPVOID lpParam)
{
	SOCKET temp,socket;
	socket=(int)lpParam;
	while(true)
	{
		temp=accept(socket,NULL,NULL);
		CreateThread(NULL,0,OnListeningRequest,(void*)temp,0,0);
	}
	closesocket(socket);
	return 1;
}

BOOL CCESDC_FrameDlg::OnOpenAsServer(int transRecv)
{
	SOCKET sockrecv;
	WSADATA wsadata;
 	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
 	{
 		AfxMessageBox("WSAStartup failed!");
 		return FALSE;
 	}
 	sockrecv=socket(AF_INET,SOCK_STREAM,0);
 	while(sockrecv==INVALID_SOCKET)
 	{
 		if(AfxMessageBox("sockrecv initial failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 
 	sockaddr_in sin;
 	sin.sin_port=htons(transRecv);
 	sin.sin_family=AF_INET;
 	sin.sin_addr.s_addr=0;
 	while(bind(sockrecv,(sockaddr*)&sin,sizeof(sockaddr))==SOCKET_ERROR)
 	{
 		if(AfxMessageBox("Bind failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 	listen(sockrecv,20);

 	HANDLE handle=CreateThread(NULL,0,AcceptRequest,(void*)sockrecv,0,0);
 	if(!handle)
 		MessageBox("CreateThread failed",0,0);

 	return TRUE;

}

LRESULT CCESDC_FrameDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_ICONMESSAGE:
		if(lParam==WM_LBUTTONDOWN)
		{
			CMenu menu;
			CPoint point;
			GetCursorPos(&point);
			menu.CreatePopupMenu();
			if(IsWindowVisible())
			{
		       menu.AppendMenu(MF_STRING,ID_MENU_SHOW,_T("最小化到托盘"));
			}
	        else
			{
		       menu.AppendMenu(MF_STRING,ID_MENU_SHOW,_T("显示主界面"));
			}
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING,ID_MENU_EXIT,_T("退出"));
			menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this,NULL);
			menu.DestroyMenu();
		} break;
	default:break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CCESDC_FrameDlg::OnMenuShow()
{
	if(IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOW);
	}
}

void CCESDC_FrameDlg::OnMenuExit()
{
	SendMessage(WM_CLOSE,0,0);
}

void CCESDC_FrameDlg::OnRestartWhenExitMeeting()
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	//初始化变量
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW;

	//打程序
	BOOL fRet=CreateProcess(NULL,
							"CESDC_ServProcess.exe",
							NULL,
							NULL,
							FALSE,
							NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
							NULL,
							NULL,
							&si,
							&pi);

}

void CCESDC_FrameDlg::OnBeginWhenStart()
{
/*	HKEY RegKey; 
    CString sPath; 
    GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
    sPath.ReleaseBuffer(); 
    int nPos; 
    nPos=sPath.ReverseFind('\\'); 
    sPath=sPath.Left(nPos); 
    CString lpszFile=sPath+"\\CESDC_Frame.exe";//这里加上你要查找的执行文件名称 
    CFileFind fFind; 
    BOOL bSuccess; 
    bSuccess=fFind.FindFile(lpszFile); 
    fFind.Close(); 
    if(bSuccess) 
	{ 
       CString fullName; 
       fullName=lpszFile; 
       RegKey=NULL; 
       RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey); 
       RegSetValueEx(RegKey,"CESDC_Frame",0,REG_SZ,(const unsigned char*)(LPCTSTR)fullName,fullName.GetLength());//这里加上你需要在注册表中注册的内容 
	} 
*/
}

BOOL CCESDC_FrameDlg::OnFindProcessName(CString strPross)
{
	//初始化
	HANDLE hProcessSnap=NULL;
	PROCESSENTRY32 pe32={0};
	//获得句柄
	hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap==(HANDLE)-1)
	{
		CloseHandle(hProcessSnap);   
		return FALSE;
	}
	pe32.dwSize=sizeof(PROCESSENTRY32);
	//对比所有进程名称
	if(Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(pe32.szExeFile==strPross)
			{
				CloseHandle(hProcessSnap);   
			    return TRUE;
			}
		}
		while(Process32Next(hProcessSnap, &pe32));//直到列举完毕

		CloseHandle(hProcessSnap);   
	    return FALSE;
	}
	else
	{
		CloseHandle(hProcessSnap);   
		return FALSE;
	}
}
