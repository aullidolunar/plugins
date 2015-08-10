!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "folderdlg"
OutFile "folderdlg.exe"
RequestExecutionLevel user
ShowInstDetails show
Unicode true
XPStyle on
InstallDir "$PROGRAMFILES\Dialogs"
Page directory
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
DetailPrint "Before Dialogs::FolderDlg -> $INSTDIR"
; Open folder dialog: 
; description, max MAX_PATH (260) chars
; Initial directory, max MAX_PATH (260) chars
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the directory chosen.
${Dialogs}::FolderDlg "Choose a good one" "$PROGRAMFILES" 0 21
${if} $0 = 1
; Print it
DetailPrint "After Dialogs::FolderDlg -> $INSTDIR"
${else}
DetailPrint "You canceled the dialog"
${endif}
!undef Dialogs
!undef NSIS_CHARSET
SectionEnd
