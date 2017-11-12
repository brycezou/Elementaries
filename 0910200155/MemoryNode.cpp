// MemoryNode.cpp : implementation file
//

#include "stdafx.h"
#include "0910200155.h"
#include "MemoryNode.h"
#include "0910200155Dlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoryNode

//IMPLEMENT_DYNCREATE(CMemoryNode, CDocument);
IMPLEMENT_SERIAL(CMemoryNode, CDocument,1)

CMemoryNode::CMemoryNode(COLORREF pencolor,COLORREF brushcolor,int mark)
{
    this->PenColor=pencolor;
	this->BrushColor=brushcolor;
	this->Mark=mark;
}
CMemoryNode::CMemoryNode()
{
}


BOOL CMemoryNode::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMemoryNode::~CMemoryNode()
{
}


BEGIN_MESSAGE_MAP(CMemoryNode, CDocument)
	//{{AFX_MSG_MAP(CMemoryNode)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryNode diagnostics

#ifdef _DEBUG
void CMemoryNode::AssertValid() const
{
	CDocument::AssertValid();
}

void CMemoryNode::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMemoryNode serialization


/////////////////////////////////////////////////////////////////////////////
// CMemoryNode commands

void CMemoryNode::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar<<PenColor<<BrushColor<<Mark;
	}
	else
	{	// loading code
        ar<<PenColor<<BrushColor<<Mark;
	}
}
