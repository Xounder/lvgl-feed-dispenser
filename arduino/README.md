# Sketch Arduino (ESP32-S3)

Pasta de atalho (`junctions`) para rodar o codigo do projeto na placa usando o
**Arduino IDE** como a equipe padrao, sem duplicar arquivos.

> **Variante de dois ESP32:** ver `.docs/14-external-esp32-communication.md`.
> Nela existem **dois sketches** (tela e externo) e cada placa precisa do **seu
> proprio compile** (detalhes na secao "Firmwares" abaixo).

## O que ha aqui

- `arduino.ino` — identifica a pasta como sketch (o codigo real esta em `src/`).
  Este e o sketch da **tela** (ESP32-S3 touch: UI + proxy UART).
- `lv_conf.h` — Config do LVGL do ESP32 (encaminha para `config/lv_conf_esp32.h`).

O outro firmware fica em `arduino_ctrl/` (ESP32 **externo**: dominio + HX711 +
servo + botoes/LED); compartilha o mesmo `src/` por junction e **nao usa LVGL**.

O **LVGL vem do Library Manager (versao 9.6.0)**, instalado na pasta global
da Arduino IDE; por isso o sketch nao tem junction `libraries/lvgl`.

Como o `src/` do projeto agora e apenas o codigo compartilhado + plataforma
ESP32 (os arquivos do PC ficam em `src_pc/`), o Arduino compila so o que
interessa ao hardware.

## Como usar

1. Rode `.\create_sketch_links.ps1` (no PowerShell, na raiz do projeto).
2. Arduino IDE: `File > Open...` e abra `arduino/arduino.ino`.
3. Board Manager: instale **ESP32 by Espressif**; selecione **ESP32S3 Dev Module**.
4. Opcoes da placa (Tools): Flash Size `16MB`, Flash Mode `QIO/OPI`, Partition `Default 16MB`, PSRAM conforme o modulo.
   - Ao selecionar a placa vem com defaults do core, que **nao sao esses**: Flash Size
     costuma vir como `8MB` e Partition como `Default 8MB` — confira e ajuste.
   - Confirme o modulo pelo rotulo no chip: `N16` = flash 16MB, `R8` = PSRAM 8MB.
     Se o modulo for de 8MB de flash, use `8MB` / `Default 8MB`.
   - Flash Mode: QIO (padrao) ou OPI se o modulo tiver flash octal.
   - PSRAM: no S3 Dev Module ja vem como `OPI PSRAM` (sugerido manter ativo).
5. Library Manager: instale as bibliotecas abaixo (nova secao "Bibliotecas").
6. Compile e faça upload.

## Dois firmwares (tela + externo)

A variante usa **dois ESP32** (ver `.docs/14-external-esp32-communication.md`): o
controle fisico (HX711, servo, botoes, LED) roda na placa externa e a tela faz so
a UI. Isso exige **um sketch por firmware** e **um compile por placa**:

| Sketch | Placa | Papel (`build_opt.h`) | Libs |
| --- | --- | --- | --- |
| `arduino/` | ESP32-S3 da tela (touch) | `-DROLE_SCREEN` | LVGL |
| `arduino_ctrl/` | ESP32 externo | `-DROLE_EXTERNAL` | HX711, ESP32Servo |

Pontos importantes:

- **Nao existe compile unico**: tela e externo sao dois programas separados. O
  Arduino IDE compila toda a arvore do sketch (inclusive a junction `src/`),
  entao o papel e fixado em **tempo de compilacao** por sketch, nao em runtime.
- Para "unir" os dois basta o **hardware**: ligar **TX<->RX cruzado** e **GND
  comum** entre as placas; a partir dai conversam por UART. Pinagem e ligacoes
  detalhadas (pino a pino) em
  `.docs/14-external-esp32-communication/07-conexoes-fisicas.md`; protocolo em
  `.docs/14-external-esp32-communication/04-protocolo-e-mensagens.md`.
- Ao alterar codigo compartilhado em `src/`, **recompile e regrave as duas
  placas** (um upload para cada).
- Cada sketch tem o seu `build_opt.h` com o define de papel; o
  `create_sketch_links.ps1` (na raiz) cria as junctions dos dois.
- Arquivos de papel unico em `src/` usam guardas `#ifdef ROLE_*` para nao
  duplicar simbolos entre os firmwares (lista em
  `.docs/14-external-esp32-communication/02-arquitetura-e-camadas.md`).
- O sketch da tela (`arduino/`) **nao** instala HX711/servo; o externo
  (`arduino_ctrl/`) **nao** usa LVGL.

## Bibliotecas (Library Manager)

Busque pelo **nome exato** e confira o **autor** antes de instalar:

| Nome no Manager | Autor | Sketch | Consumido em |
| --- | --- | --- | --- |
| **LVGL** | LVGL (kisvegabor) — **9.6.0** | `arduino/` (tela) | `src/` (toda a UI) |
| **ESP32Servo** | Kevin Harrington (madhephaestus), John K. Bennett | `arduino_ctrl/` (externo) | `src/hardware/esp32/real_dispenser.cpp` |
| **HX711** | Bogdan Necula (bogde) | `arduino_ctrl/` (externo) | `src/hardware/esp32/real_weight_sensor.cpp` |
| **Servo** (classica) | Michael Margolis / Arduino | — | somente placas AVR (Uno/Mega) — nao usar no ESP32 |

Observacoes:

- O LVGL 9.6.0 instalado via Library Manager precisa da config ao lado da pasta
  global (`Documents/Arduino/libraries/lv_conf.h`)? **Nao**: o `lv_conf.h` da
  raiz do sketch resolve pelos include paths (`__has_include`/`LV_CONF_INCLUDE_SIMPLE`
  do `lv_conf_internal.h`), sem mexer na instalacao global — portatil entre PCs.

- Se o Library Manager nao listar nada (indice nao baixa, rede bloqueada),
  instale via `Sketch > Include Library > Add .ZIP Library`:
  - `https://github.com/madhephaestus/ESP32Servo`
  - `https://github.com/bogde/HX711`
- Nao usar a biblioteca **Servo** classica (Michael Margolis / Arduino): ela so
  compila em placas AVR (Uno/Mega) e nao funciona no ESP32.

## Estado

O driver de display/touch (`src/hardware/esp32/board_display.cpp`) foi
implementado: cria o painel RGB 800x480 (ST7262), rotaciona a UI retrato
480x800 para o frame buffer físico e provê o indev do GT911. A UI sobe no
ESP32 quando `board_display_init()` retorna o `lv_display_t`.

Primeiro boot ainda é experimental: se as cores saírem trocadas (R/B),
trocar `LV_COLOR_FORMAT_RGB565` por `LV_COLOR_FORMAT_RGB565_SWAPPED` no
flush (knob em `config/lv_conf_esp32.h` já tem suporte). Se a orientação
estiver invertida, ajustar o mapeamento na rotação/touch em
`src/hardware/esp32/board_display.cpp`. Referência completa de pesquisa:
`../.docs/MIGRACAO-ESP32-NOTAS.md`.

## Referencia

Estrutura e decisoes em `.docs/11-migration-pc-to-esp32/` e
`.docs/09-pc-development-environment/`.