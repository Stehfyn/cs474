@echo off
setlocal

set root="%~dp0..\"
pushd %root%

rem make tool cache
if not exist external\nodejs\cache mkdir external\nodejs\cache

rem download
if not exist external\nodejs\cache\node-v18.17.1-x64.msi (
    curl -L --show-error https://nodejs.org/dist/v18.17.1/node-v18.17.1-x64.msi -o external\nodejs\cache\node-v18.17.1-x64.msi
)

rem install
if not exist external\nodejs\node.exe (
    msiexec /a external\nodejs\cache\node-v18.17.1-x64.msi /qn TARGETDIR="%CD%\external" /qb /l*v "install_7z.log"
)

rem cleanup
if exist external\node-v18.17.1-x64.msi (
    del /s /q /f external\node-v18.17.1-x64.msi
)

popd
endlocal