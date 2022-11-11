@echo off
cd /d %~dp0
mkdir cmake-build-debug\bin\x64
for /f %%f in ('dir /b bin\x64\') do mklink /H cmake-build-debug\bin\x64\%%f bin\x64\%%f
for /f %%f in ('dir /b bin\') do mklink /H cmake-build-debug\bin\%%f bin\%%f
