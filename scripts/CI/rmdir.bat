@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\..\"
pushd %root%

if not "%1" == "" (
    set "target=%1"
    rmdir /s /q !target! >nul 2>&1 && (echo Removed folder - %root%!target!) || (echo Could Not Find %root%!target!)
)

popd
endlocal