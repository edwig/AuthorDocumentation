// ScintillaScriptDoc.h : interface of the CScintillaScriptDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ScintillaView.h"

class CScintillaScriptDoc : public CScintillaDoc
{
protected: // create from serialization only
	CScintillaScriptDoc();
	DECLARE_DYNCREATE(CScintillaScriptDoc)

	//{{AFX_VIRTUAL(CScintillaScriptDoc)
	//}}AFX_VIRTUAL

public:
	virtual ~CScintillaScriptDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CScintillaScriptDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
