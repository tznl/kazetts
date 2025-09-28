@echo on
setlocal

set UCD_DIR=ucd-tools\src\
set UCD_IN=%UCD_DIR%case.c  %UCD_DIR%categories.c  %UCD_DIR%ctype.c  %UCD_DIR%proplist.c  %UCD_DIR%scripts.c  %UCD_DIR%tostring.c
set UCD_OUT=case.obj categories.obj ctype.obj proplist.obj scripts.obj tostring.obj
set UCD_INC=/I%UCD_DIR%include\

set E_ROOT=espeak-ng/build/
set E_INS=%E_ROOT%espeak_ng-install/
set E_BUILD=%E_ROOT%espeak_ng/
set E_SRC=%E_ROOT%espeak_ng_src/

set P_IN=piper1-gpl\libpiper\src\piper.cpp
set P_INC=/Ipiper1-gpl\libpiper\include /Ionnxruntime\include /Iespeak-ng\src\include

set CFLAGS=/nologo /W4 /std:c11

rem libucd

cl /MD /c %UCD_IN% %CFLAGS% %UCD_INC%
if errorlevel 1 exit /b 1

lib /OUT:libucd.lib *.obj
if errorlevel 1 exit /b 1

del *.obj
move libucd.lib ucd-tools\

echo.
echo Build complete: ucd-tools\libucd.lib

rem espeak

msbuild espeak-ng\src\windows\espeak-ng.sln /p:SubSystem=Console /p:Configuration=Release /p:Platform=x64

echo.
echo Build complete: espeak-ng\src\windows\x64\Debug\

rem libpiper

rem cl /MD /std:c++17 /c %P_IN% %P_INC%
rem if errorlevel 1 exit /b 1

rem lib /OUT:libpiper.lib *.obj
rem if errorlevel 1 exit /b 1

cl /LD /MD /std:c++17 %P_IN% %P_INC% /Fe:libpiper.dll /link /IMPLIB:libpiper.lib /OUT:libpiper.dll ^
	espeak-ng\src\windows\x64\Release\libespeak-ng.lib onnxruntime\lib\onnxruntime.lib

del *.obj
move libpiper.lib piper1-gpl\
move libpiper.dll piper1-gpl\

echo.
echo Build complete: piper1-gpl\libpiper\libpiper.*

endlocal