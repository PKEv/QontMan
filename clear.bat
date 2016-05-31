del /S /Q *.o
del /S /Q *.cpp
del /S /Q *.log
rem SET PATH=%PATH%; c:\Qt\Qt5.5.1\5.5\mingw492_32\bin\
rem windeployqt.exe --release --force --no-translations %CD%
