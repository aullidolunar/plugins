!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "open"
OutFile "open.exe"
RequestExecutionLevel user
ShowInstDetails show
Unicode true
XPStyle on

Page instfiles

# Just for testing
!addplugindir /x86-unicode "..\bin"

Function .onInit
InitPluginsDir
FunctionEnd


Section ""
; Open file dialog: 
; Initial directory, max MAX_PATH (260) chars
; filters, max MAX_PATH (260) chars
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the directory chosen.
Dialogs::Open "$PROGRAMFILES" "Text files (*.txt)|*.txt" 5 2
${if} $5 = 1
DetailPrint "Your file: $2"
${else}
DetailPrint "You haven't choose a file"
${endif}
SectionEnd
