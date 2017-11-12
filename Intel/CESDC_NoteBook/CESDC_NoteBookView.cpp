// CESDC_NoteBookView.cpp : implementation of the CCESDC_NoteBookView class
//

#include "stdafx.h"
#include "CESDC_NoteBook.h"
#include "fstream.h"
#include "CESDC_NoteBookDoc.h"
#include "CESDC_NoteBookView.h"

#define RED    RGB(255,0,0)
#define GREEN  RGB(0,255,0)
#define BLACK  RGB(0,0,0)
#define YELLOW RGB(255,255,0)
#define BLUE   RGB(0,0,255)
#define PINK   RGB(255,0,255)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView

IMPLEMENT_DYNCREATE(CCESDC_NoteBookView, CView)

BEGIN_MESSAGE_MAP(CCESDC_NoteBookView, CView)
	//{{AFX_MSG_MAP(CCESDC_NoteBookView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_COLOR_BLACK,ID_CHANGE,OnPenChoice)
	ON_COMMAND_RANGE(ID_COLOR_PINK,ID_CHANGE_REST,OnChooseRest)
	ON_COMMAND_RANGE(ID_BUTTON_CLEAR,ID_CHANGE_REST2,OnChooseRest2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView construction/destruction

CCESDC_NoteBookView::CCESDC_NoteBookView()
{
	// TODO: add construction code here

}

CCESDC_NoteBookView::~CCESDC_NoteBookView()
{
	delete m_pOpenLines;
	delete m_pSaveLines;
}

BOOL IsOpenFile=FALSE;
BOOL CCESDC_NoteBookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	CFileFind tempFind;
	BOOL fileExist=(BOOL)tempFind.FindFile("C:\\MyNotes");
	if(!fileExist)
		CreateDirectory("C:\\MyNotes",NULL);

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView drawing

void CCESDC_NoteBookView::OnDraw(CDC* pDC)
{
	CCESDC_NoteBookDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView printing

BOOL CCESDC_NoteBookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCESDC_NoteBookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCESDC_NoteBookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView diagnostics

#ifdef _DEBUG
void CCESDC_NoteBookView::AssertValid() const
{
	CView::AssertValid();
}

void CCESDC_NoteBookView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCESDC_NoteBookDoc* CCESDC_NoteBookView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCESDC_NoteBookDoc)));
	return (CCESDC_NoteBookDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookView message handlers

void CCESDC_NoteBookView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	StartPt.x=point.x;
	StartPt.y=point.y;	
	this->SetCapture();

	CView::OnLButtonDown(nFlags, point);
}

void CCESDC_NoteBookView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	StartPt.x=-1;
	::ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

int Width=1;
void CCESDC_NoteBookView::OnPenChoice(UINT nID)
{
	DWORD PenStyle;
	switch(nID)
	{
	   case ID_COLOR_RED:   m_penColor=RED;break;
	   case ID_COLOR_GREEN: m_penColor=GREEN;break;
	   case ID_COLOR_BLACK: m_penColor=BLACK;break;
	   case ID_COLOR_BLUE:  m_penColor=BLUE;break;
	   case ID_COLOR_YELLOW:m_penColor=YELLOW;break;
	   case ID_WIDTH1:      PenStyle=PS_SOLID;Width=1;break;
	   case ID_WIDTH2:      PenStyle=PS_SOLID;Width=2;break; 
	   case ID_BUTTON_SAVE: OnFileSave();break;
	   case ID_BUTTON_OPEN: OnFileOpen();break;
	   case ID_BUTTON_NEW:  OnFileNew(); break;
	   case ID_BUTTON_REQUEST:OnRequest();break;
	   case ID_BUTTON_CLOSE:OnCloseWindow();break;
	}
	Pen.DeleteObject();
	Pen.CreatePen(PenStyle,Width,m_penColor);
}

void CCESDC_NoteBookView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
    dc.SelectObject(&Pen);

	EndPt.x=point.x;
	EndPt.y=point.y;
	
	if(StartPt.x>=0)
	{
		dc.MoveTo(StartPt.x,StartPt.y);
		dc.LineTo(EndPt);

		m_pSaveLines=new CSpecialLine(StartPt,EndPt,Width,this->m_penColor);
		this->m_SaveLineArray.Add(m_pSaveLines);

		StartPt.x=EndPt.x;
		StartPt.y=EndPt.y;
	}

	CView::OnMouseMove(nFlags, point);
}

