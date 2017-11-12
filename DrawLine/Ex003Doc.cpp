// Ex003Doc.cpp : implementation of the CEx003Doc class
//

#include "stdafx.h"
#include "Ex003.h"

#include "Ex003Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEx003Doc

IMPLEMENT_DYNCREATE(CEx003Doc, CDocument)

BEGIN_MESSAGE_MAP(CEx003Doc, CDocument)
	//{{AFX_MSG_MAP(CEx003Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEx003Doc construction/destruction

CEx003Doc::CEx003Doc()
{
	// TODO: add one-time construction code here

}

CEx003Doc::~CEx003Doc()
{
}

BOOL CEx003Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEx003Doc serialization

void CEx003Doc::Serialize(CArchive& ar)
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
// CEx003Doc diagnostics

#ifdef _DEBUG
void CEx003Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEx003Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEx003Doc commands
