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
#pragma once

typedef enum _open_result
{
  BOR_NoFileFound
 ,BOR_NotOpenedInMode
 ,BOR_OpenedNoBom
 ,BOR_OpenedBom
 ,BOR_OpenedIncompatible
}
BOM_OpenResult;

typedef enum _bom_type
{
  BT_NO_BOM
 ,BT_BE_UTF1
 ,BT_BE_UTF7
 ,BT_BE_UTF8
 ,BT_BE_UTF16
 ,BT_BE_UTF32
 ,BT_BE_CSCU
 ,BT_LE_UTF8
 ,BT_LE_UTF16
 ,BT_LE_UTF32
 ,BT_UTF_EBCDIC
 ,BT_BOCU_1
 ,BT_GB_18030
}
BOMType;

BOM_OpenResult open_file_with_bom(FILE**   p_file        // Resulting file pointer
                                ,LPCSTR   p_filename    // File to open
                                ,LPCSTR   p_mode        // File mode to open in
                                ,int*     p_offset      // Resulting base offset after BOM
                                ,BOMType* p_type);      // Type of BOM found