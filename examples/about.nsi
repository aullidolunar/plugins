SetCompressor /SOLID lzma
Name "about"
OutFile "about.exe"
RequestExecutionLevel user
ShowInstDetails show
Unicode true
XPStyle on

Page instfiles

; Just for testing
!ifdef NSIS_UNICODE
!define NSIS_CHARSET "/x86-unicode"
!define Dialogs "DialogsW"
!else
!define NSIS_CHARSET "/x86-ansi"
!define Dialogs "DialogsA"
!endif

!addplugindir "${NSIS_CHARSET}" "..\bin"

Function .onInit
InitPluginsDir
FunctionEnd


Section ""
; You should see a MessageBox with plugin information
${Dialogs}::About
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
