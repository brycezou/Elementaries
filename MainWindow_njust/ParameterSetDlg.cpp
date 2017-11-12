// ParameterSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainWindow_njust.h"
#include "ParameterSetDlg.h"
#include "MainWindow_njustDlg.h"
#include <windowsx.h>
#include "json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParameterSetDlg dialog
//const UINT WM_MY_UPDATE=::RegisterWindowMessage(_T("User")); 

CParameterSetDlg::CParameterSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParameterSetDlg)
	m_blockNum = 0;
	m_blockSize = 0;
	m_correctNum = 0;
	m_lab = 0;
	m_editG = 0;
	m_editR = 0;
	m_scr2Height_1 = 0.0;
	m_scr2Height_2 = 0.0;
	m_scr2Height_3 = 0.0;
	m_similarity = 0.0;
	m_editB = 0;
	m_deltTime = 0;
	//}}AFX_DATA_INIT
}


void CParameterSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParameterSetDlg)
	DDX_Control(pDX, IDC_BITMAP, m_Bitmap);
	DDX_Control(pDX, IDC_LIST_SEED, m_seedList);
	DDX_Control(pDX, IDC_SLIDER_R, m_sR);
	DDX_Control(pDX, IDC_SLIDER_G, m_sG);
	DDX_Control(pDX, IDC_SLIDER_B, m_sB);
	DDX_Text(pDX, IDC_EDIT_BLOCK_NUM, m_blockNum);
	DDX_Text(pDX, IDC_EDIT_BLOCK_SIZE, m_blockSize);
	DDX_Text(pDX, IDC_EDIT_CORRECT_NUM, m_correctNum);
	DDX_Text(pDX, IDC_EDIT_LAB, m_lab);
	DDX_Text(pDX, IDC_EDIT_G, m_editG);
	DDV_MinMaxInt(pDX, m_editG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_R, m_editR);
	DDV_MinMaxInt(pDX, m_editR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SCREEN2HEIGHT_1, m_scr2Height_1);
	DDX_Text(pDX, IDC_EDIT_SCREEN2HEIGHT_2, m_scr2Height_2);
	DDX_Text(pDX, IDC_EDIT_SCREEN2HEIGHT_3, m_scr2Height_3);
	DDX_Text(pDX, IDC_EDIT_SIMILARITY, m_similarity);
	DDX_Text(pDX, IDC_EDIT_B, m_editB);
	DDV_MinMaxInt(pDX, m_editB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_TIME, m_deltTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParameterSetDlg, CDialog)
	//{{AFX_MSG_MAP(CParameterSetDlg)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_R, OnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_G, OnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_B, OnChangeEditB)
	ON_BN_CLICKED(IDC_BUTTON_SEND_PARA, OnButtonSendPara)
	ON_BN_CLICKED(IDC_BUTTON_GRAPS, OnButtonGraps)
	ON_REGISTERED_MESSAGE(WM_MY_UPDATE, OnMyUpdate)
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT_BLOCK_SIZE, OnChangeEditBlockSize)
	ON_EN_CHANGE(IDC_EDIT_BLOCK_NUM, OnChangeEditBlockNum)
	ON_EN_CHANGE(IDC_EDIT_SCREEN2HEIGHT_1, OnChangeEditScreen2height1)
	ON_EN_CHANGE(IDC_EDIT_LAB, OnChangeEditLab)
	ON_EN_CHANGE(IDC_EDIT_CORRECT_NUM, OnChangeEditCorrectNum)
	ON_EN_CHANGE(IDC_EDIT_SCREEN2HEIGHT_2, OnChangeEditScreen2height2)
	ON_EN_CHANGE(IDC_EDIT_SIMILARITY, OnChangeEditSimilarity)
	ON_EN_CHANGE(IDC_EDIT_SCREEN2HEIGHT_3, OnChangeEditScreen2height3)
	ON_EN_CHANGE(IDC_EDIT_TIME, OnChangeEditTime)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, OnButtonDeleteAll)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SHOW_PICTURE,OnShowOnlyPicture)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameterSetDlg message handlers
