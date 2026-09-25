# Execução e loop principal

[voltar ao índice](../09-pc-development-environment.md)

---

## 30. Executável

O executável atual é:

```text
.\bin\Debug\main.exe
```

A execução inicia:

```text
main()
   ↓
lv_init()
   ↓
sdl_hal_init(480, 800)
   ↓
ui_init()
   ↓
Home
```

---

## 31. `main.c`

O `main.c` é o ponto de entrada do simulador.

O fluxo principal é:

```c
lv_init();

sdl_hal_init(480, 800);

ui_init();
```

Depois existe o loop principal:

```c
while(1) {
    uint32_t sleep_time_ms = lv_timer_handler();

    ...
}
```

---

## 32. Loop principal

O loop principal entrega periodicamente o controle para o LVGL:

```text
while
  ↓
lv_timer_handler()
  ↓
LVGL processa timers/eventos/renderização
  ↓
sleep
  ↓
repete
```

Esse loop substitui, no ambiente desktop, a estrutura de execução que futuramente será utilizada no firmware do ESP32-S3.

---

## 33. `lv_timer_handler()`

A função:

```c
lv_timer_handler()
```

é responsável por permitir que o LVGL processe:

* timers;
* eventos;
* atualizações;
* tarefas internas;
* atualização da interface.

O loop chama essa função continuamente.

---

## 34. Tempo de espera

O retorno de:

```c
lv_timer_handler()
```

é utilizado para determinar aproximadamente quanto tempo o programa pode aguardar antes da próxima execução.

Quando nenhum timer está pronto:

```c
if(sleep_time_ms == LV_NO_TIMER_READY)
```

é utilizado:

```text
LV_DEF_REFR_PERIOD
```

como período padrão.

---

## 35. Compatibilidade Windows

No Windows, o código utiliza:

```c
Sleep(sleep_time_ms);
```

Em sistemas POSIX, poderia utilizar:

```c
usleep(...)
```

Essa diferença é tratada no `main.c`.

Isso evita que o loop principal precise ser reescrito completamente para diferentes plataformas.

---

## 36. HAL do simulador

A camada:

```text
src_pc/hal/
```

contém a integração específica da plataforma desktop.

Atualmente:

```text
src_pc/hal/hal.c
src_pc/hal/hal.h
```

Essa camada é importante porque o restante da aplicação não deve precisar conhecer detalhes da janela SDL.

---

## 37. `sdl_hal_init`

O simulador é inicializado através de:

```c
sdl_hal_init(480, 800);
```

Essa chamada configura o ambiente SDL utilizado pelo LVGL.

O tamanho é explicitamente alinhado ao display físico alvo (4,3"
800×480 montado em **retrato**):

```text
480 × 800 (área útil da UI)
```

---

## 38. Separação da HAL

A ideia é manter:

```text
PC-specific code
```

concentrado na camada:

```text
HAL
```

enquanto:

```text
UI
Domain
Hardware abstractions
```

permanecem mais independentes.

Conceitualmente:

```text
Application
    │
    ├── UI
    ├── Domain
    └── Hardware abstractions
             │
             ▼
            HAL
             │
             ▼
           SDL2
```

---

## 39. Estrutura do código-fonte

A organização de `src/` (`main.c`, `hal/`, `ui/`, `domain/`, `hardware/`), que separa plataforma, interface, domínio e hardware, é detalhada em [02-architecture.md](../02-architecture.md).

---

## 40. `main.c` e domínio

O `main.c` não implementa a lógica da dosagem.

Ele apenas inicializa os subsistemas necessários:

```text
LVGL
SDL2 HAL
UI
```

A partir daí:

```text
UI
 ↓
Screen Manager
 ↓
DosingController
```

assume o comportamento da aplicação.