// Show.cpp : implementation file
//

#include "stdafx.h"
#include "0910200155.h"
#include "Show.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShow dialog


CShow::CShow(CWnd* pParent /*=NULL*/)
	: CDialog(CShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShow, CDialog)
	//{{AFX_MSG_MAP(CShow)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShow message handlers

int CShow::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

BOOL CShow::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	extern ButtonState;
	if(ButtonState==1)
	{
	   CRect rect;
	   this->GetClientRect(&rect);
	   this->ScreenToClient(&rect);

	   for(int j=3;j>=1;j--)  
	   {
		   for(int i=1;i<=350;i+=3*j-2)
		   {
			   CPen pen1(PS_DOT,2,RGB(255,0,0));
	           CPen *pOldPen=dc.SelectObject(&pen1);
		       dc.MoveTo(i,0);
		       dc.LineTo(0,350-i);
  
		       CPen pen2(PS_DOT,2,RGB(0,255,255));
               dc.SelectObject(&pen2);
               dc.MoveTo(350,i);
		       dc.LineTo(i,0);

		       CPen pen4(PS_DOT,2,RGB(0,255,0));
               dc.SelectObject(&pen4);
               dc.MoveTo(350-i,350);
		       dc.LineTo(350,i);

		       CPen pen3(PS_DOT,2,RGB(255,255,0));
               dc.SelectObject(&pen3);
		       dc.MoveTo(350-i,350);
		       dc.LineTo(0,350-i);

               Sleep(5);
		   }
		   Sleep(50);
	   }
      
	   dc.SetTextColor(RGB(200,200,200));
	   dc.SetBkMode(TRANSPARENT);
	   dc.TextOut(60,150,"给我一把直尺，我送给你整个地球！");
	   dc.TextOut(95,170,"版权所有，违者必究！");
	   dc.TextOut(200,200,"--------邹城");
	   ButtonState=0;
	}

    else
	{
	   CRect rect;
	   this->GetClientRect(&rect);
	   this->ScreenToClient(&rect);

	   for(int j=3;j>=1;j--)  
	   {
		   for(int i=1;i<=350;i+=3*j-2)
		   {
			   CPen pen1(PS_DOT,2,RGB(255,0,0));
	           CPen *pOldPen=dc.SelectObject(&pen1);
		       dc.MoveTo(i,0);
		       dc.LineTo(0,350-i);
  
		       CPen pen2(PS_DOT,2,RGB(0,255,255));
               dc.SelectObject(&pen2);
               dc.MoveTo(350,i);
		       dc.LineTo(i,0);

		       CPen pen4(PS_DOT,2,RGB(0,255,0));
               dc.SelectObject(&pen4);
               dc.MoveTo(350-i,350);
		       dc.LineTo(350,i);

		       CPen pen3(PS_DOT,2,RGB(255,255,0));
               dc.SelectObject(&pen3);
		       dc.MoveTo(350-i,350);
		       dc.LineTo(0,350-i);
		   }
	   } 
	   dc.SetTextColor(RGB(200,200,200));
	   dc.SetBkMode(TRANSPARENT);
	   dc.TextOut(70,150,"给我一把直尺，我送给你地球！");
	   dc.TextOut(95,170,"版权所有，违者必究！");
	   dc.TextOut(200,200,"--------邹城");
	}

	// Do not call CDialog::OnPaint() for painting messages
}



