# Validação de pinagem — VIEWE UEDX80480043E-WB-B

Documento consolidado da validação da pinagem usada no firmware ESP32
(`src/hardware/esp32/board_config.h`) contra a placa real
**VIEWE UEDX80480043E-WB-B** (ESP32-S3 N16R8, display RGB 4,3" 800×480
ST7262E43‑G4, touch GT911 via I²C) e o **BSP oficial** da VIEWE
(`viewesmart/bsp_uedx80480043_wb_a` — WB‑A/WB‑B são software-compatíveis).

---

### 1. Pinos validados como corretos

Comparação direta com o BSP oficial:

| Função                    | `board_config.h`          | BSP oficial               | Veredito |
| ------------------------- | ------------------------- | ------------------------- | -------- |
| LCD PCLK / DE / HSYNC / VSYNC | 42 / 40 / 39 / 41     | 42 / 40 / 39 / 41         | OK       |
| LCD DATA0–15 (RGB565)     | 8, 3, 46, 9, 1, 5, 6, 7, 15, 16, 4, 45, 48, 47, 21, 14 | idem | OK |
| Backlight (PWM, ativo alto) | 2                     | 2                         | OK       |
| Touch SDA / SCL / RST     | 19 / 20 / 38 (INT = -1)   | 19 / 20 / 38 (INT não usado) | OK    |

A documentação canônica desses pinos está em
[`08-target-hardware/02-display-e-touch.md`](08-target-hardware/02-display-e-touch.md).

---

### 2. Pontos levantados — periféricos de domínio

A placa só disponibiliza como "free IOs" reais o **GPIO17** (trilho do
WS2812B) e o **GPIO18** (INT do GT911); os demais periféricos ocupam o
grupo do slot microSD (**GPIO10–13**). A melhor documentação desses
pontos é [`08-target-hardware/11-limitacoes-e-seguranca.md`](08-target-hardware/11-limitacoes-e-seguranca.md),
complementada por [`08-target-hardware/05-botoes-e-indicadores.md`](08-target-hardware/05-botoes-e-indicadores.md).

| Periférico               | Pino no projeto | Ocupação real na placa            | Situação |
| ------------------------ | --------------- | --------------------------------- | -------- |
| HX711 DT (DATA)          | GPIO11          | microSD MOSI                      | Requer slot SD sem cartão |
| HX711 SCK                | GPIO12          | microSD CLK                       | Requer slot SD sem cartão |
| Servo (PWM)              | GPIO13          | microSD MISO                      | Requer slot SD sem cartão |
| Botão liberação (1)      | GPIO10          | microSD CS                        | Requer slot SD sem cartão |
| Botão emergência (2)     | GPIO18          | INT do GT911 (não usado p/ touch) | Validar que o GT911 não aciona o pino |
| LED indicador            | GPIO17          | WS2812B on-board                  | Requer remover/desconectar o RGB |

Pontos levantados na análise:

1. **GPIO10–13 (microSD SPI)**: o datasheet da VIEWE libera esses pinos
   quando o display RGB é usado e o SD **não**. O projeto não usa SD;
   aceitável desde que **nenhum cartão fique no slot** e o SPI do SD nunca
   seja inicializado. (Ver `11-limitacoes-e-seguranca.md`.)
2. **GPIO18 (INT do GT911)**: o pino está fisicamente roteado ao flex do
   touch, que é sempre usado pela UI. Se o GT911 tiver INT habilitada,
   toques na tela podem ser lidos como "botão pressionado" ou gerar
   glitches. Validar que o GT911 não aciona o pino (config `0x8040`
   INT=0 / high‑Z) e usar pull adequado no botão.
   (Ver `05-botoes-e-indicadores.md`.)
3. **GPIO17 (WS2812B)**: não funciona como LED simples — o RGB on-board
   compartilha a linha, interpreta o sinal e puxa corrente (brilho/flicker
   aleatórios). Só é usável após **remover/desconectar o RGB** (a VIEWE
   indica o RGB removível pelo header 1,5 mm) ou usar o protocolo WS2812.
   (Ver `11-limitacoes-e-seguranca.md`.)

