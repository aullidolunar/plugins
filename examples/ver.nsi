SetCompressor /SOLID lzma
Name "ver"
OutFile "ver.exe"
RequestExecutionLevel user
Unicode true
XPStyle on

Page instfiles

# Just for testing
!addplugindir /x86-unicode "..\bin"

Function .onInit
InitPluginsDir
FunctionEnd


Section ""
; Save plugin version to $0
Dialogs::ver 0
; Printed
DetailPrint "Plugin version: $0"
SectionEnd
