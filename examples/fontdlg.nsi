!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "fontdlg"
OutFile "fontdlg.exe"
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
; Font dialog:
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the font chosen. The format of the buffer is: fontname=string,fontweight=integer,italic=0|1,fontsize=integer
${Dialogs}::FontDlg 3 8
${if} $3 = 1
; Print it
DetailPrint "This is your font: $8"
${else}
DetailPrint "You canceled the dialog"
${endif}
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
