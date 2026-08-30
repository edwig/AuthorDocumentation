//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SortStringArray.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Function to sort a string array
//
#include "stdafx.h"
#include "SortStringArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CSortStringArray::Sort()
{
  BOOL bNotDone = TRUE;   
  while (bNotDone)   
  {  
    bNotDone = FALSE;
    for(int pos = 0;pos < GetUpperBound();pos++)
    {
      bNotDone |= CompareAndSwap(pos);
    }
  }
}
BOOL CSortStringArray::CompareAndSwap( int pos )
{
   CString temp;   
   int posFirst = pos;   
   int posNext = pos + 1;
   if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)   
   {
      temp = GetAt(posFirst);      
      SetAt(posFirst, GetAt(posNext));
      SetAt(posNext, temp);      
      return TRUE;   
   }    
   return FALSE;
}