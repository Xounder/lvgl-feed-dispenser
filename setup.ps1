#requires -Version 5.1
[CmdletBinding()]
param(
    [switch]$NoRun,
    [switch]$Clean
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

function Write-Step { param([string]$Message) Write-Host "==> $Message" -ForegroundColor Cyan }

function Get-VisualStudioPaths {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere)) { return @() }
    $paths = & $vswhere -latest -products * -property installationPath 2>$null
    return @($paths | Where-Object { $_ -and (Test-Path -LiteralPath $_) } | ForEach-Object { $_.Trim() } | Select-Object -Unique)
}

function Resolve-VcpkgRoot {
    if ($env:VCPKG_ROOT -and (Test-Path -LiteralPath (Join-Path $env:VCPKG_ROOT 'scripts\buildsystems\vcpkg.cmake'))) {
        return $env:VCPKG_ROOT
    }

    $candidates = New-Object System.Collections.Generic.List[string]
    foreach ($vs in Get-VisualStudioPaths) { $candidates.Add((Join-Path $vs 'VC\vcpkg')) }
    if ($env:VCPKG_ROOT) { $candidates.Add($env:VCPKG_ROOT) }
    if ($env:ProgramFiles) { $candidates.Add((Join-Path $env:ProgramFiles 'vcpkg')) }
    if (${env:ProgramFiles(x86)}) { $candidates.Add((Join-Path ${env:ProgramFiles(x86)} 'vcpkg')) }
    if ($env:USERPROFILE) { $candidates.Add((Join-Path $env:USERPROFILE 'vcpkg')) }
    if ($env:SystemDrive) { $candidates.Add((Join-Path $env:SystemDrive 'vcpkg')) }
    $cmd = Get-Command vcpkg -ErrorAction SilentlyContinue
    if ($cmd) { $candidates.Add((Split-Path -Parent $cmd.Source)) }

    foreach ($c in $candidates) {
        if ($c -and (Test-Path -LiteralPath (Join-Path $c 'scripts\buildsystems\vcpkg.cmake'))) {
            return (Resolve-Path -LiteralPath $c).Path
        }
    }
    return $null
}

function Resolve-CMake {
    $cmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    foreach ($vs in Get-VisualStudioPaths) {
        $c = Join-Path $vs 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
        if (Test-Path -LiteralPath $c) { return $c }
    }
    if ($env:ProgramFiles) {
        $c = Join-Path $env:ProgramFiles 'CMake\bin\cmake.exe'
        if (Test-Path -LiteralPath $c) { return $c }
    }
    return $null
}

if (Test-Path -LiteralPath (Join-Path $root '.gitmodules')) {
    if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
        throw 'git nao encontrado no PATH (necessario para os submodules).'
    }
    Write-Step 'Sincronizando submodules (lvgl, FreeRTOS)'
    & git -C $root submodule update --init --recursive
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao inicializar os submodules.' }
}

foreach ($dep in @('lvgl', 'FreeRTOS')) {
    $p = Join-Path $root $dep
    $hasContent = (Test-Path -LiteralPath $p) -and (Get-ChildItem -LiteralPath $p -Force -ErrorAction SilentlyContinue | Select-Object -First 1)
    if (-not $hasContent) {
        throw "Dependencia '$dep' ausente/vazia. Rode: git submodule update --init --recursive"
    }
}

$vcpkgRoot = Resolve-VcpkgRoot
if (-not $vcpkgRoot) {
    throw 'vcpkg nao encontrado. Instale o Visual Studio Build Tools (inclui o vcpkg) ou defina a variavel de ambiente VCPKG_ROOT.'
}
$env:VCPKG_ROOT = $vcpkgRoot
Write-Host "vcpkg:  $vcpkgRoot"

$cmakeExe = Resolve-CMake
if (-not $cmakeExe) {
    throw 'cmake nao encontrado. Instale o CMake ou o Visual Studio Build Tools.'
}
$env:PATH = "$(Split-Path -Parent $cmakeExe);$env:PATH"
Write-Host "cmake:  $cmakeExe"

$buildDir = Join-Path $root 'build'
if ((Test-Path -LiteralPath (Join-Path $buildDir 'CMakeCache.txt')) -and -not (Test-Path -LiteralPath (Join-Path $buildDir 'vcpkg_installed'))) {
    Write-Step 'Removendo build/ antigo (incompativel com o modo manifest do vcpkg)'
    Remove-Item -LiteralPath $buildDir -Recurse -Force
}

& (Join-Path $root 'src_pc\build.ps1') -Clean:$Clean

$exe = Join-Path $root 'bin\Debug\main.exe'
if (-not (Test-Path -LiteralPath $exe)) { throw "Executavel nao encontrado: $exe" }

if ($NoRun) {
    Write-Step "Pronto: $exe"
    return
}

Write-Step "Executando $exe"
& $exe
