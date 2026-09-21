# Objetivo, stack e screen manager

## 1. Objetivo

Este documento descreve a interface gráfica atual do projeto, a responsabilidade de cada tela, o fluxo de navegação e a relação entre a UI e o domínio da aplicação.

A interface é construída com **LVGL** e, atualmente, executada no simulador desktop através de **SDL2**.

A UI tem como responsabilidade principal:

* apresentar informações;
* receber ações do usuário;
* solicitar mudanças de estado;
* exibir o resultado do domínio.

Ela **não deve ser responsável pelas regras de dosagem**.

A separação fundamental é:

```text
Usuário
   ↓
UI / LVGL
   ↓
Screen Manager
   ↓
Domínio
   ↓
Hardware abstraction
```

A UI apresenta e recebe comandos; o domínio decide o comportamento.

---

## 2. Objetivo da interface

A interface deve permitir que o usuário realize o ciclo básico do dosador:

```text
Iniciar
   ↓
Selecionar modo
   ↓
Configurar dosagem
   ↓
Iniciar dosagem
   ↓
Acompanhar peso/progresso
   ↓
Dosagem concluída
   ↓
Nova dosagem / Início
```

Atualmente existem dois modos de configuração:

```text
Massa
Valor (R$)
```

---

## 3. Stack da UI

A interface atual utiliza:

```text
LVGL
 └── widgets, telas, eventos e timers
```

No simulador:

```text
LVGL
   ↓
SDL2
   ↓
janela desktop
```

No hardware final:

```text
LVGL
   ↓
display/touch do ESP32-S3
```

A intenção é que a maior parte da estrutura de UI possa permanecer conceitualmente igual durante essa transição.

---

## 4. Organização dos arquivos

A UI está localizada em:

```text
src/ui/
```

Estrutura atual:

```text
src/ui/
├── ui.c
├── ui.h
├── screen_manager.c
├── screen_manager.h
└── screens/
    ├── home_screen.c
    ├── home_screen.h
    ├── mode_screen.c
    ├── mode_screen.h
    ├── config_screen.c
    ├── config_screen.h
    ├── dosing_screen.c
    ├── dosing_screen.h
    ├── completed_screen.c
    ├── completed_screen.h
    ├── interrupted_screen.c
    └── interrupted_screen.h
```

A organização separa:

```text
ui.c
```

da coordenação das telas:

```text
screen_manager.c
```

e das implementações individuais:

```text
screens/*.c
```

---

## 5. `ui.c`

O ponto de entrada da interface é:

```c
void ui_init(void)
{
    screen_manager_init();
}
```

Portanto, `ui_init()` não constrói diretamente todas as telas.

Ele inicia o gerenciamento da interface.

O fluxo é:

```text
main.c
   ↓
ui_init()
   ↓
screen_manager_init()
   ↓
Home
```

---

## 6. `Screen Manager`

O `Screen Manager` é responsável por controlar qual tela está sendo apresentada.

A enumeração atual é:

```c
typedef enum {
    SCREEN_HOME,
    SCREEN_MODE,
    SCREEN_CONFIG,
    SCREEN_DOSING,
    SCREEN_COMPLETED,
    SCREEN_INTERRUPTED
} Screen;
```

As telas existentes são:

```text
SCREEN_HOME
SCREEN_MODE
SCREEN_CONFIG
SCREEN_DOSING
SCREEN_COMPLETED
SCREEN_INTERRUPTED
```

---

## 7. Responsabilidade do Screen Manager

O `screen_manager` deve coordenar:

* tela atual;
* troca de telas;
* passagem de configuração;
* inicialização do controller quando a dosagem começa;
* criação da tela correspondente.

Ele não deve conter a lógica detalhada de cada tela.

Por exemplo, não deve implementar:

```text
como o botão +/- funciona
```

Isso pertence à tela de configuração.

---

## 8. Fluxo geral de navegação

O fluxo atual pode ser representado por:

```text
             ┌──────────────┐
             │     HOME     │
             └──────┬───────┘
                    │
                 Iniciar
                    │
                    ▼
             ┌──────────────┐
             │     MODE     │
             └──────┬───────┘
                    │
          ┌─────────┴─────────┐
          │                   │
          ▼                   ▼
        Massa             Valor (R$)
          │                   │
          └─────────┬─────────┘
                    ▼
             ┌──────────────┐
             │    CONFIG    │
             └──────┬───────┘
                    │
                Continuar
                    │
                    ▼
             ┌──────────────┐
             │    DOSING    │
             └──┬───────┬───┘
                │       │
   objetivo     │       │  Parar /
   atingido     │       │  Emergência
                │       │
                ▼       ▼
     ┌──────────────┐ ┌──────────────┐
     │  COMPLETED   │ │  INTERRUPTED │
     └──────┬───────┘ └──────┬───────┘
            │                │
     Nova dosagem     Nova dosagem
            │                │
            ▼                ▼
          MODE            MODE
```

Também existe o caminho de volta para o início a partir das telas de conclusão e interrupção:

```text
COMPLETED / INTERRUPTED
   │
Voltar ao inicio
   │
   ▼
HOME
```