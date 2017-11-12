// CESDC_NoteBookView.h : interface of the CCESDC_NoteBookView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CESDC_NOTEBOOKVIEW_H__067430DA_F788_4F91_AD4F_7A01296B5BDD__INCLUDED_)
#define AFX_CESDC_NOTEBOOKVIEW_H__067430DA_F788_4F91_AD4F_7A01296B5BDD__INCLUDED_
#include "SpecialLine.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCESDC_NoteBookView : public CView
{
protected: // create from serialization only
	CCESDC_NoteBookView();
	DECLARE_DYNCREATE(CCESDC_NoteBookView)

// Attributes
public:
	CCESDC_NoteBookDoc* GetDocument();
	CPoint StartPt;
	CPoint EndPt;
	COLORREF m_penColor;
	CPen Pen;
	CSpecialLine *m_pSaveLines;
	CSpecialLine *m_pOpenLines;
	CPtrArray m_SaveLineArray;
	CPtrArray m_OpenLineArray;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_NoteBookView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnClearRect();
	void OnChooseRest2(UINT nID);
	BOOL OnRequest();
	void OnChooseRest(UINT nID);
	void OnPenChoice(UINT nID);
	void OnFileSave();
	void OnFileOpen();
	void OnFileNew();
	void OnCloseWindow();
	virtual ~CCESDC_NoteBookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCESDC_NoteBookView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CESDC_NoteBookView.cpp
inline CCESDC_NoteBookDoc* CCESDC_NoteBookView::GetDocument()
   { return (CCESDC_NoteBookDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_NOTEBOOKVIEW_H__067430DA_F788_4F91_AD4F_7A01296B5BDD__INCLUDED_)