CParameterSetDlg *pParaDlg;
BOOL IfFirst=FALSE;
BOOL CParameterSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	pParaDlg=this;
	this->GetWindowRect(&rtMain);
    OnInitial();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern SOCKET clientSocket;
extern char *cmd[];
char pParas[10240]="\0";
DWORD WINAPI OnRefreshParaThread(LPVOID lpParam)
{
	memset(pParas, 0, 10240);///////////////////////////////////////////////////////////////////////////////////////
	CString strKey,strValue;
	if(send(clientSocket,"?\n",strlen("?\n"),0)==SOCKET_ERROR)
	{
		pParaDlg->m_sR.SetRange(0,255);
		pParaDlg->m_sG.SetRange(0,255);
	    pParaDlg->m_sB.SetRange(0,255);
		pParaDlg->m_blockSize=14;
		pParaDlg->m_blockNum=100;
		pParaDlg->m_scr2Height_1=0.2;
		pParaDlg->m_lab=2;
		pParaDlg->m_correctNum=4;
		pParaDlg->m_scr2Height_2=0.01;
		pParaDlg->m_editR=255;
		pParaDlg->m_editG=0;
		pParaDlg->m_editB=0;
		pParaDlg->m_similarity=4;
		pParaDlg->m_scr2Height_3=0.05;
		pParaDlg->m_seedList.AddString("300 250");
		pParaDlg->m_seedList.AddString("400 270");
		pParaDlg->m_sR.SetPos(pParaDlg->m_editR);
		pParaDlg->m_sG.SetPos(pParaDlg->m_editG);
		pParaDlg->m_sB.SetPos(pParaDlg->m_editB);
		pParaDlg->m_deltTime=60;
		pParaDlg->SendMessage(WM_MY_UPDATE);
		AfxMessageBox("更新参数失败!");
		return 0;
	}
	if(recv(clientSocket,pParas,10240,0)==SOCKET_ERROR)
	{
		pParaDlg->m_sR.SetRange(0,255);
		pParaDlg->m_sG.SetRange(0,255);
	    pParaDlg->m_sB.SetRange(0,255);
		pParaDlg->m_blockSize=14;
		pParaDlg->m_blockNum=100;
		pParaDlg->m_scr2Height_1=0.2;
		pParaDlg->m_lab=2;
		pParaDlg->m_correctNum=4;
		pParaDlg->m_scr2Height_2=0.01;
		pParaDlg->m_editR=255;
		pParaDlg->m_editG=0;
		pParaDlg->m_editB=0;
		pParaDlg->m_similarity=4;
		pParaDlg->m_scr2Height_3=0.05;
		pParaDlg->m_seedList.AddString("300 250");
		pParaDlg->m_seedList.AddString("400 270");
		pParaDlg->m_sR.SetPos(pParaDlg->m_editR);
		pParaDlg->m_sG.SetPos(pParaDlg->m_editG);
		pParaDlg->m_sB.SetPos(pParaDlg->m_editB);
		pParaDlg->m_deltTime=60;
		pParaDlg->SendMessage(WM_MY_UPDATE);
		AfxMessageBox("更新参数失败!");
		return 0;
	}

	json_value *val=json_parse(pParas);
	if(val->type!=json_object)
	{
		AfxMessageBox("Not an json object!");
		return 0;
	}
	else
	{
		pParaDlg->m_sR.SetRange(0,255);
		pParaDlg->m_sG.SetRange(0,255);
	    pParaDlg->m_sB.SetRange(0,255);
		for(UINT i=0;i<val->u.object.length;i++)
		{
			strKey.Format("%s", val->u.object.values[i].name);
			strValue.Format("%s",val->u.object.values[i].value->u.string.ptr);
			if(strKey==(CString)cmd[0])
				pParaDlg->m_deltTime=atoi(strValue);
			else if(strKey==(CString)cmd[1])
			{
				pParaDlg->m_editR=atoi(strValue);
				pParaDlg->m_sR.SetPos(pParaDlg->m_editR);
			}
			else if(strKey==(CString)cmd[2])
			{
				pParaDlg->m_editG=atoi(strValue);
				pParaDlg->m_sG.SetPos(pParaDlg->m_editG);
			}
			else if(strKey==(CString)cmd[3])
			{
				pParaDlg->m_editB=atoi(strValue);
				pParaDlg->m_sB.SetPos(pParaDlg->m_editB);
			}
			else if(strKey==(CString)cmd[4])
			{
				strValue+=" ";
				int j=0;
				CString str;
				CString strTemp;
				int slength=strlen(strValue);
				for(int i=0;i<slength;i++)
				{
					if(strValue.GetAt(i)==' ')
					{
						j++;
						if(j==1)  continue;
						str=strValue.Left(i);
						pParaDlg->m_seedList.AddString(str);
						strTemp=strValue;
						strValue=strTemp.Right(strTemp.GetLength()-str.GetLength()-1);
						if(strValue=="")  break;
						i=0;
						j=0;
					}
				}
			}
			else if(strKey==(CString)cmd[5])
				pParaDlg->m_similarity=atof(strValue);
			else if(strKey==(CString)cmd[6])
				pParaDlg->m_scr2Height_3=atof(strValue);
			else if(strKey==(CString)cmd[7])
				pParaDlg->m_blockSize=atoi(strValue);
			else if(strKey==(CString)cmd[8])
				pParaDlg->m_blockNum=atoi(strValue);
			else if(strKey==(CString)cmd[9])
				pParaDlg->m_scr2Height_1=atof(strValue);
			else if(strKey==(CString)cmd[10])
				pParaDlg->m_scr2Height_2=atof(strValue);
			else if(strKey==(CString)cmd[11])
				pParaDlg->m_lab=atoi(strValue);
			else if(strKey==(CString)cmd[12])
				pParaDlg->m_correctNum=atoi(strValue);
			else
				;
		}
		pParaDlg->SendMessage(WM_MY_UPDATE);
	}
	return 0;
}

