//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SortStringArray.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Function to sort a string array
//
#pragma once

class CSortStringArray : public CStringArray  
{
public:
	void Sort();
private: 
	BOOL CompareAndSwap( int pos );

};