A proposta de uso de GPIO10–13 para UART externa (TX/RX = 12/13) está em
[`14-external-esp32-communication/03-pinos-e-constantes.md`](14-external-esp32-communication/03-pinos-e-constantes.md)
e **conflita** com HX711/Servo — é alternativa para o cenário em que os
periféricos de domínio migram para o ESP32 externo.

---

### 3. Pendências de validação física

Antes de fixar a pinagem na montagem final:

- [ ] confirmar que **nenhum cartão microSD** permanece no slot
      (GPIO10–13);
- [ ] validar que o **GT911 não aciona GPIO18** durante toques
      (botão de emergência sem falsos acionamentos);
- [ ] validar que o **WS2812B em GPIO17** foi removido/desconectado
      caso o LED indicador seja digital simples;
- [ ] alimentar o **servo** por fonte própria (os pinos são apenas de
      sinal; corrente/pico tratados em `08-target-hardware/06-alimentacao.md`);
- [ ] comparar os **timings do LCD** com o BSP (PCLK 15 MHz vs 16 MHz e
      porches diferentes) no primeiro boot real.

---

### 4. Validação da montagem do projeto Arduino (PC → ESP32)

Verificação da forma atual como o projeto é montado para o Arduino IDE
(pasta `arduino/`), do ponto de vista de build/integração/execução — não
só de pinagem. Resultado: **a montagem funciona corretamente**.

| Ponto verificado        | Como está                                                             | Veredito |
| ----------------------- | --------------------------------------------------------------------- | -------- |
| Código compartilhado    | junction `arduino/src` → `src/` (recriável por `create_sketch_links.ps1`; não versionada) | OK |
| Biblioteca LVGL         | LVGL **9.6.0** do Library Manager global (sem junction `libraries/lvgl`) | OK |
| Config do LVGL          | `arduino/lv_conf.h` → `config/lv_conf_esp32.h`                | OK       |
| Malloc custom           | `LV_STDLIB_CUSTOM` (`config/lv_conf_esp32.h:36`) implementado em `src/hardware/esp32/lv_mem_custom.cpp` | OK |
| Laço de execução        | `src/main_esp32.cpp:42` (`loop()` → `lv_timer_handler()`)      | OK       |
| Domínio periódico       | timer LVGL de 300 ms (`src/ui/screens/dosing_screen.c:223`) → `dosing_controller_update()` | OK |
| Sensoriamento           | HX711 real (`real_weight_sensor.cpp`); `real_add_grams` é no-op (correto — o peso vem da balança) | OK |
| Display/touch           | framebuffer em PSRAM (`fb_in_psram`), buffer LVGL 480×800 em SPIRAM e rotação no `flush_cb` (`board_display.cpp`) | OK |

Detalhes de como o `lv_conf.h` é resolvido sem alterar a instalação global:

- o `lv_conf_internal.h` do LVGL 9.6 usa `__has_include("lv_conf.h")` e,
  achando o arquivo no *include path* do sketch (raiz de `arduino/`),
  o inclui via `LV_CONF_INCLUDE_SIMPLE`;
- `arduino/lv_conf.h` encaminha para `../config/lv_conf_esp32.h`, ou seja,
  o repo não depende de copiar config para dentro da pasta global do LVGL
  (portátil entre PCs);
- o malloc custom usa `heap_caps_malloc` (PSRAM primeiro, queda para RAM
  interna) na API do LVGL 9.6 (`lv_malloc_core`/`lv_realloc_core`/...).

Essa base consta em
[`11-migration-pc-to-esp32/09-estrutura-e-duas-plataformas.md`](11-migration-pc-to-esp32/09-estrutura-e-duas-plataformas.md)
e nas instruções de uso em `arduino/README.md`.

---

### 5. Botões físicos e LED (integrado)

Esta lacuna vinha do [`Trabalho.md`](Trabalho.md), que exige três
comandos físicos além da tela:

