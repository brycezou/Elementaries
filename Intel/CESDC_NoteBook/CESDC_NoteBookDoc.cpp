// CESDC_NoteBookDoc.cpp : implementation of the CCESDC_NoteBookDoc class
//

#include "stdafx.h"
#include "CESDC_NoteBook.h"

#include "CESDC_NoteBookDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookDoc

IMPLEMENT_DYNCREATE(CCESDC_NoteBookDoc, CDocument)

BEGIN_MESSAGE_MAP(CCESDC_NoteBookDoc, CDocument)
	//{{AFX_MSG_MAP(CCESDC_NoteBookDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookDoc construction/destruction

CCESDC_NoteBookDoc::CCESDC_NoteBookDoc()
{
	// TODO: add one-time construction code here

}

CCESDC_NoteBookDoc::~CCESDC_NoteBookDoc()
{
}

BOOL CCESDC_NoteBookDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookDoc serialization

void CCESDC_NoteBookDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookDoc diagnostics

#ifdef _DEBUG
void CCESDC_NoteBookDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCESDC_NoteBookDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCESDC_NoteBookDoc commands

void CCESDC_NoteBookDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	lpszTitle="NJUST";
	CDocument::SetTitle(lpszTitle);
}
