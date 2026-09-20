## 01. Objetivo e visão geral

[voltar ao índice](../08-target-hardware.md)

---

### 1. Objetivo

Este documento descreve o hardware físico planejado para a versão final do projeto de dosagem automática de ração.

O sistema está sendo desenvolvido inicialmente em um simulador desktop para validar a interface, o fluxo da aplicação, a lógica de dosagem e as abstrações de hardware.

O hardware físico será responsável por transformar esse comportamento simulado em um dispositivo real.

A plataforma alvo é baseada em:

```text
ESP32-S3
   │
   ├── Display 4,3" — 800×480
   ├── Touch capacitivo
   ├── HX711
   ├── Load Cell
   ├── SG90 / atuador
   ├── Botões físicos
   ├── Chave liga/desliga
   ├── LED / indicador
   ├── Reservatório
   ├── Mecanismo de dosagem
   └── Fonte de alimentação
```

---

### 2. Visão geral do dispositivo

O produto final deverá funcionar aproximadamente assim:

```text
                 ┌──────────────────────┐
                 │      DISPLAY         │
                 │      800 × 480       │
                 │    Touch capacitivo  │
                 └──────────┬───────────┘
                            │
                            ▼
                     ┌─────────────┐
                     │  ESP32-S3   │
                     │             │
                     │    LVGL     │
                     │    App      │
                     └──────┬──────┘
                            │
             ┌──────────────┼──────────────┐
             │              │              │
             ▼              ▼              ▼
          HX711          SG90          Botões
             │              │
             ▼              ▼
       Load Cell       Mecanismo
             │              │
             └──────┬───────┘
                    ▼
                  Ração
```

O ESP32-S3 será o controlador central do dispositivo.

---

### 3. ESP32-S3

O microcontrolador alvo é o:

```text
ESP32-S3
```

A placa considerada no projeto é uma variante:

```text
ESP32-S3 N16R8
```

A nomenclatura indica uma variante com memória flash e PSRAM adequadas para uma aplicação gráfica relativamente mais exigente.

O ESP32-S3 será responsável por executar:

* aplicação principal;
* lógica de dosagem;
* LVGL;
* comunicação com o display;
* leitura do touch;
* comunicação com o HX711;
* controle do servo/atuador;
* leitura dos botões;
* gerenciamento de estados;
* tratamento de erros.

---

### 4. Papel do ESP32-S3 na arquitetura

A arquitetura física pode ser representada como:

```text
ESP32-S3
│
├── Application
│   ├── UI
│   ├── Domain
│   └── Controllers
│
├── Display
│
├── Touch
│
├── WeightSensor
│   └── HX711
│
├── Dispenser
│   └── SG90 / actuator
│
├── Buttons
│
└── Indicators
```

A principal responsabilidade do microcontrolador é coordenar esses componentes.

---

[voltar ao índice](../08-target-hardware.md)