## 01. Objetivo e princípio da migração

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 1. Objetivo

Este documento define como o projeto deve evoluir do simulador desktop para o hardware físico baseado em ESP32-S3.

A migração não deve ser tratada como uma reescrita completa.

A arquitetura foi construída desde o início para permitir que partes específicas da plataforma sejam substituídas enquanto o comportamento da aplicação permanece o mais estável possível.

A ideia central é:

```text
PC
├── Windows
├── SDL2
├── SDL HAL
├── hardware simulado
└── aplicação
        │
        ▼
ESP32-S3
├── ESP32
├── display/touch real
├── HAL/driver
├── hardware real
└── mesma aplicação
```

O objetivo é substituir as dependências específicas do PC sem reescrever desnecessariamente o domínio da aplicação.

---

### 2. Visão geral

A migração pode ser resumida em:

```text
                 APLICAÇÃO
                     │
        ┌────────────┼────────────┐
        │            │            │
        ▼            ▼            ▼
       UI          Domain      Hardware
        │            │            │
        └────────────┴────────────┘
                     │
              ┌──────┴──────┐
              │             │
              ▼             ▼
             PC          ESP32-S3
```

No PC:

```text
LVGL
 ↓
SDL2
 ↓
Simulação
```

No ESP32-S3:

```text
LVGL
 ↓
display/touch drivers
 ↓
hardware real
```

---

### 3. O princípio da migração

A regra principal é:

> **Substituir implementações específicas da plataforma, não reescrever o comportamento do produto sem necessidade.**

Isso significa que a migração deve preservar, sempre que possível:

* regras de negócio;
* configuração da dosagem;
* fluxo da aplicação;
* máquina de estados;
* responsabilidades do controller;
* conceitos das abstrações de hardware;
* estrutura geral da UI.

Enquanto componentes específicos do PC serão substituídos.