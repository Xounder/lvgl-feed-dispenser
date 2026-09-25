# Recria as junctions do sketch Arduino (arduino/).
# O Arduino IDE precisa enxergar o codigo compartilhado (src/) e o LVGL
# (lvgl/) dentro da pasta do sketch, sem duplicar arquivos no repositorio.
# Rode este script apos clonar o projeto (as junctions nao sao versionadas).

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$sketch   = $PSScriptRoot
$librariesDir = Join-Path $sketch 'libraries'

if (-not (Test-Path -LiteralPath $librariesDir)) {
    New-Item -ItemType Directory -Path $librariesDir -Force | Out-Null
}

$links = @(
    @{ Path = Join-Path $sketch 'src';
       Target = Join-Path $repoRoot 'src' },
    @{ Path = Join-Path $librariesDir 'lvgl';
       Target = Join-Path $repoRoot 'lvgl' }
)

foreach ($link in $links) {
    if (Test-Path -LiteralPath $link.Path) {
        Remove-Item -LiteralPath $link.Path -Force
    }
    New-Item -ItemType Junction -Path $link.Path -Target $link.Target | Out-Null
    Write-Host "junction criada: $($link.Path) -> $($link.Target)"
}