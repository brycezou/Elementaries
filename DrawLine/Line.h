// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__9B2DE641_C0CA_4E45_8DC1_268C2A5CB9E2__INCLUDED_)
#define AFX_LINE_H__9B2DE641_C0CA_4E45_8DC1_268C2A5CB9E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLine  
{
public:
	CPoint mpt2;
	CPoint mpt1;
	CLine(CPoint pt1,CPoint pt2);
	virtual ~CLine();

};

#endif // !defined(AFX_LINE_H__9B2DE641_C0CA_4E45_8DC1_268C2A5CB9E2__INCLUDED_)
