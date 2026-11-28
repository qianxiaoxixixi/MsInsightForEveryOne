@echo off
setlocal
if not exist "%~dp0runtime\node.exe" (
    echo Bundled Node.js runtime is missing. Please repair the installation. 1>&2
    exit /b 1
)
"%~dp0runtime\node.exe" "%~dp0rag-cli.mjs" %*
exit /b %errorlevel%
