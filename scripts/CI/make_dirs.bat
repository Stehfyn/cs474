@echo off
setlocal enabledelayedexpansion

set root="%~dp0..\..\"
pushd %root%

mkdir ci-bin\imgui\Emscripten\Release
mkdir ci-bin-int\imgui\Emscripten\Release
mkdir ci-bin\cs474\Emscripten\Release
mkdir ci-bin-int\cs474\Emscripten\Release
mkdir ci-bin\cs474-client\Emscripten\Release
mkdir ci-bin-int\cs474-client\Emscripten\Release

popd
endlocal
