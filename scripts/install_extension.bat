@echo off
setlocal

set root="%~dp0..\"
pushd %root%

set vsix_installer="C:\Program Files (x86)\Microsoft Visual Studio\Installer\resources\app\ServiceHub\Services\Microsoft.VisualStudio.Setup.Service\VSIXInstaller.exe"

rem Create a marker file path for the installed extension
set marker_file="external\VSExt\cache\Emscripten.Build.Definition.installed"

rem Create tool cache directory if not exists
if not exist external\VSExt\cache mkdir external\VSExt\cache

if exist %marker_file% (
    echo The VSIX extension is already installed.
    goto :end
)

if "%~1" == "--bypass" (
    echo Bypassing installation and creating marker file.
    echo Visual Studio Emscripten Extension installation bypassed on %date% %time% > %marker_file%
    goto :end
)

if not exist external\VSExt\cache\Emscripten.Build.Definition.vsix (
    curl -L --show-error https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix -o external\VSExt\cache\Emscripten.Build.Definition.vsix
)

%vsix_installer% /q /a external\VSExt\cache\Emscripten.Build.Definition.vsix

echo Installed on %date% %time% > %marker_file%

:end
popd
endlocal