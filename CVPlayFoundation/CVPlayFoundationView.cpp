// CVPlayFoundationView.cpp : implementation of the CCVPlayFoundationView class
//

#include "stdafx.h"
#include "CVPlayFoundation.h"
#include "CVPlayFoundationDoc.h"
#include "CVPlayFoundationView.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationView

IMPLEMENT_DYNCREATE(CCVPlayFoundationView, CView)

BEGIN_MESSAGE_MAP(CCVPlayFoundationView, CView)
	//{{AFX_MSG_MAP(CCVPlayFoundationView)
	ON_COMMAND(ID_MENU_SHOW_MY_DLG, OnMenuShowMyDlg)
	ON_COMMAND(ID_MENU_SHOW_NOT_MODAL, OnMenuShowNotModal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationView construction/destruction

CCVPlayFoundationView::CCVPlayFoundationView()
{
	// TODO: add construction code here

}

CCVPlayFoundationView::~CCVPlayFoundationView()
{
}

BOOL CCVPlayFoundationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationView drawing

void CCVPlayFoundationView::OnDraw(CDC* pDC)
{
	CCVPlayFoundationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	int red=200,green=200,blue=200;
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=i;j++)
		{
			CString str;
			str.Format("%d * %d= %d ",i,j,i*j);
			pDC->SetBkColor(RGB(20,20,20));
			pDC->SetTextColor(RGB(red,green,blue));
			pDC->TextOut(71*j,80+20*i,str);	
			red-=5*i+15*j;green-=19*i+j;blue-=16*i+4*j;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationView diagnostics

#ifdef _DEBUG
void CCVPlayFoundationView::AssertValid() const
{
	CView::AssertValid();
}

void CCVPlayFoundationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCVPlayFoundationDoc* CCVPlayFoundationView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCVPlayFoundationDoc)));
	return (CCVPlayFoundationDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationView message handlers


//***************************************************************************
//***************************************************************************
//添加菜单栏按钮，ID为:ID_MENU_SHOW_MY_DLG，为其添加一个COMMAND类型消息
//***************************************************************************
//***************************************************************************


//*************************************************************创建模态对话框
void CCVPlayFoundationView::OnMenuShowMyDlg() 
{
	// TODO: Add your command handler code here
	MessageBox("呵呵。。。");
	CMyDialog dlg;		
    dlg.DoModal();
}

//***********************************************************创建非模态对话框
void CCVPlayFoundationView::OnMenuShowNotModal() 
{
	// TODO: Add your command handler code here
	CMyDialog *pdlg=new CMyDialog();
	pdlg->Create(IDD_DIALOG_MY,this);
	pdlg->ShowWindow(SW_SHOW);
}
