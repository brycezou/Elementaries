// Ex003View.cpp : implementation of the CEx003View class
//

#include "stdafx.h"
#include "Ex003.h"
#include "line.h"
#include "Ex003Doc.h"
#include "Ex003View.h"
#include "string.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx003View

IMPLEMENT_DYNCREATE(CEx003View, CScrollView)

BEGIN_MESSAGE_MAP(CEx003View, CScrollView)
	//{{AFX_MSG_MAP(CEx003View)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx003View construction/destruction

CEx003View::CEx003View()
{
	// TODO: add construction code here
	flag=0;

}

CEx003View::~CEx003View()
{
}

BOOL CEx003View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEx003View drawing

void CEx003View::OnDraw(CDC* pDC)
{
	CEx003Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	int num=pta.GetSize();
	CLine *pline;
    CClientDC dc(this);
	for(int i=0;i<num;i++)
	{
		pline=(CLine*)pta.GetAt(i);
        pDC->MoveTo(pline->mpt1);
		pDC->LineTo(pline->mpt2);
		
	    CString str;
		dc.SetBkMode(TRANSPARENT);
		str.Format("(%d,%d)",pline->mpt1.x,pline->mpt1.y);
		dc.TextOut(pline->mpt1.x+10,pline->mpt1.y-20,str,strlen(str));
        str.Format("(%d,%d)",pline->mpt2.x,pline->mpt2.y);
		dc.TextOut(pline->mpt2.x+10,pline->mpt2.y-20,str,strlen(str));

	}
	CRect rt;
	this->GetClientRect(&rt);
	CString s;
	s.Format("界面大小为  %d * %d",rt.Width(),rt.Height());
	this->GetParent()->SetWindowText(s);
	

}

/////////////////////////////////////////////////////////////////////////////
// CEx003View printing

BOOL CEx003View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEx003View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEx003View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEx003View diagnostics

#ifdef _DEBUG
void CEx003View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEx003View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEx003Doc* CEx003View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEx003Doc)));
	return (CEx003Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx003View message handlers

void CEx003View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt1=point;
	pt2=point;	
	flag=1;
	this->SetCapture();
    
    CString str;
    CClientDC dc(this);
	str.Format("(%d,%d)",point.x,point.y);
	dc.TextOut(point.x+10,point.y-20,str,strlen(str));

	CScrollView::OnLButtonDown(nFlags, point);
}

void CEx003View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CClientDC dc(this);
	
	double Rad=3.1415926535897/180;

	    if(nFlags & MK_SHIFT)
		{		   
           int y=point.y-pt1.y;
	       int x=point.x-pt1.x;
		   int X=abs(x);
		   int Y=abs(y);

           if(atan2(Y,X)<=22.5*Rad)
		   {
			  dc.MoveTo(pt1);
		      dc.LineTo(point.x,pt1.y);
			  point.y=pt1.y;
              pline=new CLine(pt1,point);
              this->pta.Add(pline);
		   }
           if(atan2(Y,X)>22.5*Rad&&atan2(Y,X)<67.5*Rad)
		   {  
			  if(x>0&&y>0)
			  { 
				  dc.MoveTo(pt1);	      
				  dc.LineTo(point.x,pt1.y+X);
				  point.y=pt1.y+X;
				  pline=new CLine(pt1,point);
                  this->pta.Add(pline);
			  }
			  if(x>0&&y<0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y-X);
                  point.y=pt1.y-X;
				  pline=new CLine(pt1,point);
                  this->pta.Add(pline);
			  }
			  if(x<0&&y>0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y+X);
				  point.y=pt1.y+X;
				  pline=new CLine(pt1,point);
                  this->pta.Add(pline);
			  }
			  if(x<0&&y<0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y-X);
				  point.y=pt1.y-X;
				  pline=new CLine(pt1,point);
                  this->pta.Add(pline);
			  }
		   }
           if(atan2(Y,X)>=67.5*Rad)
		   {
			  dc.MoveTo(pt1);
		      dc.LineTo(pt1.x,point.y);
			  point.x=pt1.x;
			  pline=new CLine(pt1,point);
              this->pta.Add(pline);
		   } 
		}
	    else
		{
			dc.SetROP2(R2_NOT);
		    dc.MoveTo(pt1);
		    dc.LineTo(pt2);	
            dc.MoveTo(pt1);
		    dc.LineTo(point);

		}

        CString str;
	    str.Format("(%d,%d)",point.x,point.y);
	    dc.TextOut(point.x+10,point.y-20,str,strlen(str));
		
        pline=new CLine(pt1,point);
        this->pta.Add(pline);
	    ::ReleaseCapture();
        flag=0;

	 CScrollView::OnLButtonUp(nFlags, point);
}

void CEx003View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	char *str="可根据标题栏中鼠标位置确定确定鼠标终点！";
	dc.SetTextColor(RGB(255,0,0));
	dc.TextOut(0,0,str,strlen(str));
    
	if(flag==1)
	{
        if(nFlags & MK_SHIFT)
		{		   
           dc.SetROP2(R2_NOT);
           double Rad=3.1415926535897/180;
	       int y=point.y-pt1.y;
	       int x=point.x-pt1.x;
	       int X=abs(x);
	       int Y=abs(y);

		   if(atan2(Y,X)<=22.5*Rad)
		   {
			  dc.MoveTo(pt1);
		      dc.LineTo(pt2);
			  dc.MoveTo(pt1);
			  dc.LineTo(point.x,pt1.y);
			  point.y=pt1.y;	
		   }
           if((atan2(Y,X)>22.5*Rad) && (atan2(Y,X)<67.5*Rad))
		   {
			  
			  if(x>0&&y>0)
			  { 
				  dc.MoveTo(pt1);	      
				  dc.LineTo(pt2);
                 dc.MoveTo(pt1);	      
				  dc.LineTo(point.x,pt1.y+X);
                  point.y=pt1.y+X;
			  }
			  if(x>0&&y<0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(pt2);
                  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y-X);
                  point.y=pt1.y-X;
			  }
			  if(x<0&&y>0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(pt2);
                  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y+X);
                  point.y=pt1.y+X;
			  }
			  if(x<0&&y<0)
			  { 
				  dc.MoveTo(pt1);
		          dc.LineTo(pt2);
                  dc.MoveTo(pt1);
		          dc.LineTo(point.x,pt1.y-X);
                  point.y=pt1.y-X;
			  }		  	 
		   }
           if(atan2(Y,X)>=67.5*Rad)
		   {
			  dc.MoveTo(pt1);
		      dc.LineTo(pt2);
			  dc.MoveTo(pt1);
		      dc.LineTo(pt1.x,point.y);
			  point.x=pt1.x;
		   } 
		}
		
	    else
		{ 
		   dc.SetROP2(R2_NOT);
		   dc.MoveTo(pt1);
		   dc.LineTo(pt2);	
           dc.MoveTo(pt1);
		   dc.LineTo(point);
		}

		pt2=point;
	}
    
	CString s;
	s.Format("鼠标当前位置为  X=%d,Y=%d",point.x,point.y);
	this->GetParent()->SetWindowText(s);

    CScrollView::OnMouseMove(nFlags, point);
}

void CEx003View::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	CSize sizeTotal;
	sizeTotal.cx =800;
	sizeTotal.cy = 600;
    this->SetScrollSizes(MM_TEXT,sizeTotal);
	
}
