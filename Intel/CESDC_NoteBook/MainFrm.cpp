// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CESDC_NoteBook.h"
#include "fstream.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    //ToolBarStyle;
	this->m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_VISIBLE | CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC);	
	this->m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
    m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(28,28));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//Disable the MinimizeBox and MaximizeBox
    LONG Style=::GetWindowLong(this->m_hWnd,GWL_STYLE);
	Style &= ~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CAPTION);
	::SetWindowLong(this->m_hWnd,GWL_STYLE,Style);

	//Transparent the Frame
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	HINSTANCE hInst=LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(GetSafeHwnd(),0,100,2);  
		FreeLibrary(hInst); 
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

