// 0910200155Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "0910200155.h"
#include "0910200155Dlg.h"
#include "Show.h"
#include "MyLoad.h"
#include "fstream.h"
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
// CMy0910200155Dlg dialog

CMy0910200155Dlg::CMy0910200155Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy0910200155Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy0910200155Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy0910200155Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy0910200155Dlg)
	DDX_Control(pDX, IDC_DRAW_PLACE, m_drawplace);
	DDX_Control(pDX, IDC_LIST_SHAPE, m_Shape);
	DDX_Control(pDX, IDC_COMBO_BRUSH_COLOR, m_BrushColor);
	DDX_Control(pDX, IDC_COMBO_PEN_COLOR, m_PenColor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy0910200155Dlg, CDialog)
	//{{AFX_MSG_MAP(CMy0910200155Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELENDOK(IDC_COMBO_PEN_COLOR, OnSelendokComboPenColor)
	ON_CBN_SELENDOK(IDC_COMBO_BRUSH_COLOR, OnSelendokComboBrushColor)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, OnButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, OnButtonShow)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)	
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy0910200155Dlg message handlers

BOOL CMy0910200155Dlg::OnInitDialog()
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
	this->m_Shape.AddString("矩形");
    this->m_Shape.AddString("圆角矩形");
    this->m_Shape.AddString("椭圆");

	this->m_PenColor.AddString("红色");
    this->m_PenColor.AddString("绿色");
    this->m_PenColor.AddString("蓝色");
    this->m_PenColor.AddString("黄色");
    this->m_PenColor.AddString("洋红");
    this->m_PenColor.AddString("青色");
    this->m_PenColor.AddString("黑色");

    this->m_BrushColor.AddString("白色");
    this->m_BrushColor.AddString("青色");
    this->m_BrushColor.AddString("洋红");
    this->m_BrushColor.AddString("黄色");
    this->m_BrushColor.AddString("蓝色");
    this->m_BrushColor.AddString("绿色");
    this->m_BrushColor.AddString("红色");

    m_PenColor.SetCurSel(0);
	m_BrushColor.SetCurSel(4);
	m_Shape.SetCurSel(2);
	m_CurrentBrushColor=RGB(0,0,255);
	m_CurrentPenColor=RGB(255,0,0);
	this->SetWindowText("简单画图形程序");
	
	RECT rect;
	this->GetWindowRect(&rect);
	CRgn rgn;
	rgn.CreateRoundRectRgn(rect.left+3,rect.top+5,rect.right-2,rect.bottom-2,40,40);
	this->SetWindowRgn((HRGN)rgn.GetSafeHandle(),TRUE);

	ifstream infile1("Index.txt",ios::nocreate);
	int num;
    infile1>>num;
	if(!(num>0))
	this->GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy0910200155Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

