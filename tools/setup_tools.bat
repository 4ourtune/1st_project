@echo off
echo [Installing required tools for Windows]

:: clang-format 설치
where clang-format >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing clang-format...
    winget install --id=LLVM.LLVM -e --accept-source-agreements
) else (
    echo clang-format is already installed.
)

:: cppcheck 설치
where cppcheck >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing cppcheck...
    winget install --id=Cppcheck.Cppcheck -e --accept-source-agreements
) else (
    echo cppcheck is already installed.
)

:: 환경 변수에 clang-format 경로 추가
where clang-format > tmp_path.txt 2>nul
set /p CLANG_PATH=<tmp_path.txt
del tmp_path.txt

:: clang-format.exe가 있는 폴더만 추출
for %%I in ("%CLANG_PATH%") do set CLANG_DIR=%%~dpI

:: PATH에 포함되어 있는지 확인
echo %PATH% | find /I "%CLANG_DIR%" >nul
if %ERRORLEVEL% NEQ 0 (
    echo Adding clang-format path to user PATH...
    setx PATH "%PATH%;%CLANG_DIR%"
) else (
    echo clang-format path already in PATH.
)

echo [Installation and setup complete.]
pause
