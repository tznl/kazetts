@echo off
setlocal

set BDIR=build\
set INC=/Ithirdparty\dirent\include\ /Ithirdparty\piper1-gpl\libpiper\include /Ithirdparty\miniaudio\
set IN=src\main.c
set CFLAGS=/std:c11 /c

rem main
rd /s /q %BDIR%

mkdir %BDIR%

cl /MD %CFLAGS% %INC% %IN% /Fo%BDIR%main.obj
if errorlevel 1 exit /b 1

echo.
echo Build complete: main

rem miniaudio

cl /MD /c /Ithirdparty\miniaudio\ thirdparty\miniaudio\miniaudio.c /Fo%BDIR%miniaudio.obj
if errorlevel 1 exit /b 1

echo.
echo Build complete: miniaudio

rem link

cl /MD /nologo /Fe%BDIR%\run.exe %BDIR%\main.obj %BDIR%\miniaudio.obj ^
    /link /SUBSYSTEM:CONSOLE ^
    thirdparty\ucd-tools\libucd.lib thirdparty\espeak-ng\src\windows\x64\Release\libespeak-ng.lib thirdparty\onnxruntime\lib\onnxruntime.lib thirdparty\piper1-gpl\libpiper.lib 
	
if errorlevel 1 exit /b 1

copy thirdparty\onnxruntime\lib\onnxruntime.dll %BDIR%
copy thirdparty\espeak-ng\src\windows\x64\Release\libespeak-ng.dll %BDIR%

mkdir %BDIR%data\
mkdir %BDIR%data\export\
mkdir %BDIR%data\voice\
mkdir %BDIR%data\voice\default\
mkdir %BDIR%data\art\

type nul > %BDIR%data\config

copy thirdparty\art\art %BDIR%data\art\art
copy voice\default\default.onnx %BDIR%data\voice\default\default.onnx
copy voice\default\default.onnx.json %BDIR%data\voice\default\default.onnx.json

echo.
echo Build complete
