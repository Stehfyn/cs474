@echo off
setlocal enabledelayedexpansion

set root="%~dp0"
call .\scripts\install_node.bat
call .\scripts\bootstrap_emcc.bat
call .\scripts\install_extension.bat %1

endlocal