int nFileIndex=0;
void CCESDC_NoteBookView::OnFileNew()
{
	int nSize=m_SaveLineArray.GetSize();
	if(nSize>0)
	{
		CString strAutoName;
	    strAutoName.Format("C:\\MyNotes\\Default_Note_%d.vPlay",++nFileIndex);
	    ofstream outfile;
        outfile.open(strAutoName);
	    outfile<<nSize<<endl;
	    for(int i=0;i<nSize;i++)
		{
			CSpecialLine pline=*(CSpecialLine*)m_SaveLineArray.GetAt(i);
		    outfile<<((pline.m_StartPoint).x)<<'\t'<<((pline.m_StartPoint).y)<<'\t';
		    outfile<<((pline.m_EndPoint).x)<<'\t'<<((pline.m_EndPoint).y)<<'\t'<<(pline.m_Width);
		    outfile<<'\t'<<(pline.m_Color)<<endl;           
		}
	    outfile.close();
		CRect rect;
        this->GetClientRect(&rect);
		CClientDC dc(this);
		CPen pen;
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
        dc.SelectObject(&pen);
		dc.Rectangle(&rect);	
        m_SaveLineArray.RemoveAll();
		
	}
	else
	{
		CRect rect;
        this->GetClientRect(&rect);
		CClientDC dc(this);
		CPen pen;
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
        dc.SelectObject(&pen);
		dc.Rectangle(&rect);
	}
}

void CCESDC_NoteBookView::OnFileOpen()
{
	char filter[]="文件(*.vPlay)|*.vPlay|所有文件(*.*)|*.*||";
	CFileDialog fdlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
	fdlg.m_ofn.lpstrInitialDir="C:\\MyNotes\\";
	if(fdlg.DoModal()==IDOK)
	{
		CString fileName=fdlg.GetPathName();
		ifstream infile;
		infile.open(fileName);
        int nSize;
		infile>>nSize;
		CPoint pt1;
		CPoint pt2;
		int width;
		COLORREF color;
		for(int i=0;i<nSize;i++)
		{
			infile>>pt1.x>>pt1.y>>pt2.x>>pt2.y>>width>>color;
			m_pOpenLines=new CSpecialLine(pt1,pt2,width,color);
		    this->m_OpenLineArray.Add(m_pOpenLines);
		}
		infile.close();
		IsOpenFile=TRUE;
	}
}

void CCESDC_NoteBookView::OnFileSave()
{
//	WinExec(_T("osk.exe"),SW_SHOW);
	char filter[]="文件(*.vPlay)|*.vPlay|所有文件(*.*)|*.*||";
	CFileDialog fdlg(false,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
	fdlg.m_ofn.lpstrInitialDir="C:\\MyNotes\\";
	if(fdlg.DoModal()==IDOK)
	{
		CString fileName=fdlg.GetPathName();
        ofstream outfile;
		outfile.open(fileName+".vPlay");
		int nSize=m_SaveLineArray.GetSize();
		outfile<<nSize<<endl;
		for(int i=0;i<nSize;i++)
		{
			CSpecialLine pline=*(CSpecialLine*)m_SaveLineArray.GetAt(i);
			outfile<<((pline.m_StartPoint).x)<<'\t'<<((pline.m_StartPoint).y)<<'\t';
			outfile<<((pline.m_EndPoint).x)<<'\t'<<((pline.m_EndPoint).y)<<'\t'<<(pline.m_Width);
			outfile<<'\t'<<(pline.m_Color)<<endl;           
		}
		outfile.close();
	}
}

void CCESDC_NoteBookView::OnCloseWindow()
{
	CString strMsg="MeetingStoped!";
	CWnd *pOtherWnd=CWnd::FindWindow(NULL,"SlaveTongBuDlg");
	if(pOtherWnd)
	{
		COPYDATASTRUCT cpd;
		cpd.dwData=0;
		cpd.cbData=strMsg.GetLength()+1;
		cpd.lpData=(void*)strMsg.GetBuffer(cpd.cbData);
		LRESULT copyData=pOtherWnd->SendMessage(WM_COPYDATA,(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),(LPARAM)&cpd);
		strMsg.ReleaseBuffer();
	}
	this->GetParentFrame()->SendMessage(WM_CLOSE,0,0);
}

void CCESDC_NoteBookView::OnPaint() 
{
	CClientDC dc(this);	
	if(IsOpenFile)
	{
		CPen pen;
		CRect rect;
        this->GetClientRect(&rect);
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
        dc.SelectObject(&pen);
		dc.Rectangle(&rect);	

		int nSize=m_OpenLineArray.GetSize();
		for(int i=0;i<nSize;i++)
		{
			CSpecialLine pline=*(CSpecialLine*)m_OpenLineArray.GetAt(i);
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID,pline.m_Width,pline.m_Color);
            dc.SelectObject(&pen);
			dc.MoveTo(pline.m_StartPoint);
			dc.LineTo(pline.m_EndPoint);
		}
		this->m_OpenLineArray.RemoveAll();
		IsOpenFile=FALSE;
	}
}