void CParameterSetDlg::OnInitial()
{
	HANDLE handle=CreateThread(NULL,0,OnRefreshParaThread,NULL,0,0);
 	if(!handle)
 		MessageBox("Creating OnRefreshParaThread thread failed!",0,0);
	
	CloseHandle(handle);
}

void CParameterSetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar != NULL)
	{
		CSliderCtrl *pS=(CSliderCtrl *)pScrollBar;
		if(*pS==m_sR)
		{
			m_editR=pS->GetPos();
			UpdateData(FALSE);
		}
		else if(*pS==m_sG)
		{
			m_editG=pS->GetPos();
			UpdateData(FALSE);
		}
		else if(*pS==m_sB)
		{
			m_editB=pS->GetPos();
			UpdateData(FALSE);
		}	
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL IsChangeEditR=FALSE;
void CParameterSetDlg::OnChangeEditR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	m_sR.SetPos(this->m_editR);
	IsChangeEditR=TRUE;
	UpdateData(FALSE);
}

BOOL IsChangeEditG=FALSE;
void CParameterSetDlg::OnChangeEditG() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	m_sG.SetPos(this->m_editG);
	IsChangeEditG=TRUE;
	UpdateData(FALSE);
}

BOOL IsChangeEditB=FALSE;
void CParameterSetDlg::OnChangeEditB() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	m_sB.SetPos(this->m_editB);
	IsChangeEditB=TRUE;
	UpdateData(FALSE);
}

