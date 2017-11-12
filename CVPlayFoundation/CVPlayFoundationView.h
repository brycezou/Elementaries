// CVPlayFoundationView.h : interface of the CCVPlayFoundationView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVPLAYFOUNDATIONVIEW_H__E79A346D_9D9D_4328_BC56_608DD0CC267E__INCLUDED_)
#define AFX_CVPLAYFOUNDATIONVIEW_H__E79A346D_9D9D_4328_BC56_608DD0CC267E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCVPlayFoundationView : public CView
{
protected: // create from serialization only
	CCVPlayFoundationView();
	DECLARE_DYNCREATE(CCVPlayFoundationView)

// Attributes
public:
	CCVPlayFoundationDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVPlayFoundationView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCVPlayFoundationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCVPlayFoundationView)
	afx_msg void OnMenuShowMyDlg();
	afx_msg void OnMenuShowNotModal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CVPlayFoundationView.cpp
inline CCVPlayFoundationDoc* CCVPlayFoundationView::GetDocument()
   { return (CCVPlayFoundationDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVPLAYFOUNDATIONVIEW_H__E79A346D_9D9D_4328_BC56_608DD0CC267E__INCLUDED_)
