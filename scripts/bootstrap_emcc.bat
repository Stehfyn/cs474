@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\"
pushd %root%

set emsdkboot=.\external\emsdk\emsdk.bat
call %emsdkboot% install latest && %emsdkboot% activate latest

popd
endlocal