int flag=0;
int mark;
int index;
int flag1=0;
void CMy0910200155Dlg::OnPaint() 
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
	if(index<=0)
	{
		this->GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(FALSE);
	}
    if(flag==1||flag==2||flag==3)
	{
		CClientDC dc(this); // device context for painting
		{	
			CPen pNewPen1;
            pNewPen1.CreatePen(PS_SOLID,2,RGB(80,80,180));
	        CPen *pOldPen1=dc.SelectObject(&pNewPen1);
			CBrush pNewBrush1;
            pNewBrush1.CreateSolidBrush(RGB(80,80,180));
            CBrush *pOldBrush1=dc.SelectObject(&pNewBrush1);
	        CRect rect1;
	        this->GetDlgItem(IDC_DRAW_PLACE)->GetWindowRect(&rect1);
	        this->ScreenToClient(&rect1);
	        CRect draw1(rect1.left+3,rect1.top+9,rect1.right-2,rect1.bottom-2);
		    dc.Rectangle(draw1);
		    dc.SelectObject(pOldPen1);
	        dc.SelectObject(pOldBrush1);
		}		
		if((flag==1||flag==3)&&flag!=2)
		{
			CPen pNewPen;
            pNewPen.CreatePen(PS_SOLID,2,this->m_CurrentPenColor);
	        CPen *pOldPen=dc.SelectObject(&pNewPen);
	        CBrush pNewBrush;
            pNewBrush.CreateSolidBrush(this->m_CurrentBrushColor);
            CBrush *pOldBrush=dc.SelectObject(&pNewBrush);
	        CRect rect;
	        this->GetDlgItem(IDC_DRAW_PLACE)->GetWindowRect(&rect);
	        this->ScreenToClient(&rect);
	        CRect draw(rect.left+3,rect.top+9,rect.right-2,rect.bottom-2);
	        if(this->m_Shape.GetSel(0))
			{		
				dc.Rectangle(draw);
			    mark=0;
			}
	        else if(this->m_Shape.GetSel(1))
			{		
			    dc.RoundRect(draw,CPoint(100,100));
			    mark=1;
			}
	        else 
			{	
			    dc.Ellipse(draw);
			    mark=2;
			}
			dc.SelectObject(pOldPen);
	        dc.SelectObject(pOldBrush);
			this->GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(TRUE);
		    if(flag==1&&flag!=3&&flag1==1)
			{
				pmN=new CMemoryNode(m_CurrentPenColor,m_CurrentBrushColor,mark);
		   	    pta.Add(pmN);
			    OnFileWrite();	
				flag1=0;
			}
		}
		else if(flag==2)
		{    
			if(index==0)
			{   
				CPen pNewPen;
                pNewPen.CreatePen(PS_SOLID,2,RGB(80,80,180));
	            CPen *pOldPen=dc.SelectObject(&pNewPen);
				CBrush pNewBrush;
                pNewBrush.CreateSolidBrush(RGB(80,80,180));
                CBrush *pOldBrush=dc.SelectObject(&pNewBrush);
	            CRect rect;
	            this->GetDlgItem(IDC_DRAW_PLACE)->GetWindowRect(&rect);
	            this->ScreenToClient(&rect);
	            CRect draw(rect.left+3,rect.top+9,rect.right-2,rect.bottom-2);
				dc.Rectangle(draw);
				dc.SelectObject(pOldPen);
	            dc.SelectObject(pOldBrush);
			}
			if(index>0)
			{
				CMemoryNode *pmN;
			    pmN=(CMemoryNode *)pta.GetAt(index-1);
                CPen pNewPen1;
                pNewPen1.CreatePen(PS_SOLID,2,RGB(80,80,180));
	            CPen *pOldPen1=dc.SelectObject(&pNewPen1);
				CBrush pNewBrush1;
                pNewBrush1.CreateSolidBrush(RGB(80,80,180));
                CBrush *pOldBrush1=dc.SelectObject(&pNewBrush1);
	            CRect rect1;
	            this->GetDlgItem(IDC_DRAW_PLACE)->GetWindowRect(&rect1);
	            this->ScreenToClient(&rect1);
	            CRect draw1(rect1.left+3,rect1.top+9,rect1.right-2,rect1.bottom-2);
				dc.Rectangle(draw1);
				dc.SelectObject(pOldPen1);
	            dc.SelectObject(pOldBrush1);

	            CPen pNewPen;
				pNewPen.CreatePen(PS_SOLID,2,pmN->PenColor);
	            CPen *pOldPen=dc.SelectObject(&pNewPen);
                CBrush pNewBrush;
				pNewBrush.CreateSolidBrush(pmN->BrushColor);
                CBrush *pOldBrush=dc.SelectObject(&pNewBrush);
	            CRect rect;
	            this->GetDlgItem(IDC_DRAW_PLACE)->GetWindowRect(&rect);
	            this->ScreenToClient(&rect);
	            CRect draw(rect.left+3,rect.top+9,rect.right-2,rect.bottom-2);
				if(pmN->Mark==0)
				{			    
				    dc.Rectangle(draw);		    
				}
	            else if(pmN->Mark==1)
				{		
			        dc.RoundRect(draw,CPoint(100,100));		   
				}
	            else 
				{	
			        dc.Ellipse(draw);	   
				} 
	            dc.SelectObject(pOldPen);
	            dc.SelectObject(pOldBrush);	
				if(flag1==2)
				{
					pta.ElementAt(index-1)=NULL;
	                this->pta.RemoveAt(index-1);
					flag1=0;
				}
			}
		} 
	} 
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy0910200155Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy0910200155Dlg::OnSelendokComboPenColor() 
{
	// TODO: Add your control notification handler code here
	int n=m_PenColor.GetCurSel();
	switch(n)
	{
	   case  0:m_CurrentPenColor=RGB(255,0,0);break;
       case  1:m_CurrentPenColor=RGB(0,255,0);break;
       case  2:m_CurrentPenColor=RGB(0,0,255);break;
       case  3:m_CurrentPenColor=RGB(255,255,0);break;
       case  4:m_CurrentPenColor=RGB(255,0,255);break;
       case  5:m_CurrentPenColor=RGB(0,255,255);break;
       default:m_CurrentPenColor=RGB(0,0,0);break;
		   
	}
	
}

