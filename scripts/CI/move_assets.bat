@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\"
pushd %root%

for /D %%G in ("assets") do (
    move "%%G" "src\assets"
)

popd
endlocal