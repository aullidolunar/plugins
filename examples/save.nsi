!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "save"
OutFile "save.exe"
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
; Save file dialog: 
; Initial directory, max MAX_PATH (260) chars
; filters, max MAX_PATH (260) chars
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the directory chosen.
${Dialogs}::Save "$PROGRAMFILES" "Text files (*.txt)|*.txt" 5 2
${if} $5 = 1
DetailPrint "Your file: $2"
${else}
DetailPrint "You haven't choose a file"
${endif}
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
