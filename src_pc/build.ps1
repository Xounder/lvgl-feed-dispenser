#requires -Version 5.1
[CmdletBinding()]
param(
    [switch]$Clean
)

$ErrorActionPreference = 'Stop'

$vcpkgToolchain = $null
if ($env:VCPKG_ROOT) { $vcpkgToolchain = Join-Path $env:VCPKG_ROOT 'scripts\buildsystems\vcpkg.cmake' }
if (-not $vcpkgToolchain -or -not (Test-Path -LiteralPath $vcpkgToolchain)) {
    throw 'VCPKG_ROOT nao definido/encontrado. Rode o script da raiz: .\setup.ps1'
}

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw 'cmake nao encontrado no PATH. Rode o script da raiz: .\setup.ps1'
}

$repoRoot = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..')).Path
$buildDir = Join-Path $repoRoot 'build'
if ($Clean -and (Test-Path -LiteralPath $buildDir)) {
    Write-Host "==> Removendo build/: $buildDir"
    Remove-Item -LiteralPath $buildDir -Recurse -Force
}

Push-Location $PSScriptRoot
try {
    Write-Host '==> cmake --preset default'
    cmake --preset default
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao configurar (cmake --preset default).' }

    Write-Host '==> cmake --build --preset debug'
    cmake --build --preset debug
    if ($LASTEXITCODE -ne 0) { throw 'Falha no build (cmake --build --preset debug).' }
}
finally {
    Pop-Location
}
