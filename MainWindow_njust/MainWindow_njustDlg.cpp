// MainWindow_njustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainWindow_njust.h"
#include "MainWindow_njustDlg.h"
#include "ParameterSetDlg.h"
#include "CatchShow.h"
#include <windowsx.h>
#include "json.h"
#include "fstream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWindow_njustDlg dialog
//#define WM_SHOW_PICTURE WM_USER+101

CMainWindow_njustDlg::CMainWindow_njustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainWindow_njustDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainWindow_njustDlg)
	m_addr = _T("");
	m_sysState = _T("");
	m_port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainWindow_njustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainWindow_njustDlg)
	DDX_Control(pDX, IDC_LIST_INSTRCTION, m_InstrList);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_addr);
	DDX_Text(pDX, IDC_EDIT_SYSTEM_STATE, m_sysState);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainWindow_njustDlg, CDialog)
	//{{AFX_MSG_MAP(CMainWindow_njustDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PARA, OnButtonPara)
	ON_BN_CLICKED(IDC_BUTTON_INSTRUCT, OnButtonInstruct)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_GRASP, OnButtonGrasp)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INSTRCTION, OnClickListInstrction)
	ON_MESSAGE(WM_SHOW_PICTURE,OnShowOnlyPicture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWindow_njustDlg message handlers
CMainWindow_njustDlg *pMainDlg;
BOOL whetherFirstCreate=FALSE;
BOOL CMainWindow_njustDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	pMainDlg=this;
	ifstream infile;
    char chIp[20]="";
	UINT uniPort;
	infile.open("serverInfo.njust");
	infile>>chIp;
	infile>>uniPort;
	if((CString)chIp=="")
	{
		this->m_addr="192.168.1.108";
		this->m_port=1400;
	}
	else
	{
		this->m_addr=(CString)chIp;
		this->m_port=uniPort;
	}
	infile.close();
	this->m_sysState=" 系统未连接服务器!";
	this->UpdateData(FALSE);
    whetherFirstCreate=TRUE;
	OnInitialListFrame();
	this->SetTimer(1,1,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainWindow_njustDlg::OnPaint() 
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
HCURSOR CMainWindow_njustDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL IsWindowExist=FALSE;
void CMainWindow_njustDlg::OnButtonPara() 
{
	// TODO: Add your control notification handler code here
//	CParameterSetDlg dlg;
//	dlg.DoModal();
	if(!IsWindowExist)
	{
		this->GetDlgItem(IDC_BUTTON_PARA)->EnableWindow(FALSE);
		pSdlg.Create(IDD_DIALOG_PARA_SET); 
        pSdlg.ShowWindow(SW_SHOW); 
	}
	else
	{
		this->GetDlgItem(IDC_BUTTON_PARA)->EnableWindow(FALSE);
		pSdlg.DestroyWindow();
		pSdlg.Create(IDD_DIALOG_PARA_SET); 
		pSdlg.ShowWindow(SW_SHOW);
	}
}

static CRect rectsmall,rectlarge;
void CMainWindow_njustDlg::OnButtonInstruct() 
{
	// TODO: Add your control notification handler code here
	MoveWindow(rectlarge.left ,rectlarge.top ,rectlarge.Width(),rectlarge.Height());
	this->GetDlgItem(IDC_BUTTON_INSTRUCT)->EnableWindow(FALSE);
	OnButtonRefresh();
}

SOCKET clientSocket=-2;
CString severIP="127.0.0.1";
unsigned int severPort=1400;
DWORD WINAPI OnConnectThread(LPVOID lpParam)
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);
	clientSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(severPort);
	sin.sin_addr.S_un.S_addr=inet_addr(severIP);
	while(connect(clientSocket,(sockaddr*)&sin,sizeof(sockaddr))==SOCKET_ERROR)
	{
		if(AfxMessageBox("Failed!\nTry again?",MB_YESNO)==IDNO)
			return 0;
	};
	ofstream outfile;
	outfile.open("serverInfo.njust");
	outfile<<severIP<<endl<<severPort;
	outfile.close();

    pMainDlg->SetTimer(2,1,NULL);

	return 0;
}

BOOL whetherConnected=FALSE;
void CMainWindow_njustDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	if(!whetherConnected)
	{
		this->UpdateData(TRUE);
		severIP=this->m_addr;
		severPort=this->m_port;
		HANDLE handle=CreateThread(NULL,0,OnConnectThread,NULL,0,0);
		if(!handle)
 			MessageBox("Creating OnConnectThread thread failed!",0,0);

		CloseHandle(handle);
	}
	else
	{
		closesocket(clientSocket);
	    this->m_sysState=" 系统未连接服务器!";
		whetherConnected=FALSE;
		this->GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText("系统连接");
		this->UpdateData(FALSE);
	}
}