| Requisito | Exigência (Trabalho.md)                                              |
| --------- | -------------------------------------------------------------------- |
| **RS14**  | Liberação manual direta por **botão físico** (mantido pressionado).   |
| **RS15**  | Impedir liberação manual enquanto a dosagem automática estiver ativa. |
| **RS16**  | **LED** aceso enquanto o modo de liberação manual estiver ativo.      |
| **RS11/RS12** | **Botão físico de emergência** com prioridade sobre a dosagem.   |

**Situação no firmware (ajustada):** os pinos
(`BOARD_BUTTON_1_PIN` 10, `BOARD_BUTTON_2_PIN` 18, `BOARD_LED_PIN` 17 em
`src/hardware/esp32/board_config.h`) agora são lidos/acionados pelo
firmware. A integração foi feita seguindo a abstração de hardware já
existente:

- `src/hardware/button.h` / `indicator.h` — interfaces (`Button`,
  `Indicator`), análogas a `dispenser.h`/`weight_sensor.h`;
- `src/hardware/esp32/real_buttons.cpp` — leitura dos GPIOs com
  **debounce** por software (`BOARD_BUTTON_DEBOUNCE_MS`);
- `src/hardware/esp32/real_indicator.cpp` — acionamento do LED;
- `src/hardware/esp32/physical_controls.cpp` — glue que espelha os
  comandos do simulador: botão de liberação → `manual_release_start/stop`
  (respeitando `..._is_allowed()`, RS14/RS15); botão de emergência →
  `dosing_controller_cancel()` durante `DOSING` (RS11/RS12), ficando a
  troca para a tela Interrompida a cargo do timer da tela de dosagem
  (mesmo padrão do estado Concluído); LED →
  `manual_release_is_active()` (RS16);
- `src/main_esp32.cpp` — `physical_controls_init()` no `setup()` e
  `physical_controls_poll()` no `loop()`.

Ou seja, além dos comandos **via touch**, os botões físicos e o LED
passam a atuar no ESP32. As pendências de validação **elétrica** dos pinos
(seção 3) continuam valendo.

**Relação com o simulador (já pronto):** o comportamento lógico já existe
e foi validado no PC, então a lacuna é de integração de hardware, não de
domínio:

- `dosing_controller_manual_release_start/stop` +
  `..._is_allowed()` implementam RS14/RS15 (bloqueio em `DOSING`);
- `src/ui/components/manual_release_widget.c` faz o "segura para liberar"
  (equivalente ao botão de liberação);
- o indicador verde da Home representa o LED do RS16.

**Encaixe previsto nas docs do projeto:**

- entrada física como `GPIO → HAL/driver → aplicação`, sem espalhar o
  número do pino no domínio
  ([`11-migration-pc-to-esp32/03-lvgl-ui-e-interacao-fisica.md`](11-migration-pc-to-esp32/03-lvgl-ui-e-interacao-fisica.md) §13–§15);
- espelhar exatamente os comandos já validados no simulador, com debounce
  ([`08-target-hardware/05-botoes-e-indicadores.md`](08-target-hardware/05-botoes-e-indicadores.md) §22–§25);
- tratar como um incremento da sequência de migração
  ([`11-migration-pc-to-esp32/07-tempos-gpios-board-config-e-incremento.md`](11-migration-pc-to-esp32/07-tempos-gpios-board-config-e-incremento.md) §45);
- as implicações elétricas dos pinos (GPIO10/17/18) já estão na seção 2.

---

### 6. Notas

- `MIGRACAO-ESP32-NOTAS.md` contém uma proposta histórica de pinagem
  (HX711 6/16, servo 15, botões 35/36, LED 20) de outro layout de placa
  — **não corresponde** ao `board_config.h` atual nem ao BSP da VIEWE;
  não usar como referência.
- Regra de design de GPIOs (concentrar pinos no hardware e não no
  domínio): [`11-migration-pc-to-esp32/07-tempos-gpios-board-config-e-incremento.md`](11-migration-pc-to-esp32/07-tempos-gpios-board-config-e-incremento.md).