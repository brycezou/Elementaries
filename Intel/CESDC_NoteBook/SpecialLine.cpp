// SpecialLine.cpp: implementation of the CSpecialLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CESDC_NoteBook.h"
#include "SpecialLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpecialLine::CSpecialLine()
{

}

CSpecialLine::~CSpecialLine()
{

}

CSpecialLine::CSpecialLine(CPoint startPoint,CPoint endPoint,int width,	COLORREF color)
{
	this->m_StartPoint=startPoint;
	this->m_EndPoint=endPoint;
	this->m_Width=width;
	this->m_Color=color;
}
