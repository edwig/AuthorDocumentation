// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#define VC_EXTRALEAN		        // Exclude rarely-used stuff from Windows headers
#include <targetver.h>

#define _CRT_SECURE_NO_WARNINGS

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxdisp.h>    // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls
//#include <afxcontrolbars.h>
#include <afxtoolbar.h>
#include <afxstatusbar.h>
#include <afxmdiframewndex.h>
#include <afxmdiclientareawnd.h>
#include <afxdockablepane.h>

#include <afxdlgs.h>
#include <atlbase.h>
#include <afxhtml.h>
#include <atlcom.h>

#include <afxdhtml.h>

#include "Platform.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "BasisUI.h"            // Use these UI elements

// Automatic linking with the BaseLibrary
#include <BaseLibrary.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif