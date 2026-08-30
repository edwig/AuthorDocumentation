//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ScanBOM.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Scan for a byte-order-mark (BOM)
//
#include "StdAfx.h"
#include "ScanBOM.h"
#include <io.h>

BOM_OpenResult 
open_file_with_bom(FILE**   p_file
                  ,LPCSTR   p_filename
                  ,LPCSTR   p_mode
                  ,int*     p_offset
                  ,BOMType* p_type)
{
  // Reset output parameters
  *p_file   = NULL;
  *p_offset = 0;
  *p_type   = BT_NO_BOM;
  
  // Check if file exists
  if(_access(p_filename,4) != 0)
  {
    // FILE pointer NULL
    return BOR_NoFileFound;
  }
  
  // Try opening the file
  fopen_s(p_file,p_filename,p_mode);
  
  // Test if opened
  if(*p_file == NULL)
  {
    return BOR_NotOpenedInMode;
  }
  
  // Get first four characters in the file
  int c1 = fgetc(*p_file);
  int c2 = fgetc(*p_file);
  int c3 = fgetc(*p_file);
  int c4 = fgetc(*p_file);
  
  // Check if Big-Endian UTF-8 BOM
  if(c1 == 0xEF && c2 == 0xBB && c3 == 0xBF)
  {
    // Yes BE-BOM in UTF-8
    fseek(*p_file,3L,SEEK_SET);
    *p_offset = 3;
    *p_type   = BT_BE_UTF8;
    return BOR_OpenedBom;
  }
  // Check UTF-8 BOM in other Endian 
  if(c1 == 0xBB || c1 == 0xBF)
  {
    // UTF-8 but incompatible. Might work yet!!
    fseek(*p_file,3L,SEEK_SET);
    *p_offset = 3;
    *p_type   = BT_LE_UTF8;
    return BOR_OpenedIncompatible;
  }
  // Check Big-Endian UTF-16
  if(c1 == 0xFE && c2 == 0xFF)
  {
    fseek(*p_file,2L,SEEK_SET);
    *p_offset = 2;
    *p_type   = BT_BE_UTF16;
    return BOR_OpenedBom;
  }
  // Check Little-ENdian UTF-16/UTF32
  if(c1 == 0xFF && c2 == 0xFE)
  {
    if(c3 == 0x0 && c4 == 0x0)
    {
      *p_offset = 4;
      *p_type   = BT_LE_UTF32;
      return BOR_OpenedIncompatible;
    }
    fseek(*p_file,2L,SEEK_SET);
    *p_offset = 2;
    *p_type   = BT_LE_UTF16;
    return BOR_OpenedIncompatible;
  }
  // Check Big-Endian UTF-32
  if(c1 == 0 && c2 == 0 && c3 == 0xFE && c4 == 0xFF)
  {
    *p_offset = 4;
    *p_type   = BT_BE_UTF32;
    return BOR_OpenedIncompatible;
  }
  // Check for UTF-7 special case
  if(c1 == 0x2B && c2 == 0x2F && c3 == 0x76)
  {
    if(c4 == 0x38 || c4 == 39 || c4 == 0x2B || c4 == 0x2F)
    {
      // Application still has to proces lower 2 bits 
      // of the 4th character. Re-spool to that char.
      fseek(*p_file,3L,SEEK_SET);
      *p_offset = 3;
      *p_type   = BT_BE_UTF7;
      return BOR_OpenedIncompatible;
    }
  }
  // Check for UTF-1 special case
  if(c1 == 0xF7 && c2 == 0x64 && c3 == 0x4C)
  {
    fseek(*p_file,3L,SEEK_SET);
    *p_offset = 3;
    *p_type   = BT_BE_UTF1;
    return BOR_OpenedIncompatible;
  }
  // Check for UTF-EBCDIC IBM set
  if(c1 == 0xDD && c2 == 0x73 && c3 == 0x66 && c4 == 0x73)
  {
    *p_offset = 4;
    *p_type   = BT_UTF_EBCDIC;
    return BOR_OpenedIncompatible;
  }
  // Check for CSCU 
  if(c1 == 0x0E && c2 == 0xFE && c3 == 0xFF)
  {
    fseek(*p_file,3L,SEEK_SET);
    *p_offset = 3;
    *p_type   = BT_BE_CSCU;
    return BOR_OpenedIncompatible;
  }
  // Check for BOCU-1
  if(c1 == 0xFB && c2 == 0xEE && c3 == 0x28)
  {
    fseek(*p_file,3L,SEEK_SET);
    *p_offset = 3;
    *p_type   = BT_BOCU_1;
    return BOR_OpenedIncompatible;
  }
  // Check GB-18030
  if(c1 == 0x84 && c2 == 0x31 && c3 == 0x95 && c4 == 0x33)
  {
    *p_offset = 4;
    *p_type   = BT_GB_18030;
    return BOR_OpenedIncompatible;
  }
  // GEEN BOM !!
  fseek(*p_file,0L,SEEK_SET);
  return BOR_OpenedNoBom;
}

