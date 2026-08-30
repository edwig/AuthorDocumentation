@echo off
echo HTML tree cleaner
echo Cleans complete source tree
echo .

echo Clean AuthorHTML\Builds
rmdir /q /s    AuthorHTML\Builds
rmdir /q /s    AuthorHTML\BaseLibrary\x64
rmdir /q /s    AuthorHTML\lib


echo Clean AuthorHTML\.vs visual studio cache
rmdir /q /s    AuthorHTML\.vs

echo Clean bin directories
rmdir /q /s    bin
rmdir /q /s    AuthorHTML\Bin_Debugx64
rmdir /q /s    AuthorHTML\Bin_Releasex64

echo Clean AuthorHTML other files
del   /q /s /f AuthorHTML\AuthorHTML.vcxproj.user

echo .
echo Tree cleaned.....
