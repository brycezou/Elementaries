#if !defined(AFX_MEMORYNODE_H__40EDE21E_2DF4_4EDC_AE8B_FFFEA144D079__INCLUDED_)
#define AFX_MEMORYNODE_H__40EDE21E_2DF4_4EDC_AE8B_FFFEA144D079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoryNode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMemoryNode document

class CMemoryNode : public CDocument
{
protected:
	// protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CMemoryNode)
    DECLARE_SERIAL(CMemoryNode)
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoryNode)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF BrushColor;
	COLORREF PenColor;
	int Mark;	
    CMemoryNode(COLORREF pencolor,COLORREF brushcolor,int mark);
	CMemoryNode::CMemoryNode();
	virtual ~CMemoryNode();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMemoryNode)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYNODE_H__40EDE21E_2DF4_4EDC_AE8B_FFFEA144D079__INCLUDED_)
