$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $ScriptDir "build"
$WindowsExportDir = Join-Path $BuildDir "WindowsExport"
$LinuxExportDir = Join-Path $BuildDir "LinuxExport"

# ensure build folder exists
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

# cleanup build content except exports
Get-ChildItem -Path $BuildDir | Where-Object {
    $_.Name -ne "WindowsExport" -and $_.Name -ne "LinuxExport"
} | Remove-Item -Recurse -Force

# configure once
cmake -S $ScriptDir -B $BuildDir

# build both configs
cmake --build $BuildDir --config Debug
cmake --build $BuildDir --config Release

# recreate export dirs
if (Test-Path $WindowsExportDir) {
    Remove-Item $WindowsExportDir -Recurse -Force
}

New-Item -ItemType Directory -Force -Path $WindowsExportDir | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $WindowsExportDir "Debug") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $WindowsExportDir "Release") | Out-Null

# copy header
Copy-Item (Join-Path $ScriptDir "PlajaLogger.h") $WindowsExportDir -Force

# copy Debug artifacts
$DebugDir = Join-Path $BuildDir "Debug"
if (Test-Path $DebugDir) {
    Get-ChildItem -Path $DebugDir -File | Where-Object {
        $_.Extension -eq ".dll" -or $_.Extension -eq ".lib"
    } | ForEach-Object {
        Copy-Item $_.FullName (Join-Path $WindowsExportDir "Debug") -Force
    }
}

# copy Release artifacts
$ReleaseDir = Join-Path $BuildDir "Release"
if (Test-Path $ReleaseDir) {
    Get-ChildItem -Path $ReleaseDir -File | Where-Object {
        $_.Extension -eq ".dll" -or $_.Extension -eq ".lib"
    } | ForEach-Object {
        Copy-Item $_.FullName (Join-Path $WindowsExportDir "Release") -Force
    }
}

# cleanup build content except exports
Get-ChildItem -Path $BuildDir | Where-Object {
    $_.Name -ne "WindowsExport" -and $_.Name -ne "LinuxExport"
} | Remove-Item -Recurse -Force

Write-Host "DONE"
Write-Host "Output:"
Get-ChildItem $WindowsExportDir -Recurse