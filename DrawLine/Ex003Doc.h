// Ex003Doc.h : interface of the CEx003Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EX003DOC_H__656B0366_A041_4DAB_8EF0_0FEB5A4F3272__INCLUDED_)
#define AFX_EX003DOC_H__656B0366_A041_4DAB_8EF0_0FEB5A4F3272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEx003Doc : public CDocument
{
protected: // create from serialization only
	CEx003Doc();
	DECLARE_DYNCREATE(CEx003Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx003Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEx003Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEx003Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX003DOC_H__656B0366_A041_4DAB_8EF0_0FEB5A4F3272__INCLUDED_)