CString strBlockSize;
CString strBlockNum;
CString strScr2Height_1;
CString strLab;
CString strCorrectNum;
CString strScr2Height_2;
CString strEditR;
CString strEditG;
CString strEditB;
CString strSimilarity;
CString strScr2Height_3;
CString strDeltTime;
CString strAllSeeds;
DWORD WINAPI OnSendParaThread(LPVOID lpParam)
{
	char response[5];
	if(send(clientSocket,strBlockSize,strlen(strBlockSize),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败00!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strBlockNum,strlen(strBlockNum),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败01!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strScr2Height_1,strlen(strScr2Height_1),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败02!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strLab,strlen(strLab),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败03!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strCorrectNum,strlen(strCorrectNum),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败04!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strScr2Height_2,strlen(strScr2Height_2),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败05!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strEditR,strlen(strEditR),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败06!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strEditG,strlen(strEditG),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败07!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strEditB,strlen(strEditB),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败08!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strSimilarity,strlen(strSimilarity),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败09!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strScr2Height_3,strlen(strScr2Height_3),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败10!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strDeltTime,strlen(strDeltTime),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败11!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);
	if(send(clientSocket,strAllSeeds,strlen(strAllSeeds),0)==SOCKET_ERROR)
	{
		AfxMessageBox("参数设置失败12!");
		return 0;
	}
	Sleep(5);
	recv(clientSocket,response,strlen(response),0);

	AfxMessageBox("参数设置成功!");
	return 0;
}

void CParameterSetDlg::OnButtonSendPara() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	strDeltTime.Format("config.interval=%d\n",this->m_deltTime);

	strBlockSize.Format("config.filling.block.size=%d\n",this->m_blockSize);
	strBlockNum.Format("config.filling.block.num=%d\n",this->m_blockNum);
	strScr2Height_1.Format("config.filling.border=%f\n",this->m_scr2Height_1);

    strLab.Format("config.evaluation.labThreshold=%d\n",this->m_lab);
    strCorrectNum.Format("config.evaluation.iteration=%d\n",this->m_correctNum);
    strScr2Height_2.Format("config.evaluation.border=%f\n",this->m_scr2Height_2);

	strEditR.Format("config.matting.RGB.R=%d\n",this->m_editR);
	strEditG.Format("config.matting.RGB.G=%d\n",this->m_editG);
	strEditB.Format("config.matting.RGB.B=%d\n",this->m_editB);
    strSimilarity.Format("config.matting.similarity=%f\n",this->m_similarity);
	strScr2Height_3.Format("config.matting.border=%f\n",this->m_scr2Height_3);

	CString strSeeds="";
	CString str;
	for(int i=0;i<m_seedList.GetCount();i++)
	{
		m_seedList.GetText(i,str);
		if(i==0)
			strSeeds+=str;
		else
			strSeeds+=" "+str;
	}
	strAllSeeds.Format("config.matting.seeds=\'%s\'\n",strSeeds);
	HANDLE handle=CreateThread(NULL,0,OnSendParaThread,NULL,0,0);
 	if(!handle)
 		MessageBox("Creating OnSendParaThread thread failed!",0,0);
	
	CloseHandle(handle);	
}

