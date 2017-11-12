// SpecialLine.h: interface for the CSpecialLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECIALLINE_H__FC9D3FF1_E767_4AD6_9110_41C23FE352D3__INCLUDED_)
#define AFX_SPECIALLINE_H__FC9D3FF1_E767_4AD6_9110_41C23FE352D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSpecialLine  
{
public:
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	int m_Width;
	COLORREF m_Color;
	CSpecialLine(CPoint,CPoint,int,COLORREF);
	CSpecialLine();
	virtual ~CSpecialLine();

};

#endif // !defined(AFX_SPECIALLINE_H__FC9D3FF1_E767_4AD6_9110_41C23FE352D3__INCLUDED_)