void CCESDC_NoteBookView::OnChooseRest(UINT nID)
{
	DWORD PenStyle;
	switch(nID)
	{
	   case ID_COLOR_PINK:  m_penColor=PINK;break;
	   case ID_WIDTH4:      PenStyle=PS_SOLID;Width=4;break; 
	}
	Pen.DeleteObject();
	Pen.CreatePen(PenStyle,Width,m_penColor);
}

typedef struct transSendMsg
{
	int type;
	unsigned long length;
}transSendMsg;

char AskAndAnswer[20];
DWORD WINAPI OnConnectMaster(LPVOID lpParam)
{
	SOCKET sock;
    WSADATA wsadata;
	while(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
	}
	while((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
	{
	}
	char chIP[15];
	ifstream infile;
	infile.open("strMasterIP.txt");
	infile>>chIP;
	infile.close();
	CString strIp=(CString)chIP;
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(30000);
    sin.sin_addr.S_un.S_addr=inet_addr(strIp);
	while(connect(sock,(sockaddr*)&sin,sizeof(sockaddr))==-1)
	{
	}
	transSendMsg sendMsg;
    sendMsg.type=127;
	sendMsg.length=strlen("wantobethehost");
	send(sock,(const char *)&sendMsg,sizeof(sendMsg),0);
	send(sock,"wantobethehost",strlen("wantobethehost"),0);

	int bytes;
	bytes=recv(sock,(char*)&sendMsg,sizeof(sendMsg),0);
    if(bytes==-1)
	{
		closesocket(sock);
		return 0;
	}
    else if(bytes!=sizeof(sendMsg))
	{
		closesocket(sock);
		return 0;
	}
    else if(sendMsg.type==127)
	{
	    recv(sock,AskAndAnswer,sendMsg.length,0);
		AskAndAnswer[sendMsg.length]='\0';
		CString str=(CString)AskAndAnswer;
		if(str=="YES")
		{
			CString strMsg="ReadytoChange?";
			CWnd *pOtherWnd=CWnd::FindWindow(NULL,"SlaveTongBuDlg");
			if(pOtherWnd)
			{
				COPYDATASTRUCT cpd;
				cpd.dwData=0;
				cpd.cbData=strMsg.GetLength()+1;
				cpd.lpData=(void*)strMsg.GetBuffer(cpd.cbData);
				LRESULT copyData=pOtherWnd->SendMessage(WM_COPYDATA,(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),(LPARAM)&cpd);
				strMsg.ReleaseBuffer();
			}

		}
		else if(str=="NO")
		{
		}
	}

	return 1;
}

BOOL CCESDC_NoteBookView::OnRequest()
{
	HANDLE handle=CreateThread(NULL,0,OnConnectMaster,NULL,0,0);
	if(!handle)
		MessageBox("CreateThread i failed!",0,0);

	return 1;
}

void CCESDC_NoteBookView::OnChooseRest2(UINT nID)
{
	switch(nID)
	{
	   case ID_BUTTON_CLEAR: OnClearRect(); break;
	}
}

void CCESDC_NoteBookView::OnClearRect()
{
	m_SaveLineArray.RemoveAll();
	CRect rect;
    this->GetClientRect(&rect);
    CClientDC dc(this);
	CPen pen;
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
    dc.SelectObject(&pen);
	dc.Rectangle(&rect);	
}