char *cmd[]={
	"config.interval",
    "config.matting.RGB.R",
	"config.matting.RGB.G",
	"config.matting.RGB.B",
	"config.matting.seeds",
	"config.matting.similarity",
	"config.matting.border",
	"config.filling.block.size",
	"config.filling.block.num",
	"config.filling.border",
	"config.evaluation.border",
	"config.evaluation.labThreshold",
	"config.evaluation.iteration" ,
	"matting()",
	"filling()",
	"evaluating()" };

char *meaning[]={
	"设置处理时间间隔",
	"设置抠图初始图像R通道",
	"设置抠图初始图像G通道",
	"设置抠图初始图像B通道",
	"设置抠图初始种子点",
	"设置抠图相似性阈值",
	"设置抠图屏高比",
	"设置纹理合成块大小",
	"设置纹理合成块数目",
	"设置纹理合成屏高比",
	"设置颜色纠正屏高比",
	"设置颜色纠正阈值",
	"设置颜色纠正迭代次数",
	"抠图", 
	"纹理合成",
	"颜色纠正" };

char pInstraction[10240]="\0";
DWORD WINAPI OnRefreshListThread(LPVOID lpParam)
{
	memset(pInstraction, 0, 10240);///////////////////////////////////////////////////////////////////////////////////////
	char chId[5];
	CString strKey,strValue,strTemp;

	LV_ITEM new_item; 
 	new_item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
 	new_item.state=0;
 	new_item.stateMask=0;
	new_item.iImage=0;
	new_item.iItem=0;
	new_item.iSubItem=0;

	if(send(clientSocket,"@\n",strlen("@\n"),0)==SOCKET_ERROR)
	{
		AfxMessageBox("请求更新失败!");
		return 0;
	}
	if(recv(clientSocket,pInstraction,10240,0)==SOCKET_ERROR)
	{
		AfxMessageBox("请求更新失败!");
		return 0;
	}
//	AfxMessageBox(pInstraction);
	json_value *val=json_parse(pInstraction);
	if(val->type!=json_object)
	{
		AfxMessageBox("Not an json object!");
		return 0;
	}
	else
	{
		pMainDlg->m_InstrList.DeleteAllItems();
		for(UINT i=0;i<val->u.object.length;i++)
		{
			strKey.Format("%s", val->u.object.values[i].name);
			strValue.Format("%s",val->u.object.values[i].value->u.string.ptr);
		    for(int j=0;j<13;j++)
			{
				strTemp=strValue.Left(strValue.ReverseFind('='));
				if(strTemp==(CString)cmd[j])
				{
					for(int k=0;k<strKey.GetLength();k++)
					{
				        chId[k]=strKey.GetAt(k);
					}
			        chId[strKey.GetLength()]='\0';
					new_item.pszText=chId; //项目文本；
 	                pMainDlg->m_InstrList.InsertItem(&new_item); //向列中添加主项目；
 	                pMainDlg->m_InstrList.SetItemText(0,1,meaning[j]); //添加子项目；
				//	break;
				}
			} 
			for(j=13;j<16;j++)
			{
				if(strValue==(CString)cmd[j])
				{
					for(int k=0;k<strKey.GetLength();k++)
					{
				        chId[k]=strKey.GetAt(k);
					}
			        chId[strKey.GetLength()]='\0';
					new_item.pszText=chId; //项目文本；
 	                pMainDlg->m_InstrList.InsertItem(&new_item); //向列中添加主项目；
 	                pMainDlg->m_InstrList.SetItemText(0,1,meaning[j]); //添加子项目；
				//	break;
				}
			}
		}
	}
	return 0;
}

void CMainWindow_njustDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	HANDLE handle=CreateThread(NULL,0,OnRefreshListThread,NULL,0,0);
 	if(!handle)
 		MessageBox("Creating OnRefreshListThread thread failed!",0,0);
	
	CloseHandle(handle);	
}

void CMainWindow_njustDlg::OnInitialMain()
{
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
}

