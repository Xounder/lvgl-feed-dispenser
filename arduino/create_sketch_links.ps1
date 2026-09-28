# Recria as junctions do sketch Arduino (arduino/).
# O Arduino IDE precisa enxergar o codigo compartilhado (src/) dentro da pasta
# do sketch, sem duplicar arquivos no repositorio.
# Rode este script apos clonar o projeto (as junctions nao sao versionadas).
#
# O LVGL NAO entra mais aqui: usa-se o LVGL 9.6.0 do Library Manager (global),
# e o `lv_conf.h` da raiz do sketch (arduino/lv_conf.h) encaminha para
# `config/lv_conf_esp32.h`.

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$sketch   = $PSScriptRoot

$links = @(
    @{ Path = Join-Path $sketch 'src';
       Target = Join-Path $repoRoot 'src' }
)

foreach ($link in $links) {
    if (Test-Path -LiteralPath $link.Path) {
        # `cmd /c rmdir` remove apenas o link da junction (sem tocar no destino),
        # diferente do Remove-Item que pede confirmacao interativa.
        cmd /c rmdir "$($link.Path)" 2>$null
    }
    New-Item -ItemType Junction -Path $link.Path -Target $link.Target | Out-Null
    Write-Host "junction criada: $($link.Path) -> $($link.Target)"
}