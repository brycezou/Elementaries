// TongBuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "TongBuDlg.h"
#include "FriendInfo.h"
#include "fstream.h"
#include "MyAddr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTongBuDlg dialog

CTongBuDlg::CTongBuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTongBuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTongBuDlg)
	//}}AFX_DATA_INIT
}


void CTongBuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTongBuDlg)
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTongBuDlg, CDialog)
	//{{AFX_MSG_MAP(CTongBuDlg)
	ON_WM_TIMER()
	ON_WM_COPYDATA()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTongBuDlg message handlers

CTongBuDlg *dlg;
BOOL CTongBuDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LONG Style=::GetWindowLong(this->m_hWnd,GWL_STYLE);
	Style &=~WS_CAPTION;
	::SetWindowLong(this->m_hWnd,GWL_STYLE,Style);

	this->ShowWindow(SW_SHOWMAXIMIZED);
	CRect wdrect;
	GetDesktopWindow()->GetClientRect(wdrect);
	m_height=wdrect.Height();
	m_width=wdrect.Width();
	OnInitialAsServer(32767);
    this->SetTimer(1,1,NULL);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


typedef struct transRecvMsg
{
	int type;
	unsigned long length;
}transRecvMsg;

CPtrArray pReceivedMember;
CString strMasterIP;
CFriendInfo *pInfo;
SOCKET socket2=0;
BOOL IsMeetingSlaveEnd=FALSE;
char slaveRecvBuffer[SLAVE_BUFFER_SIZE];
DWORD WINAPI OnListening(LPVOID lpParam)
{
	socket2=(int)lpParam;
	transRecvMsg recvhMsg;
	int bytes;
	unsigned long recvSizeNow=0;
	while(true)
	{
		if(IsMeetingSlaveEnd)
		{
			closesocket(socket2);		
			return 0;
		}
		bytes=recv(socket2,(char*)&recvhMsg,sizeof(recvhMsg),0);
		if(bytes==-1)
		{
			 //AfxMessageBox("连接主机失败!");
			 closesocket(socket2);		 
		     return 0;
		}
		else if(bytes!=sizeof(recvhMsg))
		{
           //AfxMessageBox("Received Error!");
		   closesocket(socket2);	   
		   return 0;
		}
		else if(recvhMsg.type==127)
		{
			recv(socket2,slaveRecvBuffer,recvhMsg.length,0);
			slaveRecvBuffer[recvhMsg.length]='\0';
		    pInfo=new CFriendInfo("",slaveRecvBuffer);
	        pReceivedMember.Add(pInfo);
		}
		else if(recvhMsg.type==15)
			break;
	}
if(recvhMsg.type==15)
{
	int nIndexMaster=pReceivedMember.GetSize();
	CFriendInfo fI=*(CFriendInfo*)pReceivedMember.GetAt(nIndexMaster-1);
    strMasterIP=fI.m_IP;
	ofstream outfile;
	outfile.open("strMasterIP.txt");
	outfile<<strMasterIP;
	outfile.close();

	CMyAddr myIp;
	ofstream outmemfile;
	outmemfile.open("strAllMemberIPs.txt");
	outmemfile<<nIndexMaster-1<<endl;
	for(int i=0;i<nIndexMaster;i++)
	{
	   CFriendInfo fInf=*(CFriendInfo*)pReceivedMember.GetAt(i);
       if(fInf.m_IP!=myIp.OnGetMyIP())
		   outmemfile<<(fInf.m_IP)<<endl;
	} 
	outmemfile<<myIp.OnGetMyIP()<<endl;
    outmemfile.close(); 
	pReceivedMember.RemoveAll();
}
	while(true)
	{
		if(IsMeetingSlaveEnd)
		{
			closesocket(socket2);
			break;
		}
		if(recvhMsg.type==15)
		{
			HANDLE hFile=CreateFile("C:\\Received.bmp",GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
			if(hFile==INVALID_HANDLE_VALUE)
			{
			//	AfxMessageBox("Create Receive.bmp file failed!");
			}
			unsigned long recvSizeTotal=0,b;
			while(recvSizeTotal<recvhMsg.length)
			{
				recvSizeNow=recv(socket2,slaveRecvBuffer,32768,0);
				if(recvSizeNow==-1)
				{
			        //AfxMessageBox("会议已结束!");
		            closesocket(socket2);
					CloseHandle(hFile);
			        return 0;
				}
				WriteFile(hFile,slaveRecvBuffer,recvSizeNow,&b,0);
				recvSizeTotal+=recvSizeNow;
				if(IsMeetingSlaveEnd)
				{
		         	closesocket(socket2);	
					CloseHandle(hFile);
		        	return 0;
				}
			}
            CloseHandle(hFile);
			while(!::CopyFile("C:\\Received.bmp","C:\\OnShow.bmp",FALSE))
			{	 
			   	//AfxMessageBox("复制图片失败!");
				if(IsMeetingSlaveEnd)
				{
		         	closesocket(socket2);	
		        	return 0;
				}
			}
			dlg->OnShowPicture();
		}

		bytes=recv(socket2,(char*)&recvhMsg,sizeof(recvhMsg),0);
		if(bytes==-1)
		{
			 //AfxMessageBox("会议已结束!");
		     closesocket(socket2);		 
			 break;
		}
	}  
	return 1;
} 

DWORD WINAPI AcceptSock(LPVOID lpParam)
{
	SOCKET temp,socket;
	socket=(int)lpParam;
	while(true)
	{
		temp=accept(socket,NULL,NULL);
		IsMeetingSlaveEnd=FALSE;
		if(socket2!=0)
		{
			closesocket(socket2);
            ::DeleteFile("C:\\Received.bmp");
	        ::DeleteFile("C:\\OnShow.bmp");
		}
		CreateThread(NULL,0,OnListening,(void*)temp,0,0);
	}
	closesocket(socket);
	return 1;
}

BOOL CTongBuDlg::OnInitialAsServer(int recvport)
{
	WSADATA wsadata;
 	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
 	{
 		AfxMessageBox("WSAStartup failed!");
 		return FALSE;
 	}
 	m_socksev=socket(AF_INET,SOCK_STREAM,0);
 	while(m_socksev==INVALID_SOCKET)
 	{
 		if(AfxMessageBox("m_socksev initial failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 
 	sockaddr_in sin;
 	sin.sin_port=htons(recvport);
 	sin.sin_family=AF_INET;
 	sin.sin_addr.s_addr=0;
 	while(bind(m_socksev,(sockaddr*)&sin,sizeof(sockaddr))==SOCKET_ERROR)
 	{
 		if(AfxMessageBox("Bind failed!\nTry again?",MB_YESNO)==IDNO)
 		return FALSE;
 	}
 	listen(m_socksev,200);
 
 	HANDLE handle=CreateThread(NULL,0,AcceptSock,(void*)m_socksev,0,0);
 	if(!handle)
 		MessageBox("CreateThread failed",0,0);

 	return TRUE;
}

void CTongBuDlg::OnShowPicture()
{
	hBitmap=(HBITMAP)LoadImage(NULL,"C:\\OnShow.bmp",IMAGE_BITMAP,m_width,m_height,LR_LOADFROMFILE);
	m_Picture.SetBitmap(hBitmap);
	DeleteObject(hBitmap);
}

void CTongBuDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	this->KillTimer(1);
    dlg=this;

	CDialog::OnTimer(nIDEvent);
}

extern HANDLE hNoteBook;
BOOL CTongBuDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CString strMsg=(LPCSTR)(pCopyDataStruct->lpData);
	if(strMsg=="MeetingStoped!")
	{
		this->SendMessage(WM_CLOSE);
	}
	else if(strMsg=="ReadytoChange?")
	{
		ofstream ofst;
		ofst.open("theChangingRoles.txt");
		ofst<<4<<endl;
		ofst.close();

		if(hNoteBook)
			TerminateProcess(hNoteBook,0);

		hNoteBook=NULL;

		this->SendMessage(WM_CLOSE);
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CTongBuDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	IsMeetingSlaveEnd=TRUE;
	closesocket(socket2);
    closesocket(m_socksev);
	WSACleanup();

	CDialog::OnClose();
}

void CTongBuDlg::OnDestroy() 
{
	IsMeetingSlaveEnd=TRUE;
	closesocket(socket2);	
    closesocket(m_socksev);
	WSACleanup();

	OnCreateMyDoctor();
	this->GetParent()->SendMessage(WM_CLOSE);
	CDialog::OnDestroy();
}

void CTongBuDlg::OnCreateMyDoctor()
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
