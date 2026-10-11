# Recria as junctions src/ dos dois sketches Arduino (arduino/ e arduino_ctrl/).
# O Arduino IDE precisa enxergar o codigo compartilhado (src/) dentro da pasta
# do sketch, sem duplicar arquivos no repositorio.
# Rode este script (na raiz do repositorio) apos clonar o projeto: as junctions
# nao sao versionadas.
#
# Sao dois firmwares independentes (variante de dois ESP32):
#   arduino/       (tela)     -> build_opt.h: -DROLE_SCREEN
#   arduino_ctrl/  (externo)  -> build_opt.h: -DROLE_EXTERNAL
# Detalhes em .docs/14-external-esp32-communication.md.
#
# O LVGL NAO entra aqui: usa-se o LVGL 9.6.0 do Library Manager (global),
# e o `lv_conf.h` da raiz do sketch da tela (arduino/lv_conf.h) encaminha para
# `config/lv_conf_esp32.h`.

$ErrorActionPreference = 'Stop'

$repoRoot = $PSScriptRoot
$src      = Join-Path $repoRoot 'src'

$dirs = @(
    (Join-Path $repoRoot 'arduino'),
    (Join-Path $repoRoot 'arduino_ctrl')
)

foreach ($dir in $dirs) {
    $link = Join-Path $dir 'src'

    if (Test-Path -LiteralPath $link) {
        # `cmd /c rmdir` remove apenas o link da junction (sem tocar no destino),
        # diferente do Remove-Item que pede confirmacao interativa.
        cmd /c rmdir "$link" 2>$null
    }

    New-Item -ItemType Junction -Path $link -Target $src | Out-Null
    Write-Host "junction criada: $link -> $src"
}
