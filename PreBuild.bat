@echo off
cd /d %~dp0
set CMakeOutputDir=%1
IF "%CMakeOutputDir%"=="" (
    set CMakeOutputDir=cmake-build-debug
)
echo CMAKE output dir: %CMakeOutputDir%

if not exist %CMakeOutputDir%\bin\x64\. mkdir %CMakeOutputDir%\bin\x64
for /f %%f in ('dir /b bin\x64\') do if not exist %CMakeOutputDir%\bin\x64\%%f\. mklink /H %CMakeOutputDir%\bin\x64\%%f bin\x64\%%f
for /f %%f in ('dir /b bin\') do if not exist %CMakeOutputDir%\bin\%%f\. mklink /H %CMakeOutputDir%\bin\%%f bin\%%f

for /f %%f in ('dir /b resources\') do if exist resources\%%f\* if not exist %CMakeOutputDir%\bin\x64\%%f\. mklink /D %CMakeOutputDir%\bin\x64\%%f ..\..\..\resources\%%f
for /f %%f in ('dir /b resources\') do if exist resources\%%f if not exist %CMakeOutputDir%\bin\x64\%%f mklink /H %CMakeOutputDir%\bin\x64\%%f resources\%%f
