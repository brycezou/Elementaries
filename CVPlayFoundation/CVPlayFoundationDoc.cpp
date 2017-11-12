// CVPlayFoundationDoc.cpp : implementation of the CCVPlayFoundationDoc class
//

#include "stdafx.h"
#include "CVPlayFoundation.h"

#include "CVPlayFoundationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationDoc

IMPLEMENT_DYNCREATE(CCVPlayFoundationDoc, CDocument)

BEGIN_MESSAGE_MAP(CCVPlayFoundationDoc, CDocument)
	//{{AFX_MSG_MAP(CCVPlayFoundationDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationDoc construction/destruction

CCVPlayFoundationDoc::CCVPlayFoundationDoc()
{
	// TODO: add one-time construction code here

}

CCVPlayFoundationDoc::~CCVPlayFoundationDoc()
{
}

BOOL CCVPlayFoundationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationDoc serialization

void CCVPlayFoundationDoc::Serialize(CArchive& ar)
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
// CCVPlayFoundationDoc diagnostics

#ifdef _DEBUG
void CCVPlayFoundationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCVPlayFoundationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCVPlayFoundationDoc commands