void CMy0910200155Dlg::OnSelendokComboBrushColor() 
{
	// TODO: Add your control notification handler code here
    int n=m_BrushColor.GetCurSel();
	switch(n)
	{
	   case  6:m_CurrentBrushColor=RGB(255,0,0);break;
       case  5:m_CurrentBrushColor=RGB(0,255,0);break;
       case  4:m_CurrentBrushColor=RGB(0,0,255);break;
       case  3:m_CurrentBrushColor=RGB(255,255,0);break;
       case  2:m_CurrentBrushColor=RGB(255,0,255);break;
       case  1:m_CurrentBrushColor=RGB(0,255,255);break;
       default:m_CurrentBrushColor=RGB(255,255,255);break;
		   
	}
	
}

void CMy0910200155Dlg::OnButtonDraw() 
{
	flag=1;
	flag1=1;
	OnPaint();
	this->GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
}

int ButtonState=0;
void CMy0910200155Dlg::OnButtonShow() 
{
	// TODO: Add your control notification handler code here
	CShow dlg;
	ButtonState=1;
	dlg.DoModal();
}

HBRUSH CMy0910200155Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_COMBO_PEN_COLOR)
	{
		hbr=::CreateSolidBrush(this->m_CurrentPenColor);
		pDC->SetBkColor(this->m_CurrentPenColor);
	}
    if(pWnd->GetDlgCtrlID()==IDC_COMBO_BRUSH_COLOR)
	{
		hbr=::CreateSolidBrush(this->m_CurrentBrushColor);
	    pDC->SetBkColor(this->m_CurrentBrushColor);
	}   	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CMy0910200155Dlg::OnButtonClean() 
{
	// TODO: Add your control notification handler code here
    flag=2;
	flag1=2;
	index=this->pta.GetUpperBound();
	OnPaint(); 
}

void CMy0910200155Dlg::OnFileWrite()
{
	int sum=pta.GetSize();
    ofstream outfile;
    ofstream outfile1;
	outfile.open("Histories.txt",ios::app);
    outfile1.open("Index.txt");
	if(!outfile||!outfile1)
	{
        MessageBox("数据无法保存!");
	}
    else 
	{   
		outfile<<m_CurrentPenColor<<'\t';
        outfile<<m_CurrentBrushColor<<'\t';
        outfile<<mark<<endl;
		outfile1<<sum<<endl;
	}
	outfile.close();
    outfile1.close();
}

void CMy0910200155Dlg::OnFileRead()
{
     ifstream infile("Histories.txt",ios::nocreate);
	 if(!infile)
	 { 
		MessageBox("文件不存在或不完整,无法读取!");
		this->GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(FALSE);
	 }
	int Mark;
    COLORREF BrushColor;
	COLORREF PenColor;	
	for(int i=0;infile;i++)
	{	
		infile>>PenColor;
		infile>>BrushColor;
		infile>>Mark;	
		m_CurrentPenColor=PenColor;
        m_CurrentBrushColor=BrushColor;
        switch(Mark)
		{
		case  0:m_Shape.GetSel(0);break;
		case  1:m_Shape.GetSel(1);break;
		default:m_Shape.GetSel(2);break;
		}
		pmn=new CMemoryNode(PenColor,BrushColor,Mark);
		pta.Add(pmn);
		flag=3;
	    OnPaint();
	} 
	infile.close();
}

void CMy0910200155Dlg::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	ifstream infile2("Index.txt",ios::nocreate);
	int num;
	infile2>>num;
	index=num;
	this->GetDlgItem(IDC_BUTTON_CLEAN)->EnableWindow(TRUE);
    OnFileRead();
	this->GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
}
