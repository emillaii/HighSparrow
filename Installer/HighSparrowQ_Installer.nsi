;NSIS Modern User Interface
;HighSparrowQ Installer
;Written by Edward Zhou

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "HighSparrowQ"
  OutFile "HighSparrowQ_Installer.exe"

  ;Default installation folder
  InstallDir "C:\HighSparrowQ\"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\HighSparrowQ" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var StartMenuFolder
  
;--------------------------------
;Interface Settings

  !define MUI_ICON "sparrow.ico"

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\HighSparrowQ" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "HighSparrowQ" SecHighSparrowQ

  ; HighSparrowQ Binary Files
  SetOutPath "$INSTDIR"  
  SetOverwrite on
  Delete "$INSTDIR\*"
  File ".\deploy\*"  

  ;Store installation folder
  WriteRegStr HKCU "Software\HighSparrowQ" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts with hard coded path
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortcut "$DESKTOP\HighSprrowQ.lnk" "$INSTDIR\HighSprrowQ.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\HighSprrowQ.lnk" "$INSTDIR\HighSprrowQ.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecHighSparrowQ ${LANG_ENGLISH} "HighSparrowQ Binary Files"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecHighSparrowQ} $(DESC_SecHighSparrowQ)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\*"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  
  Delete "$DESKTOP\HighSprrowQ.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\HighSprrowQ.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey /ifempty HKCU "Software\HighSparrowQ"

SectionEnd