CFtpConnection *pCaptureConnection;	//连接对象
extern CString severIP;
BOOL IsGrasped=FALSE;
DWORD WINAPI OnCaptureThread(LPVOID lpParam)
{
	try 
	{ 
		//新建连接对象
		pCaptureConnection=pParaDlg->m_pInetSession->GetFtpConnection(severIP,"njust","njust");
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
	if(pCaptureConnection==NULL)
	{
		AfxMessageBox("传输图片失败!");
		return 0;
	}
//	DeleteFile("Received.jpg");
	pCaptureConnection->GetFile("\\cap.jpg","Received.jpg");
    pParaDlg->SendMessage(WM_SHOW_PICTURE);
	IsGrasped=TRUE;

	return 0;
}

int picWidth;
int picHeight;
void CParameterSetDlg::OnButtonGraps() 
{
	// TODO: Add your control notification handler code here
//	OnCaptureFrame();
	DeleteFile("Received.jpg");
	m_pInetSession=new CInternetSession("MR",INTERNET_OPEN_TYPE_PRECONFIG);
	HANDLE handle=CreateThread(NULL,0,OnCaptureThread,NULL,0,0);
	if(!handle)
 		MessageBox("Creating OnCaptureThread thread failed!",0,0);

	CloseHandle(handle);
	IfFirst=TRUE;
//	IsGrasped=TRUE;
}

void CParameterSetDlg::OnCaptureFrame()
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

void CParameterSetDlg::OnMyUpdate()
{
	this->UpdateData(FALSE);
//	SetWindowPos(NULL,rtMain.left ,rtMain.top ,rtMain.Width (),rtMain.Height (),SWP_NOZORDER|SWP_NOMOVE);
}

CPtrArray ptCpoint;
void CParameterSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(IsGrasped)
	{
		CRect rect;
		this->GetDlgItem(IDC_FRAME_TWO)->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		int dX=point.x-rect.left;
		int dY=point.y-rect.top;
        
		if(dX>=0 && dY>=0 && dX<=rect.Width() && dY<=rect.Height())
		{
			CString str;
			str.Format("%.0f %.0f",dX*picWidth*1.0/rect.Width(),dY*picHeight*1.0/rect.Height());
			
			CClientDC dc(this);
			CPen pen(PS_DOT,6,RGB(255,0,0));
	        CPen *pOldPen=dc.SelectObject(&pen);
		    dc.MoveTo(point);
		    dc.LineTo(point);

			int num=m_seedList.AddString(str);
			m_seedList.SetCurSel(num);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL IsChangeBlockSize=FALSE;
void CParameterSetDlg::OnChangeEditBlockSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeBlockSize=TRUE;
}

BOOL IsChangeEditBlockNum=FALSE;
void CParameterSetDlg::OnChangeEditBlockNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditBlockNum=TRUE;
}

BOOL IsChangeEditScreen2height1=FALSE;
void CParameterSetDlg::OnChangeEditScreen2height1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditScreen2height1=TRUE;
}

BOOL IsChangeEditLab=FALSE;
void CParameterSetDlg::OnChangeEditLab() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditLab=TRUE;
}

BOOL IsChangeEditCorrectNum=FALSE;
void CParameterSetDlg::OnChangeEditCorrectNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditCorrectNum=TRUE;
}

BOOL IsChangeEditScreen2height2=FALSE;
void CParameterSetDlg::OnChangeEditScreen2height2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditScreen2height2=TRUE;
}

BOOL IsChangeEditSimilarity=FALSE;
void CParameterSetDlg::OnChangeEditSimilarity() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditSimilarity=TRUE;
}

BOOL IsChangeEditScreen2height3=FALSE;
void CParameterSetDlg::OnChangeEditScreen2height3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditScreen2height3=TRUE;
}

BOOL IsChangeEditTime=FALSE;
void CParameterSetDlg::OnChangeEditTime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	IsChangeEditTime=TRUE;
}

void CParameterSetDlg::OnButtonDeleteAll() 
{
	// TODO: Add your control notification handler code here
	if(IfFirst)
		OnPaintPicture();
	int sum=m_seedList.GetCount();
	for(int i=0;i<sum;i++)
		this->m_seedList.DeleteString(0);

}

void CParameterSetDlg::OnPaintPicture()
{
	CClientDC  dc(this); 
    Graphics  graphics(dc.GetSafeHdc());   //   Create   a   GDI+   graphics   object 
    CRect rt;
	GetDlgItem(IDC_FRAME_TWO)->GetWindowRect(&rt);
	this->ScreenToClient(&rt);
    Bitmap  bitmap(L"Received.jpg"); 
    picWidth=bitmap.GetWidth();
	picHeight=bitmap.GetHeight();
    graphics.DrawImage(&bitmap,rt.left,rt.top,rt.Width(),rt.Height()); 
}


void CParameterSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(IfFirst)
		OnPaintPicture();
	// Do not call CDialog::OnPaint() for painting messages
}

void CParameterSetDlg::OnShowOnlyPicture()
{
	OnPaintPicture();
}

extern BOOL IsWindowExist;
extern CMainWindow_njustDlg *pMainDlg;
void CParameterSetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	IfFirst=FALSE;
	IsGrasped=FALSE;
	IsWindowExist=TRUE;
	pMainDlg->GetDlgItem(IDC_BUTTON_PARA)->EnableWindow(TRUE);

	CDialog::OnClose();
}
