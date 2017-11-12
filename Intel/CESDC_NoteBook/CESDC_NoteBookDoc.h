// CESDC_NoteBookDoc.h : interface of the CCESDC_NoteBookDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CESDC_NOTEBOOKDOC_H__A99A4FF5_C163_4B15_ABD7_9C28E46D3935__INCLUDED_)
#define AFX_CESDC_NOTEBOOKDOC_H__A99A4FF5_C163_4B15_ABD7_9C28E46D3935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCESDC_NoteBookDoc : public CDocument
{
protected: // create from serialization only
	CCESDC_NoteBookDoc();
	DECLARE_DYNCREATE(CCESDC_NoteBookDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCESDC_NoteBookDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCESDC_NoteBookDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCESDC_NoteBookDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESDC_NOTEBOOKDOC_H__A99A4FF5_C163_4B15_ABD7_9C28E46D3935__INCLUDED_)
