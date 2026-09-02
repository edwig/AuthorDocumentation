;-------------------------------------------------------
;
; File      : Installer.nsi
; Written by: W.E. Huisman
;
; Date of last change: 01-09-2026
; Version:             1.0.1
;-------------------------------------------------------
 !define PRODUCT_NAME                         "AuthorDocumentation"
 !define PRODUCT_VERSION                      "1.0"
 !define PRODUCT_SP                           "1"
 !define PRODUCT_BUILDNUMBER                  "936"
 !define PRODUCT_PUBLISHER                    "EDO"
 !define PRODUCT_WEB_SITE                     "https://github.com/edwig/AuthorDocumentation"
 !define PRODUCT_DIR_REGKEY                   "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}"
 !define PRODUCT_UNINST_KEY                   "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
 !define PRODUCT_UNINST_ROOT_KEY              "HKLM"
 !define MUI_HEADERIMAGE
 ;!define MUI_HEADERIMAGE_BITMAP              "header.bmp"
 !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
 !define MUI_DIRECTORYPAGE_VERIFYONLEAVE
 !define LogFile                              "AuthorDocumentation"
  
;--------------------------------------------------------------------------------------------------------

 !include MUI2.nsh
 !include WordFunc.nsh
 !include LogicLib.nsh
 !include FileFunc.nsh
 !include x64.nsh
 !include WinMessages.nsh
;--------------------------------------------------------------------------------------------------------
 SetPluginUnload alwaysoff
 BrandingText /TRIMRIGHT "AuthorDocumentation for CHM"
 InstallColors 000000 FFFFFF
 XPStyle on
 RequestExecutionLevel admin
 Target amd64-unicode
 OutFile "Setup_AuthorDocumentation_${PRODUCT_VERSION}.${PRODUCT_SP}.exe"

;--------------------------------------------------------------------------------------------------------
 ; title of the setup
 Name "${PRODUCT_NAME} ${PRODUCT_VERSION}.${PRODUCT_SP}"
;--------------------------------------------------------------------------------------------------------
 ; Places it in the standard installation folder
 ; IIS is never in program files, but in it's own directory under MS-Windows
 InstallDir  "C:\Program Files\EDO\AuthorDocumentation"

;--------------------------------------------------------------------------------------------------------
 ; MUI Settings
 !define MUI_ICON   "AuthorHTML\res\AuthorHTML.ico"
 !define MUI_UNICON "AuthorHTML\res\AuthorHTML.ico"
 !define MUI_BGCOLOR F0F0F0
 !define MUI_HEADER_TRANSPARENT_TEXT
 !define MUI_COMPONENTSPAGE_NODESC
 !define MUI_COMPONENTSPAGE_TEXT_COMPLIST "Components to install"
 !define MUI_FINISHPAGE_NOAUTOCLOSE
 !define MUI_UNFINISHPAGE_NOAUTOCLOSE
 !define MUI_ABORTWARNING
 !define MUI_ABORTWARNING_CANCEL_DEFAULT
 !define MUI_UNABORTWARNING
 !define MUI_UNABORTWARNING_CANCEL_DEFAULT
;--------------------------------------------------------------------------------------------------------
 ; Language Selection Dialog Settings
 !define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
 !define MUI_LANGDLL_REGISTRY_KEY  "${PRODUCT_UNINST_KEY}"
 !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
 !define MUI_LANGDLL_WINDOWTITLE "Language"
 !define MUI_LANGDLL_INFO "Select a language for the ${PRODUCT_NAME} installer"
 !define MUI_FINISHPAGE_RUN "$INSTDIR\bin\AuthorHTML.exe"

;--------------------------------------------------------------------------------------------------------
 ; Welcome page
 !insertmacro MUI_PAGE_WELCOME
;--------------------------------------------------------------------------------------------------------
 !insertmacro MUI_PAGE_DIRECTORY
 !insertmacro MUI_PAGE_INSTFILES
 !insertmacro MUI_PAGE_FINISH
;--------------------------------------------------------------------------------------------------------
 !insertmacro MUI_UNPAGE_CONFIRM
 !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------------------------------------------------------------------------------
; Language files
 !insertmacro MUI_LANGUAGE "English"
;--------------------------------------------------------------------------------------------------------
 ShowInstDetails   show
 ShowUnInstDetails show
;--------------------------------------------------------------------------------------------------------


;--------------------------------------------------------------------------------------------------------
Function .onInit
; !insertmacro MUI_LANGDLL_DISPLAY
functionend

;--------------------------------------------------------------------------------------------------------
Section "AuthorDocumentation"

; Explicitly naming the installed files
  SetOverwrite on

  SetOutPath "$INSTDIR\bin"
  
  File "bin\AuthorHTML.exe"
  
  SetOutPath "$INSTDIR\doc"
  
  File "doc\HtmlRef.chm"
  File "doc\W3C_CSS2.pdf"
  File "doc\W3C_HTML401.pdf"
  
  SetOutPath "$INSTDIR\ms"
  
  File "ms\API.chm"
  File "ms\HHAXref.chm"
  File "ms\htmlhelp.chm"
  File "ms\HTMLref.chm"
  File "ms\ViewHlp.chm"
  File "ms\HtmlHelp.exe"
 
  SetOutPath "$INSTDIR\spell"
 
  File "AuthorHTML\spell\*.dict"
  
 ClearErrors

SectionEnd

;--------------------------------------------------------------------------------------------------------
Section -Post
 WriteUninstaller "$INSTDIR\Uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.${PRODUCT_SP}.exe"

 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName"     "$(^Name)"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.${PRODUCT_SP}.exe"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon"     "$INSTDIR\uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.${PRODUCT_SP}.exe"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"  "${PRODUCT_VERSION}.${PRODUCT_SP}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout"    "${PRODUCT_WEB_SITE}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher"       "${PRODUCT_PUBLISHER}"

 CreateShortCut "$DESKTOP\AuthorHTML.lnk"      "$INSTDIR\bin\AuthorHTML.exe"


 ; This was the last section
 ; We can now close the logfile
 MessageBox MB_ICONQUESTION|MB_OK|MB_DEFBUTTON2 "The installation of ${PRODUCT_NAME} ${PRODUCT_VERSION} is ready" /SD IDNO IDNO 
SectionEnd

;--------------------------------------------------------------------------------------------------------
Function un.onUninstSuccess
 HideWindow
 MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) has been removed from your computer." /SD IDOK 
FunctionEnd

;--------------------------------------------------------------------------------------------------------
Function un.onInit
 MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure that you want to remove $(^Name) and all its components completely?" /SD IDYES IDYES +2
 Abort
FunctionEnd
;--------------------------------------------------------------------------------------------------------

Section Uninstall
 SetAutoClose true

 RmDir /r /REBOOTOK "$INSTDIR"
 Delete "$DESKTOP\AuthorHTML.lnk"
  
 ;De-Registration of the product.
 DetailPrint "De-register of ${PRODUCT_NAME}"
 DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
 
SectionEnd

;--------------------------------------------------------------------------------------------------------
; End of the script
