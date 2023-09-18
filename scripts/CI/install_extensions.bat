@echo off
setlocal

set root="%~dp0..\..\"
pushd %root%

set vsix_installer="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\VSIXInstaller.exe"

rem make tool cache
if not exist external\VSExt\cache mkdir external\VSExt\cache

rem download
if not exist external\VSExt\cache\Emscripten.Build.Definition.vsix (
    curl -L --show-error https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix -o external\VSExt\cache\Emscripten.Build.Definition.vsix
)
if not exist external\VSExt\cache\Emscripten.ExtensionPack.vsix (
    curl -L --show-error https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.ExtensionPack.vsix -o external\VSExt\cache\Emscripten.ExtensionPack.vsix
)

rem install
%vsix_installer% /q /a external\VSExt\cache\Emscripten.Build.Definition.vsix

popd
endlocal