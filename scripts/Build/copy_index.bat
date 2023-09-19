@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\..\"
pushd %root%

if "%~1"=="" (
    echo No destination directory specified. Exiting.
    exit
)

if not exist "%~1\" (
    echo The specified directory does not exist. Exiting.
    exit
)

xcopy src\index.html "%~1\" /y /d

popd
endlocal