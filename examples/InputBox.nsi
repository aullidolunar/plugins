!include "LogicLib.nsh"

SetCompressor /SOLID lzma
Name "InputBox"
OutFile "InputBox.exe"
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
; InputBox dialog: 
; Type: 0=text,1=password
; Dialog title, max 64 chars
; Secundary text, max 64 chars
; label ok, max 12 chars
; label cancel, max 12 chars
; Return var for the button pressed: 0=cancel, 1=ok
; Return var with the inputed text
Dialogs::Ver 9
Dialogs::InputBox 1 "Dialogs plugin version $9" "I need some foo text" "Go!" "Close Me" 4 6
${if} $4 = 1
DetailPrint "Your foo text: $6"
${else}
DetailPrint "You don't want foo text"
${endif}
SectionEnd
