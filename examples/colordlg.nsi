!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "colordlg"
OutFile "colordlg.exe"
RequestExecutionLevel user
ShowInstDetails show
Unicode true
XPStyle on
InstallDir "$PROGRAMFILES\Dialogs"

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
; Color dialog: 
; type: 0=decimal, 1=#rrggbb; 2=rr,gg,bb; 3=0x00rrggbb
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the color chosen.
${Dialogs}::ColorDlg 1 7 9
${if} $7 = 1
; Print it
DetailPrint "This is your color: $9"
${else}
DetailPrint "You canceled the dialog"
${endif}
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
