#include "stdafx.h"
#include "AuthorHTML.h"
#include "ScintillaScriptDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CScintillaScriptDoc, CScintillaDoc)

BEGIN_MESSAGE_MAP(CScintillaScriptDoc, CScintillaDoc)
	//{{AFX_MSG_MAP(CScintillaScriptDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CScintillaScriptDoc::CScintillaScriptDoc()
{
}

CScintillaScriptDoc::~CScintillaScriptDoc()
{
}

#ifdef _DEBUG
void CScintillaScriptDoc::AssertValid() const
{
	CScintillaDoc::AssertValid();
}

void CScintillaScriptDoc::Dump(CDumpContext& dc) const
{
	CScintillaDoc::Dump(dc);
}
#endif //_DEBUG
