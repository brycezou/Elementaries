// CVPlayFoundationDoc.h : interface of the CCVPlayFoundationDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVPLAYFOUNDATIONDOC_H__3E0D593C_70F6_4179_A274_954BB6946A79__INCLUDED_)
#define AFX_CVPLAYFOUNDATIONDOC_H__3E0D593C_70F6_4179_A274_954BB6946A79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCVPlayFoundationDoc : public CDocument
{
protected: // create from serialization only
	CCVPlayFoundationDoc();
	DECLARE_DYNCREATE(CCVPlayFoundationDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVPlayFoundationDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCVPlayFoundationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCVPlayFoundationDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVPLAYFOUNDATIONDOC_H__3E0D593C_70F6_4179_A274_954BB6946A79__INCLUDED_)
