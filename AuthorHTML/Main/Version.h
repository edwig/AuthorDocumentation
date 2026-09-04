//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Version.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// The definition of the application name and version labels
// 
#define REGISTER_APP      "EDO\\AuthorDocumentation"          // Registry key for settings under HKCU\Software
#define TECHNICAL_APP     "AuthorHTML.exe"                    // GetExePath uses the **REAL** technical name
#define APPLICATION_NAME  "AuthorHTML"                        // Name for the outside world (Messsage boxes etc)
#define GLYPH_SEES_EXE    "res://authorhtml.exe/GIFS/"        // Glyphs used by the MSHTML.DLL renderer for the preview window

// Version information

#define VERSION_MAJOR     1
#define VERSION_MINOR     0
#define VERSION_SP        2
#define BUILD_NUMBER      944
#define VERSION_DATES     "2007 - 2026"

#define STRINGIZE_(x)     #x
#define STRINGIZE(x)      STRINGIZE_(x)

// Compounded version information

#define VERSION_NUMBER    STRINGIZE(VERSION_MAJOR) "." STRINGIZE(VERSION_MINOR) "." STRINGIZE(VERSION_SP)
#define VERSION_COMPLETE  STRINGIZE(VERSION_MAJOR) "." STRINGIZE(VERSION_MINOR) "." STRINGIZE(VERSION_SP) ":" STRINGIZE(BUILD_NUMBER)
