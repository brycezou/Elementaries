// Ex003View.h : interface of the CEx003View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EX003VIEW_H__70255875_9A9C_4F90_94D1_37FB1C4ED206__INCLUDED_)
#define AFX_EX003VIEW_H__70255875_9A9C_4F90_94D1_37FB1C4ED206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEx003View : public CScrollView
{
protected: // create from serialization only
	CEx003View();
	DECLARE_DYNCREATE(CEx003View)

// Attributes
public:
	CEx003Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx003View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CLine * pline;
	CPtrArray pta;
	int flag;
	CPoint pt2;
	CPoint pt1;
	virtual ~CEx003View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEx003View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Ex003View.cpp
inline CEx003Doc* CEx003View::GetDocument()
   { return (CEx003Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX003VIEW_H__70255875_9A9C_4F90_94D1_37FB1C4ED206__INCLUDED_)
