param(
    [Parameter(Mandatory=$true)]
    [string]$TestName
)

$TestName = $TestName -replace '\.cpp$', ''

Write-Host "========================================" -ForegroundColor Green
Write-Host "Testing: $TestName.cpp" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Check if we're in the right directory
if (-not (Test-Path "CMakeLists.txt")) {
    if ((Test-Path "test") -and (Test-Path "test/CMakeLists.txt")) {
        Write-Host "Moving to test directory..." -ForegroundColor Yellow
        Set-Location "test"
    } else {
        Write-Error "CMakeLists.txt not found! Please run from test directory or project root."
        exit 1
    }
}

# Clean build
if (Test-Path "build") {
    Write-Host "Cleaning previous build..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force "build"
}

# Try to find and setup Visual Studio environment
$vsPath = @(
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat",
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

$vsFound = $false
foreach ($path in $vsPath) {
    if (Test-Path $path) {
        Write-Host "Setting up Visual Studio environment..." -ForegroundColor Yellow
        cmd /c "`"$path`" && set" | ForEach-Object {
            if ($_ -match "^(.*?)=(.*)$") {
                Set-Content "env:\$($matches[1])" $matches[2]
            }
        }
        $vsFound = $true
        break
    }
}

if (-not $vsFound) {
    Write-Warning "Visual Studio not found. Using system defaults."
}

# Configure
Write-Host "Configuring..." -ForegroundColor Yellow

# Try different generators
$generators = @(
    "Visual Studio 17 2022",
    "Visual Studio 16 2019", 
    "Ninja",
    "Unix Makefiles"
)

$configSuccess = $false
foreach ($gen in $generators) {
    try {
        Write-Host "Trying generator: $gen" -ForegroundColor Cyan
        if ($gen -like "Visual Studio*") {
            cmake -B build -G $gen -A x64 -DCMAKE_BUILD_TYPE=Debug 2>$null
        } else {
            cmake -B build -G $gen -DCMAKE_BUILD_TYPE=Debug 2>$null
        }
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Successfully configured with: $gen" -ForegroundColor Green
            $configSuccess = $true
            break
        }
    } catch {
        continue
    }
}

if (-not $configSuccess) {
    Write-Host "Trying default generator..." -ForegroundColor Yellow
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    if ($LASTEXITCODE -ne 0) {
        Write-Error "All configuration attempts failed!"
        Write-Host ""
        Write-Host "Please try one of these solutions:" -ForegroundColor Yellow
        Write-Host "1. Run from Visual Studio Developer Command Prompt"
        Write-Host "2. Install Visual Studio 2022 with C++ workload"
        Write-Host "3. Install MinGW-w64 or MSYS2"
        exit 1
    }
}

# Build
Write-Host "Building..." -ForegroundColor Yellow
cmake --build build --config Debug
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}

# Find test executable
$testExe = @(
    "build\Debug\test_runner.exe",
    "build\test_runner.exe",
    "build\test_runner"
) | Where-Object { Test-Path $_ } | Select-Object -First 1

if (-not $testExe) {
    Write-Error "Test executable not found!"
    Write-Host "Contents of build directory:" -ForegroundColor Yellow
    Get-ChildItem build -Recurse | Format-Table Name, FullName
    exit 1
}

# Run test
Write-Host "Running test..." -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Green
& $testExe --gtest_filter="*$TestName*" --gtest_color=yes
$testResult = $LASTEXITCODE

Write-Host "========================================" -ForegroundColor Green
if ($testResult -eq 0) {
    Write-Host "Tests PASSED for $TestName" -ForegroundColor Green
} else {
    Write-Host "Tests FAILED for $TestName" -ForegroundColor Red
}
Write-Host "========================================" -ForegroundColor Green