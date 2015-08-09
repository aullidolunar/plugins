SetCompressor /SOLID lzma
Name "about"
OutFile "about.exe"
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
; You should see a MessageBox with plugin information
Dialogs::About
SectionEnd
