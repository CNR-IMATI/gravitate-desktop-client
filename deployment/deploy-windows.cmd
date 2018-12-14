@ECHO OFF

REM This script should be executed on a Release build. Other build configurations may give
REM unexpected results.

SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

REM IF EXIST deploy-conf.cmd (
REM     CALL deploy-conf.cmd
REM )

SET "VCVARSALL=@VCVARSALL_BAT@"

REM IF %VCVARSALL% == "VCVARSALL_BAT-NOTFOUND" (
REM    SET /p VSDIR=Enter the path to your Visual Studio installation (2015 or 2017)
REM
REM IF %VSDIR~ (
REM        SET "VCVARSALL=%VSDIR%\VC\Auxiliary\Build\vcvarsall.bat"
REM    )
REM )


REM Date formatting code from: https://stackoverflow.com/a/10945887

for /f %%x in ('wmic path win32_localtime get /format:list ^| findstr "="') do set %%x
set Month=00%Month%
set Day=00%Day%
set TODAY=%Year%-%Month:~-2%-%Day:~-2%

SET "QTDIR=@QTDIR@"
SET "VTKDIR=@VTK_DIR@"
SET "BUILD_DIR=@BUILD_DIR@"

SET "CURRENTDIR=%~dp0"
SET "TARGET_APP=gravitate-client.exe"
SET "WINDEPLOYQT=@QTDIR@\bin\windeployqt.exe"
SET "TARGET_EXE=@BUILD_DIR@\%TARGET_APP%"
SET "STANDALONE_EXE=@BUILD_DIR@\standalone-mesh-viewer.exe"
SET "DIST_DIR=%CURRENTDIR%\gravitate-client-Win64-%TODAY%"
	
SET ERROR=false

IF NOT DEFINED VCVARSALL (
	ECHO "Visual Studio installation not found"
	SET ERROR=true
)

IF NOT EXIST "%VCVARSALL%" (
	ECHO "%VCVARSALL%" does not exist
	SET ERROR=true
)

IF NOT DEFINED QTDIR (
	ECHO Please set QTDIR variable
	SET ERROR=true
)

IF NOT DEFINED BUILD_DIR (
	ECHO Please set BUILD_DIR variable
	SET ERROR=true
)

IF NOT EXIST "%WINDEPLOYQT%" (
	ECHO windeployqt not found
	SET ERROR=true
)

IF NOT EXIST %TARGET_EXE% (
	ECHO %TARGET_APP% not found in %BUILD_DIR%
	SET ERROR=true
)

IF %ERROR% equ true (
	EXIT /b
)

@echo & SET "VCVARSALL=%VCVARSALL%"> deploy-conf.cmd

SET "VTKBINDIR=%VTKDIR%\bin\@CMAKE_BUILD_TYPE@"

mkdir "%DIST_DIR%"
CALL "%VCVARSALL%" x64
CALL "%QTDIR%\bin\qtenv2.bat"

COPY /b/v/y "%TARGET_EXE%" "%DIST_DIR%\"
COPY /b/v/y "%STANDALONE_EXE%" "%DIST_DIR%\"

"%WINDEPLOYQT%" "%TARGET_EXE%" --dir "%DIST_DIR%" --compiler-runtime

REM copy other dependencies
copy /b/v/y "%VTKBINDIR%\*.dll" "%DIST_DIR%"
copy /b/v/y "%CURRENTDIR%\libeay32.dll" "%DIST_DIR%"
copy /b/v/y "%CURRENTDIR%\ssleay32.dll" "%DIST_DIR%"

REM Copy User Manual files
robocopy "%BUILD_DIR%\docs\help" "%DIST_DIR%\docs\help" desktop-help.qhc
robocopy "%BUILD_DIR%\docs\help" "%DIST_DIR%\docs\help" gravitate-manual.qch



