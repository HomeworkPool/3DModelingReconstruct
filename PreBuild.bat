@echo off
cd /d %~dp0
if not exist cmake-build-debug\bin\x64\. mkdir cmake-build-debug\bin\x64
for /f %%f in ('dir /b bin\x64\') do if not exist cmake-build-debug\bin\x64\%%f\. mklink /H cmake-build-debug\bin\x64\%%f bin\x64\%%f
for /f %%f in ('dir /b bin\') do if not exist cmake-build-debug\bin\%%f\. mklink /H cmake-build-debug\bin\%%f bin\%%f

for /f %%f in ('dir /b resources\') do if exist resources\%%f\* if not exist cmake-build-debug\bin\x64\%%f\. mklink /D cmake-build-debug\bin\x64\%%f ..\..\..\resources\%%f
for /f %%f in ('dir /b resources\') do if exist resources\%%f if not exist cmake-build-debug\bin\x64\%%f mklink /H cmake-build-debug\bin\x64\%%f resources\%%f
