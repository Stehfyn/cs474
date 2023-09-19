@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\..\"
pushd %root%

for /D %%G in ("assets") do (
    xcopy "%%G" "src\%%~nxG" /e /i /y /d
)

popd
endlocal