void CMainWindow_njustDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(whetherFirstCreate)
	{
		this->KillTimer(1);
	    OnInitialMain();
		whetherFirstCreate=FALSE;
	}
	else
	{
		this->KillTimer(2);
		CString str=" 系统已连接至服务器: "+severIP;
	    this->m_sysState=str;
		whetherConnected=TRUE;
		this->GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText("断开连接");
		this->UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMainWindow_njustDlg::OnButtonBack() 
{
	// TODO: Add your control notification handler code here
    SetWindowPos(NULL,rectsmall.left ,rectsmall.top ,rectsmall.Width (),rectsmall.Height (),SWP_NOZORDER|SWP_NOMOVE);
	this->GetDlgItem(IDC_BUTTON_INSTRUCT)->EnableWindow(TRUE);
}

CFtpConnection *pFtpConnection;	//连接对象
DWORD WINAPI OnConnectFtpThread(LPVOID lpParam)
{
	try 
	{ 
		//新建连接对象
		pFtpConnection=pMainDlg->m_pInetSession->GetFtpConnection(severIP,"njust","njust");
	} 
	catch(CInternetException *pEx) 
	{
		//获取错误
		TCHAR szError[1024];
		if(pEx->GetErrorMessage(szError,1024))
			AfxMessageBox(szError);
		else  
			AfxMessageBox("There was an exception!");
		pEx->Delete();
		return 0;
	}
	if(pFtpConnection==NULL)
	{
		AfxMessageBox("Ftp失败!");
		return 0;
	}
//	DeleteFile("Received.jpg");
	pFtpConnection->GetFile("\\cap.jpg","Received.jpg");
    pMainDlg->SendMessage(WM_SHOW_PICTURE);

	return 0;
}

void CMainWindow_njustDlg::OnButtonGrasp() 
{
	// TODO: Add your control notification handler code here
	DeleteFile("Received.jpg");
	m_pInetSession=new CInternetSession("MR",INTERNET_OPEN_TYPE_PRECONFIG);
	HANDLE handle=CreateThread(NULL,0,OnConnectFtpThread,NULL,0,0);
	if(!handle)
 		MessageBox("Creating OnConnectFtpThread thread failed!",0,0);

	CloseHandle(handle);
}

void CMainWindow_njustDlg::OnCaptureFrame()
{
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

	CFile file;
	BITMAPFILEHEADER bfh;
	bfh.bfReserved1=bfh.bfReserved2=0;
	bfh.bfType=((WORD)('M'<< 8)|'B');
	bfh.bfSize=54+size;
	bfh.bfOffBits=54;
	if(file.Open("OnShow.bmp",CFile::modeCreate|CFile::modeWrite))
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

void CMainWindow_njustDlg::OnInitialListFrame()
{
	LV_COLUMN col_num,col_instrucion; //先定义创建列表视图控件列；

	col_num.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	col_num.fmt=LVCFMT_LEFT; //列文本的对齐方式；
	col_num.cx=36;          //列的宽度；
	col_num.iSubItem=0;
	col_num.pszText="序号";   //列标题字符串；

    col_instrucion.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	col_instrucion.fmt=LVCFMT_LEFT;
	col_instrucion.cx=134;
	col_instrucion.iSubItem=0;
	col_instrucion.pszText="    指 令 队 列";

	m_InstrList.InsertColumn(0,&col_num); //插入列至控件中；
	m_InstrList.InsertColumn(1,&col_instrucion);
}

BOOL IsRunning=TRUE;
void CMainWindow_njustDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if(IsRunning)
	{
		CString strState;
		strState.Format("config.run=false\n");
	    char backId[5];
	    if(send(clientSocket,strState,strlen(strState),0)==SOCKET_ERROR)
		{
			AfxMessageBox("系统连接异常!");
			return;
		}
		Sleep(5);
		recv(clientSocket,backId,strlen(backId),0);
		IsRunning=FALSE;
		this->GetDlgItem(IDC_BUTTON_STOP)->SetWindowText("运行");
		AfxMessageBox("系统已暂停运行!");
	}
	else
	{
		CString strState;
		strState.Format("config.run=true\n");
	    char backId[5];
	    if(send(clientSocket,strState,strlen(strState),0)==SOCKET_ERROR)
		{
			AfxMessageBox("系统连接异常!");
			return;
		}
		Sleep(5);
		recv(clientSocket,backId,strlen(backId),0);
		IsRunning=TRUE;
		this->GetDlgItem(IDC_BUTTON_STOP)->SetWindowText("停止");
		AfxMessageBox("系统继续运行!");
	}
}

BOOL IfDeleteSelected=FALSE;
void CMainWindow_njustDlg::OnClickListInstrction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	IfDeleteSelected=TRUE;
	*pResult = 0;
}

void CMainWindow_njustDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	char answer[5];
	if(IfDeleteSelected)
	{	
	  int index=this->m_InstrList.GetNextItem(-1,LVNI_SELECTED);
	  if(index!=-1)
	  {
		  CString strId=m_InstrList.GetItemText(index,0);
		  CString str;
		  str.Format("-%s\n",strId);
		  if(send(clientSocket,str,strlen(str),0)==SOCKET_ERROR)
		  {
			  AfxMessageBox("请求更新失败!");
			  return;
		  }
		  Sleep(5);   
	      recv(clientSocket,answer,strlen(answer),0);
		  this->m_InstrList.DeleteItem(index);
	      IfDeleteSelected=FALSE;
	  }
	}
	else
		MessageBox("请选中一条指令!");

}

void CMainWindow_njustDlg::OnShowOnlyPicture()
{
	CCatchShow pShowDlg;
	pShowDlg.DoModal();
}
