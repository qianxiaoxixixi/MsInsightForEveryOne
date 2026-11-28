@echo off
setlocal
node "%~dp0rag-cli.mjs" %*
exit /b %errorlevel%
