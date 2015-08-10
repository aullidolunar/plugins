SetCompressor /SOLID lzma
Name "ver"
OutFile "ver.exe"
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
; Save plugin version to $0
${Dialogs}::Ver 0
; Printed
DetailPrint "Plugin version: $0"
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
