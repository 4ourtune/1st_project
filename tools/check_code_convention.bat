@echo off
setlocal enabledelayedexpansion

echo [Code Convention Check - Windows]

:: Move to project root (this script is in tools/)
cd /d "%~dp0\.."

:: Define directories
set SRC_DIRS=src include tools\test_code
set "REPORT_FILE=tools\code-convention-report.txt"

:: Clear previous report
if exist %REPORT_FILE% del %REPORT_FILE%

:: Set paths manually (adjust as needed)
for %%I in ("C:\Program Files\LLVM\bin") do set "LLVM_BIN=%%~sI"
for %%I in ("C:\Program Files\Cppcheck") do set "CPPCHECK_BIN=%%~sI"
set "PATH=%PATH%;%LLVM_BIN%;%CPPCHECK_BIN%"

:: Step 1: clang-format
echo [Step 1] clang-format check >> %REPORT_FILE%

set FORMAT_FAIL=0
for %%D in (%SRC_DIRS%) do (
    for %%F in (%%D\*.c %%D\*.h) do (
        if exist "%%F" (
            clang-format --dry-run --Werror "%%F" 2>> %REPORT_FILE%
            if !ERRORLEVEL! EQU 0 (
                echo OK: %%F >> %REPORT_FILE%
            ) else (
                echo Formatting issue: %%F >> %REPORT_FILE%
                set FORMAT_FAIL=1
            )
        )
    )
)

:: Step 2: cppcheck
echo. >> %REPORT_FILE%
echo [Step 2] cppcheck >> %REPORT_FILE%

cppcheck --enable=all --std=c99 --quiet -Iinclude %SRC_DIRS% 2>> %REPORT_FILE%

:: Output report to console
type %REPORT_FILE%

:: Check for errors in report
findstr "error:" %REPORT_FILE% >nul
set "CPPCHECK_FAIL=%ERRORLEVEL%"

:: Report summary
if %FORMAT_FAIL% NEQ 0 (
    echo clang-format check failed. >> %REPORT_FILE%
)
if %CPPCHECK_FAIL% EQU 0 (
    echo cppcheck found errors. >> %REPORT_FILE%
)

if %FORMAT_FAIL% NEQ 0 (
    exit /b 1
)
if %CPPCHECK_FAIL% EQU 0 (
    exit /b 1
)

echo All checks passed. >> %REPORT_FILE%
exit /b 0
