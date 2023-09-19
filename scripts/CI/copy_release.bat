@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\..\"
pushd %root%

for /D %%G in ("bin\cs474-client\Emscripten\Release") do (
    xcopy "%%G" ".\Release" /e /i /y
)

popd
endlocal
