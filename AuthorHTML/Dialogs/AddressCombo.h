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
// AddressCombo window

class AddressCombo : public CComboBox
{
  DECLARE_DYNAMIC(AddressCombo)
// Construction
public:
	AddressCombo();
  virtual ~AddressCombo();
  void FillWithHistory();

protected:
	DECLARE_MESSAGE_MAP()
};

