//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AddressCombo.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Combobox with the history of the addresses of documentation files
//              used in the AuthorHTML application
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAddressCombo window

class CAddressCombo : public CComboBox
{
// Construction
public:
	CAddressCombo();
	void FillWithHistory();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddressCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	CFont m_font;
	virtual ~CAddressCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAddressCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

