!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "drivesdlg"
OutFile "drivesdlg.exe"
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
; DrivesDlg dialog:
; title of the dialog (max 64 chars)
; Caption of the combobox (max 64 chars)
; label for button ok (max 12 chars)
; label for button cancel (max 12 chars)
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the drive letter chosen.
${Dialogs}::Ver 10
${Dialogs}::DrivesDlg "foo title: $R0" "Choose a drive letter" "Do it" "B0rked" 1 7
${if} $1 = 1
; Print it
DetailPrint "This is your drive: $7"
${else}
DetailPrint "You canceled the dialog"
${endif}
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
